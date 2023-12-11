function (add_logger_target_properties TARGET_NAME)

target_include_directories(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/App/Logger/"
    "${PROJECT_SOURCE_DIR}/App/Logger/repo/easylogger/inc/"
)

target_link_libraries(
    ${TARGET_NAME} PRIVATE
)

target_link_directories(
    ${TARGET_NAME} PRIVATE
)

target_sources(
    ${TARGET_NAME} PRIVATE
    "${PROJECT_SOURCE_DIR}/App/Logger/repo/easylogger/src/elog_async.c"
    "${PROJECT_SOURCE_DIR}/App/Logger/repo/easylogger/src/elog_buf.c"
    "${PROJECT_SOURCE_DIR}/App/Logger/repo/easylogger/src/elog_utils.c"
    "${PROJECT_SOURCE_DIR}/App/Logger/repo/easylogger/src/elog.c"
    "${PROJECT_SOURCE_DIR}/App/Logger/elog_port.c"
)

endfunction()