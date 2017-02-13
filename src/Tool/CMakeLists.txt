add_library(tool
           BellerophonTool.cpp 
          )
target_include_directories(tool
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                          )

target_link_libraries(tool
                      core)
