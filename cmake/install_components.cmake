include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

function(install_component TYPE COMPONENT_NAME INCLUDE_PATH)
    set(lib_name "${PROJECT_NAME}-${COMPONENT_NAME}")

    set(possible_types INTERFACE PUBLIC)
    if(NOT TYPE IN_LIST possible_types)
        message(FATAL_ERROR "Argument TYPE=${component} of function install_component is not allowed. Allowed values are ${possible_types}")
    endif()

    if("${TYPE}" STREQUAL "INTERFACE")
        target_include_directories(
                ${lib_name} INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}/${COMPONENT_NAME})

        install(TARGETS ${lib_name}
                EXPORT ${lib_name}-config
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
        )
    else()
        set_target_properties(${lib_name} PROPERTIES
                VERSION ${PROJECT_VERSION}
                SOVERSION ${PROJECT_VERSION_MAJOR}
                CXX_STANDARD 23
                CXX_EXTENSIONS OFF
                CXX_STANDARD_REQUIRED ON)
        target_include_directories(
                ${lib_name} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}/${COMPONENT_NAME})

        install(TARGETS ${lib_name}
                EXPORT ${lib_name}-config
                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
                INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
        )
    endif()

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}/
            DESTINATION include/${PROJECT_NAME}/${COMPONENT_NAME}/
            FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

    install(
            EXPORT ${lib_name}-config
            FILE ${lib_name}-config.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/${COMPONENT_NAME}/)

endfunction()

function(install_package)

    write_basic_package_version_file("${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/main-component-config.cmake.in"
            "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/)


    install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/)
endfunction()