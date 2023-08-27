set(TARGET_NAME ${PROJECT_NAME}-test)

set(SOURCES market_status.cpp reference_data.cpp top_of_book.cpp main.cpp)

roq_gitignore(OUTPUT .gitignore SOURCES ${TARGET_NAME})

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE ${PROJECT_NAME}-codec roq-logging::roq-logging
                                             Catch2::Catch2)

set_target_properties(${TARGET_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME})
