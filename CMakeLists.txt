cmake_minimum_required (VERSION 3.8)

project (Ray_tracing_renderer)

set(CMAKE_PREFIX_PATH "D:/QT/6.2.4/msvc2019_64")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON) 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Widgets)

file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE inc_files ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h)
file(GLOB_RECURSE ui_files ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/*.ui)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

add_executable(${PROJECT_NAME} ${inc_files} ${src_files} ${ui_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core)


