cmake_minimum_required(VERSION 2.8.9)

project("Qt Widget Inspector")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(USE_QT5 OFF)
if (NOT USE_QT5)
  message("Building with Qt 4. Enable the USE_QT5 option to build with Qt 5")
endif()

if (USE_QT5)
  find_package(Qt5Core REQUIRED)
  find_package(Qt5Concurrent REQUIRED)
  find_package(Qt5Network REQUIRED)
  find_package(Qt5Widgets REQUIRED)
else()
  find_package(Qt4 REQUIRED)
  set(QT_USE_QTNETWORK true)
  include(${QT_USE_FILE})
endif()

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

if (MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

add_subdirectory(lib)

set(
	CLASSES
	ExternalObjectProxy
	ExternalWidgetPicker
	ObjectInspector
	ObjectPropertyModel
	ObjectTreeModel
	OutOfProcessClipboard
	TargetApplicationProxy
	WidgetInspector
	WidgetInspectorShortcut
)

if(UNIX)
  set(CLASSES ${CLASSES} GdbLibraryInjector PreloadInjector)
endif()

foreach(CLASS ${CLASSES})
	set(SOURCES ${SOURCES} ${CLASS}.cpp)
	set(HEADERS ${HEADERS} ${CLASS}.h)
endforeach()

add_executable(
	qtinspector
	main.cpp
	${SOURCES}
)
if (UNIX AND NOT APPLE)
	# compile main.cpp as position-independent code so
	# that taking the address of the qtInspectorInit symbol
	# in order to get the path of the libQtInspector library works
	# as expected.
	#
	# See the notes in the 'Bugs' section for the dladdr() function
	# at http://linux.die.net/man/3/dlopen
	set_target_properties(qtinspector PROPERTIES COMPILE_FLAGS -fPIC)
endif()

target_link_libraries(
	qtinspector
	QtInspector 
	${QT_LIBRARIES}
)

if (USE_QT5)
  qt5_use_modules(qtinspector Network Widgets)
endif()

install(TARGETS qtinspector RUNTIME DESTINATION bin)

