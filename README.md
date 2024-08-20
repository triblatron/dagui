# A GUI for nodebackend

## Progress

* Shapes hit test
  * ~~Rounded rectangle~~
  * ~~Circle~~
  * ~~CompositeShape: union of Shape~~
* Widget
  * has-a Shape
* ~~Config tree~~
* Window
  * round-tripping of features and status to string and back to flags

## Requirements
* User facing rather than quick throw-away debugging for developers
  * Use ImGUI for the latter
* Easy to use API
  * basic tasks should be easy
  * advanced tasks ahould be straightforward
* Expose widget tree to user of API
  * scene graph with rigid body transforms
  * strict tree
  * rendering backend
    * Use concept of painter?
      * draw primitives such as Shape
    * Generate textured triangles
    * Provide interface to be implemented by integration
    * Agnostic to graphics API
      * OpenGL
      * Vulkan
      * Metal
      * DirectX
    * Needs to be efficient
      * submit batches of triangles rather than one vertex per call like in the days of old
    * Generate text for rendering by integration
      * puts dependencies in integration
        * harfbuzz
        * pango
        * freetype
* Efficiency promoting API
  * Pass by value and allow optimiser to make it fast
  * check whether move constructor can be elided
  * Use std::vector rather than std::list or std::map wherever possible to maximise cache usage.
  * roll our own based on std::vector
  * use stack wrapping a vector
  * use index chasing end for queue
    * keeps memory so not suitable for large queues 
  * use a pool allocator to improve locality of reference for std::list
  * use indexing because they are not invalidated by adding elements at the end
  * reserve() elements when we know how many will be added
  * chasing pointers causes cache misses with at least 10x slowdown
* User-friendly widgets
  * Make it obvious how to use them
    * A clear small set of interactions
    * consistency so that a particular interaction always has the same effect where available
    * no tutorial required 
    * no internet search required
  * Provide help on why a widget is disabled/unavailable
    * tooltips even when widget is disabled
    * hover events
* ~~Support config tree for declarative layout of widgets~~
* Support notion of nothing to do
  * no events
  * no redrawing
  * no frames
* Support canvas
  * Grid with snap
  * Zoom
  * Scrollable infinitely
  * Pan
  * Fit to view
    * Set bounds to show all content
    * Orthographic projection matrix?
      * Would need to be 4x4
* Support pop-up menu
    * searchable
    * list matches as actions as for pull-down menus
* Support scientific visualisation
  * graphs of signals against time
  * live capture
  * playback
  * multiple plots on one graph
  * multiple graphs
  * easy splitting of view
  * select signals
  * same signal can appear on more than one plot
* Support notion of time provider
  * Allow faking of time to test time-dependent event dispatch 
* Support scheduling of events
* Support concept of space occupied by a widget
  * Reserve space on creation
  * Give back space on deletion
* Support animation of properties
  * position
  * rotation
  * colour
  * alpha
  * spatial occupancy e.g. creation makes room, deletion takes back space
* Support menu bar
  * menus accessible by mouse or keyboard
  * searchable with listing of matches as actions
* Support STL containers in widgets
  * std::list where insertion in middle is common
  * template parameter of container with sensible default
* track cursor position for events
  * can be used to position child windows
* Support child windows as first class citizens
  * dragging happens automatically without having to compute coordinates in event handler
* Support dual-listbox widget
  * available list
  * active list
  * transfer items between lists
  * move items up and down lists
  * use std::list and splicing operations
  * explorer-style selection:
    * Ctrl-click
    * Shift-click
    * Drag a box
  * no indexing
* Support MVC models
  * ideally using POD types without having to inherit framework classes
* Support panes
  * scrollable when required
  * proportinoally sized scroll bars 
* Support markdown in text
  * labels
  * text fields
  * tooltips
* Support different mouse cursor
  * small image
  * property of a widget
* Support mouse enter and leave events
  * start and stop operation
* Support tables
  * configurable alternating background colours
  * tooltips to show full text of cells where it is truncated
  * variable column width by dragging splitters
  * draggable columns
  * custom sorting of data
* Support trees
  * expand node
  * collapse node
  * expand all
  * collapse all
* Support tabbed toolbar
  * avoid design patent
* Support status bar
* Support notifications
  * disappear after configurable period
* Support user settings config
  * Use same format
  * generate settings dialogue
* Support explorer style selection by meta key and click combinations or dragging
  * for all widgets that contain other widgets such as panes
* Support drag and drop modes including none to allow dragging of child windows
  * Allow drag and drop of icons in a palette onto the work area to instantiate an object
  * Allow dragging of child windows without triggering a transfer
* Allow dragging from any point on a child window
* Support prototype pattern by clone()ing widgets from a library into a tree.
  * be careful about which elements are deep copied
* High performance < 1ms render time per frame on all platforms
  * Raspberry Pi 4 1920x1080@60
  * Raspberry Pi 5 1920x1080@120?
  * Windows/Linux/Mac Apple Silicon 4k@120Hz
  * Mac Intel 1920x1080@60Hz
* Testable headless to verify interactions and presence of widgets
  * Use identifiers of widgets rather than positions to make tests robust to layout changes
* Efficient representation to avoid redundant state changes
  * Sort by texture to avoid swapping repeatedly
  * Use texture atlases for fonts and icons to limit number of textures required
  * Sort transparent polygons after opaque polygons to get correct rendering order.
  * Generate whichever is more efficient: indexed triangles or triangle strips.
    * Profile on target hardware, possibly change answer on discrete desktop GPU versus Pi 4.
    * Selectable at runtime by passing different mode token in OpenGL or topology token in Vulkan.
    * Might change based on what is being drawn: text versus widgets
* Support tooltips with markdown
  * Allow Unicode codepoints such as emojis and small images as arbitrary icons.
  * Allow rich content without the overhead of (X)HTML/CSS/Javascript.
* Layout managers with constraint based layout
  * Use well known algorithm suitable for GUIs
* ~~Rounded rectangles~~
* ~~Circles~~
* Bezier curves with handles
  * For signal paths
  * Multi-segment to support complex signal paths
  * quadratic/one handle should be sufficient
  * optional
* Support lines with handles as a simpler alternative to Bezier curves
  * For signal paths
  * Multi-segment to support complex signal paths
  * one handle to change alignment
  * required
* Host widgets at arbitrary points in the widget tree, such as children of rectangles, by allowing different shapes for widgets
* ~~Allow drawing circles outside the bounds of a rectangle to make a composite shape~~
  * should we allow 2D boolean operations?
    * would require a binary tree with operators at the internal nodes and primitives at the leaf nodes
    * can use an external library to solve drawing as long as it generates triangles
* Simple to integrate by rendering textured triangles
  * OpenGL for simplicity
  * Vulkan for performance
* Support push buttons
  * default control
  * click event handler
* Support checkboxes
  * individual or as items in another control
* Support radio buttons
  * individual or as items in another control
  * exclusion groups
* Support disabled controls greying out
  * using colour manipulation or artist input
* Support tabbing between controls
  * Fires onblur as focus leaves a control
  * Fires onfocus as focus enters a control
  * Allow configurable tab order to increase convenience of using keyboard and accessibility
    * Try to avoid dead-ends such as text areas where tab is interpreted as a tab character
    * Provide a meta-key combination to toggle the interpretation of tab by a control between moving focus and control-specific
* Support splitters
* Support collapsible panes
* Support horizontal and vertical layout
* Support grid layout 
* Support input validation
  * Restrict input to valid characters based on expected type and range
  * give tooltip help on why an input is invalid
* Support keyboard accelerators
  * Modify menu item and label text to include shortcut
  * Detect combination
    * State machine e.g. Ctrl + Shift + C can be done in different orders like Shift + Ctrl + C so it is best thought of as a chord followed by a character rather than a pure sequence
* Simple event system without code generation
* Support high-quality text rendering
* Support Windows 10,11
* Support Linux Ubuntu 2204,2404
* Support macOS on Intel, Apple Silicon
* Support Raspberry pi 4,5 Debian 12
* Support reading mouse and keyboard events
  * abstraction of native events
* Support Clean Architecture view model 
* Support data binding with property change events without requiring the GUI to be visible
* Support separation of visual logic from application logic
  * visual logic goes in the code behind
    * should we use nodes to evaluate the visual logic?
      * could connect clicked output on a button to enabled on another control
      * more conplex scenarios would still require code behind so it might be more complexity for not much benefit
* Support declarative creation of widgets using a simple file format
* Focus and blur events
* Support text effects such as bold and underline
  * not as important because we will put the shortcut in a tooltip with a different font
* Use modern C++ techniques
  * lamdas as callbacks
  * std::thread, std::mutex and std::atomic<>
  * range-based for
  * for-scope local variables
  * std::unique_ptr<> and std::make_unique<>() to automate memory management and avoid leaks, use after free, dangling pointers etc.
    * be wary of performance overhead
* Support actions to allow more than one way to invoke functionality
* Support icons with text in menu items
* Support searchable menus with completion
  * Benchmark trie versus substring for partial matching
    * substring wins by orders of magnitude, but still takes 1100ns to search on a representative set of Blender-style menu paths
* Support tutorial mode
  * Dynamic disabling of widgets to guide user
  * Help notifications
  * Highlighting of widgets
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
* Hit detection of shapes
* Support keyboard shortcuts
* Possible boolean operations on shapes
* Activation events
* Inherit visibility from parent
* SVG shapes if parser supports translation to primitives
* No code generation
  * At least C++, could allow Lua instead because it does not require explicit compilation
    * Supported in Lua persistent format as Lua event handlers
    * might need to SWIG the C++ API to make this possible
    * alternative is to have yielding to C++ when a service is required.
      * lightweight but possibly restrictive
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
    * might have head-explosion issues
* Messaging system similar to signal-slots without the MOC-related cruft.
  * put it behind an interface
  * thread safe
  * no blocking of gui thread
  * cannot use futures since they are for sending values to another thread
  * Could use coroutines if we bump to C++20 or use Boost or similar
    * another dependency
  * It is unclear how to call a method in another thread safely
  * locks but how do we make it work for an arbitrary method on an arbitrary object?
* Integration
  * define interface to generate textured triangles
  * define interface to generate text
  * define interface to rasterise SVG
  * they might be the same
  * rasterising text is required of the Integration
## Dependencies
* C++17 compiler (gcc 11+, clang16+, Visual Studio 2017+)
* Lua-5.4.x for the declarative format
* CMake for the build system
* VulkanSceneGraph for integration demos
* md4c for markdown parsing
* svgpp for SVG parsing
* nodebase for basic features such as Lua support, configuration and streams.
* math library with 2D, 3D vectors and 3x3 matrices

## Tips
* SWIG xml mode works only if we %define NFE_API %enddef, otherwise it just generaates code tags instead of the parse tree.
