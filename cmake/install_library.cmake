include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

function(install_component COMPONENT_NAME INCLUDE_PATH)

    target_include_directories(
            ${COMPONENT_NAME} PUBLIC $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}/${COMPONENT_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}/
            DESTINATION include/${PROJECT_NAME}/${COMPONENT_NAME}/
            FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

    install(TARGETS ${COMPONENT_NAME}
            EXPORT ${COMPONENT_NAME}-config
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
            )

    install(
            EXPORT ${COMPONENT_NAME}-config
            FILE ${COMPONENT_NAME}-config.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/${COMPONENT_NAME}/)

    write_basic_package_version_file(${COMPONENT_NAME}-config-version.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMinorVersion)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${COMPONENT_NAME}-config-version.cmake DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/)
endfunction()

function(install_interface_component COMPONENT_NAME INCLUDE_PATH)

    target_include_directories(
            ${COMPONENT_NAME} INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}/${COMPONENT_NAME})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${INCLUDE_PATH}/
            DESTINATION include/${PROJECT_NAME}/${COMPONENT_NAME}/
            FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")

    install(TARGETS ${COMPONENT_NAME}
            EXPORT ${COMPONENT_NAME}-config
            INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/
            )

    install(
            EXPORT ${COMPONENT_NAME}-config
            FILE ${COMPONENT_NAME}-config.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/${COMPONENT_NAME}/)

    write_basic_package_version_file(${COMPONENT_NAME}-config-version.cmake
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMinorVersion)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${COMPONENT_NAME}-config-version.cmake DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${COMPONENT_NAME}/)
endfunction()

function(install_package)

    write_basic_package_version_file("${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    configure_package_config_file(
            "${PROJECT_SOURCE_DIR}/cmake/main-component-config.cmake.in"
            "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/)

    write_basic_package_version_file(
            "${PROJECT_NAME}-config-version.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion)

    install(FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/cmake/${PROJECT_NAME}/)
endfunction()

