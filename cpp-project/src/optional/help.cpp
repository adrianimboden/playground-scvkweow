#include <iostream>
#include <optional>

void print_value(std::optional<int> optional_value) {
    if (optional_value) {
        std::cout << optional_value.value() << "\n";
    } else {
        std::cout << "n/a\n";
    }
}

std::optional<int> read_number() {
    return 5;
}

int main() {
    auto input_number = std::optional<int>{};

    print_value(input_number);
    input_number = 5;
    print_value(input_number);

    if (auto opt_num = read_number()) {
        std::cout << opt_num.value() << "\n";
    }

    std::cout << read_number().value_or(7) << "\n";
}
