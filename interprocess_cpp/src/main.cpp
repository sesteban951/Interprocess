#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <iostream>
#include <thread>
#include <mutex>

using namespace boost::interprocess;

struct SharedData {
    double data_A;
    double data_B;
    bool updated_A;
    bool updated_B;
};

int main() {
    try {
        managed_shared_memory shm(open_only, "SharedMemory");
        SharedData* data = shm.find<SharedData>("SharedData").first;
        named_mutex mutex(open_only, "SharedMemoryMutex");
        named_condition cond_var(open_only, "SharedMemoryCondition");

        while (true) {
            std::unique_lock<named_mutex> lock(mutex);

            // Wait for Repo A's update
            cond_var.wait(lock, [&] { return data->updated_A; });
            std::cout << "Received from Repo A: " << data->data_A << std::endl;
            data->updated_A = false;

            // Update data_B
            data->data_B = data->data_A * 2;  // Example transformation
            data->updated_B = true;

            // Notify Repo A
            cond_var.notify_one();
        }
    } catch (const interprocess_exception& e) {
        std::cerr << "Shared memory not found! Ensure Repo A is running." << std::endl;
    }

    return 0;
}
