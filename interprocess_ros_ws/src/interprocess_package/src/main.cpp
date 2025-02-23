#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/interprocess/sync/named_condition.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <rclcpp/rclcpp.hpp>
#include <chrono>
#include <mutex>

using namespace boost::interprocess;

struct SharedData {
    double data_A;
    double data_B;
    bool updated_A;
    bool updated_B;
};

class SharedMemoryNode : public rclcpp::Node {
public:
    SharedMemoryNode() : Node("shared_memory_node") {
        // Clean up any existing shared memory
        shared_memory_object::remove("SharedMemory");
        named_mutex::remove("SharedMemoryMutex");
        named_condition::remove("SharedMemoryCondition");

        // Create shared memory and shared data structure
        managed_shared_memory shm(create_only, "SharedMemory", 1024);
        data_ = shm.construct<SharedData>("SharedData")();
        data_->data_A = 0.0;
        data_->data_B = 0.0;
        data_->updated_A = false;
        data_->updated_B = false;

        mutex_ = std::make_unique<named_mutex>(create_only, "SharedMemoryMutex");
        cond_var_ = std::make_unique<named_condition>(create_only, "SharedMemoryCondition");

        // Timers to update data_A and data_B
        timer_A_ = this->create_wall_timer(
            std::chrono::milliseconds(10),
            std::bind(&SharedMemoryNode::update_data_A, this));

        timer_B_ = this->create_wall_timer(
            std::chrono::milliseconds(20),
            std::bind(&SharedMemoryNode::update_data_B, this));
    }

    ~SharedMemoryNode() {
        shared_memory_object::remove("SharedMemory");
        named_mutex::remove("SharedMemoryMutex");
        named_condition::remove("SharedMemoryCondition");
    }

private:
    void update_data_A() {
        std::scoped_lock<named_mutex> lock(*mutex_);
        
        // Update data_A
        data_->data_A += 0.1;
        data_->updated_A = true;

        // Notify Repo B to process data_B
        cond_var_->notify_one();
    }

    void update_data_B() {
        std::scoped_lock<named_mutex> lock(*mutex_);
        
        // Update data_B if data_A is updated
        if (data_->updated_A) {
            data_->data_B = data_->data_A * 2.0;
            data_->updated_B = true;

            // Notify Repo A that data_B has been updated
            cond_var_->notify_one();
        }
    }

    SharedData* data_;
    std::unique_ptr<named_mutex> mutex_;
    std::unique_ptr<named_condition> cond_var_;
    rclcpp::TimerBase::SharedPtr timer_A_;
    rclcpp::TimerBase::SharedPtr timer_B_;
};

int main(int argc, char* argv[]) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SharedMemoryNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
