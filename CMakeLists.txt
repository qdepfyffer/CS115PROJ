cmake_minimum_required(VERSION 3.30)
project(CSCI115_PROJ)
set(CMAKE_CXX_STANDARD 26)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE ALL_SOURCES "${SRC_DIR}/*.cpp")

set(SOURCES "")
foreach(src ${ALL_SOURCES})
    get_filename_component(fname "${src}" NAME)
    string(SUBSTRING "${fname}" 0 1 first_char)
    if(NOT first_char STREQUAL ".")
        list(APPEND SOURCES "${src}")
    endif()
endforeach()

set(OUTPUT_DIR ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})

add_executable(CSCI115_PROJ ${SOURCES})

if(CMAKE_CONFIGURATION_TYPES)
    foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
        set(CONFIG_OUTPUT_DIR ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG})
        add_custom_command(TARGET CSCI115_PROJ POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_SOURCE_DIR}/maps"
                "${CONFIG_OUTPUT_DIR}/maps"
                COMMENT "Copying maps folder to ${CONFIG_OUTPUT_DIR}/maps for ${OUTPUTCONFIG}")
    endforeach()
else()
    add_custom_command(TARGET CSCI115_PROJ POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/maps"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/maps"
            COMMENT "Copying maps folder to ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/maps")
endif()