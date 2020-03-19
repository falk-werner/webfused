find_package(PkgConfig)
pkg_check_modules(PC_PAM QUIET pam)
set(PAM_DEFINITIONS ${PC_PAM_CFLAGS_OTHER})

find_path(PAM_INCLUDE_DIR security/pam_appl.h
    HINTS ${PC_PAM_INCLUDEDIR} ${PC_PAM_INCLUDE_DIRS}
)

find_library(PAM_LIBRARY NAMES pam
    HINTS ${PC_PAM_LIBDIR} ${PC_PAM_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(Pam DEFAULT_MSG
    PAM_LIBRARY PAM_INCLUDE_DIR
)

mark_as_advanced(PAM_INCLUDE_DIR PAM_LIBRARY)

set(PAM_LIBRARIES ${PAM_LIBRARY})
set(PAM_INCLUDE_DIRS ${PAM_INCLUDE_DIR})
