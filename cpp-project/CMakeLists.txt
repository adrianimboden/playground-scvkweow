cmake_minimum_required (VERSION 3.11)

function(add_example name)
  add_executable(${name} src/${name}/main.cpp src/${name}/example.cpp)
  add_executable(${name}_solution src/${name}/main.cpp src/${name}/solution.cpp)
  add_executable(${name}_help src/${name}/help.cpp)

  target_compile_options(${name} PRIVATE
    -Wall
    -Werror
  )

  target_compile_options(${name}_solution PRIVATE
    -Wall
    -Werror
  )

  target_compile_options(${name}_help PRIVATE
    -Wall
    -Werror
  )
endfunction(add_example)

add_example(enum)
add_example(optional)
add_example(variant)

add_example(final)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
