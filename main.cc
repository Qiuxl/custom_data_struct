#include "rw_lock/atomic_rw_lock.h"

#include <iostream>
#include <vector>

void test_atomic_rw_lock() {
    AtomicRWLock my_rwlock;
    std::vector<std::thread> read_threads;
    std::vector<std::thread> write_threads;
    for (int i = 0; i < 10; i++) {
        read_threads.emplace_back([&my_rwlock](){
            my_rwlock.rLock();
            std::cout << std::this_thread::get_id() << " get read lock\n";
            my_rwlock.rUnlock();
        });
        write_threads.emplace_back([&my_rwlock](){
            my_rwlock.wLock();
            std::cout << std::this_thread::get_id() << " get write lock\n";
            my_rwlock.wUnLock();
        });
    }
    for (int i = 0; i < read_threads.size(); i++) {
        read_threads[i].join();
        write_threads[i].join();
    }
}

int main(int argc, char const *argv[])
{
    test_atomic_rw_lock();
    return 0;
}
