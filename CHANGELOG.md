# 0.0.1 - 02.05.2024

- Started simple build
- Started logger
- Added application and engine concept
- Structure

# 0.0.2 - 09.05.2024

- Added GLFW
- Added Scripts
- Added Window
- Engine State structure
- Started Size Modes
- Added more configuration to window (decoration, VSync, ...)
- Started Input
- Added Layers

# 0.0.3 - 10.05.2024

- Added GLAD
- Started Renderer
- Renderer structure
- Started shaders
- Added Buffer
- Added FrameBuffer
- Added VertexArray
- Reused code
- Added Shaders need
- Other stuff

# 0.0.4 - 02.06.2024

- Started objects
- Added Object Shader
- Added Viewport
- Added GPUScreen

# 0.0.5 - 22.07.2024

- Added ShaderSystem
- Reused old matrices code
- Started Cameras
- Added Vector3
- Added CameraSystem

# 0.0.6 - 22.07.2024

- Added ImGui
- Started materials
- Screen texture bug fixes
- Started Textures
- Texture structure
- Added TextureSystem
- Added "TexturePair" concept
- Added material functions for textures

# 0.0.7 - 18.09.2024

- Fixed bugs
- Started MaterialSystem
- Added material functions to meshes
- Started Geometry
- Added geometry functionality to meshes

# 0.0.8 - 21.09.2024

- Started Actors
- Started Scene
- Started World
- Tied up World in Editor Layer (ATM)
- Added Components
- MAde scenes work through the World system

# 0.0.9 - 23.09.2024

- Started Editor work
- Added Panel/PanelSystem
- Added ImGui Dockspace
- Added Scene Hierarchy Panel
- Actor UI gets rendered
- Added CeMemory
- Added Transform (along side shader transform and complexities that come along (future world/local matrices for parenting/ownership between actors))
- Added temporary position/rotation transform control in scene hierarchy panel

# 1.0.0 - 24.09.2024

- Started Parenting
- Added parenting related functions to actors
- Transforms are now calculated correctly based on its owner
- Added child searching functions
- Changed some structural application concepts

# 1.0.1 - 01.10.2024

- Added Transform GUI
- Added Mesh GUI
- Actor component related stuff
- Added Delta Time
- Added Editor camera
- Added Fonts to GUI
- Other stuff