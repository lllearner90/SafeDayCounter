cmake_minimum_required(VERSION 3.20)

project("Software" C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/st-project.cmake)
include(cmake/logger.cmake)

include_directories(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/App"
    "${PROJECT_SOURCE_DIR}/App/Display"
    "${PROJECT_SOURCE_DIR}/App/Display/driver"
    "${PROJECT_SOURCE_DIR}/App/Calendar"

    "${PROJECT_SOURCE_DIR}/App/Utility"

    "${PROJECT_SOURCE_DIR}/App/Config"
)

add_executable(${PROJECT_NAME}
    "App/safe_days.cpp"
    "App/Display/display.cpp"
    "App/Display/driver/TLC591x.cpp"
    "App/Calendar/calendar_stm32.cpp"
    "App/Calendar/calendar.cpp"

    "App/Utility/bcd.cpp"
    "App/Utility/conv.cpp"

    "App/Config/config_manager.cpp"
)

add_st_target_properties(${PROJECT_NAME})
add_logger_target_properties(${PROJECT_NAME})
