set(TARGET_NAME ${PROJECT_NAME}-codec)

set(SOURCES decoder.cpp encoder.cpp)

add_library(${TARGET_NAME} OBJECT ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE roq-api::roq-api fmt::fmt)
