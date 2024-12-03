# 최소 CMake 버전 설정
cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름 및 사용 언어 설정
project(OpenGL_Project LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenGL 라이브러리 찾기 (macOS에서는 기본 제공)
find_package(OpenGL REQUIRED)

# GLAD 파일 포함
set(GLAD_INCLUDE_DIR "/Users/sihkang/openGL/glad/include")
set(GLAD_SOURCE "/Users/sihkang/openGL/glad/src/glad.c")

# GLFW 경로 설정
set(GLFW_INCLUDE_DIR "/Users/sihkang/openGL/glfw/include")
set(GLFW_LIB_DIR "/Users/sihkang/openGL/glfw/lib-universal")

# GLM 설정
set(GLM_LIB_DIR "/Users/sihkang/openGL/glm")
set(GLM_INCLUDE_DIR "/Users/sihkang/openGL/glm")

# 프로젝트의 소스 파일 추가
add_executable(OpenGLApp ${GLAD_SOURCE}
            src/main.cpp 
            src/common.cpp src/common.h
            )

if (APPLE)
    # macOS에서 필요한 프레임워크 찾기
    find_library(COCOA_FRAMEWORK Cocoa)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    
    # 각 라이브러리가 제대로 찾혔는지 확인
    if (NOT COCOA_FRAMEWORK)
        message(FATAL_ERROR "Cocoa framework not found")
    endif()
    if (NOT IOKIT_FRAMEWORK)
        message(FATAL_ERROR "IOKit framework not found")
    endif()
    if (NOT COREFOUNDATION_FRAMEWORK)
        message(FATAL_ERROR "CoreFoundation framework not found")
    endif()
endif()

# include 경로 추가
target_include_directories(OpenGLApp PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
)

# glad 라이브러리와 GLFW, OpenGL 링크
target_link_libraries(OpenGLApp PRIVATE
    ${COCOA_FRAMEWORK}
    ${IOKIT_FRAMEWORK}
    ${COREFOUNDATION_FRAMEWORK}
    OpenGL::GL
    ${GLFW_LIB_DIR}/libglfw3.a
)
