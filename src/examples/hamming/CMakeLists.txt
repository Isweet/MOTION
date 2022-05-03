add_executable(hamming hamming.cpp common/hamming.cpp)

if (NOT MOTION_BUILD_BOOST_FROM_SOURCES)
    find_package(Boost
            COMPONENTS
            program_options
            REQUIRED)
endif ()

target_link_libraries(hamming
        MOTION::motion
        Boost::program_options
        )
