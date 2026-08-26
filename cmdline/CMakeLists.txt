cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include("${CMAKE_SOURCE_DIR}/cmake/version.cmake")

add_executable(cmdline)
add_executable(textoolkit::cmdline ALIAS cmdline)

target_compile_definitions(cmdline
    PRIVATE 
        GLM_ENABLE_EXPERIMENTAL=1
        $<$<CONFIG:Debug>:DEBUG>
)

set_target_properties(cmdline PROPERTIES CXX_STANDARD 17)

target_include_directories(cmdline PUBLIC "include")
target_include_directories(cmdline PUBLIC "res")

target_link_libraries(cmdline PRIVATE textoolkit::common)
target_link_libraries(cmdline PRIVATE textoolkit::texture)
target_link_libraries(cmdline PRIVATE textoolkit::dds)
target_link_libraries(cmdline PRIVATE textoolkit::bmp)

target_link_libraries(cmdline PRIVATE libcmdline::libcmdline)

target_link_libraries(cmdline PRIVATE glm::glm)

add_subdirectory("include/cmdline")
add_subdirectory("src")

# Copy generated files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/mainwindow.h")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/generated/mainwindow.h" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include/gui")
endif()
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/generated/mainwindow.cpp")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/generated/mainwindow.cpp"  DESTINATION"${CMAKE_CURRENT_SOURCE_DIR}/src")
endif()

# Copy DLLs to exe directory
add_custom_command(TARGET cmdline POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:cmdline> $<TARGET_RUNTIME_DLLS:cmdline>
  COMMAND_EXPAND_LISTS
)
