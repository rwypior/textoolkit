cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(bmp ${OPTIONS})
add_library(texutil::bmp ALIAS bmp)

target_compile_definitions(bmp
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(bmp PROPERTIES CXX_STANDARD 17)

target_include_directories(bmp PUBLIC "include")

target_link_libraries(bmp PUBLIC textoolkit::common)

target_link_libraries(bmp PRIVATE glm::glm)

add_subdirectory("include/bmp")
add_subdirectory("src")
add_subdirectory("tests")