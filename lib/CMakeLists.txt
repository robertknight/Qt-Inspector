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

protobuf_generate_cpp(PROTO_SOURCES PROTO_HEADERS inspector.proto)

add_library(
	QtInspector
	SHARED
	libmain.cpp
	${INSPECTOR_LIB_SOURCES}
	${PROTO_SOURCES}
)
target_link_libraries(QtInspector ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

install(TARGETS QtInspector LIBRARY DESTINATION lib)
