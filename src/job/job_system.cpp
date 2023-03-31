#include "glibby/job/job_system.h"

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <thread>

namespace glibby::job
{
    template <typename T, size_t capacity>
    class ThreadSafeRingBuffer
    {
    public:
        inline bool push_back(const T& item) // fails if there is insufficient space
        {
            bool result = false;
            lock.lock();
            size_t next = (head + 1) % capacity;
            if (next != tail)
            {
                data[head] = item;
                head = next;
                result = true;
            }
            lock.unlock();
            return result;
        }

        inline bool pop_front(T& item)
        {
            bool result = false;
            lock.lock();
            if (tail != head)
            {
                item = data[tail];
                tail = (tail + 1) % capacity;
                result = true;
            }
            lock.unlock();
            return result;
        }

    private:
        T data[capacity];
        size_t head = 0;
        size_t tail = 0;
        std::mutex lock; // this just works better than a spinlock here (on windows)
    };

    uint32_t numThreads = 0; // number of worker threads
    ThreadSafeRingBuffer<std::function<void()>, 256> jobPool; // threadsafe job queue
    std::condition_variable wakeCondition;  // used in conjunction with the wakeMutex below. Worker threads just sleep when there is no job, and the main thread can wake them up
    std::mutex wakeMutex;
    uint64_t currentLabel = 0;              // tracks the state of execution of the main thread
    std::atomic<uint64_t> finishedLabel;    // track the state of execution across background worker threads

    void Initialize()
    {
        finishedLabel.store(0); // initialize worker execution state to 0
        uint32_t numCores = std::thread::hardware_concurrency();
        numThreads = std::max(1u, numCores);

        for (uint32_t threadID = 0; threadID < numThreads; threadID++)
        {
            std::thread worker([] {
                std::function<void()> job;

                while (true)
                {
                    if (jobPool.pop_front(job)) // try to grab and execute a job
                    {
                        job();
                        finishedLabel.fetch_add(1);
                    }
                    else
                    {
                        std::unique_lock<std::mutex> lock(wakeMutex);
                        wakeCondition.wait(lock); // sleep while no job
                    }
                }

                });

            worker.detach();
        }
    }

    // This little helper function will not let the system to be deadlocked while the main thread is waiting for something
    inline void Poll()
    {
        wakeCondition.notify_one();
        std::this_thread::yield(); // allow the main thread to be captured for a job
    }

    void Execute(const std::function<void()>& job)
    {
        currentLabel += 1;
        while (!jobPool.push_back(job))
        {
            Poll();
        }

        wakeCondition.notify_one(); // wake one thread
    }

    void Dispatch(uint32_t jobCount, uint32_t groupSize, const std::function<void(JobDispatchArgs)>& job)
    {
        if (jobCount == 0 || groupSize == 0) return;

        // Calculate the amount of job groups to dispatch (overestimate, or "ceil"):
        const uint32_t groupCount = (jobCount + groupSize - 1) / groupSize;

        currentLabel += groupCount;

        for (uint32_t groupIndex = 0; groupIndex < groupCount; groupIndex++)
        {
            const auto& jobGroup = [jobCount, groupSize, job, groupIndex]() {
                const uint32_t groupJobOffset = groupIndex * groupSize;
                const uint32_t groupJobEnd = std::min(groupJobOffset + groupSize, jobCount);

                JobDispatchArgs args;
                args.groupIndex = groupIndex;

                for (uint32_t i = groupJobOffset; i < groupJobEnd; i++)
                {
                    args.jobIndex = i;
                    job(args);
                }
            };

            while (!jobPool.push_back(jobGroup))
            {
                Poll();
            }

            wakeCondition.notify_one();
        }
    }

    bool IsBusy()
    {
        // Whenever the main thread label is not reached by the workers, it indicates that some worker is still alive
        return finishedLabel.load() < currentLabel;
    }

    void Wait()
    {
        while (IsBusy())
        {
            Poll();
        }
    }
}
