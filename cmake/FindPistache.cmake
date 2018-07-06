set(Pistache_ROOT_DIR
        "${Pistache_ROOT_DIR}"
        CACHE
        PATH
        "Where to start looking for this component.")

if(WIN32)
    find_path(Pistache_INCLUDE_DIR
            NAMES
            pistache
            PATHS
            "C:\\Program Files"
            HINTS
            ${Pistache_ROOT_DIR}
            PATH_SUFFIXES
            include)

    find_library(Pistache_LIBRARY
            NAMES
            libnet
            libnet_static
            HINTS
            ${Pistache_ROOT_DIR}
            PATH_SUFFIXES
            lib)

else()
    find_path(Pistache_INCLUDE_DIR
            NAMES
            pistache/cookie.h
            cookie.h
            HINTS
            ${Pistache_ROOT_DIR}
            PATHS
            /usr/local/include
            /usr/include
            PATH_SUFFIXES
            include)

    find_library(Pistache_LIBRARY
            NAMES
            libnet.so
            libnet_static.sa
            libpistache.a
            HINTS
            ${Pistache_ROOT_DIR}
            PATHS
            usr/local
            usr/local/lib
            PATH_SUFFIXES
            lib64
            lib)
endif()

mark_as_advanced(Pistache_INCLUDE_DIR
        Pistache_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Pistache
        DEFAULT_MSG
        Pistache_INCLUDE_DIR
        Pistache_LIBRARY)

if(Pistache_FOUND)
    set(Pistache_INCLUDE_DIRS
            "${Pistache_INCLUDE_DIR}")
    # Add any dependencies here
    set(Pistache_LIBRARIES
            "${Pistache_LIBRARY}")
    # Add any dependencies here
    mark_as_advanced(Pistache_ROOT_DIR)
endif()
