# Core
add_subdirectory(Core)

# ExecutionEngineHelper
add_subdirectory(ExecutionEngineHelper)

# Tool
add_subdirectory(Tool)

# Plugins
add_subdirectory(Plugins)


add_library(log
            Log.cpp
            )

target_include_directories(log
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )



