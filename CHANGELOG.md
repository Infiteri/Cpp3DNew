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

# 0.1.0 - 24.09.2024

- Started Parenting
- Added parenting related functions to actors
- Transforms are now calculated correctly based on its owner
- Added child searching functions
- Changed some structural application concepts

# 0.1.1 - 01.10.2024

- Added Transform GUI
- Added Mesh GUI
- Actor component related stuff
- Added Delta Time
- Added Editor camera
- Added Fonts to GUI
- Other stuff

# 0.1.2 - 04.10.2024

- Started single file shader
- Fixed ImGui Image framebuffer issue (lost my mind (can go into details tho))
- Editor actor children get rendered
- Moving actors in the hierarchy and parenting to others is now possible

# 0.1.3 - 07.10.2024

- Update BuildVendor.bat
- Fixed mouse bug when moving the camera
- Added YAMl
- Core Serializer structure
- Started Scene serializer
- Added Component serializer
- Tied Component Serializer with Scene Serializer
- Some Renderer specific changes
- Fixed Serializer bugs
- Some code clean-up
- Fixed bugs related to textures and meshes
- Other stuff
- Fixed mistake in the CHANGELOG.md file

# 0.1.4 - 18.10.2024

- Started CubeMapTexture
- Started Sky
- Skys now have different modes
- Shader sky mode is added
- Test sky shader
- Added CeData
- Fixed issues
- Data Set components added
- Data sets get now serialized, deserialized and edited in the editor
- Sky shader refactor
- Other

# 0.1.5 - 22.10.2024

- Fixed Sky rendering issues
- Cubemap config files exist
- Material config files exist
- Editor changes to fit engine changes
- Other changes
- Scripts Fixed
- Started Content Browser Panel
- Platform API code added
- Scene changing from drag-drop
- Some postprocessing stuff
- Some fun with shaders (temporary)

# 0.1.6 - 27.10.2024

- Timer and Instrumentation (buggy TODO)
- Some postprocessing shaders
- Started platform library concept
- Added Scene Runtime to the editor
- Goofy ahh bug fixe :skull:
- Added Camera Component
- Bug fixes
- Other

# 0.1.7 - 31.10.2024

- Fixed Sky issue with color
- Light system setup
- Started lights
- Added Directional Light
- Added Point Light

# 0.1.8 - 03.11.2024

- Started Physics
- Math changes
- Fixed most of gizmo rotation bugs
- Basic Physics code
- Bodies were added
- Added Rigid Body components
- Added physics math
- Added Quaternions
- Other

# 0.1.9 - 07.11.2024

- Serializing / Deserializing scene environment added
- Refactored serializing / deserializing of CeDataSet into a class of its own
- Editor improvements
- Editor Settings
- Other improvements

# 0.2.0 - 10.11.2024

- Changed rotation to being in degrees, necessary radian casting is done in the right places.
- Started Physics Collision
- Somewhat working AABB collision detection
- Added Tag components and all the specifics
- Fixed camera mouse movement when the window is not fullscreen
- Added sphere geometry

# 0.2.1 - 23.11.2024

- Started physics contact
- Added Contact collision response
- Got mad

# 0.2.2 - 13.12.2024

- Added project to Engine
- Added buggy StaticBody
- Tied project more into engine
- Tied script library with editor
- Updated old dll's (libgcc_s_sjlj-1.dll missing fixed)
- Updated Vendor build scripts

# 0.2.3 - 21.12.2024

- Added a logger panel
- Added more functionality to the content browser panel (broken still)
- Stuff

# 0.2.4 - 05.01.2025

- Changed version year (Happy new Year)
- Changed physics to bullet physics
- Fixed bugs in the project editing tab
- Removed old code
- Fixed actor children not having their Runtime components loaded    
- Added basic post processing utility in the Renderer to be used with scripts
- Fixed bug where one actor couldn't have multiple scripts
- Removed unnecessary complexity between Renderer and Mesh (should have better performance)
- Encountered bugs with rotations (changing soon to Quaternions maybe)

# 0.2.5 - 13.01.2025

- Fixed some bugs with rotations
- Fixed aspect ratio being wrong in CameraComponent
- Added KinematicBodies and KinematicComponent (along serialization/deserialization)