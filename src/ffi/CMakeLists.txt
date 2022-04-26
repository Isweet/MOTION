set(CMAKE_MACOSX_RPATH 1)
set(CMAKE_BUILD_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

add_library(motionffi SHARED
        motion.h
        motion.cpp
        )

target_link_libraries(motionffi PRIVATE
       MOTION::motion)

install(TARGETS motionffi DESTINATION lib)
install(FILES motion.h DESTINATION include)
