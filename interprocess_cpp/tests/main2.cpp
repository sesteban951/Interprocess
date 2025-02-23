#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace boost::interprocess;

int main() {
    try {
        // Open existing shared memory
        shared_memory_object shm(open_only, "SharedMemory", read_write);

        // Map shared memory
        mapped_region region(shm, read_write);
        int* shared_value = static_cast<int*>(region.get_address());

        // Open existing named mutex
        named_mutex mutex(open_only, "SharedMutex");

        for (int i = 0; i < 5; ++i) {
            // Lock mutex before reading shared memory
            mutex.lock();
            std::cout << "Process 2 read: " << *shared_value << std::endl;
            mutex.unlock();

            // Sleep to simulate some work
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    } catch (const std::exception& e) {
        std::cerr << "Process 2 error: " << e.what() << std::endl;
    }

    return 0;
}
