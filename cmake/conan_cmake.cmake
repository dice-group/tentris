macro(install_packages_via_conan conanfile conan_options)

    list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})


    if (NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                TLS_VERIFY ON)
    endif ()

    include(${CMAKE_BINARY_DIR}/conan.cmake)

    conan_cmake_autodetect(settings)
    conan_check(VERSION 1 DETECT_QUIET)
    if (CONAN_CMD)
        conan_cmake_install(PATH_OR_REFERENCE ${conanfile}
                BUILD missing
                SETTINGS ${settings}
                OPTIONS "${conan_options}"
                ENV_HOST "CC=${CMAKE_C_COMPILER};CXX=${CMAKE_CXX_COMPILER}")
    else ()
        message(WARNING "No conan executable was found. Dependency retrieval via conan is disabled. System dependencies will be used if available.")
    endif ()
endmacro()