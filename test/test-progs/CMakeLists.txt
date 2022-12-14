add_executable(if_else_basic if_else_basic.cc)
set_target_properties(if_else_basic PROPERTIES COMPILE_FLAGS -save-temps)
set_target_properties(if_else_basic PROPERTIES COMPILE_FLAGS "-O0")

add_executable(predicate_nopredicate_test predicate_nopredicate_test.cc)
set_target_properties(predicate_nopredicate_test PROPERTIES COMPILE_FLAGS -DCMOV)
set_target_properties(if_else_basic PROPERTIES COMPILE_FLAGS "-O0")