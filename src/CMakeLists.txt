include_directories ("${PROJECT_SOURCE_DIR}/lib")

# add_library(corct-support summarize_command_line.cc)

set(APPS_LIBRARIES
  refactor
#   corct-support
  clangTooling
  ${TINFO_LIB}
  z #libz
  c #libc
)

macro(add_refactor_exe app_name app_src)
  add_executable(${app_name} ${app_src} )
  target_link_libraries(${app_name} ${APPS_LIBRARIES} )
endmacro()

add_refactor_exe(callsite-lister CallsiteLister.cc)
add_refactor_exe(branchsite-lister BranchsiteLister.cc)
add_refactor_exe(branch-converter BranchConverter.cc)

# End of file
