Qt Inspector
============

Qt Inspector is a tool to browse the object/widget tree of Qt applications.

![Screenshot](http://2.bp.blogspot.com/-gZW9n7V-zgs/TiinbhFZNpI/AAAAAAAAABc/F3zpGbS73bE/s1600/inspector-dolphin-settings.png)


Qt Inspector starts a specified application or connects to a specified process
and once connected it can:

 - Browse the object tree of Qt applications.
 - View properties of objects
 - Edit properties of objects
 - Locate a widget in the object tree by clicking on it in the application

### Building Qt inspector

Qt Inspector currently runs on Linux and Mac.

You will need `protoc` in your PATH. On Debian/Ubuntu, install `protobuf-compiler`.

```
	cmake <path to Qt inspector src>
	make -j2
```

### Usage

#### without gdb

 Qt Inspector can either attach to an existing application or launch
 a specified application and then attach to it.
 
 Recommended usage is to specify the program name and arguments.

```
	./qtinspector <program name> <args>
	./qtinspector <process ID of running Qt app>
```

#### with gdb (linux workflow)

 Attach gdb with the running Qt Application and Inside gdb load the libQtInspector.so

```
(gdb) call dlopen("/path/to/libQtInspector.so",2)
(gdb) print (char *) dlerror() // After dlopen , you can check for errors
(gdb) call qtInspectorInit()  //This is final call to start qtInspector

```

> It's a good idea to check the dependency of libQtInspector.so (using ldd) before opening in gdb try to resolve all the dependencies by appending the required .so files to LD_LIBRARY_PATH

### Design

 Qt Inspector operates by injecting a helper library into the target process
 when starting it using LD_PRELOAD / DYLD_INSERT_LIBRARIES or via gdb
 (if attaching to an existing process).
 
 This helper library sets up a local socket and listens for requests
 from the inspector process.  The inspector and target process communicate via
 protocol buffer messages over this socket.

