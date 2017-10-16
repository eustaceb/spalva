# Spalva - A small scale graphics engine
Spalva is a C++ OpenGL rendering engine that provides the infrastructure for developing and running visual simulations. It is in continuous development as I am learning new techniques in computer graphics, therefore the project is subject to frequent changes and refactoring. The target audience is someone who is starting OpenGL programming and is looking for examples or a straightforward environment to develop in.

### Key Features
 * Resource management
 * Multiple scene support
 * Model loading
 * Scene-specific easily configurable GUI
 * Dynamically generated shader configuration GUI element
 * A selection of renderable objects
 * Custom renderers

### Controls
 * Pressing **Alt** shows/hides the interface.
 * Camera is controlled with **WASD** and mouse (when GUI is hidden).
 * **Space** turns on wireframe mode.
 * **Esc** to quit.

### Required libraries:
 * [GLFW](http://www.glfw.org/)
 * [Glad](http://glad.dav1d.de/)
 * [GLM](https://glm.g-truc.net/)
 * [Stb_Image](https://github.com/nothings/stb/blob/master/stb_image.h)
 * [Assimp](https://github.com/assimp/assimp) (model loading)
 * [ImGUI](https://github.com/ocornut/imgui) (comes with Spalva)

## Resource Manager
The **Resource Manager** is a key component that divides resources into resource groups, allows lazy loading and enables caching. A specific memory limit (cap) and a caching flag has to be set in Definitions.h for it to work properly. 
If the memory limit is reached, a LRU optimization is launched, removing the least recently used inactive resource group. Each resource has to be registered with the manager on instantiation as resources are loaded through it.
![ResourceManager](https://i.imgur.com/I8ObDOm.png)

## Examples
**Spalva** comes with multiple simulations that serve as an example.
### Particles scene
A particle emitter in a black background with many options to configure.
![Particles scene](https://i.imgur.com/Z9m2xqy.gif)
### Basic scene
Contains a skybox, loaded model and multiple configurable point lights with Phong shading and attenuation.
![Basic scene](http://i.imgur.com/s6LB6YC.png)
### Adjustable scene
![Adjustable scene](http://i.imgur.com/9rMFad1.png)
Allows spawning objects at specifiable coordinates and some minimal configuration. Object modification - transformations, shader and texture selection is a work in progress.
### Skybox scene
![Skybox scene](http://i.imgur.com/8m1Hale.jpg)
A cube that has reflective and refractive options, mapping the environment (skybox).
### Instancing scene
![Instancing scene](http://i.imgur.com/qrV3TK1.jpg)
A planet simulation that allows to change the count, radius and dispersion of asteroids. An example of a technique called instancing that allows drawing a huge amount of objects while maintaining good performance.
### Advanced Lightning scene
![Advanced Lightning scene](http://i.imgur.com/8CPYDzq.jpg)
A directional light that casts shadows, together with Blinn-Phong shading.

## Tips
 * If you get an error saying that you need assimp.dll (in my case, assimp-vc140-mt.dll) to start the program, compiling assimp on your system will produce the said dll file. You will need to place it where the compiled binaries are.

## Things I borrowed
Most of this engine was built while doing [Joey de Vries](https://twitter.com/JoeyDeVriez) [tutorials](https://learnopengl.com/), therefore some of the examples closely resemble his work. All models and textures were taken from the series as well. The skyboxes came from [93i.de](https://93i.de/p/free-skybox-texture-set/) and [StumpyStrust](https://opengameart.org/content/space-skyboxes-0).