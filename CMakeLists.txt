cmake_minimum_required(VERSION 3.5)

project(mpdr-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

### add project files
# find headers and source files
file(GLOB_RECURSE MPDR_SOURCES "DoorRandomizer/*.cpp")
file(GLOB_RECURSE MPDR_HEADERS "DoorRandomizer/*.h")
file(GLOB_RECURSE MPDR_FORMS "DoorRandomizer/*.ui")
file(GLOB_RECURSE TS_FILES "LanguagePacks/*.ts")

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mpdr-qt)

set(MPDR_DIRS "")
foreach (_headerFile ${MPDR_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND MPDR_DIRS ${_dir})
endforeach()

# add the executable
add_executable(mpdr-qt
    ${MPDR_SOURCES}
    ${MPDR_FORMS}
    ${MPDR_HEADERS}
    ${TS_FILES}
  )

### randomprime

include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/Rust)

# add randomprime as a CMake target
ExternalProject_Add(
    randomprime
    DOWNLOAD_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cargo build COMMAND cargo build --release
    BINARY_DIR "${CMAKE_SOURCE_DIR}/randomprime"
    INSTALL_COMMAND ""
    USES_TERMINAL_BUILD ON
    LOG_BUILD ON)

add_dependencies(mpdr-qt randomprime)

#find library
set(RANDOMPRIME_DIR $<IF:$<CONFIG:Debug>,debug,release>)

if(MSVC)
    set(RANDOMPRIME_LIB "${CMAKE_SOURCE_DIR}/randomprime/target/${RANDOMPRIME_DIR}/randomprime.lib")
elseif(UNIX)
    set(RANDOMPRIME_LIB "${CMAKE_SOURCE_DIR}/randomprime/target/${RANDOMPRIME_DIR}/randomprime.a")
endif()

target_link_libraries(mpdr-qt PUBLIC Qt5::Widgets ${RANDOMPRIME_LIB})
target_include_directories(mpdr-qt PUBLIC ${MPDR_DIRS})

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
