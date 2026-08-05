cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_executable(commontest)
target_sources(commontest PRIVATE 
    "main.cpp" "common.hpp"
    "pixeltest.cpp"
)

set_property(TARGET commontest PROPERTY CXX_STANDARD 17)

target_compile_definitions(commontest
    PRIVATE
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

target_link_libraries(commontest PRIVATE textoolkit::common)

target_link_libraries(commontest PRIVATE glm::glm)
target_link_libraries(commontest PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(commontest)
