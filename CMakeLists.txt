cmake_minimum_required(VERSION 3.20)
project(mathgame)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_subdirectory(external/enet EXCLUDE_FROM_ALL)

add_executable(client WIN32
  client/src/main.cpp
)

add_executable(server WIN32
  server/src/main.cpp
)

target_link_libraries(client PRIVATE enet)
target_link_libraries(server PRIVATE enet)
