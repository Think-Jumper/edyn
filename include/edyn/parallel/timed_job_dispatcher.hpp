#ifndef EDYN_PARALLEL_TIME_JOB_DISPATCHER_HPP
#define EDYN_PARALLEL_TIME_JOB_DISPATCHER_HPP

#include <thread>
#include <memory>
#include <cstdint>
#include <vector>
#include <mutex>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include "edyn/parallel/job.hpp"

namespace edyn {

class job_dispatcher;

struct timed_job {
    using time_point = std::chrono::time_point<std::chrono::high_resolution_clock>;
    time_point m_timestamp;
    std::shared_ptr<job> m_job;

    timed_job(const time_point &timestamp, std::shared_ptr<job> j)
        : m_timestamp(timestamp)
        , m_job(j)
    {}
};

class timed_job_dispatcher {
public:
static timed_job_dispatcher &global();

    ~timed_job_dispatcher();

    void start(job_dispatcher &);
    void stop();

    void schedule(std::shared_ptr<timed_job>);

    void timer_main();

private:
    std::unique_ptr<std::thread> m_thread;
    std::atomic<bool> m_running {false};
    job_dispatcher *m_job_dispatcher;
    std::vector<std::shared_ptr<timed_job>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
};

}


#endif // EDYN_PARALLEL_TIME_JOB_DISPATCHER_HPP