add_library(physical_object physical_object.cpp)
add_library(world world.cpp)

target_link_libraries (physical_object
    Boost::system
)
target_link_libraries (world
    Boost::system
)
