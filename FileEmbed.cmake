function(FileEmbedGenerate file generated_c)

    get_filename_component(base_filename ${file} NAME)
    set(output_filename "${base_filename}.c")
    string(MAKE_C_IDENTIFIER ${base_filename} c_name)
    file(READ ${file} content HEX)
    message(${content})

    # Separate into individual bytes.
    string(REGEX MATCHALL "([A-Fa-f0-9][A-Fa-f0-9])" SEPARATED_HEX ${content})

    set(output_c "")

    set(counter 0)
    foreach (hex IN LISTS SEPARATED_HEX)
        string(APPEND output_c "0x${hex},")
        MATH(EXPR counter "${counter}+1")
        if (counter GREATER 16)
            string(APPEND output_c "\n    ")
            set(counter 0)
        endif ()
    endforeach ()

    set(output_c "
#include \"${c_name}.h\"
uint8_t ${c_name}_data[] = {
    ${output_c}
}\;
unsigned ${c_name}_size = sizeof(${c_name}_data)\;
")

    set(output_h "
#ifndef ${c_name}_H
#define ${c_name}_H
#include \"stdint.h\"
extern uint8_t ${c_name}_data[]\;
extern unsigned ${c_name}_size\;
#endif // ${c_name}_H
    ")


    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}file_embed)
    endif ()


    file(WRITE ${CMAKE_BINARY_DIR}/file_embed/${c_name}.c
            ${output_c})

    file(WRITE ${CMAKE_BINARY_DIR}/file_embed/${c_name}.h
            ${output_h})

    set(${generated_c} ${CMAKE_BINARY_DIR}/file_embed/${c_name}.c PARENT_SCOPE)

endfunction()


# This function is used to create the file_embed target which is used to build
# the generated source files from the imported binary file.
function(FileEmbedSetup)
	# Make sure the directory exists where the generated source files end up
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed)
        file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}file_embed)
    endif ()
	# CMake does not allow libraries with no source files.
	# So create an empty source file if necessary to create the library with.
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c)
        file(WRITE ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c "")
    endif ()
	
	# Create the file_embed library which will be used to compile all the
	# generated source files.
    add_library(file_embed ${CMAKE_BINARY_DIR}/file_embed/file_embed_empty.c)
    # When linking against this library make sure the include directory is 
    # added to the library.
    target_include_directories(file_embed PUBLIC ${CMAKE_BINARY_DIR}/file_embed)
endfunction()

function(FileEmbedAdd file)
	# Contains all the code from above to read in a file contents
	# and write the array to a generated .h/.c file.
    FileEmbedGenerate(${file} var)
    # target sources linkes a source file to the specified library.
    # the var varibale contains the name of the source file that
    # contains the generated array for the embedded data.
    target_sources(file_embed PUBLIC ${var})

	# This command adds a custom command that indicates that the 
	# generated source file is dependent on the source binary 
	# file.  THis means that if the source file changes then
	# the generated embedded file should also be updated.
    add_custom_command(
            OUTPUT ${var}
            COMMAND ${CMAKE_COMMAND}
            -DRUN_FILE_EMBED_GENERATE=1
            -DFILE_EMBED_GENERATE_PATH=${file}
            -P ${CMAKE_SOURCE_DIR}/FileEmbed.cmake
            MAIN_DEPENDENCY ${file}
    )
endfunction()