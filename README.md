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

```
	cmake <path to Qt inspector src>
	make -j2
```

### Usage

 Qt Inspector can either attach to an existing application or launch
 a specified application and then attach to it.
 
 Recommended usage is to specify the program name and arguments.

```
	./qtinspector <program name> <args>
	./qtinspector <process ID of running Qt app>
```

### Design

 Qt Inspector operates by injecting a helper library into the target process
 when starting it using LD_PRELOAD / DYLD_INSERT_LIBRARIES or via gdb
 (if attaching to an existing process).
 
 This helper library sets up a local socket and listens for requests
 from the inspector process.  The inspector and target process communicate via
 protocol buffer messages over this socket.

