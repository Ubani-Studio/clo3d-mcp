#--------------------------------------------------------------------------------
# Copyright 2009-2025 CLO Virtual Fashion. All rights reserved.
# Sample CMakeLists.txt file for CloEventAPI_Plugin

#--------------------------------------------------------------------------------
# Minimum CMake version
cmake_minimum_required(VERSION 3.20)

#--------------------------------------------------------------------------------
# Options
if (APPLE)
    option(USE_HOMEBREW OFF)
    message("-- Option (USE_HOMEBREW): ${USE_HOMEBREW}")

    if (APPLE AND USE_HOMEBREW)
        execute_process(COMMAND bash -c "echo $(brew --prefix)" OUTPUT_VARIABLE HOMEBREW_PREFIX_RAW)
        string(STRIP ${HOMEBREW_PREFIX_RAW} HOMEBREW_PREFIX)
    endif()

    OPTION(DEPLOYMENT_BUILD "DEPLOYMENT_BUILD Option" ON)
    message("-- Option (DEPLOYMENT_BUILD): ${DEPLOYMENT_BUILD}")

endif()

#--------------------------------------------------------------------------------
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

cmake_host_system_information(RESULT NUM_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
set(CMAKE_AUTOGEN_PARALLEL ${NUM_LOGICAL_CORES})
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#--------------------------------------------------------------------------------
# Target definitions
message("\n>> Copyright 2009-2025 CLO Virtual Fashion. All rights reserved.")
message("\n>> Configuring Sample Project for CloEventAPIPlugin\n")

project(
    CloEventAPI_Plugin
    DESCRIPTION "Sample CLO Event Plugin for CLO"
    VERSION 2024.2
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} SHARED "")

#--------------------------------------------------------------------------------
# Compiler settings
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

#--------------------------------------------------------------------------------
# Runtime library options
if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
    )
endif()

#--------------------------------------------------------------------------------
# Preprocessor definitions
if(APPLE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        CLO_API_INTERFACE_STATIC_BUILD
    )
elseif(MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        UNICODE
        WIN32
        NDEBUG
        CLO_EVENT_API_PLUGIN
        CLO_SCENE_LIB_STATIC
    )
endif()

#--------------------------------------------------------------------------------
# macOS specific settings
if(APPLE)
    # Deployment target
    set(CMAKE_OSX_DEPLOYMENT_TARGET 12.0) # Monterey
    message("-- macOS deployment target: ${CMAKE_OSX_DEPLOYMENT_TARGET}")
    # Target architectures
    if ("${CMAKE_OSX_ARCHITECTURES}" STREQUAL "")
        execute_process(COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE osx_architecture)
        set(CMAKE_OSX_ARCHITECTURES ${osx_architecture})
    elseif( NOT ((${CMAKE_OSX_ARCHITECTURES} MATCHES "x86_64") OR (${CMAKE_OSX_ARCHITECTURES} MATCHES "arm64")) )
        message(FATAL_ERROR "Target architecture not supported: ${CMAKE_OSX_ARCHITECTURES}")
    endif()
    message("-- Target macOS architecture (${PROJECT_NAME}): ${CMAKE_OSX_ARCHITECTURES}")
endif()

#--------------------------------------------------------------------------------
# Source
if (MSVC)
    target_sources(${PROJECT_NAME} PRIVATE
        dllmain.cpp
    )
endif()

target_sources(${PROJECT_NAME} PRIVATE
    CloEventPlugin.cpp
    CloEventPlugin.h
)

#--------------------------------------------------------------------------------
# CLOAPIInterface
find_library(CLO_API_INTERFACE NAMES CLOAPIInterface PATHS ../../CLOAPIInterface/Lib REQUIRED)

if (MSVC)
    find_file(CLO_API_INTERFACE_DLL NAMES CLOAPIInterface.dll PATHS ../../CLOAPIInterface/Lib REQUIRED)
    list(APPEND STATIC_LIBRARIES ${CLO_API_INTERFACE})
    list(APPEND SHARED_LIBRARIES ${CLO_API_INTERFACE_DLL})
elseif (APPLE)
    list(APPEND SHARED_LIBRARIES ${CLO_API_INTERFACE})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ../../CLOAPIInterface
)

#--------------------------------------------------------------------------------
if(APPLE)
    # AGL
    find_library(AGL_LIBRARY AGL REQUIRED)
    list(APPEND SYSTEM_FRAMEWORKS ${AGL_LIBRARY})

    # Cocoa
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    list(APPEND SYSTEM_FRAMEWORKS ${COCOA_LIBRARY})
endif()

#--------------------------------------------------------------------------------
# Linking
if (MSVC)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${STATIC_LIBRARIES}
    )
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SHARED_LIBRARIES}
        ${SYSTEM_FRAMEWORKS}
    )
endif()

#--------------------------------------------------------------------------------
# Post build events

# [macOS] Ensure to adjust install names to use the same libraries as the host app
if (APPLE)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
            \"@rpath/libCLOAPIInterface.dylib\"
            \"@executable_path/../Frameworks/libCLOAPIInterface.dylib\"
            "lib${PROJECT_NAME}.dylib"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    if (DEPLOYMENT_BUILD)
        add_custom_command(
            TARGET ${PROJECT_NAME}
            POST_BUILD
            COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
                ${WIDGETS_INSTALL_NAME}
                \"lib${PROJECT_NAME}.dylib\"
            COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
                ${GUI_INSTALL_NAME}
                "lib${PROJECT_NAME}.dylib"
            COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change
                ${CORE_INSTALL_NAME}
                "lib${PROJECT_NAME}.dylib"
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()

endif()