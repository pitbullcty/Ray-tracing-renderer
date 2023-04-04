cmake_minimum_required (VERSION 3.8)

project (Ray_tracing_renderer)

set(CMAKE_PREFIX_PATH "D:/QT/6.2.4/msvc2019_64;")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

find_package(Qt6 COMPONENTS Core)
find_package(Qt6 COMPONENTS Gui)
find_package(Qt6 COMPONENTS Widgets)
find_package(Qt6 COMPONENTS OpenGL)
find_package(Qt6 COMPONENTS OpenGLWidgets)
find_package(Qt6 COMPONENTS Concurrent)
find_package(ASSIMP MODULE REQUIRED)
find_package(OIDN MODULE REQUIRED)
find_package(OpenMP REQUIRED)


file(GLOB_RECURSE src_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE inc_files ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)
file(GLOB_RECURSE gizmo_src_files ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libgizmo/*.cpp)
file(GLOB_RECURSE gizmo_inc_files ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/libgizmo/*.h)

include_directories(${ASSIMP_INCLUDE_DIR} ${OIDN_INCLUDE_DIR})

qt_add_resources(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources/shader/shader.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/theme/theme.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/theme/messagebox.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/fonts/font.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/icons.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/translator/translator.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/lights/lights.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/docs/docs.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/skybox/cubemap/Standard-Cube-Map1/skybox.qrc
)

add_executable(${PROJECT_NAME} 
        WIN32
        ${inc_files} 
        ${src_files} 
        ${gizmo_inc_files} 
        ${gizmo_src_files} 
        ${SOURCES}
)


target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core
    Qt::Gui
    Qt::OpenGL
    Qt::OpenGLWidgets
    Qt::Widgets
    Qt6::Concurrent
    OpenMP::OpenMP_CXX
    ${OIDN_LIBRARIES}
    optimized 	${ASSIMP_LIBRARY_RELEASE}
	debug		${ASSIMP_LIBRARY_DEBUG}
)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/dlls/assimp-${ASSIMP_MSVC_VERSION}-mtd.dll 	${CMAKE_BINARY_DIR}
      COMMAND_EXPAND_LISTS
     )
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}>
      COMMAND ${CMAKE_COMMAND} -E copy ${ASSIMP_ROOT_DIR}/dlls/assimp-${ASSIMP_MSVC_VERSION}-mt.dll  ${CMAKE_BINARY_DIR}
      COMMAND_EXPAND_LISTS
     )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${OIDN_LIB_DIR}/OpenImageDenoise.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy ${OIDN_LIB_DIR}/tbb.dll ${CMAKE_CURRENT_BINARY_DIR}
)
