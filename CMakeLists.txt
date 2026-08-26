cmake_minimum_required (VERSION 3.28)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(CMAKE_DEBUG_POSTFIX "d")

if(${APP_PLATFORM} STREQUAL "windows")
    message("Configuring for Windows platform")
endif()

project ("textoolkit")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find packages
find_package(events REQUIRED)
find_package(iniparser REQUIRED)
find_package(libcmdline REQUIRED)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(gli CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base html propgrid xml aui gl)

# Unit tests
if(${BUILD_TESTING})
    message("Building unit tests")
    find_package(Catch2 3.6.0 REQUIRED)
    include(CTest)
endif()

# Subdirs
add_subdirectory("common")
add_subdirectory("operator")
add_subdirectory("texture")
add_subdirectory("dds")
add_subdirectory("bmp")
add_subdirectory("renderer")
add_subdirectory("gui")
add_subdirectory("cmdline")

set(VS_STARTUP_PROJECT textoolkit)

# Graphviz
find_program(GRAPHVIZ dot)
if(GRAPHVIZ)
    message("Found Graphviz")

    add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/_graphviz
        COMMAND ${CMAKE_COMMAND} "--graphviz=graphviz/game.dot" .
        COMMAND dot -Tsvg graphviz/game.dot -o deps.svg
        COMMAND dot -Tpng graphviz/game.dot -o deps.png
        COMMENT "Plotting dependencies graph"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
    add_custom_target(graphviz ALL
        DEPENDS ${CMAKE_BINARY_DIR}/_graphviz)
    set_target_properties(graphviz 
        PROPERTIES EXCLUDE_FROM_ALL TRUE)
else()
    message("Graphviz not found")
endif()