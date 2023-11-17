include(CheckParseArguments)

function(convert_bool_to_yes_no)
    set(PREFIX ${CMAKE_CURRENT_FUNCTION})
    set(BOOL_VALUE ${ARGV0})
    cmake_parse_arguments(PARSE_ARGV 1 ${PREFIX} "" "OUTPUT_VARIABLE" "")
    check_parse_arguments(${PREFIX})

    # TODO, this is unset in the parent scope.
    if (${BOOL_VALUE})
        set(${PREFIX}_OUTPUT_VARIABLE "YES")
    else()
        set(${PREFIX}_OUTPUT_VARIABLE "NO")
    endif()
endfunction()
