cmake_minimum_required(VERSION 3.28.3)

project(netx32)

set(SRC ./src/constants.cpp)

add_executable(${PROJECT_NAME}_s ./src/server.cpp ${SRC})
add_executable(${PROJECT_NAME}_c ./src/client.cpp ${SRC})

target_link_libraries(${PROJECT_NAME}_s PRIVATE ws2_32)
target_link_libraries(${PROJECT_NAME}_c PRIVATE ws2_32)