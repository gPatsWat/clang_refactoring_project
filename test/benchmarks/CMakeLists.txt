macro(add_bm_exe bm_name bm_src)
  add_executable(${bm_name} ${bm_src} )
  target_include_directories(${bm_name}
  PUBLIC
    ${googlebenchmark_SOURCE_DIR}/include/
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/../lib
  )
  target_link_libraries(${bm_name} ${BM_LIBRARIES} )
endmacro()

set(BM_LIBRARIES
  benchmark
  ${TINFO_LIBS}
  z
)

add_bm_exe(bm_OR bm_OR.cc)
add_bm_exe(bm_PLUS bm_PLUS.cc)