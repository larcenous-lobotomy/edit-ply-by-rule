# Problem Statement

1. Read the file.
2. Compute the area of each face in the graphical object, and sort the faces by area.
3. Each face is a triangle, hence, the formula for area of the triangle can be used.
4. For polygon with more than 3 vertices, you can pick one vertex and introduce edges
from the vertex to all other vertices, in addition to its existing edges. The new edges
“subdivide” the polygon to triangles. The area of the polygon is the sum total of the
areas of the new triangles.
5. Discard faces which are in the lower 10% of the sorted list of triangles.
6. Prune vertices which are now not shared by any faces.
7. Output the filtered surface mesh in a `.ply` file format

# Convention
The program is written following the OOP paradigm. The Task1 object inherits from the Base class, which provides methods for read, operate and write. 

# Instructions
The command `g++ test.cpp Task1.cpp Base.cpp` will compile the files and produce an executable, which when run, will dump the output to the file `output.ply`. 
