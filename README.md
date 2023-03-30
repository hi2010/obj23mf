A program to convert obj or binary stl files to 3mf files.
Properties from obj files get ignored.

## How to build
On linux:
cd into build dir
```bash
cmake ..
cmake -j 20 build
```
On windows: ? just use VisualStudio and build (use cmakeProject = true)

## Built with
* [OBJ Loader](https://github.com/Bly7/OBJ-Loader) - For reading / conversion of obj files to 3mf files.
* [lib3mf](https://github.com/3MFConsortium/lib3mf) - Used for the c++ program. Reading and writing of 3mf files.
* [cmake](https://cmake.org/) - As build system.
* [gcc or g++](https://gcc.gnu.org/) - As compiler.

## License
Use any license you want that works with the used libraries.
The licenses of the used libs can be seen in the licenses.txt file.
