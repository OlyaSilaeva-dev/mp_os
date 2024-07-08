cmake_minimum_required(VERSION 3.21)
project(mp_os_main)

set(CMAKE_CXX_STANDARD 14)

add_executable(mp_os_main olya_main.cpp)

target_link_libraries(
        mp_os_main PUBLIC
        mp_os_lggr_lggr mp_os_lggr_clnt_lggr
)