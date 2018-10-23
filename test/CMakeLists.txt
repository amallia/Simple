file(GLOB TEST_SOURCES *_test.cpp)
foreach(TEST_SRC ${TEST_SOURCES})
  get_filename_component (TEST_SRC_NAME ${TEST_SRC} NAME_WE)
  add_executable(${TEST_SRC_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_SRC_NAME}
    gtest
    gmock
    FastPFor
    )
    target_include_directories(${TEST_SRC_NAME} BEFORE PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
        ${BENCH_SRC_NAME} BEFORE PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/external>
    )
  add_test(${TEST_SRC_NAME} ${TEST_SRC_NAME})
endforeach(TEST_SRC)
