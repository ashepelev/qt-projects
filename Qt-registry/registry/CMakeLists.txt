cmake_minimum_required(VERSION 2.8.2)
project(registryedit)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR})
ADD_DEFINITIONS(${QT_DEFINITIONS})

set(Registry_CPP about.cpp dialog.cpp main.cpp mainwindow.cpp registrymodel.cpp spinboxdelegate.cpp tablemodel.cpp)
set(Registry_H about.h dialog.h mainwindow.h registrymodel.h spinboxdelegate.h tablemodel.h)
set(Registry_UI about.ui dialog.ui mainwindow.ui)
set(Registry_TS registry_ru.ts)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option (UPDATE_TRANSLATIONS "Update source translation (files translations /*.ts).")
list(APPEND TRANSLATE_FILES ${Registry_CPP} ${Registry_H} ${Registry_UI})
set(TS_FILES registry_ru.ts)
set(QM_FILES registry_ru.qm)

if(UPDATE_TRANSLATIONS)
	message(
		WARNING
		" UPDATE_TRANSLATIONS_WITH_DANGER option is set , turn it off "
		" after generating translation files as soon as possible !")
	foreach(src_name IN LISTS TRANSLATE_FILES)
		get_filename_component(
			src_path ${src_name} ABSOLUTE)
		list(APPEND TRANSLATE_PATHS ${src_path})
	endforeach()
	qt5_create_translation(QM_FILES ${TRANSLATE_PATHS} ${TS_FILES})
else()
	qt5_add_translation(QM_FILES ${TS_FILES})
endif()

add_custom_target(translations DEPENDS ${QM_FILES})

qt5_wrap_cpp(Registry_H_MOC ${Registry_H})
qt5_wrap_ui(Registry_UI_H ${Registry_UI})

if(WIN32)
  set(gui_type WIN32)
endif()

add_executable(registry ${gui_type} ${Registry_CPP} ${Registry_H_MOC} ${Registry_H} ${Registry_UI_H})
#target_link_libraries(registry ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
target_link_libraries(registry Qt5::Widgets)
set(QM_FILES registry_ru.qm)
install(TARGETS registry RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_PREFIX}/)
