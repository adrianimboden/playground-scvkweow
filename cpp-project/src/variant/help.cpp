// { autofold
#include <iostream>

// likely part of C++20
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;
// }

#include <variant>

int main() {
    std::variant<int, std::string> num_or_str = 7;

    if (std::holds_alternative<int>(num_or_str)) {
        std::cout << std::get<int>(num_or_str) << "\n";
    }

    if (auto* p_num = std::get_if<int>(&num_or_str)) {
        std::cout << *p_num << "\n";
    }

    std::visit(
        [](auto streamable) {
            if constexpr (std::is_same_v<decltype(streamable), int>) {
                std::cout << streamable << "\n";
            }
        },
        num_or_str);

    std::visit(
        overloaded{
            [](int num) { std::cout << num << "\n"; },
            [](std::string) {},
        },
        num_or_str);

    std::visit([](auto streamable) { std::cout << streamable << "\n"; }, num_or_str);

    struct PrintVisitor {
        void operator()(int num) {
            std::cout << "num: " << num << "\n";
        }
        void operator()(std::string str) {
            std::cout << "str: " << str << "\n";
        }
    };
    std::visit(PrintVisitor{}, num_or_str);
}
