project("Qt Widget Inspector")

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
set(QT_USE_QTNETWORK true)
include(${QT_USE_FILE})

find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})

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

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS inspector.proto)

add_executable(
	qtinspector
	main.cpp
	${SOURCES}
	${MOC_SOURCES}
)

target_link_libraries(
	qtinspector
	QtInspector 
	${QT_LIBRARIES}
)

set(INSPECTOR_LIB_CLASSES
	DirectObjectProxy
	DirectWidgetPicker
	NetstringReader
	NetstringWriter
	InspectorServer
	ObjectIdMap
	WidgetPicker
	VariantSerializer
)

foreach(CLASS ${INSPECTOR_LIB_CLASSES})
	set(INSPECTOR_LIB_SOURCES ${INSPECTOR_LIB_SOURCES} ${CLASS}.cpp)
	set(INSPECTOR_LIB_HEADERS ${INSPECTOR_LIB_HEADERS} ${CLASS}.h)
endforeach()

qt4_wrap_cpp(INSPECTOR_LIB_MOC_SOURCES ${INSPECTOR_LIB_HEADERS})

add_library(
	QtInspector
	SHARED
	libmain.cpp
	${INSPECTOR_LIB_SOURCES}
	${INSPECTOR_LIB_MOC_SOURCES}
	${PROTO_SOURCES}
)
target_link_libraries(QtInspector ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

