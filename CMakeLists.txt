cmake_minimum_required(VERSION 3.6)
project(seatrac_simple_pinger_example VERSION 0.1)

# find_package(seatrac_driver REQUIRED)
if(NOT TARGET seatrac_driver)
    find_package(seatrac_driver QUIET)
    if(NOT TARGET seatrac_driver)
        include(FetchContent)
        FetchContent_Declare(seatrac_driver
            GIT_REPOSITORY https://Clayton314@bitbucket.org/frostlab/seatrac_driver.git
            GIT_TAG        main
        )
        FetchContent_MakeAvailable(seatrac_driver)
    endif()
endif()

add_executable(seatrac_logger
    src/seatrac_logger.cpp
)
target_link_libraries(seatrac_logger PRIVATE seatrac_driver)
