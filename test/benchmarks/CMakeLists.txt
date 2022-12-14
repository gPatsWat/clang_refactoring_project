set( CORCT_MICROBENCHMARKS
#   lib/callsite_expander_test.cc
  predicate_nopredicate_test.cc
  # lib/predicate_nopredicate_test.cc
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

add_executable(corct_microbenchmarks ${CORCT_MICROBENCHMARKS})

target_include_directories(corct_microbenchmarks
  PUBLIC
    ${googletest_SOURCE_DIR}/googletest/include/gtest
    ${googlebenchmark_SOURCE_DIR}/include/
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../lib
)

target_link_libraries(corct_microbenchmarks
  benchmark
  ${TINFO_LIBS}
  z
  )