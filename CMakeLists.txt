cmake_minimum_required(VERSION 3.1.0)

set(PROJECT_NAME "SFMLSandbox")
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

include_directories("third_party")

file(
    GLOB_RECURSE
    SOURCE_FILES
    "src/*.cpp"
    "include/*.h*"
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

target_link_libraries(
    ${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
)
