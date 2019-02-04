#include <future>

void example() {
    auto       serialPort = SerialPort{};
    const auto t1         = std::async(std::launch::async, [&] {
        serialPort.connect();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        auto response = serialPort.send_request("cmd");
        BOOST_ASSERT(response == "ack");
    });
    const auto t2         = std::async(std::launch::async, [&] {
        for (size_t i = 0; i < 12; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(40));
            serialPort.print_state(std::cout);
        }
    });
    t1.wait();
    t2.wait();
}
