# A companion project to nodebackend that provides a GUI.

## Requirements
* User facing rather than quick throw-away debugging for developers
* Easy to use API
* User-friendly widgets
* High performance < 1ms render time per frame on all platforms
* Testable headless to verify interactions and presence of widgets
* Support tooltips with markdown
* Layout managers with constraint based layout
* Rounded rectangles
* Bezier curves with handles
* Host widgets at arbitrary points in the widget tree, such as children of rectangles, by allowing different shapes for windows
* Allow drawing circles outside the bounds of a rectangle to make a composite shape
* Allow dragging of child windows without invoking drag and drop
* Simple to integrate by rendering textured triangles
* Simple Lua based persistent format not xml or javascript
* Support disabled controls greying out
* Support tabbing between controls
* Support input validation
* Support keyboard shortcuts
* Simple event system without code generation
* Support high-quality text rendering
* Support Windows 10,11
* Support Linux Ubuntu 2204,2404
* Support macOS Intel and Apple Silicon
* Support Raspberry pi 4,5
* Support reading mouse and keyboard events
* Support Clean Architecture view model 
* Support data binding with property change events without requiring the GUI to be visible
* Support separation of visual logic from application logic
* Support declarative creation of widgets using a simple file format
* Focus and blur events
* Support text effects such as bold and underline
* Support default buttons
* Support selection similar to Windows explorer
* Support composing of widgets
* Support prototype pattern
* Support actions to allow more than one way to invoke functionality
* Support icons with text in menu items
* Support searchable menus with completion
* Support tutorial mode
* Support rigid body transforms on widgets
* Support integeration with VulkanSceneGraph
* No programmer art
* Support compound shapes as the bounds of a widget
* Client area and system area on windows
* Component system with composition over inheritance
* Texture atlas for interface elements such as system buttons and handles for collapsible panes
* Property notification system with change events
* Optional system controls and title bar
* Hit detection of shapes
* Possible boolean operations on shapes
* Activation events
* Inherit visibility from parent
* SVG shapes if parser supports translation to primitives

## Dependencies
* C++17 compiler (gcc 11+, clang16+, Visual Studio 2017+)
* CMake for the build system
* VulkanSceneGraph for integration demos
* md4c for markdown parsing
* nanosvg for SVG parsing
