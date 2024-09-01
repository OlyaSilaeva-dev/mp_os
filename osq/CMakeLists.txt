cmake_minimum_required(VERSION 3.21)
project(mp_os_osq)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        mp_os_osq
        mp_os_osq.cpp
        src/collection.cpp src/scheme.cpp include/scheme.h src/pull.cpp include/pull.h src/client.cpp include/client.h)

target_include_directories(
        mp_os_osq
        PUBLIC
        ../allocator
        ../logger
        ../associative_container
        ../associative_container/search_tree/indexing_tree/b_tree
        ./include
        ./src
)

target_link_libraries(
        mp_os_osq
        PUBLIC
        mp_os_cmmn
        mp_os_lggr_clnt_lggr
        mp_os_allctr_allctr
        mp_os_allctr_allctr_glbl_hp
        mp_os_assctv_cntnr_srch_tr_indxng_tr_b_tr
)