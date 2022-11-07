A program to convert obj files to 3mf files.
Properties from obj files get ignored.

## How to build
Currently only works on linux.
To use with windows: adjust the lib3mf that is included and prbly the cmake file and recompile for windows.

On linux:
cd into build dir
```bash
cmake ..
cmake -j 20 build
```
On windows probably the same after the whole mess with compiler or cmake not in %PATH% is "fixed".
If this does not work check if the dynamic lib lib3mf gets copied to the correct path.

## Built with
* [OBJ Loader](https://github.com/Bly7/OBJ-Loader) - For reading / conversion of obj files to 3mf files.
* [lib3mf](https://github.com/3MFConsortium/lib3mf) - Used for the c++ program. Reading and writing of 3mf files.
* [cmake](https://cmake.org/) - As build system.
* [gcc or g++](https://gcc.gnu.org/) - As compiler.

## License
Use any license you want that works with the used libraries.
The licenses of the used libs can be seen in the licenses.txt file.
