add_library(motionffi SHARED
        motion.h
        motion.cpp
        )

target_link_libraries(motionffi PRIVATE
       MOTION::motion)
