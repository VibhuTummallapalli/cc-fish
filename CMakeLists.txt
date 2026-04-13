cmake_minimum_required(VERSION 3.20)
project(Fish VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific settings
if(WIN32)
    add_definitions(-DUNICODE -D_UNICODE -DWIN32_LEAN_AND_MEAN -DNOMINMAX)
endif()

# Find JNI
find_package(JNI REQUIRED)

# Source files
set(SOURCES
    java/dllmain_file.cpp
    java/java/java.cpp
)

# Header files (for IDE)
file(GLOB_RECURSE HEADERS
    "java/*.h"
)

# Create DLL
add_library(Fish SHARED ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(Fish PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/java
    ${CMAKE_CURRENT_SOURCE_DIR}/java/java
    ${CMAKE_CURRENT_SOURCE_DIR}/java/java/internal
    ${CMAKE_CURRENT_SOURCE_DIR}/java/main
    ${JNI_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(Fish PRIVATE
    ${JNI_LIBRARIES}
)

# Compiler-specific options
if(MSVC)
    target_compile_options(Fish PRIVATE
        /W4           # Warning level 4
        /WX-          # Don't treat warnings as errors (for now)
        /MP           # Multi-processor compilation
        /Zi           # Debug information
        /EHsc         # Exception handling
    )
    
    # Linker options
    target_link_options(Fish PRIVATE
        /DEBUG        # Generate debug info
    )
else()
    target_compile_options(Fish PRIVATE
        -Wall -Wextra -Wpedantic
    )
endif()

# Output directory
set_target_properties(Fish PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Create injector executable
add_executable(FishInjector java/injector/injector.cpp)

target_compile_definitions(FishInjector PRIVATE
    UNICODE _UNICODE WIN32_LEAN_AND_MEAN
)

if(MSVC)
    target_compile_options(FishInjector PRIVATE /W4 /WX- /MP)
endif()

set_target_properties(FishInjector PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Print configuration info
message(STATUS "===========================================")
message(STATUS "Fish - Minecraft Auto-Fisher")
message(STATUS "===========================================")
message(STATUS "JNI Include: ${JNI_INCLUDE_DIRS}")
message(STATUS "JNI Libraries: ${JNI_LIBRARIES}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "===========================================")
