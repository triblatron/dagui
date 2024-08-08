# A GUI for nodebackend

## Requirements
* User facing rather than quick throw-away debugging for developers
* Easy to use API
* User-friendly widgets
  * Provide help on why a widget is disabled/unavailable
    * tooltips even when widget is disabled
    * hover events
* ~~Support config tree for declarative layout of widgets~~
* Support tables with configurable alternating background colours
* Support drag and drop modes including none to allow dragging of child windows
  * Allow drag and drop of icons in a palette onto the work area to instantiate an object
  * Allow dragging of child windows without triggering a transfer
* Allow dragging from any point on a child window
* High performance < 1ms render time per frame on all platforms
  * Raspberry Pi 4 1920x1080@60
  * Raspberry Pi 5 1920x1080@120?
  * Windows/Linux/Mac Apple Silicon 4k@120Hz
  * Mac Intel 1920x1080@60Hz
* Testable headless to verify interactions and presence of widgets
* Efficient representation to avoid redundant state changes
* Support tooltips with markdown
* Layout managers with constraint based layout
  * Use well known algorithm suitable for GUIs
* ~~Rounded rectangles~~
* ~~Circles~~
* Bezier curves with handles
  * For signal paths
  * Multi-segment to support complex signal paths
  * quadratic/one handle should be sufficient
* Host widgets at arbitrary points in the widget tree, such as children of rectangles, by allowing different shapes for widgets
* ~~Allow drawing circles outside the bounds of a rectangle to make a composite shape~~
  * should we allow 2D boolean operations?
    * would require a binary tree with operators at the nodes
* Simple to integrate by rendering textured triangles
  * OpenGL for simplicity
  * Vulkan for performance
* Simple Lua-based persistent format not xml or javascript
* Support push buttons
* Support checkboxes
* Support radio buttons
* Support disabled controls greying out
  * using colour manipulation or artist input
* Support tabbing between controls
* Support splitters
* Support collapsible panes
* Support trees
* Support horizontal and vertical layout
* Support grid layout 
* Support input validation
* Support keyboard shortcuts
* Simple event system without code generation
* Support high-quality text rendering
* Support Windows 10,11
* Support Linux Ubuntu 2204,2404
* Support macOS on Intel, Apple Silicon
* Support Raspberry pi 4,5
* Support reading mouse and keyboard events
* Support Clean Architecture view model 
* Support data binding with property change events without requiring the GUI to be visible
* Support separation of visual logic from application logic
  * visual logic goes in the code behind
    * should we use nodes to evaluate the visual logic?
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
  * Benchmark trie versus substring for partial matching
* Support tutorial mode
* Support rigid body transforms on widgets
* Support integration with VulkanSceneGraph
* No programmer art
  * Need an artist to create visual snippets such as close icon and collapse handle
* Support compound shapes as the bounds of a widget
* Client area and system area on windows
* Component system with composition over inheritance
* Texture atlas for interface elements such as system buttons and handles for collapsible panes
* Property notification system with change events
* Optional system controls and title bar
* Hit detection of shapesSupport keyboard shortcuts
* Possible boolean operations on shapes
* Activation events
* Inherit visibility from parent
* SVG shapes if parser supports translation to primitives
* No code generation
* No interference with the CMake configure and generate process.
* One obvious place to change to add a new property, event, whatever.
* Modular composable components
  * Entity-component system?
  * Widgets as nodes?
* Render only changed areas of the view
* Separate visual style from structure
  * stylesheets?
  * Skinnable
* Designer using its own widgets
  * Generate declarative Lua config
  * self hosting: design the designer using the designer
* Messaging system similar to signal-slots without the MOC-related cruft.
  * thread safe
  * no blocking of gui thread
  * cannot use futurws since they are for sending values to another thread
  * Could use coroutines if we bump to C++20
  * It is unclear how to call a method in another thread safely

## Dependencies
* C++17 compiler (gcc 11+, clang16+, Visual Studio 2017+)
* Lua-5.4.x for the declarative format
* CMake for the build system
* VulkanSceneGraph for integration demos
* md4c for markdown parsing
* svgpp for SVG parsing
* nodebase for basic features such as Lua support, configuration and streams.

## Tips
* SWIG xml mode works only if we %define NFE_API %enddef, otherwise it just generaates code tags instead of the parse tree.
