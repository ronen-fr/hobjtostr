cmake_minimum_required(VERSION 3.22)


set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_EXTENSIONS OFF)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


project(hobjperf CXX)

#dd_compile_options(-std=c++20)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -O3 -Wall -fomit-frame-pointer -I/home/rfriedma/apps/fmt/include -L/home/rfriedma/apps/fmt /home/rfriedma/apps/fmt/libfmt.a")
find_package(benchmark REQUIRED)



set(SOURCE hobject.cc h.cc)


add_executable(${PROJECT_NAME} ${SOURCE})
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(hobjperf benchmark::benchmark /home/rfriedma/apps/fmt/libfmt.a)


