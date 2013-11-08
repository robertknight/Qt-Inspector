project("Qt Widget Inspector")

cmake_minimum_required(VERSION 2.8.6)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(lib)

set(
	CLASSES
	ExternalWidgetPicker
	ObjectInspector
	ObjectTreeModel
	ObjectPropertyModel
	OutOfProcessClipboard
	WidgetInspector
	WidgetInspectorShortcut
	TargetApplicationProxy
	GdbLibraryInjector
	ExternalObjectProxy
)

foreach(CLASS ${CLASSES})
	set(SOURCES ${SOURCES} ${CLASS}.cpp)
	set(HEADERS ${HEADERS} ${CLASS}.h)
endforeach()

add_executable(
	qtinspector
	main.cpp
	${SOURCES}
)

target_link_libraries(
	qtinspector
	QtInspector 
	${QT_LIBRARIES}
)

install(TARGETS qtinspector RUNTIME DESTINATION bin)

