# ------------------------------------------------------------------------
#
# check compare-xyz
#

set(TEST_BASE_DIR ${PROJECT_SOURCE_DIR}/tests/compare-xyz)
set(GAMA_LOCAL_DEFORMATION ${CMAKE_BINARY_DIR}/compare-xyz)
set(RESULT_TOP tests/compare-xyz/results/${PROJECT_VERSION})

#
# ------------------------------------------------------------------------
#
# check ...
#

set(INPUT_DIR ${TEST_BASE_DIR}/input)
set(RESULT_DIR ${CMAKE_BINARY_DIR}/${RESULT_TOP})
file(MAKE_DIRECTORY ${RESULT_DIR})

if(UNIX)
  add_test(NAME check_compare_xyz_clean_results_dir
    COMMAND file(REMOVE_RECURSE ${RESULT_DIR})
    )
endif()

add_test(NAME compare_xyz_implicit_help
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
  )

add_test(NAME compare_xyz_implicit_tolerance
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz --tolerance
  )

# example 01 is a variant of Ghilani
add_test(NAME compare_xyz_example_01_FAIL
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
  ${INPUT_DIR}/example-01-g3-adj.xml
  ${INPUT_DIR}/example-01-local-adj.xml
  )

set_tests_properties(compare_xyz_example_01_FAIL PROPERTIES
  WILL_FAIL TRUE
  )

add_test(NAME compare_xyz_example_01_increase_tolerance
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz --set-tolerance 5e-5
  ${INPUT_DIR}/example-01-g3-adj.xml
  ${INPUT_DIR}/example-01-local-adj.xml
  )

add_test(NAME compare_xyz_example_02_a
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
  ${INPUT_DIR}/example-02-a-g3-adj.xml
  ${INPUT_DIR}/example-02-a-local-adj.xml
  )

add_test(NAME compare_xyz_example_02_b
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
  ${INPUT_DIR}/example-02-b-g3-adj.xml
  ${INPUT_DIR}/example-02-b-local-adj.xml
  )

add_test(NAME compare_xyz_example_ghilani_FAIL
  COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
  ${INPUT_DIR}/ghilani-g3-adj.xml
  ${INPUT_DIR}/ghilani-local-adj.xml
  )

set_tests_properties(compare_xyz_example_ghilani_FAIL PROPERTIES
  WILL_FAIL TRUE
  )


# add_test(NAME compare_xyz_example_01_fail COMMAND ${CMAKE_BINARY_DIR}/compare-xyz
#   ${INPUT_DIR}/example-01-g3-adj.xml
#   ${INPUT_DIR}/example-01-local-adj.xml
#   )
#
# set_tests_properties(compare_xyz_example_01_fail PROPERTIES
#   WILL_FAIL TRUE
# )
