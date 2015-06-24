cmake_minimum_required(VERSION 2.8.2)
project(canny)
find_package(Qt5Widgets REQUIRED)
find_package(OpenCV REQUIRED)

set(CPPS mainwindow.cpp worker.cpp main.cpp about.cpp)
set(HS mainwindow.h worker.h about.h)
set(UIS mainwindow.ui about.ui)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt5_wrap_cpp(H_MOC ${HS})
qt5_wrap_ui(H_UI ${UIS})

if (WIN32)
	set(gui_type WIN32)
endif()

add_executable(canny ${gui_type} ${CPPS} ${H_MOC} ${HS} ${H_UI})
target_link_libraries(canny Qt5::Widgets ${OpenCV_LIBS})
install(TARGETS canny RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/)