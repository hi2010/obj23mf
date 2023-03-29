#pragma once
#include "OBJ_Loader.h"
#include "SimpleLogger.h"
#include <numeric>


namespace objl {
	class StlBinaryLoader {

	public:

		// Default Constructor
		StlBinaryLoader()
		{

		}
		~StlBinaryLoader()
		{
			LoadedMeshes.clear();
		}

		// Loaded Mesh Objects
		std::vector<Mesh> LoadedMeshes;
		// Loaded Vertex Objects
		std::vector<Vertex> LoadedVertices;
		// Loaded Index Positions
		std::vector<unsigned int> LoadedIndices;
		// Loaded Material Objects
		std::vector<Material> LoadedMaterials;

		uint32_t readUint32(std::ifstream* file) {
			char data[4]{};
			for (auto i = 0; i < 4; i++) {
				file->get(data[i]);
			}
			uint32_t val;
			memcpy(&val, data, 4);
			return val;
		}

		float readReal32(std::ifstream* file) {
			char data[4]{};
			for (auto i = 0; i < 4; i++) {
				file->get(data[i]);
			}
			float f;
			memcpy(&f, &data, 4);
			return f;
		}

		Vector3 readVector3d(std::ifstream* file) {
			auto p0 = readReal32(file);
			auto p1 = readReal32(file);
			auto p2 = readReal32(file);
			return Vector3(p0, p1, p2);
		}


		bool LoadFile(std::string Path) {
			// If the file is not an .stl file return false
			if (Path.substr(Path.size() - 4, 4) != ".stl")
				return false;

			std::ifstream file(Path, std::ios::binary);

			if (!file.is_open())
				return false;

			LoadedMeshes.clear();
			LoadedVertices.clear();
			LoadedIndices.clear();

			std::vector<Vector3> Normals;

			std::vector<Vertex> Vertices;

			Mesh tempMesh;

			char stlFileHeader[80];
			file.read(stlFileHeader, 80);
			if (file.gcount() != 80) {
				return false;
			}
			std::cout << "<stl_header>";
			for (auto i = 0; i < 80; i++) {
				std::cout << stlFileHeader[i];
			}
			std::cout << "</stl_header>";
			std::cout << std::endl;
			char buffNtris[4];
			auto nTris = readUint32(&file);

			logger.log(10, "nTris: " + std::to_string(nTris));

			auto lastTellg = file.tellg();
			while(file.eof() == 0) {
				// 12 byte per read vec
				// byte 0 - 11
				auto normal = readVector3d(&file);
				// byte 12 - 23
				auto v0p = readVector3d(&file);
				// byte 24 - 35
				auto v1p = readVector3d(&file);
				// byte 36 - 47
				auto v2p = readVector3d(&file);

				auto v0 = Vertex();
				v0.Normal = normal;
				v0.Position = v0p;

				auto v1 = Vertex();
				v1.Normal = normal;
				v1.Position = v1p;

				auto v2 = Vertex();
				v2.Normal = normal;
				v2.Position = v2p;

				Normals.push_back(normal);
				Vertices.push_back(v0);
				Vertices.push_back(v1);
				Vertices.push_back(v2);

				// 16 bit that should be 0; (attribute byte count)
				// trash to bytes -> 48 - 49
				char trash;
				file.get(trash);
				file.get(trash);
				if (file.eof()) {
					logger.log(10, "reached eof");
					break;
				}
			}
			logger.log(0, "read verts, could read: "+ std::to_string(Vertices.size()));

			if (Vertices.size() != ((size_t)nTris * 3)) {
				logger.log(8, "n verts from file and readable differ");
				logger.log(8, "isVerts: " + std::to_string(Vertices.size()) + " shouldVerts: " + std::to_string(nTris*3));
			}
			nTris = Vertices.size() / 3;

			tempMesh.Vertices = Vertices;
			tempMesh.Indices = std::vector<unsigned int>(Vertices.size());
			std::iota(std::begin(tempMesh.Indices), std::end(tempMesh.Indices), 0);
			file.close();

			LoadedMeshes.push_back(tempMesh);

			if (LoadedMeshes.empty() && LoadedVertices.empty() && LoadedIndices.empty())
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		
	};
}