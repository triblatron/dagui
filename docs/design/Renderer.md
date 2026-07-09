# Renderer

## Design philosophy

The idea is to have a scene graph without any backend-specific data structures or code.
It acts as the data for the Renderer backend.
The scene graph contains:
* Shapes
* Rigid body transforms
* Meshes
* Textures
* Materials

Having shapes might be a mixture of abstraction levels.
We could put Meshes inside Shapes to avoid another hierarchy.
The scene graph is separate from the Widget tree and is used to implement it.
The Widget tree consists of Windows, Inputs, Layouts etc.

We could have a declarative widget tree and use diffing to update only the parts that have changed.

The widget tree could use the ConfigurationElement as its config format.

We will need to investigate what a declarative widget tree resembles.  it might be similar to XAML or qml or Flutter.
