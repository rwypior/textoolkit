cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include("${CMAKE_SOURCE_DIR}/cmake/version.cmake")

add_library(texture)
add_library(textoolkit::texture ALIAS texture)

target_compile_definitions(texture
    PUBLIC 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(texture PROPERTIES CXX_STANDARD 17)

target_include_directories(texture PUBLIC "include")

target_link_libraries(texture PUBLIC textoolkit::common)
target_link_libraries(texture PUBLIC textoolkit::bmp)
target_link_libraries(texture PUBLIC textoolkit::dds)

target_link_libraries(texture PUBLIC glm::glm)

add_subdirectory("include/texture")
add_subdirectory("src")