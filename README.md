textoolkit
==========

A set of libraries, viewer program and command line utility which allows users to view, create and perform basic operations on image files.
A user can view a texture, and all of it's subtextures (layers, faces and levels) in 2D view or in builtin 3D renderer.
Currently supports BMP and DDS formats, and hopefully more in the future.

<img width="1173" height="703" alt="textoolkit" src="https://github.com/user-attachments/assets/83b8ddef-13a2-41c5-8b18-be7c8142188c" />

User may import an already existing texture into currently loaded texture by right-clicking desired layer, face or level and selecting
"import" option. This functionality is also provided from the command line utility.

Dependencies
------------

This project is using the following dependencies, which must be installed on your system in order to build and install it.

- cppevents (https://github.com/rwypior/cppevents)
- iniparser (https://github.com/rwypior/iniparser)
- libcmdline (https://github.com/rwypior/libcmdline)
- GLEW (https://github.com/nigels-com/glew)
- GLM (https://github.com/g-truc/glm)
- GLI (https://github.com/rwypior/gli)
- wxWidgets (https://github.com/wxWidgets/wxWidgets)

Building and installation
-------------------------

This project is based on CMake building system. First build and install dependencies listed above, and then use the following instruction to get, 
build and install the project.

```bash
git clone https://github.com/rwypior/textoolkit.git
cd textoolkit
mkdir build && cd build
cmake ..
sudo cmake --install .
```
