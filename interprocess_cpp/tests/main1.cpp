#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <iostream>
#include <cstring>
#include <thread>
#include <chrono>

using namespace boost::interprocess;

int main() 
{
    try 
    {
        // Remove old shared memory and mutex if they exist
        shared_memory_object::remove("SharedMemory");
        named_mutex::remove("SharedMutex");

        // Create shared memory
        shared_memory_object shm(create_only, "SharedMemory", read_write);
        shm.truncate(sizeof(int));

        // Map shared memory
        mapped_region region(shm, read_write);
        int* shared_value = static_cast<int*>(region.get_address());

        // Initialize shared variable
        *shared_value = 0;

        // Create named mutex
        named_mutex mutex(create_only, "SharedMutex");

        for (int i = 1; i <= 5; ++i) {
            // Lock mutex before modifying shared memory
            mutex.lock();
            *shared_value += i;
            std::cout << "Process 1 wrote: " << *shared_value << std::endl;
            mutex.unlock();

            // Sleep to simulate some work
            std::this_thread::sleep_for(std::chrono::seconds(3));
        }

        // Cleanup
        named_mutex::remove("SharedMutex");
        shared_memory_object::remove("SharedMemory");

    } catch (const std::exception& e) {
        std::cerr << "Process 1 error: " << e.what() << std::endl;
    }

    return 0;
}
