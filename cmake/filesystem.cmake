#
# macro for finding files based on a patter
#
macro(find_files_by_pattern pattern out)
    file(GLOB_RECURSE ${out} "${pattern}")
endmacro()