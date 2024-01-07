cmake_minimum_required(VERSION 3.5)

# set the project name and version
project(ZqPubSub VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add the executable
add_executable(ZqPubSub src/main.cpp)

target_link_libraries(ZqPubSub PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
target_include_directories(ZqPubSub PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

