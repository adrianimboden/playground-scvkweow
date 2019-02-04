#include <iostream>
#include <string>

enum class Color { Red, Blue, Yellow };

std::string enum_to_string(Color color) {
    switch (color) {
    case Color::Red:
        return "red";
    case Color::Blue:
        return "blue";
    case Color::Yellow:
        return "yellow";
    }
    // the compiler detects when not all cases are checked
    // try it out
}

int main() {
    auto background_color = Color::Red;
    std::cout << enum_to_string(background_color) << "\n";
}
