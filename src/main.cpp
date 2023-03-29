#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// P* are pointers
#include "lib3mf_implicit.hpp"
#include "OBJ_Loader.h"
#include "stl_binary_loader.h"
#include "licensesAsStr.hpp"

#include "SimpleLogger.h"

using namespace Lib3MF;

typedef std::vector<objl::Mesh>::size_type tMeshIdx;
typedef std::vector<objl::Vertex>::size_type tVertIdx;
typedef std::vector<unsigned int>::size_type tIndIdx;


void convertObjlMesh23mfMesh(objl::Mesh* meshObjl, PMeshObject mesh3mf) {
    std::string strNVerts = std::to_string(meshObjl->Vertices.size());
    std::string strNIdcs = std::to_string(meshObjl->Indices.size());
    logger.log(0, "in convertObjlMesh23mfMesh, nVertices: "+strNVerts+" nIdcs: "+strNIdcs);


    // copy vertices
    for (tVertIdx i = 0; i < meshObjl->Vertices.size(); i ++) {
        auto curPos = meshObjl->Vertices.at(i).Position;
        sPosition p = {curPos.X, curPos.Y, curPos.Z};
        mesh3mf->AddVertex(p);
    }

    // copy indices -> tris have 3 indices
    // this is valid because the loader triangulates the input data if n verts in face > 3
    for (tIndIdx i = 0; i < meshObjl->Indices.size()-2; i += 3) {
        sTriangle tri = {meshObjl->Indices.at(i), meshObjl->Indices.at(i+1), meshObjl->Indices.at(i+2)};
        mesh3mf->AddTriangle(tri);
    }

    mesh3mf->SetName(meshObjl->MeshName);
    // TODO: add material properties
}

// returns false if it failed
bool loadObjFileAndAdd2Model(std::string inPath, PModel resModel, PWrapper wrapper) {
    // create loader and load file
    logger.log(0, "trying to load input file");
    objl::Loader Loader;
    bool loaded = Loader.LoadFile(inPath);
    bool useBinStlLoader = false;
    objl::StlBinaryLoader binStlLoader;

    if (loaded == false) {
        logger.log(9, "input file path is invalid, loading failed");
        //return false;
    }
    if (loaded == false) {
        logger.log(1, "trying bin stl");
        loaded = binStlLoader.LoadFile(inPath);
        logger.log(10, "bin stl success: " + std::to_string(loaded));
        if (loaded == false) {
            logger.log(9, "input file path is invalid, loading failed");
            return false;
        }
        useBinStlLoader = true;
    }
    logger.log(0, "input file loaded successfull");

    // loop over the meshes and add them to the new model
    size_t nMeshes;
    if (useBinStlLoader == false) {
        nMeshes = Loader.LoadedMeshes.size();
        logger.log(5, "3mf: convert: nMeshes: " + std::to_string(nMeshes));
        for (tMeshIdx i = 0; i < nMeshes; i++) {
            objl::Mesh curMesh = Loader.LoadedMeshes.at(i);
            auto mesh3mf = resModel->AddMeshObject();
            convertObjlMesh23mfMesh(&curMesh, mesh3mf);

            // Add build item
            resModel->AddBuildItem(mesh3mf.get(), wrapper->GetIdentityTransform());
        }
    }
    else {
        nMeshes = binStlLoader.LoadedMeshes.size();
        logger.log(5, "stl: convert: nMeshes: " + std::to_string(nMeshes));
        for (tMeshIdx i = 0; i < nMeshes; i++) {
            objl::Mesh curMesh = binStlLoader.LoadedMeshes.at(i);
            auto mesh3mf = resModel->AddMeshObject();
            convertObjlMesh23mfMesh(&curMesh, mesh3mf);

            // Add build item
            resModel->AddBuildItem(mesh3mf.get(), wrapper->GetIdentityTransform());
        }
    }
    logger.log(0, "converted "+std::to_string(nMeshes)+" meshes");

    return true;
}

int handleInputArgs(int argc, char* argv[], std::string& inFileP, std::string& outFileP) {
    if (argc == 2) {
        logger.log(0, "only input path given");
        inFileP = argv[1];
        auto dotPos = inFileP.find_last_of(".");
        logger.log(0, "dot position is: "+std::to_string(dotPos));
        if (dotPos == std::string::npos || dotPos == 0) {
            return -1;
        }
        std::string inFormat = inFileP.substr(dotPos+1);
        logger.log(0, "in format is: " + inFormat);
        std::string strObj = "obj";
        if (inFormat.compare(strObj) && 0) {
            // wrong file format only obj supported
            return -1;
        }
        logger.log(0, "in format is: " + inFormat);
        std::cout << dotPos << std::endl;
        outFileP = inFileP.substr(0, dotPos) + ".3mf";
    } else {
        // argc == 3
        inFileP = argv[1];
        outFileP = argv[2];
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // allowed modes are: input file, output file in same folder (1 arg + this)=2
    // and input file and output file (2 args + this)=3
    if (argc < 2 || argc > 3) {
        logger.log(9, "wrong usage, this program only supports usage like: \"programname inputFilePath (outputPath)\"\n outputPath is optional, if none is given the input path with .3mf ending is chosen \n\n you can also insert license to see the licenses of used libs");
        return -1;
    }
    std::string inFileP;
    std::string outFileP;
    bool argSuccess = handleInputArgs(argc, argv, inFileP, outFileP);
    if (argSuccess < 0) {
        logger.log(9, "not the right amount of input args were given");
        return -1;
    }
    logger.log(0, "process started with input path: " + inFileP + " outputFilePath: " + outFileP);

    if (inFileP == "licenses" || inFileP == "license") {
        logger.log(9, "selected licesnes, will \"only\" print lincenses");
        logger.log(9, LICENSES);
        // successfull
        return 0;
    }

    PWrapper wrapper = CWrapper::loadLibrary();
    // from here on the library loading worked
    PModel model = wrapper->CreateModel();
    auto mod = model->AddMeshObject();
    
    auto loadSuccess = loadObjFileAndAdd2Model(inFileP, model, wrapper);
    if (loadSuccess == false) {
        return -1;
    }
    
    logger.log(0, "3mf model created, preparing to write");
    PWriter writer = model->QueryWriter("3mf");
    logger.log(0, "writer object initialized");
    writer->WriteToFile(outFileP);
    logger.log(0, "3mf model written to disk");
    
    // read obj file, then convert it to a mesh obj and add it to the model after that write it out
    logger.log(4, "file from:\n" + inFileP + "\nhas been written to:\n" + outFileP);    
}