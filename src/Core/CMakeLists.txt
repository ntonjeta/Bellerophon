add_library(core
           EvolutionContext.cpp
           AprxContext.cpp
           )

target_include_directories(core
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                          )
