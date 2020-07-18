cmake_minimum_required(VERSION 3.16)
project(Timber)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

find_package(SFML COMPONENTS System Audio Graphics Network Window REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Sprite.cpp Sound.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARY})

file(GLOB DLLS ${SFML_LIBRARY_DIR}/*.dll)
file(COPY ${DLLS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY asset DESTINATION ${CMAKE_CURRENT_BINARY_DIR})