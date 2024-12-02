macro(boilerplate_init)
    ## enforce standard compliance
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    if (PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)  # need fPIC to build tests
    endif ()

    ## C++ compiler flags
    if (MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
    else ()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wold-style-cast -Wcast-qual")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")

        if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
            # -Wchanges-meaning is not useful
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-changes-meaning")
        endif ()
    endif ()

    ## C++ language visibility configuration
    if (NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
            NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
        set(CMAKE_CXX_VISIBILITY_PRESET default)
        set(CMAKE_VISIBILITY_INLINES_HIDDEN NO)
    endif ()

    # conan requires cmake build type to be specified and it is generally a good idea
    if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/CMakeCache.txt)
        if (NOT CMAKE_BUILD_TYPE)
            set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
        endif ()
    endif ()
endmacro()