// { autofold
#include <boost/assert.hpp>
#include <iostream>
#include <mutex>
#include <thread>

void long_running_task() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
// }

class SerialPort {
public:
    void        connect();
    std::string send_request(const std::string& request);
    void        print_state(std::ostream& os) const;

private:
    mutable std::mutex guard_;
    bool               is_connected_ = false;
    bool               is_sending_   = false;
    bool               is_receiving_ = false;
};

void SerialPort::connect() {
    long_running_task();

    std::scoped_lock lock{guard_};
    BOOST_ASSERT_MSG(not is_connected_, "port already connected yet");
    is_connected_ = true;
}

std::string SerialPort::send_request(const std::string& request) {
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(is_connected_, "port not connected yet");
        BOOST_ASSERT_MSG(not is_sending_, "detected race");
        BOOST_ASSERT_MSG(not is_receiving_, "detected race");
        is_sending_ = true;
    }
    long_running_task();
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(is_connected_, "detected race");
        BOOST_ASSERT_MSG(is_sending_, "detected race");
        BOOST_ASSERT_MSG(not is_receiving_, "detected race");
        is_sending_   = false;
        is_receiving_ = true;
    }
    long_running_task();
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(is_connected_, "detected race");
        BOOST_ASSERT_MSG(not is_sending_, "detected race");
        BOOST_ASSERT_MSG(is_receiving_, "detected race");
        is_receiving_ = false;
    }
    return "ack";
}

void SerialPort::print_state(std::ostream& os) const {
    std::scoped_lock lock{guard_};
    os << "connected=" << is_connected_ << ", sending=" << is_sending_ << ", receiving=" << is_receiving_ << "\n";
}
