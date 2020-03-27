cmake_minimum_required(VERSION 3.5)

project(mpdr-qt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INSTALL_PREFIX /usr)

if(MSVC)
    set(QT_EXTRA_COMPONENTS WinExtras)
elseif(APPLE)
    set(QT_EXTRA_COMPONENTS MacExtras)
endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools Help WebEngineWidgets ${QT_EXTRA_COMPONENTS} REQUIRED)

### add project files
# find headers and source files
file(GLOB_RECURSE MPDR_SOURCES "DoorRandomizer/*.cpp")
file(GLOB_RECURSE MPDR_HEADERS "DoorRandomizer/*.h")
file(GLOB_RECURSE MPDR_FORMS "DoorRandomizer/*.ui")
file(GLOB_RECURSE TS_FILES "LanguagePacks/*.ts")

set(MPDR_DIRS "")
foreach (_headerFile ${MPDR_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND MPDR_DIRS ${_dir})
endforeach()

# translations
# To make or update a new translation file, fill the NEW_TS variable with "LanguagePacks/mpdr_x.ts"
# where "x" is the locale for a corresponding language.
# Run CMake, build the project and empty the NEW_TS variable again so that it can
# compile the program correctly.

set(NEW_TS "")
set_source_files_properties(${NEW_TS} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/LanguagePacks")
set_source_files_properties(${TS_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/LanguagePacks")
qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES} ${NEW_TS} OPTIONS -source-language en)
qt5_add_translation(QM_FILES ${TS_FILES} ${NEW_TS})
qt5_add_resources(MPDR_SOURCES "${CMAKE_SOURCE_DIR}/mpdr.qrc")

# add the executable
if(MSVC)
    add_executable(mpdr-qt
        WIN32
        ${MPDR_SOURCES}
        ${MPDR_FORMS}
        ${MPDR_HEADERS}
        ${QM_FILES}
        mpdr.rc
    )
else()
    add_executable(mpdr-qt
        ${MPDR_SOURCES}
        ${MPDR_FORMS}
        ${MPDR_HEADERS}
        ${QM_FILES}
        ${RESOURCE_FILES}
      )
endif()

### randomprime

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeRust/cmake/")

enable_language(Rust)
include(CMakeCargo)

add_subdirectory(randomprime)

#compiler definitions
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS=1,_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING=1)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /MD")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mpdr-qt)
endif()

#make install configuration
if(UNIX)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/mpdr-qt.svg DESTINATION share/icons/hicolor/scalable/apps/)
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/mpdr.desktop DESTINATION share/applications/)
    install(TARGETS mpdr-qt DESTINATION bin)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share/mpdr)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/LanguagePacks DESTINATION share/mpdr FILES_MATCHING PATTERN "*.qm")
    install(DIRECTORY ${CMAKE_PREFIX_PATH}/share/qt/translations DESTINATION share/mpdr FILES_MATCHING PATTERN "qtbase_*")
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources/maps DESTINATION share/mpdr/resources)
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resources/img DESTINATION share/mpdr/resources)
endif()

# add documentation
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/doc/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/doc)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/maps DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/img DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(GLOB DOC_PROJECTS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "doc/*.qhcp")
if(MSVC)
    foreach(DOC_PROJECT ${DOC_PROJECTS})
        string(REPLACE ".qhcp" ".qhc" DOC_CONTENTS ${DOC_PROJECT})
        add_custom_command(TARGET mpdr-qt POST_BUILD COMMAND ${CMAKE_PREFIX_PATH}/bin/qhelpgenerator.exe ARGS "${CMAKE_CURRENT_BINARY_DIR}/${DOC_PROJECT}" -o ${CMAKE_CURRENT_BINARY_DIR}/${DOC_CONTENTS})
    endforeach()
elseif(UNIX)
    foreach(DOC_PROJECT ${DOC_PROJECTS})
        string(REPLACE ".qhcp" ".qhc" DOC_CONTENTS ${DOC_PROJECT})
        add_custom_command(TARGET mpdr-qt POST_BUILD COMMAND ${CMAKE_PREFIX_PATH}/bin/qhelpgenerator ARGS "${CMAKE_CURRENT_BINARY_DIR}/${DOC_PROJECT}" -o "${CMAKE_CURRENT_BINARY_DIR}/${DOC_CONTENTS}")
    endforeach()
endif()

#find library
set(RANDOMPRIME_DIR $<IF:$<CONFIG:Debug>,debug,release>)

if(MSVC)
    set(RANDOMPRIME_LIB Qt5::WinExtras ws2_32 userenv advapi32)
elseif(APPLE)
    set(RANDOMPRIME_LIB Qt5::MacExtras)
elseif(UNIX)
    set(RANDOMPRIME_LIB libdl.so pthread.so)
endif()

target_link_libraries(mpdr-qt PUBLIC Qt5::Widgets Qt5::Help Qt5::WebEngineWidgets randomprime ${RANDOMPRIME_LIB})
target_include_directories(mpdr-qt PUBLIC ${MPDR_DIRS})
