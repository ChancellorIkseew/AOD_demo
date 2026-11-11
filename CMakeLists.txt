cmake_minimum_required(VERSION 3.20)
project(AoD_Demo)
#--------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
#--------------------------------------
set(CMAKE_INSTALL_RPATH "$ORIGIN")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
#--------------------------------------
#-----find_sources---------------------
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp" "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/include"
)
#------OS_settings---------------------
if(WIN32)
    set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/MSVC_lib")
    target_link_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE "${LIBS_DIR}/SDL3.lib")
elseif(UNIX AND NOT APPLE)
    set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/GCC_so")
    target_link_directories(${PROJECT_NAME} PRIVATE ${LIBS_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE "${LIBS_DIR}/libSDL3.so.0")
endif()
