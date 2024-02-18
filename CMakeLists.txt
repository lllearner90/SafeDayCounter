cmake_minimum_required(VERSION 3.20)

project("Software" C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/st-project.cmake)
# include(cmake/logger.cmake)

include_directories(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/App"
    "${PROJECT_SOURCE_DIR}/App/Display"
    "${PROJECT_SOURCE_DIR}/App/Display/driver"
    "${PROJECT_SOURCE_DIR}/App/Calendar"

    "${PROJECT_SOURCE_DIR}/App/Utility"

    "${PROJECT_SOURCE_DIR}/App/Config"
    "${PROJECT_SOURCE_DIR}/App/Logger"
)

add_executable(${PROJECT_NAME}
)

# add_logger_target_properties(${PROJECT_NAME})
add_st_target_properties(${PROJECT_NAME})

