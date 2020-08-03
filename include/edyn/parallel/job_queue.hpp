#ifndef EDYN_PARALLEL_JOB_QUEUE_HPP
#define EDYN_PARALLEL_JOB_QUEUE_HPP

#include <mutex>
#include <atomic>
#include <deque>
#include <memory>
#include <condition_variable>
#include "edyn/parallel/job.hpp"

namespace edyn {

class job_queue {
public:
    void push_back(std::shared_ptr<job> j);
    void push_front(std::shared_ptr<job> j);

    std::shared_ptr<job> pop();

    std::shared_ptr<job> try_pop();

    void unblock();

    size_t size();

private:
    std::mutex m_mutex;
    std::deque<std::shared_ptr<job>> m_jobs;
    std::condition_variable m_cv;
    std::atomic_bool m_unblock {false};
};

}

#endif // EDYN_PARALLEL_JOB_QUEUE_HPP