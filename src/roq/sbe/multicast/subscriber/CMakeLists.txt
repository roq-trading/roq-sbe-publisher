set(TARGET_NAME ${PROJECT_NAME}-subscriber)

add_subdirectory(flags)

add_executable(${TARGET_NAME} application.cpp config.cpp controller.cpp settings.cpp main.cpp)

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-flags-autogen-headers)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE ${TARGET_NAME}-flags
          roq-io::roq-io
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          fmt::fmt
          ${RT_LIBRARIES})

target_compile_definitions(${TARGET_NAME} PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

if(APPLE)
  target_compile_definitions(${TARGET_NAME} PRIVATE FMT_USE_NONTYPE_TEMPLATE_ARGS=1)
endif()

install(TARGETS ${TARGET_NAME})
