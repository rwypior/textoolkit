cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include("${CMAKE_SOURCE_DIR}/cmake/version.cmake")

add_executable(textoolkit ${EXE_OPTIONS})

target_compile_definitions(textoolkit
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(textoolkit PROPERTIES CXX_STANDARD 17)

target_include_directories(textoolkit PUBLIC "include")
target_include_directories(textoolkit PUBLIC "res")

target_link_libraries(textoolkit PRIVATE textoolkit::common)
target_link_libraries(textoolkit PRIVATE textoolkit::dds)
target_link_libraries(textoolkit PRIVATE textoolkit::bmp)
target_link_libraries(textoolkit PRIVATE textoolkit::renderer)

target_link_libraries(textoolkit PRIVATE GLEW::glew)
target_link_libraries(textoolkit PRIVATE glm::glm)
target_link_libraries(textoolkit PRIVATE wx::core wx::base wx::html wx::propgrid wx::xml wx::aui wx::gl)

add_subdirectory("include/gui")
add_subdirectory("src")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include/gui")
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp"  DESTINATION"${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

# Copy DLLs to exe directory
add_custom_command(TARGET textoolkit POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:textoolkit> $<TARGET_RUNTIME_DLLS:textoolkit>
  COMMAND_EXPAND_LISTS
)

# Copy resources
add_custom_command(
  TARGET textoolkit POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/res" "${CMAKE_CURRENT_BINARY_DIR}/res"
)
