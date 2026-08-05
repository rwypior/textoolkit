cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(dds ${OPTIONS})
add_library(texutil::dds ALIAS dds)

target_compile_definitions(dds
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(dds PROPERTIES CXX_STANDARD 17)

target_include_directories(dds PUBLIC "include")

target_link_libraries(dds PUBLIC textoolkit::common)

target_link_libraries(dds PRIVATE glm::glm)

add_subdirectory("include/dds")
add_subdirectory("src")
add_subdirectory("tests")