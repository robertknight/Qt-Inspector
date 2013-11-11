set(INSPECTOR_LIB_CLASSES
	DirectObjectProxy
	DirectWidgetPicker
	NetstringReader
	NetstringWriter
	InspectorServer
	ObjectIdMap
	PlatformUtils
	StartupHelper
	WidgetPicker
	VariantSerializer
)

foreach(CLASS ${INSPECTOR_LIB_CLASSES})
	set(INSPECTOR_LIB_SOURCES ${INSPECTOR_LIB_SOURCES} ${CLASS}.cpp)
	set(INSPECTOR_LIB_HEADERS ${INSPECTOR_LIB_HEADERS} ${CLASS}.h)
endforeach()

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS inspector.proto)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_definitions(-DLIB_QTINSPECTOR_BUILD)

add_library(
	QtInspector
	SHARED
	libmain.cpp
	${INSPECTOR_LIB_SOURCES}
	${PROTO_SOURCES}
)
target_link_libraries(QtInspector ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (USE_QT5)
  qt5_use_modules(QtInspector Concurrent Widgets Network)
endif()

install(TARGETS QtInspector DESTINATION lib)
