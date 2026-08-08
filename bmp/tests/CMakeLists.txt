cmake_minimum_required (VERSION 3.26)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

add_executable(bmptest)
target_sources(bmptest PRIVATE 
    "main.cpp" "common.hpp"
    "utest_img.bmp.hpp"
    "loadingtest.cpp"
)

set_property(TARGET bmptest PROPERTY CXX_STANDARD 17)

target_compile_definitions(bmptest
    PRIVATE
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

target_link_libraries(bmptest PRIVATE textoolkit::bmp)

target_link_libraries(bmptest PRIVATE glm::glm)
target_link_libraries(bmptest PRIVATE Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${Catch2_SOURCE_DIR}/extras)
include(Catch)
catch_discover_tests(bmptest)
