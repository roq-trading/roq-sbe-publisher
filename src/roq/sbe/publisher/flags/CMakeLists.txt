set(TARGET_NAME ${PROJECT_NAME}-flags)

include(RoqAutogen)

set(NAMESPACE "roq/sbe/publisher/flags")

set(AUTOGEN_SCHEMAS flags.json)

roq_autodoc(
  OUTPUT
  AUTOGEN_DOC
  NAMESPACE
  ${NAMESPACE}
  OUTPUT_TYPE
  "doc"
  FILE_TYPE
  "rstinc"
  SOURCES
  ${AUTOGEN_SCHEMAS})

roq_autogen(
  OUTPUT
  AUTOGEN_HEADERS
  NAMESPACE
  ${NAMESPACE}
  OUTPUT_TYPE
  "state"
  FILE_TYPE
  "hpp"
  SOURCES
  ${AUTOGEN_SCHEMAS})

add_custom_target(${TARGET_NAME}-autogen-headers ALL DEPENDS ${AUTOGEN_HEADERS})

roq_autogen(
  OUTPUT
  AUTOGEN_SOURCES
  NAMESPACE
  ${NAMESPACE}
  OUTPUT_TYPE
  "state"
  FILE_TYPE
  "cpp"
  SOURCES
  ${AUTOGEN_SCHEMAS})

roq_gitignore(OUTPUT .gitignore SOURCES ${TARGET_NAME} ${AUTOGEN_HEADERS} ${AUTOGEN_SOURCES})

add_library(${TARGET_NAME} OBJECT ${SOURCES} ${AUTOGEN_SOURCES})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-autogen-headers)

target_link_libraries(${TARGET_NAME} absl::flags)

# install

set(TARGET_DIR ${CMAKE_INSTALL_DATADIR}/doc/${PROJECT_NAME})

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${TARGET_DIR}/rst
  FILES_MATCHING
  PATTERN "*.rstinc"
  PATTERN "CMakeFiles" EXCLUDE)
