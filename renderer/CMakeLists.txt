cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_library(renderer ${OPTIONS})
add_library(textoolkit::renderer ALIAS renderer)

target_compile_definitions(renderer
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set(TEXTOOLKIT_RENDERER_SHADERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shader" CACHE STRING "Path to directory containing shaders")

set_target_properties(renderer PROPERTIES CXX_STANDARD 17)

target_include_directories(renderer PUBLIC "include")

target_link_libraries(renderer PRIVATE textoolkit::common)

target_link_libraries(renderer PRIVATE iniparser::iniparser)

target_link_libraries(renderer PRIVATE OpenGL::GL)
target_link_libraries(renderer PRIVATE GLEW::glew)
target_link_libraries(renderer PRIVATE glm::glm)
target_link_libraries(renderer PRIVATE wx::core wx::base wx::html wx::propgrid wx::xml wx::aui wx::gl)

add_subdirectory("include/renderer")
add_subdirectory("src")