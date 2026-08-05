cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(common ${OPTIONS})
add_library(textoolkit::common ALIAS common)

target_compile_definitions(common
    INTERFACE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(common PROPERTIES CXX_STANDARD 17)

target_include_directories(common PUBLIC "include")

target_link_libraries(common PUBLIC glm::glm)

add_subdirectory("include/common")
add_subdirectory("src")
add_subdirectory("tests")