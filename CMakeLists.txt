cmake_minimum_required( VERSION 3.5 )
project(test CXX)

IF (WIN32)
    set(MY_LIBRARY_PATH "C:/Users/Owner/Library/etc/lib")
    include_directories("C:/Users/Owner/Library/etc/include")
    
    find_package(OpenGL REQUIRED)
    link_libraries(${OpenGL_LIBRARIES} )
    
    set(OpenCV_DIR "C:/Users/Owner/Library/Opencv/opencv-3.4/build_gpp")
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        include_directories(${OpenCV_INCLUDE_DIRS})
        link_libraries(${OpenCV_LIBS})
    endif()
    
    set(CMAKE_CXX_FLAGS "-O2 -std=c++20 -Wall -Wsign-compare")
    
    set(FREETYPE_DIR "C:/Users/Owner/Library/freetype-2.10.4")
    include_directories(${FREETYPE_DIR}/include)
    
    find_package(OpenGL REQUIRED)
    link_libraries(${OpenGL_LIBRARIES} )
    
    # file(GLOB_RECURSE src_glui "${CMAKE_CURRENT_SOURCE_DIR}/glUI/*.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/glUI/*.cpp" main.cpp)
    
ELSE()
    set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
    set(CMAKE_CXX_FLAGS "-O2 -std=c++2a -Wall -Wsign-compare  -L /lib64")
    # set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
    # don't read CMAKE_BUILD_TYPE, it has no meaning with multiconfig
    # generators
    # set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/build")
    
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(GLEW 2.0 REQUIRED)
    
    # include_directories(${OPENGL_INCLUDE_DIR})
    
    link_libraries(${OPENGL_gl_LIBRARY} glfw)
    
    # add_custom_command(TARGET LearnGLSL POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_directory
    #     ${CMAKE_SOURCE_DIR}/assets
    #     ${PROJECT_BINARY_DIR}
    #     COMMENT "Copy resources to build tree")
    
    link_libraries(OpenGL::GL GLEW::GLEW)

    # For OpenCV
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        include_directories(${OpenCV_INCLUDE_DIRS})
        link_libraries(${OpenCV_LIBS})
    endif()

    # Freetype
    find_package(Freetype REQUIRED)
    # SET(CMAKE_EXE_LINKER_FLAGS "-v")
    link_libraries(${FREETYPE_LIBRARIES})
    include_directories(${FREETYPE_INCLUDE_DIRS})

ENDIF()


set(GLUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/glUI/glUI.cpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/widget.cpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/renderer.cpp")
    
set(GLUI_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/glUI/glUI.hpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/widget.hpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/common.hpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/logger.hpp" 
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/renderer.hpp"
                 "${CMAKE_CURRENT_SOURCE_DIR}/glUI/MessageBox.hpp")

file(COPY misc/Meiryo.ttf misc/cat.raw DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY misc/Meiryo.ttf misc/cat.raw DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)

add_subdirectory(test)


