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
    enum class State { Init, Connected, Sending, Receiving };

public:
    void        connect();
    std::string send_request(const std::string& request);
    void        print_state(std::ostream& os) const;

private:
    mutable std::mutex guard_;
    State              state_ = State::Init;
};

void SerialPort::connect() {
    long_running_task();

    std::scoped_lock lock{guard_};
    BOOST_ASSERT_MSG(state_ == State::Init, "port already connected");
    state_ = State::Connected;
}

std::string SerialPort::send_request(const std::string& request) {
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(state_ == State::Connected, "port not connected yet");
        state_ = State::Sending;
    }
    long_running_task();
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(state_ == State::Sending, "detected race");
        state_ = State::Receiving;
    }
    long_running_task();
    {
        std::scoped_lock lock{guard_};
        BOOST_ASSERT_MSG(state_ == State::Receiving, "detected race");
        state_ = State::Connected;
    }
    return "ack";
}

void SerialPort::print_state(std::ostream& os) const {
    std::scoped_lock lock{guard_};
    switch (state_) {
    case State::Init:
        os << "init\n";
        return;
    case State::Connected:
        os << "connected\n";
        return;
    case State::Receiving:
        os << "receiving\n";
        return;
    case State::Sending:
        os << "sending\n";
        return;
    }
}
