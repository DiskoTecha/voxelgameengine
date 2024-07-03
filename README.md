<h1>Voxel-Based Game Engine</h1>

<h2>Voxels in Memory</h2>

<p>
  The voxels are kept in a "Chunk" class. The Chunk class keeps track of all the voxels in a std::vector of Color4 values, which just hold four floats. 
  A Color4 value where the alpha value (a) is zero indicates that there is no voxel in that location. This might be changed to a vector that holds a decorator for the Color4 and some extra metadata for the voxel. 
  The voxel position is based on the index of the color in the vector. It is flattened as a 1D vector from the get-go, and accessed by using three dimensional indices like a 3D array. 
  The formula for converting from 3D indices to the index in the vector is:
</p>
```c++
index = x + y * chunkSize + z * chunkSize * chunkSize;
```
<p>
  As of now, this class is pretty open, with the chunk size and voxel size easily changeable. In the future, the structure of the chunks will be stabilized and the class will become more encapsulated.
</p>

<h2>Voxels on the Screen</h2>

<p>
  The voxels are rendered by painting individual pixels onto an image, and then rendering that image in a simple vertex/fragment pipeline.
  Using OpenGL's GPGPU compute shader, the pixels to be painted are found by raymarching per pixel out into the 3D space and checking if a voxel is within that ray.
  I don't use signed distance functions or a immutable step distance when marching the rays. Instead I use a variable step distance by checking the shortest distance to the next voxel grid intersection.
  Then, you look at the next voxel it is going to hit. If there is a voxel there, then paint the pixel the color of the voxel. Otherwise, continue.
  There is a paper that details this formula which inspired me to try this out, but I forgot what it was called. Once I find it again I'll link it here. 
  The voxel data is sent to the compute shader using a buffer texture with chunk information in the first RGBA32F value, and the rest is just the chunk's flattened vector array.
  In the future, I will experiment with signed distance functions for whole chunks, to quickly march through extremely sparse areas. 
  For now, I'm still working on speeding up the compute shader as fast as possible. For a 600x600 screen, it is running about 90 fps with no voxels in it.
  As soon as there are voxels, the framerate jumps up extremely quickly, depending on the depth of the voxels from the camera. 
  I'm learning OpenGL as I create this, so everything is at a snail's pace, but at the moment I believe it's performing quite well with a basic variable step ray marching technique.
</p>
