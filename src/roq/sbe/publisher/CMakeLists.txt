set(TARGET_NAME ${PROJECT_NAME})

add_subdirectory(flags)

set(SOURCES
    application.cpp
    base.cpp
    config.cpp
    controller.cpp
    incremental.cpp
    instrument.cpp
    settings.cpp
    shared.cpp
    snapshot.cpp
    main.cpp)

add_executable(${TARGET_NAME} ${SOURCES})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-flags-autogen-headers)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE ${TARGET_NAME}-flags
          roq-client::roq-client
          roq-client::roq-client-flags
          roq-io::roq-io
          roq-codec::roq-codec
          roq-cache::roq-cache
          roq-logging::roq-logging
          roq-logging::roq-logging-flags
          roq-flags::roq-flags
          roq-api::roq-api
          unordered_dense::unordered_dense
          fmt::fmt)

if(ROQ_BUILD_TYPE STREQUAL "Release")
  set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
endif()

target_compile_definitions(${TARGET_NAME} PRIVATE ROQ_PACKAGE_NAME="${PROJECT_NAME}")

install(TARGETS ${TARGET_NAME})

if(BUILD_DOCS)

  install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME}
    FILES_MATCHING
    PATTERN "*.rst*"
    PATTERN "CMakeFiles" EXCLUDE)

endif()
