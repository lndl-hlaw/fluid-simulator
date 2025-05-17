cmake_minimum_required(VERSION 3.10)
project(FluidSimulator LANGUAGES CUDA C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate build symbols for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add GLAD library form lib
add_library(GLAD dependencies/lib/glad.c)
target_include_directories(GLAD PUBLIC dependencies/include)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


file(GLOB_RECURSE CUDA_SOURCES "src/*.cu")
file(GLOB_RECURSE CUDA_HEADERS "src/*.cuh")
file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")
file(GLOB_RECURSE CPP_HEADERS "src/*.hpp" "src/*.h")

add_executable(fluid_simulator ${CUDA_SOURCES} ${CUDA_HEADERS} ${CPP_SOURCES} ${CPP_HEADERS})

target_include_directories(fluid_simulator PRIVATE ${GLFW_INCLUDE_DIRS})

target_link_libraries(fluid_simulator
    GLAD
    ${GLFW_LIBRARIES}
    cudart
    cuda
)

# set_target_properties(fluid_simulator PROPERTIES
#     CUDA_SEPARABLE_COMPILATION ON
#     # CUDA_ARCHITECTURES 60 70 75 80
# )

# On Linux, also link system libraries for OpenGL
if(UNIX)
    target_link_libraries(fluid_simulator dl GL X11 pthread Xrandr Xi Xxf86vm Xcursor)
endif()
