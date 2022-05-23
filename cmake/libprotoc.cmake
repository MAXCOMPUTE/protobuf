# CMake definitions for libprotoc (the protobuf compiler library).

include(${protobuf_SOURCE_DIR}/src/file_lists.cmake)

add_library(libprotoc ${protobuf_SHARED_OR_STATIC}
  ${libprotoc_srcs}
  ${libprotoc_hdrs}
  ${protobuf_version_rc_file})
if(protobuf_HAVE_LD_VERSION_SCRIPT)
  if(${CMAKE_VERSION} VERSION_GREATER 3.13 OR ${CMAKE_VERSION} VERSION_EQUAL 3.13)
    target_link_options(libprotoc PRIVATE -Wl,--version-script=${protobuf_SOURCE_DIR}/src/libprotoc.map)
  elseif(protobuf_BUILD_SHARED_LIBS)
    target_link_libraries(libprotoc PRIVATE -Wl,--version-script=${protobuf_SOURCE_DIR}/src/libprotoc.map)
  endif()
  set_target_properties(libprotoc PROPERTIES
    LINK_DEPENDS ${protobuf_SOURCE_DIR}/src/libprotoc.map)
endif()
target_link_libraries(libprotoc PRIVATE libprotobuf)
target_include_directories(libprotoc
  PRIVATE ${ABSL_ROOT_DIR}
)
if(protobuf_BUILD_SHARED_LIBS)
  target_compile_definitions(libprotoc
    PUBLIC  PROTOBUF_USE_DLLS
    PRIVATE LIBPROTOC_EXPORTS)
endif()
set_target_properties(libprotoc PROPERTIES
    COMPILE_DEFINITIONS LIBPROTOC_EXPORTS
    VERSION ${protobuf_VERSION}
    SOVERSION 32
    OUTPUT_NAME ${LIB_PREFIX}protoc
    DEBUG_POSTFIX "${protobuf_DEBUG_POSTFIX}")
add_library(protobuf::libprotoc ALIAS libprotoc)
