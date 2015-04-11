#Arkengine
<center>A Modern Renderer developed with C++ and Direct3D</center> 
------------
This is a hobby project of achieving a modern renderer coded in C++ D3D 11.I am planning it to extend to D3D 12 when it comes out.
Changelog 

_**Build Alpha 0.012b**_
- Fully functional FPS camera with mouse and keyboard movement
- Basic HUD text drawing with SpriteRenderer

![HUD](/Resources/Images/2015-04-11 - 14.59.29 ~ capture.png)
- Multi thread support for pipeline execution and resource creation
- UOA,SRV and Sampler parameters
- CS,SO,Domain,Geometry and Hull shaders added
- DrawIndexedInstanced executor helper class
- Phong Shading demo

![Phong](/Resources/Images/phong.png)
- GDI+ Font Loader

_**Build Alpha 0.012a**_
- Complete rework of how engine handles matrices.
- Added ArkTimer
- Actor, Node and Entity system for scene-graph
- Rework of base camera system and First Person Camera.
- Introduced Parameter Writer,current only write Constant Buffer,Vector and Matrix param.
- Basic 3D shapes for scene-graph calculations (Sphere,Ray,Plane,Frustum)
- Geometry generator for said shapes.
- More mouse and keyboard events
- Pipeline executor system introduced

_**Build Alpha 0.012**_
- Pipeline State Monitoring added for fixed and common shader stages.
- DSV,DS State,Blend State and Rasterizer state added.
- Cube demo is available to test <br>
![Cube Demo](/Resources/Images/2015-03-27 - 23.40.23 ~ capture.png)

_**Build Alpha 0.011**_
-  Parameter System Introduced
-  Currently supports for Matrix,Vector and Matrix Array
-  Shader creation,loading and reflection system introduced.
-  Pipeline system improved ( IA,OM,Rasterizer )
-  File and Log system improved

_**Build Alpha 0.010**_
- Core Win32 code.
- Event System.
- Core Direct3D code to initialize,create swap chain.
- Core pipeline code.OM,IA and Rasterizer stages are present.
- File system and Log system.





