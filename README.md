Remonttimies
============
A hobby-project raytracer with real-time OpenGL preview of the scene. The name originates from a Finnish raytracing pun.

Requirements
------------
Remonttimies requires the following libraries to run:

* Open Asset Import Library ( http://assimp.sourceforge.net/ )
* GLFW 3 ( http://www.glfw.org/ )
* GLEW ( http://glew.sourceforge.net/ )
* GLM ( http://glm.g-truc.net/ )

_assimp.dll_, _glew32.dll_ and _glfw3.dll_ need to be available for the program to run. Assimp had to be build from source, since the binaries available at the time were broken.

The project is being currently developed with Visual Studio 2013. The libraries are added to the project via Visual Studio Property Sheets.

__NOTE:__ Currently the property sheets define User Macros that point to the relevant library directories. Since the paths to these directories will most likely be different on your computer, you will need to edit the macros by hand. This will be replaced with a more sane solution later. If you want to reproduce the paths originally used, place the libraries under `../Libraries`.

How to run
----------
The program is simply run by giving the 3D model filename as the command-line parameter:

    Remonttimies.exe foo.obj

The program has only been tested with a few OBJ files so far.

__WASD keys__ move the camera forward, backward and sideways. __R__ and __F__ move the camera up and down. The arrow keys turn the camera. Pressing the __spacebar__ renders the image to _output.ppm_.

License
-------
The project is licensed under the MIT license, reproduced below.

The MIT License (MIT)

Copyright (c) 2014 Samuli Vuorinen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
