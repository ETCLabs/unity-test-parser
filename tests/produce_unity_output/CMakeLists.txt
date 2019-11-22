cmake_minimum_required(VERSION 3.3)
project(product_unity_output)

set(UNITY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/../../external/unity)

enable_testing()

add_compile_definitions(UNITY_INCLUDE_CONFIG_H)
include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(${UNITY_SOURCE_DIR} unity)

add_library(unity_fixture
  ${UNITY_SOURCE_DIR}/extras/fixture/src/unity_fixture.h
  ${UNITY_SOURCE_DIR}/extras/fixture/src/unity_fixture_internals.h
  ${UNITY_SOURCE_DIR}/extras/fixture/src/unity_fixture.c
)
target_compile_definitions(unity_fixture PUBLIC UNITY_FIXTURE_NO_EXTRAS)
target_include_directories(unity_fixture PUBLIC ${UNITY_SOURCE_DIR}/extras/fixture/src)
target_link_libraries(unity_fixture PUBLIC unity::framework)

add_executable(produce_normal_unity_output
  normal_unity_test_output.c
)
target_link_libraries(produce_normal_unity_output PRIVATE unity::framework)
add_test(NAME ProduceNormalUnityOutput
  COMMAND $<TARGET_FILE:produce_normal_unity_output>
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(produce_unity_fixture_output
  unity_fixture_test_output.c
)
target_link_libraries(produce_unity_fixture_output PRIVATE unity_fixture)
# add_test(NAME ProduceUnityFixtureOutput
#   COMMAND $<TARGET_FILE:produce_unity_fixture_output>
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )
add_test(NAME ProduceUnityFixtureOutputVerbose
  COMMAND $<TARGET_FILE:produce_unity_fixture_output> -v
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
# add_test(NAME ProduceUnityFixtureOutputSilent
#   COMMAND $<TARGET_FILE:produce_unity_fixture_output> -s
#   WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
# )
