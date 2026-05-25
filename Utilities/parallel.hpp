#ifndef BITFAKE_PARALLEL_HPP
#define BITFAKE_PARALLEL_HPP

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>

namespace bitfake::parallel {

inline std::size_t DetectHardwareThreads() {
    const unsigned int hardwareThreads = std::thread::hardware_concurrency();
    return hardwareThreads > 0 ? static_cast<std::size_t>(hardwareThreads) : static_cast<std::size_t>(1);
}

inline std::size_t ComputeWorkerCount(std::size_t taskCount, bool parallelEnabled, std::size_t requestedThreads = 0) {
    if (!parallelEnabled || taskCount <= 1) {
        return 1;
    }

    const std::size_t desiredWorkers =
        requestedThreads > 0 ? requestedThreads : std::max<std::size_t>(1, DetectHardwareThreads() / 2);

    return std::min(taskCount, desiredWorkers);
}

template <typename Fn>
std::exception_ptr ParallelFor(std::size_t taskCount, std::size_t workerCount, Fn &&fn) {
    if (taskCount == 0) {
        return nullptr;
    }

    if (workerCount <= 1) {
        for (std::size_t i = 0; i < taskCount; ++i) {
            fn(i);
        }
        return nullptr;
    }

    std::atomic<std::size_t> nextIndex{0};
    std::exception_ptr firstException = nullptr;
    std::mutex exceptionMutex;

    std::vector<std::thread> workers;
    workers.reserve(workerCount);

    for (std::size_t worker = 0; worker < workerCount; ++worker) {
        workers.emplace_back([&]() {
            while (true) {
                const std::size_t index = nextIndex.fetch_add(1, std::memory_order_relaxed);
                if (index >= taskCount) {
                    break;
                }

                try {
                    fn(index);
                } catch (...) {
                    std::lock_guard<std::mutex> lock(exceptionMutex);
                    if (!firstException) {
                        firstException = std::current_exception();
                    }
                }
            }
        });
    }

    for (auto &worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }

    return firstException;
}

} // namespace bitfake::parallel

#endif // BITFAKE_PARALLEL_HPP
