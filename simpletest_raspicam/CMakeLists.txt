#####################################
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake/${CMAKE_MODULE_PATH}") 
cmake_minimum_required (VERSION 2.8) 
project (raspicam_test)
find_package(raspicam REQUIRED)
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
#add_executable (simpletest_raspicam simpletest_raspicam.cpp)
#add_executable (simpletest_raspicam raspicam_still_test.cpp) # for bmp
#add_executable (simpletest_raspicam draw.cpp) # for draw bitmap
#add_executable (simpletest_raspicam raspicam_test.cpp)  
add_executable (simpletest_raspicam server.cpp)  #for socket server.cpp
#add_executable (simpletest_raspicam gui_hello.cpp)  #for GUI
#add_executable (simpletest_raspicam camera_gui.cpp)  #for GUI & camera
#add_executable (simpletest_raspicam gtk_hello.cpp)  #gtk
target_link_libraries (simpletest_raspicam ${raspicam_LIBS} X11 ${GTK3_LIBRARIES})
#####################################
