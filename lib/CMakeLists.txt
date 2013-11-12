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

add_custom_command(OUTPUT inspector.pb.h inspector.pb.cc
  COMMAND ${PROTOBUF_PROTOC_EXECUTABLE} --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=dllexport_decl=LIB_QTI_EXPORT:${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/inspector.proto
  DEPENDS inspector.proto
)

include_directories(${PROTOBUF_INCLUDE_DIR})

add_definitions(-DLIB_QTINSPECTOR_BUILD)

if(WIN32)
  set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
endif()

add_library(
	QtInspector
	SHARED
	libmain.cpp
	inspector.pb.cc
	${INSPECTOR_LIB_SOURCES}
)
target_link_libraries(QtInspector ${QT_LIBRARIES} ${PROTOBUF_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})


if(UNIX)
  set(ADD_INCLUDE_FLAG "-include ")
elseif(WIN32)
  set(ADD_INCLUDE_FLAG "/FI")
endif()
set_source_files_properties(inspector.pb.cc PROPERTIES COMPILE_FLAGS ${ADD_INCLUDE_FLAG}${CMAKE_CURRENT_SOURCE_DIR}/../libQtInspectorExport.h)

if (USE_QT5)
  qt5_use_modules(QtInspector Concurrent Widgets Network)
endif()

install(TARGETS QtInspector DESTINATION lib)
