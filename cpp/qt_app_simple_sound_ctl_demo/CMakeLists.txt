cmake_minimum_required(VERSION 4.2.0)

project(qt_app_simple_sount_ctl_demo VERSION 0.1
                                     LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)




include(FetchContent)

# Qt
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Widgets)

set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

# Add dbus-cxx
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    dbus-cxx
    GIT_REPOSITORY https://github.com/dbus-cxx/dbus-cxx.git
    GIT_TAG master
)

FetchContent_MakeAvailable(dbus-cxx)




add_executable(${PROJECT_NAME}_exec 
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/media_ctl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/media_ctl_dbus.cpp
)

target_link_libraries(${PROJECT_NAME}_exec PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Widgets
    dbus-cxx
)

target_include_directories(${PROJECT_NAME}_exec PRIVATE

    ${CMAKE_CURRENT_LIST_DIR}/include/ 
)

qt_add_qml_module(${PROJECT_NAME}_exec
    URI app 
    VERSION 1.0
    QML_FILES qml/main.qml qml/MediaButton.qml qml/MediaSystemTray.qml
    RESOURCES
        resources.qrc
        assets/icon.png
    SOURCES
        include/media_ctl.h
        src/media_ctl.cpp
)

qt6_add_resources(RESOURCES
    resources.qrc
)

target_sources(${PROJECT_NAME}_exec PRIVATE
    ${${PROJECT_NAME}_QML_SOURCES}
    ${RESOURCES}
)

install(TARGETS ${PROJECT_NAME}_exec
    RUNTIME DESTINATION bin
)

install(TARGETS ${PROJECT_NAME}_exec
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET QmlSources DESTINATION qml
)

