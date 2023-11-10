################################################################################
# Compile Options
################################################################################
option(ENABLE_WARNING_AS_ERROR "Enalbes that every warning is treated as error" OFF)

################################################################################
# Compiler-Settings
################################################################################
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-O0 -g3 -fno-inline)

        if(ENABLE_TEST)
            #add_compile_options(-fprofile-arcs -ftest-coverage)
        endif()
    else()
        add_compile_options(-O2)
    endif()

    add_compile_options(-Wall -Wpedantic -fpic -Wextra -Wunused-function -fvisibility=hidden -fmessage-length=0)
    if(ENABLE_WARNING_AS_ERROR)
        add_compile_options(-Werror)
    endif()
endif()

add_definitions("-DQT_NO_KEYWORDS")
add_definitions("-DINSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}")
