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

    if (IS_TOP_LEVEL AND BUILD_TESTING)
        set(CONAN_HYPERTRIE_WITH_TEST_DEPS "True")
    else()
        set(CONAN_HYPERTRIE_WITH_TEST_DEPS "False")
    endif()
    conan_cmake_install(PATH_OR_REFERENCE ${conanfile}
            BUILD missing
            SETTINGS ${settings}
            OPTIONS "${conan_options}"
            GENERATOR "CMakeDeps")
endmacro()