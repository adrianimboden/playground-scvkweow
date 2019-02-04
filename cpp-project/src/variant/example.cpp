#include <boost/assert.hpp>
#include <cstdint>
#include <iostream>
#include <optional>

enum class ErrorType {
    OutOfRangeForCheck,
    Timeout,
};

struct ErrorInfo {
    ErrorType   type;
    std::string detailInfo;
};

struct Quality {
    std::optional<uint32_t>  result;
    std::optional<ErrorInfo> errorInfo;
};

Quality check_quality(uint8_t production_result) {
    if (production_result < 10 || production_result > 20) {
        return Quality{std::nullopt, ErrorInfo{ErrorType::OutOfRangeForCheck, "must be x >= 10 and x <= 20"}};
    }
    return Quality{production_result * 2, std::nullopt};
}

void print_quality(Quality quality) {
    BOOST_ASSERT(quality.errorInfo || quality.result);

    if (quality.errorInfo) {
        std::cout << "Error: " << quality.errorInfo.value().detailInfo << "\n";
    } else {
        std::cout << "Quality: " << quality.result.value() << "\n";
    }
}

void do_one_production_cycle() {
    print_quality(check_quality(3));
    print_quality(check_quality(12));
    print_quality(check_quality(17));
}
