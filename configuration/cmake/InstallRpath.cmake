include(GNUInstallDirs)

# blabby_set_install_rpath(<target> [DESTINATION <dir>])
#
# Makes the installed <target> find the Blabby libraries in CMAKE_INSTALL_LIBDIR without LD_LIBRARY_PATH.
# The RPATH is relative to the installed binary ($ORIGIN), so the install tree stays relocatable.
# DESTINATION is the directory <target> is installed to, relative to CMAKE_INSTALL_PREFIX. It defaults to
# CMAKE_INSTALL_BINDIR for executables and CMAKE_INSTALL_LIBDIR for libraries.
# Directories of linked libraries outside the build tree (e.g. a Qt not installed in a system directory) are
# added as well.
function(blabby_set_install_rpath target)
    cmake_parse_arguments(PARSE_ARGV 1 ARG "" "DESTINATION" "")

    if(NOT ARG_DESTINATION)
        get_target_property(type ${target} TYPE)
        if(type STREQUAL "EXECUTABLE")
            set(ARG_DESTINATION "${CMAKE_INSTALL_BINDIR}")
        else()
            set(ARG_DESTINATION "${CMAKE_INSTALL_LIBDIR}")
        endif()
    endif()

    cmake_path(ABSOLUTE_PATH ARG_DESTINATION BASE_DIRECTORY "${CMAKE_INSTALL_PREFIX}" OUTPUT_VARIABLE destination)
    file(RELATIVE_PATH libDir "${destination}" "${CMAKE_INSTALL_FULL_LIBDIR}")
    string(REGEX REPLACE "/$" "" libDir "${libDir}")
    if(libDir)
        set(rpath "$ORIGIN/${libDir}")
    else()
        set(rpath "$ORIGIN")
    endif()

    set_target_properties(${target} PROPERTIES
        INSTALL_RPATH "${rpath}"
        INSTALL_RPATH_USE_LINK_PATH TRUE
    )
endfunction()
