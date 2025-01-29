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
