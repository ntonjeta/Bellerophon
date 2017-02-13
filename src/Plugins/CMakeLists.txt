# Create an interface library
set(plugin_lib "plugins")
add_library(${plugin_lib} INTERFACE)

# List all directories, which should contain Aproximate Context and Technique 
file(GLOB children ${CMAKE_CURRENT_SOURCE_DIR}/*)
foreach(child ${children})
  if(IS_DIRECTORY ${child})
    message(STATUS "Approximate Technique directory: ${child}")
    # Add the CMakeLists.txt inside the subdirectory
    add_subdirectory(${child})
  endif(IS_DIRECTORY ${child})
endforeach(child)

# By default, load all plugin library in ${CMAKE_SOURCE_DIR}/lib/plugins
file(GLOB libs ${CMAKE_SOURCE_DIR}/lib/plugins/lib*.a)
foreach(lib ${libs})
  message(STATUS "Approximate Technique library: ${lib}")
  # Link the library found
  target_link_libraries(${plugin_lib} INTERFACE
                        ${lib}
                        )
endforeach(lib)


