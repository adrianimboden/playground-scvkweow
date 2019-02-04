# Create datatypes which do not allow invalid state
Many user defined datatypes created in "classical" C++ allow many states which should not happen in production.

This playground has the goal to show some of them and how to avoid them using C++14/17 standard library type additions.

The build environment is:
* Clang 7.0.1
* -std=c++17
* -Wall -Werror
* Libraries: Boost 1.68
