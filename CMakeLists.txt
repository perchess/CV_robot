cmake_minimum_required(VERSION 3.0.0)

project(lesson1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(OpenCV REQUIRED)
#find_package(catkin REQUIRED COMPONENTS
#  OpenCV
#)

add_executable(${PROJECT_NAME} "main.cpp" "my_robot/my_robot")

include_directories(my_robot ${OpenCV_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} Qt5::Core ${OpenCV_LIBS})
