add_library(exec_engine_helper
            ExecutionEngineHelper.cpp
           )
target_include_directories(exec_engine_helper
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )

target_link_libraries(exec_engine_helper
                      core)
