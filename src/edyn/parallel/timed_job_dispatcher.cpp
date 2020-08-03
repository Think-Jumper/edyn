#include "edyn/parallel/timed_job_dispatcher.hpp"
#include "edyn/parallel/job_dispatcher.hpp"
#include <algorithm>
#include <chrono>

namespace edyn {

void timed_job_dispatcher::start(job_dispatcher &dispatcher) {
    m_job_dispatcher = &dispatcher;
    m_running = true;
    m_thread = std::make_unique<std::thread>(&timed_job_dispatcher::timer_main, this);
}

timed_job_dispatcher::~timed_job_dispatcher() {
    stop();
}

void timed_job_dispatcher::stop() {
    if (m_thread) {
        m_running.store(false, std::memory_order_release);
        m_cv.notify_one();
        m_thread->join();
        m_thread.reset();
    }
}

void timed_job_dispatcher::schedule(std::shared_ptr<timed_job> j) {
    std::lock_guard lock(m_mutex);
    m_queue.push_back(j);
    std::sort(m_queue.begin(), m_queue.end(), [] (auto &&lhs, auto &&rhs) { return lhs->m_timestamp < rhs->m_timestamp; });
    m_cv.notify_one();
}

void timed_job_dispatcher::timer_main() {
    std::chrono::nanoseconds time_until_next_job(0);

    while (m_running.load(std::memory_order_acquire)) {
        std::unique_lock lock(m_mutex);

        if (time_until_next_job > std::chrono::nanoseconds::zero()) {
            m_cv.wait_for(lock, time_until_next_job, 
                          [&] { return !m_queue.empty() || !m_running; });
        } else {
            m_cv.wait(lock, [&] { return !m_queue.empty() || !m_running; });
        }

        if (m_queue.empty()) {
            time_until_next_job = std::chrono::nanoseconds::zero();
            continue;
        }

        auto now = std::chrono::high_resolution_clock::now();
        size_t i = 0;

        for (; i < m_queue.size(); ++i) {
            if (m_queue[i]->m_timestamp <= now) {
                m_job_dispatcher->async_asap(m_queue[i]->m_job);
            } else {
                break;
            }
        }

        if (i < m_queue.size()) {
            time_until_next_job = m_queue[i]->m_timestamp - now;
        } else {
            time_until_next_job = std::chrono::nanoseconds::zero();
        }

        m_queue.erase(m_queue.begin(), m_queue.begin() + i);
    }
}

}