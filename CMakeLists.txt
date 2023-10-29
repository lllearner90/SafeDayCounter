cmake_minimum_required(VERSION 3.20)

project("Software" C CXX ASM)

include(cmake/st-project.cmake)

include_directories(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/App"
    "${PROJECT_SOURCE_DIR}/App/Display"
    "${PROJECT_SOURCE_DIR}/App/Display/driver"
    "${PROJECT_SOURCE_DIR}/App/Calendar"

    "${PROJECT_SOURCE_DIR}/App/Utility"
)

add_executable(${PROJECT_NAME}
    "App/safe_days.cpp"
    "App/Display/display.cpp"
    "App/Display/driver/TLC591x.cpp"
    "App/Calendar/calendar_stm32.cpp"
    "App/Calendar/calendar.cpp"

    "App/Utility/bcd.cpp"
)

add_st_target_properties(${PROJECT_NAME})
