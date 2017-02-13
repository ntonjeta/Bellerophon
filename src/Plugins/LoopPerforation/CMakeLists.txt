# CMakeLists for Flap AprxTechnique
# Change the target name
set (target_name "LoopAprx")
set (target_lib_name ${target_name})

# Create a library target. 
add_library(${target_name}
            LoopAprxContext.cpp
            LoopAprxTechnique.cpp
            )
# Add the include directory
target_include_directories(${target_name}
                           PRIVATE ${CMAKE_SOURCE_DIR}/include
                           )
# Link to the core library, necessary to make visible the Core classes
target_link_libraries(${target_name}
                      core
                     )


# Modification of the target properties 
## Name: Change the output name of the library (the default is the same as the target name)
set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${target_lib_name})
## Output directory: Save the compiled static library in the proper folder
set_target_properties(${target_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/plugins)

# Add to the plugin library: ${plugin_lib}
target_link_libraries(${plugin_lib} INTERFACE
                      ${target_name}
                     )
