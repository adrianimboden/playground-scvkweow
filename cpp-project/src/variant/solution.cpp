// { autofold
#include <iostream>
#include <variant>

// likely part of C++20
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;
// }

enum class ErrorType {
    OutOfRangeForCheck,
    Timeout,
};

struct ErrorInfo {
    ErrorType   type;
    std::string detailInfo;
};

using Quality = std::variant<uint32_t, ErrorInfo>;

Quality check_quality(uint8_t production_result) {
    if (production_result < 10 || production_result > 20) {
        return ErrorInfo{ErrorType::OutOfRangeForCheck, "must be x >= 10 and x <= 20"};
    }
    return uint32_t{production_result} * 2;
}

void print_quality(Quality quality) {
    std::visit(
        overloaded{
            [](ErrorInfo error) { std::cout << "Error: " << error.detailInfo << "\n"; },
            [](uint32_t quality) { std::cout << "Quality: " << quality << "\n"; },
        },
        quality);
}

void do_one_production_cycle() {
    print_quality(check_quality(3));
    print_quality(check_quality(12));
    print_quality(check_quality(17));
}
