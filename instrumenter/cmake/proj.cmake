function(add_proj_library name)
  set(singleValueArgs TYPE)
  set(multiValueArgs FILES INCLUDES INCLUDES_BEFORE DEPENDS OPTIONS)
  cmake_parse_arguments(OPT ""
    "${singleValueArgs}" "${multiValueArgs}" ${ARGV})
  file(GLOB SRCS ${OPT_FILES})
  if(DEFINED OPT_TYPE)
    set(LIBTYPE ${OPT_TYPE})
  else()
    set(LIBTYPE STATIC)
  endif()
  add_library(${name} ${LIBTYPE} ${SRCS})
  if(DEFINED OPT_DEPENDS)
	target_link_libraries(${name} ${OPT_DEPENDS})
  endif()
  if(DEFINED OPT_OPTIONS)
    target_compile_options(${name} PRIVATE ${OPT_OPTIONS})
  endif()
  if(DEFINED OPT_INCLUDES_BEFORE)
    target_include_directories(${name} BEFORE PRIVATE ${OPT_INCLUDES_BEFORE})
  endif()
  if(DEFINED OPT_INCLUDES)
    target_include_directories(${name} PRIVATE ${OPT_INCLUDES})
  endif()
  add_custom_command(
    TARGET ${name}
    POST_BUILD
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/lib
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}>
    ${CMAKE_BINARY_DIR}/lib/$<TARGET_FILE_NAME:${name}>
    DEPENDS ${name}
    )
endfunction()

function(add_proj_program name)
  set(singleValueArgs)
  set(multiValueArgs INCLUDES INCLUDES_BEFORE FILES DEPENDS OPTIONS)
  cmake_parse_arguments(OPT ""
    "${singleValueArgs}" "${multiValueArgs}" ${ARGV})
  file(GLOB SRCS ${OPT_FILES})
  add_executable(${name} ${SRCS})
  if(DEFINED OPT_DEPENDS)
	target_link_libraries(${name} ${OPT_DEPENDS})
  endif()
  if(DEFINED OPT_OPTIONS)
    target_compile_options(${name} PRIVATE ${OPT_OPTIONS})
  endif()
  if(DEFINED OPT_INCLUDES_BEFORE)
    target_include_directories(${name} BEFORE PRIVATE ${OPT_INCLUDES_BEFORE})
  endif()
  if(DEFINED OPT_INCLUDES)
    target_include_directories(${name} PRIVATE ${OPT_INCLUDES})
  endif()
  add_custom_command(
    TARGET ${name}
    POST_BUILD
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/bin
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${name}>
    ${CMAKE_BINARY_DIR}/bin/$<TARGET_FILE_NAME:${name}>
    DEPENDS ${name}
    )
endfunction()

