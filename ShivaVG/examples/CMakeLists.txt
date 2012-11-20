cmake_minimum_required(VERSION 2.8)
# TODO headers as dependencies
# TODO images as dependencies
include_directories(${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})

set(EXAMPLE_SRCS test.c)
set(EXAMPLE_HRRS test.h)
set(EXAMPLE_LIBS ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
set(EXAMPLE_tiger_SRC test_tiger_paths.c)
set(EXAMPLE_image_LIBS ${JPEG_LIBRARIES})
set(EXAMPLE_pattern_LIBS ${JPEG_LIBRARIES})

foreach (example vgu dash linear radial interpolate tiger image pattern blend eb)
    string(TOUPPER ${example} EXAMPLE)
    if(BUILD_${EXAMPLE})
        add_executable(test_${example} ${EXAMPLE_SRCS} test_${example}.c ${EXAMPLE_${example}_SRC})
        target_link_libraries(test_${example} ${EXAMPLE_LIBS} OpenVG ${EXAMPLE_${example}_LIBS})
    endif()
endforeach()