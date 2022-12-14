add_executable(if_else_basic if_else_basic.cc)
set_target_properties(if_else_basic PROPERTIES COMPILE_FLAGS -save-temps)
set_target_properties(if_else_basic PROPERTIES COMPILE_FLAGS "-O0")