#include "utils/GLTFLoader.h"

#include <algorithm>
#include "core/SimulationApp.h"
#include "utils/SystemUtil.hpp"

#include <iostream>
#include <vector>
#include <tiny_gltf.h>

namespace sf
{

    Mesh *LoadGLTF(const std::string &path, GLfloat scale)
    {
        std::cout << "loading gltf from path: " << path << "\n";
        tinygltf::Model model;
        tinygltf::TinyGLTF loader;
        std::string err, warn;

        TexturableMesh *mesh_ = new TexturableMesh;
        std::vector<Face> faces;
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, path);
        // For binary files use:
        // bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, path);

        if (!warn.empty())
        {
            cError("Warning: %s!", warn);
        }
        if (!err.empty())
        {
            cError("Error: %s!", err);
        }
        if (!ret)
        {
            cError("Failed to load glTF file.");
            return mesh_;
        }

        for (const auto &mesh : model.meshes)
        {
            for (const auto &primitive : mesh.primitives)
            {
                if (primitive.mode != TINYGLTF_MODE_TRIANGLES)
                {
                    cError("Non-triangle primitive encountered, skipping.");
                    continue;
                }

                // load faces
                const tinygltf::Accessor &indexAccessor = model.accessors[primitive.indices];
                const tinygltf::BufferView &bufferView = model.bufferViews[indexAccessor.bufferView];
                const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
                const unsigned char *data = buffer.data.data() + bufferView.byteOffset + indexAccessor.byteOffset;

                for (size_t i = 0; i < indexAccessor.count; i += 3)
                {
                    Face face;
                    if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
                    {

                        face.vertexID[0] = ((unsigned short *)data)[i];
                        face.vertexID[1] = ((unsigned short *)data)[i + 1];
                        face.vertexID[2] = ((unsigned short *)data)[i + 2];
                    }
                    else if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
                    {
                        face.vertexID[0] = ((unsigned int *)data)[i];
                        face.vertexID[1] = ((unsigned int *)data)[i + 1];
                        face.vertexID[2] = ((unsigned int *)data)[i + 2];
                    }
                    mesh_->faces.push_back(face);
                }

                // Load vertex positions
                const auto &posAccessor = model.accessors[primitive.attributes.at("POSITION")];
                const auto &posBufferView = model.bufferViews[posAccessor.bufferView];
                const auto &posBuffer = model.buffers[posBufferView.buffer];
                const float *posData = reinterpret_cast<const float *>(
                    posBuffer.data.data() + posBufferView.byteOffset + posAccessor.byteOffset);
                // vertex normals
                const auto &normalAccessor = model.accessors[primitive.attributes.at("NORMAL")];
                const auto &normalBufferView = model.bufferViews[normalAccessor.bufferView];
                const auto &normalBuffer = model.buffers[normalBufferView.buffer];
                const float *normalData = reinterpret_cast<const float *>(
                    normalBuffer.data.data() + normalBufferView.byteOffset + normalAccessor.byteOffset);
                // uv coords
                const auto &uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto &uvBufferView = model.bufferViews[uvAccessor.bufferView];
                const auto &uvBuffer = model.buffers[uvBufferView.buffer];
                const float *uvData = reinterpret_cast<const float *>(
                    uvBuffer.data.data() + uvBufferView.byteOffset + uvAccessor.byteOffset);

                size_t vertexCount = posAccessor.count;
                TexturableVertex v;
                for (size_t i = 0; i < vertexCount; ++i)
                {
                    v.pos.x = posData[i * 3 + 0];
                    v.pos.y = posData[i * 3 + 1];
                    v.pos.z = posData[i * 3 + 2];

                    v.normal.x = normalData[i * 3 + 0];
                    v.normal.y = normalData[i * 3 + 1];
                    v.normal.z = normalData[i * 3 + 2];

                    v.uv.x = uvData[i * 2 + 0]; // u
                    v.uv.y = uvData[i * 2 + 1]; // v
                    mesh_->vertices.push_back(v);
                }
            }
        }

        return mesh_;
    }

}
