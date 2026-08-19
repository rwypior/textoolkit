cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include("${CMAKE_SOURCE_DIR}/cmake/version.cmake")

add_library(common)
add_library(textoolkit::common ALIAS common)

target_compile_definitions(common
    PUBLIC 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

configure_file("${CMAKE_SOURCE_DIR}/cmake/textoolkit.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/common/textoolkit.hpp" @ONLY)
target_sources(common PUBLIC "include/common/textoolkit.hpp")

set_target_properties(common PROPERTIES CXX_STANDARD 17)

target_include_directories(common PUBLIC "include")

target_link_libraries(common PUBLIC glm::glm)

add_subdirectory("include/common")
add_subdirectory("src")
add_subdirectory("tests")