#include "rw_lock/atomic_rw_lock.h"

void AtomicRWLock::rLock() {
    int cur_cnt = lock_cnt_.load(std::memory_order_relaxed);
    if (cur_cnt >= 0) {
        bool succ =
            lock_cnt_.compare_exchange_strong(cur_cnt, cur_cnt + 1, std::memory_order_relaxed);
        if (succ) {
            return;
        }
    }
    while (true) {
        // 在写锁中, 或者读锁加失败, 自旋尝试获取几次
        if (core_num_ > 1) {
            for (int i = 0; i < 10; i++) {
                int cur_cnt = lock_cnt_.load(std::memory_order_relaxed);
                if (cur_cnt >= 0 &&
                    lock_cnt_.compare_exchange_strong(cur_cnt, cur_cnt + 1, std::memory_order_relaxed)) {
                    return;
                }
            }
        }
        // 让出cpu
        std::this_thread::yield();
    }
}


void AtomicRWLock::rUnlock() {
    // 解锁
    int dec_cnt = 0;
    while (true) {
        int cur_cnt = lock_cnt_.load(std::memory_order_relaxed);
        // 一定是使用方式不对
        if (cur_cnt <= 0) {
            return;
        }
        if (lock_cnt_.compare_exchange_strong(cur_cnt, cur_cnt - 1, std::memory_order_relaxed)) {
            return;
        }
        // 自旋超过10次或者单核情况下
        if (core_num_ <= 1 || dec_cnt > 10) {
            std::this_thread::yield();
        }
        ++dec_cnt;
    }
}

void AtomicRWLock::wLock() {
    int try_lock_cnt = 0;
    while (true) {
        int cur_cnt = lock_cnt_.load(std::memory_order_relaxed);
        if (cur_cnt == 0 &&
            lock_cnt_.compare_exchange_strong(cur_cnt, RW_LOCK_IN_WRITE_STATE, std::memory_order_relaxed)) {
            //
            return;
        }
        try_lock_cnt++;
        if (core_num_ <= 1 || try_lock_cnt > 10) {
            std::this_thread::yield();
        }
    }
}


void AtomicRWLock::wUnLock() {
    while (true) {
        int cur_cnt = lock_cnt_.load(std::memory_order_relaxed);
        if (cur_cnt >= 0) {
            return;
        }
        if (lock_cnt_.compare_exchange_strong(cur_cnt, 0, std::memory_order_relaxed)) {
            return;
        }
    }
}