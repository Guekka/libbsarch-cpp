set(HEADER_PATH ${PROJECT_SOURCE_DIR}/include)

set(HEADERS
    ${HEADER_PATH}/libbsarch.hpp
    ${HEADER_PATH}/base_types.hpp
    ${HEADER_PATH}/bsa.hpp
    ${HEADER_PATH}/bsa_saver.hpp
    ${HEADER_PATH}/bsa_entry_list.hpp
    ${HEADER_PATH}/transform_archive.hpp
    )

set(PRIVATE_HEADERS
    ${HEADER_PATH}/detail/scope_fail.hpp
    ${HEADER_PATH}/detail/string_convert.hpp
    ${HEADER_PATH}/detail/for_each.hpp
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/base_types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bsa.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bsa_saver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/bsa_entry_list.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/transform_archive.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/detail/string_convert.cpp
    )

add_library(libbsarch-cpp STATIC ${SOURCES} ${HEADERS} ${PRIVATE_HEADERS})

find_package(libbsarch CONFIG REQUIRED)
target_link_libraries(libbsarch-cpp PUBLIC libbsarch)

target_include_directories(libbsarch-cpp PUBLIC
    $<BUILD_INTERFACE:${HEADER_PATH}> # for client in subdirectory
    $<INSTALL_INTERFACE:include> # for client in install mode
    PRIVATE
    $<BUILD_INTERFACE:${HEADER_PATH}/detail>
)

target_compile_features(libbsarch-cpp PUBLIC cxx_std_17)
target_compile_definitions(libbsarch-cpp PRIVATE _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

install(DIRECTORY ${HEADER_PATH} DESTINATION ${CMAKE_INSTALL_PREFIX})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${PROJECT_BINARY_DIR}/cmake/libbsarch-cpp-config-version.cmake
  VERSION ${libbsarch-cpp-version}
  COMPATIBILITY AnyNewerVersion)

install(TARGETS libbsarch-cpp
  EXPORT libbsarch-cpp-targets
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/libbsarch-cpp-config.cmake.in
  ${PROJECT_BINARY_DIR}/cmake/libbsarch-cpp-config.cmake
  INSTALL_DESTINATION cmake/})

install(EXPORT libbsarch-cpp-targets
  FILE libbsarch-cpp-targets.cmake
  DESTINATION cmake/)

install(FILES
    ${PROJECT_BINARY_DIR}/cmake/libbsarch-cpp-config.cmake
    ${PROJECT_BINARY_DIR}/cmake/libbsarch-cpp-config-version.cmake
  DESTINATION cmake/)


