cmake_minimum_required(VERSION 3.26)
project(MyOpenGL)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyOpenGL src/main.cpp)
include_directories(/Users/summerbaby/Desktop/summmbaby/myOpenGL/dependence/glfw/include /usr/local/include)

link_directories(/Users/summerbaby/Desktop/summmbaby/myOpenGL/dependence/glfw)

find_library(GLFW_LIBRARY glfw3 NAMES glfw3 glfw PATHS /Users/summerbaby/Desktop/summmbaby/myOpenGL/dependence/glfw)
find_library(GLEW_LIBRARY glew NAMES glew PATHS /usr/local/lib)
find_library(COCOA_LIBRARY Cocoa)
find_library(OPENGL_LIBRARY OpenGL)
find_library(IOKIT_LIBRARY IOKit)
find_library(COREVIDEO_LIBRARY CoreVideo)

target_link_libraries(MyOpenGL ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${COCOA_LIBRARY} ${OPENGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
