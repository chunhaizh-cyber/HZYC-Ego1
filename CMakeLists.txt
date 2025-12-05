cmake_minimum_required(VERSION 3.10)
project(DigitalLifeVisionSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 简化版本，使用基本的C++标准库
# 由于环境限制，我们创建一个不需要外部依赖的演示版本

# Source files
set(SOURCES
    main.cpp
    Vector3D.cpp
    ExistenceFeature.cpp
    PointCloudProcessor.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link basic libraries
target_link_libraries(${PROJECT_NAME} 
    pthread
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)