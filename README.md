Remonttimies v0.2
=================
A hobby-project raytracer with real-time OpenGL preview of the scene. Currently the raytracer supports pathtracing with Russian roulette.

![Cornell box rendered with Remonttimies](Images/cornell_100_samples_per_pixel.jpg?raw=true "Cornell box rendered with Remonttimies")
Cornell box rendered with Remonttimies

Requirements
------------
Remonttimies requires the following libraries to run:

* Open Asset Import Library ( http://assimp.sourceforge.net/ )
* GLFW 3 ( http://www.glfw.org/ )
* GLEW ( http://glew.sourceforge.net/ )
* GLM ( http://glm.g-truc.net/ )

The project is currently being developed with Visual Studio 2013. The libraries are included in the project via NuGet packages. _NupenGL Core_ was used instead of _GLEW_ and _GLFW_ because the existing _GLEW_ package refused to work with VS2013. Unfortunately _NupenGL Core_ includes the _FreeGLUT_ library, which is not actually needed. _opengl32.lib_ had to be linked manually to get the project to work.

How to run
----------
The program is simply run by giving the 3D model filename as the command-line parameter:

    Remonttimies.exe foo.obj

The program has only been tested with a few OBJ files so far.

__WASD keys__ move the camera forward, backward and sideways. __R__ and __F__ move the camera up and down. The arrow keys turn the camera. Pressing the __spacebar__ renders the image, shows it on screen and saves to _output.ppm_. Pressing __spacebar__ again hides the rendered image.

License
-------
The project is licensed under the MIT license, reproduced below.

The MIT License (MIT)

Copyright (c) 2014-2015 Samuli Vuorinen

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
