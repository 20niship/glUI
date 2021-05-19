add_executable(log_test ../glUI/logger.hpp logger_test.cpp)
add_executable(main_test ${GLUI_SOURCES} ${GLUI_HEADERS} main_test.cpp customWidget.cpp)
add_executable(drawlist_test ${GLUI_SOURCES} ${GLUI_HEADERS} drawlist_test.cpp)
add_executable(widget_test ${GLUI_SOURCES} ${GLUI_HEADERS} widget_test.cpp)

#target_link_libraries( test glew32 opengl32 glfw3dll)

if(WIN32)
    target_link_libraries( main_test ${MY_LIBRARY_PATH}/libglew32.a opengl32 ${MY_LIBRARY_PATH}/libglfw3dll.a)
    target_link_libraries( main_test ${FREETYPE_DIR}/build/libfreetype.a ${OpenCV_LIBRARIES} ${OpenCV_LIBRARIES})

    target_link_libraries( drawlist_test ${MY_LIBRARY_PATH}/libglew32.a opengl32 ${MY_LIBRARY_PATH}/libglfw3dll.a)
    target_link_libraries( drawlist_test ${FREETYPE_DIR}/build/libfreetype.a ${OpenCV_LIBRARIES} ${OpenCV_LIBRARIES})

    target_link_libraries( widget_test ${MY_LIBRARY_PATH}/libglew32.a opengl32 ${MY_LIBRARY_PATH}/libglfw3dll.a)
    target_link_libraries( widget_test ${FREETYPE_DIR}/build/libfreetype.a ${OpenCV_LIBRARIES} ${OpenCV_LIBRARIES})
endif()

