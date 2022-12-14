# CMakeLists.txt for unit tests
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

include(FetchContent)
FetchContent_Declare(
  googlebenchmark
  # Specify the commit you depend on and update it regularly.
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.7.1
)

FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(googlebenchmark)

set( CORCT_UNITTESTS_SRC
#   lib/callsite_expander_test.cc
  lib/callsite_lister_test.cc
  lib/if_else_simple_refactorer_test.cc
#   lib/clang_utilities_test.cc
#   lib/function_common_test.cc
#   lib/function_def_lister_test.cc
#   lib/function_sig_exp_test.cc
  # lib/function_sig_matchers_test.cc   ## not working on Linux??
#   lib/global_matchers_test.cc
#   lib/small_matchers_test.cc
#   lib/struct_field_users_test.cc
#   lib/template_var_matchers_test.cc
#   lib/utilities_test.cc
)

add_executable(corct_unittests ${CORCT_UNITTESTS_SRC})

target_include_directories(corct_unittests
  PUBLIC
    ${googletest_SOURCE_DIR}/googletest/include/gtest
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../lib
)

target_link_libraries(corct_unittests
  gtest
  gtest_main
  refactor
  ${CLANG_LIBRARIES}
  ${TINFO_LIBS}
  z
  )

  add_subdirectory(test-progs)

# End of file