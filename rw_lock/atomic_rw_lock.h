#pragma once

#include <atomic>
#include <thread>

#define RW_LOCK_IN_WRITE_STATE -1

class AtomicRWLock final
{
private:
    std::atomic_int32_t lock_cnt_;
    unsigned core_num_;
public:
    AtomicRWLock() : lock_cnt_(0) {
        core_num_ = std::thread::hardware_concurrency();
    };
    ~AtomicRWLock() {};

    void rLock();
    void rUnlock();

    void wLock();
    void wUnLock();
};

