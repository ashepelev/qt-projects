cmake_minimum_required(VERSION 2.8.2)

project(gipo)

find_package(Qt5Widgets REQUIRED)

if(WIN32)
	set(gui_type WIN32)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
qt5_wrap_cpp(MOC_WRAPPER mainwindow.h dialog.h about.h)

qt5_wrap_ui(UIC_WRAPPER mainwindow.ui dialog.ui about.ui)

add_executable(main ${gui_type}
            main.cpp
            mainwindow.cpp mainwindow.h
            dialog.cpp dialog.h
            about.cpp about.h
            ${MOC_WRAPPER}
            ${UIC_WRAPPER}
)

target_link_libraries(main Qt5::Widgets)
