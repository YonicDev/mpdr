cmake_minimum_required(VERSION 3.5)

project(mpdr-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

# translations
# To make a new translation file, fill the NEW_TS variable with "LanguagePacks/mpdr_x.ts"
# where "x" is the locale for a corresponding language.
# Run CMake, build the project and empty the NEW_TS variable again so that it can
# compile the program correctly.

set(NEW_TS "")
set_source_files_properties(${NEW_TS} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/LanguagePacks")
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/LanguagePacks")
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} ${NEW_TS} OPTIONS -source-language en)
qt5_add_translation(QM_FILES ${TS_FILES} ${NEW_TS})

# add the executable
add_executable(mpdr-qt
    ${MPDR_SOURCES}
    ${MPDR_FORMS}
    ${MPDR_HEADERS}
    ${QM_FILES}
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

#compiler definitions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1,_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING=1)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MD")
endif()

#find library
set(RANDOMPRIME_DIR $<IF:$<CONFIG:Debug>,debug,release>)

if(MSVC)
    set(RANDOMPRIME_LIB "${CMAKE_SOURCE_DIR}/randomprime/target/${RANDOMPRIME_DIR}/randomprime.lib")
elseif(UNIX)
    set(RANDOMPRIME_LIB "${CMAKE_SOURCE_DIR}/randomprime/target/${RANDOMPRIME_DIR}/librandomprime.a")
endif()

target_link_libraries(mpdr-qt PUBLIC Qt5::Widgets ${RANDOMPRIME_LIB})
target_include_directories(mpdr-qt PUBLIC ${MPDR_DIRS})
