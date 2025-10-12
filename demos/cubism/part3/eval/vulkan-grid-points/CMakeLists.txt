cmake_minimum_required(VERSION 3.10)
project(VulkanCube)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# Add GLM
find_package(glm REQUIRED)

# Source files
set(SOURCES
    main.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    Vulkan::Vulkan
    glfw
    glm::glm
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${Vulkan_INCLUDE_DIRS}
)

# Compile shaders
find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/bin)

if(GLSLC)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
        COMMAND ${GLSLC} ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert -o ${CMAKE_CURRENT_BINARY_DIR}/vert.spv
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shader.vert
        COMMENT "Compiling vertex shader"
    )
    
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
        COMMAND ${GLSLC} ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag -o ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/shader.frag
        COMMENT "Compiling fragment shader"
    )
    
    add_custom_target(shaders ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/vert.spv ${CMAKE_CURRENT_BINARY_DIR}/frag.spv
    )
    
    add_dependencies(${PROJECT_NAME} shaders)
else()
    message(WARNING "glslc not found - shaders will not be compiled automatically")
endif()
