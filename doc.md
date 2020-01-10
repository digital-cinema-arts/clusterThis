/**
@mainpage clusterThis mantra geometry instancer

The @b clusterThis mantra geometry instancing procedural DSO/OTL will instance geometry to
incoming points and will process the point attributes in a number of ways for the instanced geometry.
Delays geomtery instantiation until render time thus, reduces size of IFD file and network bandwidth,
allows access to render time data, add additional detail to scene.

@b Features:
@li Instance primitives at render time (sphere, cube, grid, tube, circle, curve, metaball)
@li Instance disk file (and apply CVEX deformation and attribute processing)
@li Open VDB post processing of instanced geometry and it's attributes
@li Motion blur
@li CVEX processing of instanced geometry
@li Cache file stores data when generating deep shadow maps to optimize instancing.

@b clusterThis is maintained by
<A TARGET=_blank HREF="http://www.digitalcinemaarts.com/software/">Digital Cinema Arts</A>
and was developed primarily by
- Debra Peri

See the @subpage overview "Overview" for an introduction to clusterThis.

See the @subpage faq "FAQ" for frequently asked questions about @b clusterThis.

See the @subpage exampleScenes "Examples" for examples of @b clusterThis.

See the @subpage changes "Release Notes" for what's new in this version of @b clusterThis.

@page overview clusterThis Overview

@section Contents
- @ref secParameters
- @ref secInstall
- @ref secNotes
- @ref secDesignNotes
- @ref secDebugTips
<!-- - @ref secRegistry -->

@section secParameters Parameters

The current set of parameters are:

@b Setup @b Tab:
@li  Min/Max Bounds: The bounding box of the objects/volume being instanced
@li Use Geo File: The filename of th egeometry file to use instead point cloud from the IFD file.
@li Type: The type of geometry to instance
@li Num. Copies:	Number of copies of the geometry to instance per point of input geometry
@li Recursion: Number of copies of the geometry per copy per point of input geometry
@li Radius: The distance from the center of the point to the instanced geometry
@li Size: The size (X, Y, Z) of the instanced geometry primitive
@li Frequency:	Frequency X,Y,Z
@li Offset: Offset X,Y,Z
@li Instance Probability: Controls the amount of "randomness" in choosing to instance an object or not.
@li Instance Geometry File: Geometry disk file to be instanced (is there's no geo_fname point attribute)
@li Deferred Instancing: This will generate additional VRAY_Procedural objects, which then instance the geometry as needed by mantra. If unchecked, all the
geometry will be instanced at one time
@li Motion Blur: Motion blur type: None, Velocity or Deformation
@li Shutter: The open and close shutter value for motion blur
@li Messages: The verbosity of the messages sent to the console during rendering

@b Noise @b Tab:
@li Type: Simple noise generator noise types Perlin, Sparse, Alligator
@li Amplitude: Amplitude of noise applied to the position of the instanced geometry Noise Roughness: The "roughness" of the noise function
@li Roughness: Roughness of the generated noise
@li Fractal Depth: Depth of recursion for the noise generator
@li Attenuation: Amount of attenuation applied to the noise generator
@li Seed: Seed for the noise generator

@b CVEX @b Tab:

@li Instance: Will run CVEX on each instance
@li Pre-Process: Will run CVEX on incoming point cloud
@li Instance: Will run CVEX on the entire set of instanced geometry

@li CVEX Point Shader: The file name of the .vex shader to be used on points
@li Execute Point VEX Code: Execute the VEX code on the instanced points
@li CVEX Primitive Shader: The file name of the .vex shader to be used on prinitves
@li Execute Primitive VEX Code: Execute the VEX code on the instanced primitives

@li CVEX Vars (point) 	The point attributes sent to the CVEX code for processing
@li CVEX Vars (primitive) 	The primitive attributes sent to the CVEX code for processing

@b Defaults @b Tab:
@li TODO

@b Post Process @b Tab:
@li Enable
@li Particle Footprint
@li WS Units
@li Instance Offset
@li Voxel Size
@li Radius Min
@li Half Bandwidth
@li Fog Volume
@li Gradient Width
@li Falloff
@li Filter: Median Mean, Mean Curvature, Laplacian. Offset (with offset amount)
@li Position Influence
@li Velocity Influence
@li Normal Influence
@li Write Debug Files

@b Misc @b Tab:
@li Temp File Path: Temporary file for the instanced geometry
@li Save the geometry in a temp cache file (for optimization)
@li Save temp cache file: 	Do not delete cache file (to save the temp geometry cache file for later use)


@section secInstall Installation

To install the "clusterThis" mantra procedural DSO, there are 4 components that need to be installed: <br>
 <br>
1) The "executable" that runs when mantra calls the clusterThis functions <br>
2) The VRAYprocedural configuration file to tell mantra where to find the clusterThis DSO or DLL <br>
3) The OTL, which is the interface to the mantra geometry procedural allowing the user to control it's behaviour <br>
4) The OpenVDB libraries <br>


@b Step @b 1:
<pre>
The "executables":
VRAY_clusterThis.so
VRAY_clusterThis.dylib

Place one of these (depending on operating system) in a typical mantra DSO/DLL location, e.g.:
$HOME/houdiniX.XX/dso/mantra/
$JOB/houdiniX.XX/dso/mantra/
</pre>


@b Step @b 2:
<pre>
The mantra procedural configuration file:
VRAYprocedural

Place this file (or append the contents to your current VRAYprocedural file) in a location searched by Houdini:
$HOME/houdiniX.XX/
$JOB/houdiniX.XX/
</pre>


@b Step @b 3:
<pre>
Geometry Shader OTL:
VM_GEO_clusterThis.otl

Place this file in a location searched by Houdini:
$HOME/houdiniX.XX/otls/
$JOB/houdiniX.XX/otls/
</pre>

@b Step @b 4:
<pre>
The Open VDB Libraries:
Copy the openvdb lib files (libopenvdb.so& libopenvdb_houdini.so) to a location on your system if you do not already have them installed.  Be sure to set the LD_LIBRARY_PATH if needed to find the libaries.
</pre>

@section secNotes Notes

NOTES:


@section secDesignNotes

Flowcharts:

@b Overall mantra procedural call sequence:
http://www.digitalcinemaarts.com/dev/clusterThis/design/clusterThis_overall.pdf

@b render() method algorithm:
http://www.digitalcinemaarts.com/dev/clusterThis/design/clusterThis_flowchart.pdf

@b postProcess() algorithm:
http://www.digitalcinemaarts.com/dev/clusterThis/design/clusterThis_postProcess.pdf

@section secDebugTips Debug Tips

1) To make sure the clusterThis DSO/DLL is available to mantra, from a UNIX (or CygWIN on Windows) shell, execute the following command:

<pre>
earth:~> mantra -V4
Registering procedural 'clusterThis'
Registering procedural 'sprite'
Registering procedural 'image3d'
Registering procedural 'metapoly'
Registering procedural 'file'
Registering procedural 'fur'
Registering procedural 'program'
Registering procedural 'hscriptinstance'
Registering procedural 'image3dvolume'
mantra Version 9.5.169 (Compiled on 07/17/08)
</pre>

The verbosity level of 4 or greater for mantra will report which mantra procedural DSO's are available.
If clusterThis is not in this list, the DSO is not in the right location, or the VRAYprocedural file isn't correct.




*/
