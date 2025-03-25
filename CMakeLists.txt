cmake_minimum_required(VERSION 3.10)
project(SlotMachine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(USE_LOCAL_SFML "Use local SFML installation" OFF)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/SFML/Graphics.hpp")
    set(USE_LOCAL_SFML ON)
    unset(CMAKE_TOOLCHAIN_FILE CACHE)
    unset(VCPKG_ROOT CACHE)
endif()

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(USE_LOCAL_SFML)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include/SFML/Graphics.hpp")
        message(FATAL_ERROR "Local SFML not found in external/SFML")
    endif()

    target_include_directories(${PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/include
    )
    
    if(WIN32)
        set(SFML_COMPONENTS graphics window audio network system)
        
        set(SFML_DEBUG_SUFFIX "-d")
        
        foreach(COMPONENT ${SFML_COMPONENTS})
            target_link_libraries(${PROJECT_NAME} PRIVATE 
                optimized "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/lib/sfml-${COMPONENT}.lib"
                debug "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/lib/sfml-${COMPONENT}${SFML_DEBUG_SUFFIX}.lib"
            )
        endforeach()
        
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/bin"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    else()
        set(SFML_COMPONENTS graphics window audio network system)
        
        foreach(COMPONENT ${SFML_COMPONENTS})
            target_link_libraries(${PROJECT_NAME} PRIVATE 
                "${CMAKE_CURRENT_SOURCE_DIR}/external/SFML/lib/libsfml-${COMPONENT}.so"
            )
        endforeach()
    endif()
else()
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
        message(FATAL_ERROR "vcpkg not found! Please run build_smart.bat to install it.")
    endif()
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")
    set(VCPKG_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg")
    
    find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System)
endif()

foreach(RESOURCE_DIR fonts)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/${RESOURCE_DIR}
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${RESOURCE_DIR}
    )
endforeach()

message(STATUS "Project ${PROJECT_NAME} configured successfully")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Resources will be copied to: $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts") 