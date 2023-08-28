set(TARGET_NAME ${PROJECT_NAME}-flags)

include(RoqAutogen)

set(AUTOGEN_SCHEMAS flags.json)

roq_autogen(
  OUTPUT
  AUTOGEN_HEADERS
  NAMESPACE
  "roq/sbe/publisher/flags"
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
  "roq/sbe/publisher/flags"
  OUTPUT_TYPE
  "state"
  FILE_TYPE
  "cpp"
  SOURCES
  ${AUTOGEN_SCHEMAS})

roq_gitignore(OUTPUT .gitignore SOURCES ${TARGET_NAME} ${AUTOGEN_HEADERS} ${AUTOGEN_SOURCES})

add_library(${TARGET_NAME} OBJECT ${SOURCES} ${AUTOGEN_SOURCES})

add_dependencies(${TARGET_NAME} ${TARGET_NAME}-autogen-headers)

if(APPLE)
  target_compile_definitions(${TARGET_NAME} PRIVATE FMT_USE_NONTYPE_TEMPLATE_ARGS=1)
endif()

target_link_libraries(${TARGET_NAME} absl::flags)
