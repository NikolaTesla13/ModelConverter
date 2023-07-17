//
// Created by Stefan on 7/16/2023.
//

#include <fstream>
#include <stb_image_write.h>

#include "GLTFModel.h"

namespace ModelConverter {

    GLTFModel::GLTFModel(std::filesystem::path& path) : Model(path) {
        std::string ext = path.extension().string(), err, warn;
        bool success;

        if(ext == ".glb") {
            success = m_Loader.LoadBinaryFromFile(&m_Model, &err, &warn, path.string());
        } else if(ext == ".gltf") {
            success = m_Loader.LoadASCIIFromFile(&m_Model, &err, &warn, path.string());
        } else throw std::exception("Unknown file extension");

        if(!success)
            throw std::exception(err.c_str());

        BuildInternal();
    }

    GLTFModel::~GLTFModel() = default;

    void GLTFModel::BuildInternal() {
        for(const auto& mesh: m_Model.meshes) {
            for(const auto& primitive: mesh.primitives) {
                const tinygltf::Accessor &positionsAccessor = m_Model.accessors[primitive.attributes.at("POSITION")];
                const tinygltf::BufferView &positionsBufferView = m_Model.bufferViews[positionsAccessor.bufferView];
                const tinygltf::Buffer &positionsBuffer = m_Model.buffers[positionsBufferView.buffer];
                const auto *positions = reinterpret_cast<const float *>(&positionsBuffer.data[positionsBufferView.byteOffset +
                                                                                              positionsAccessor.byteOffset]);

                m_Positions.reserve(positionsAccessor.count * 3);
                for (size_t i = 0; i < positionsAccessor.count; ++i) {
                    m_Positions.push_back(positions[i * 3 + 0]);
                    m_Positions.push_back(positions[i * 3 + 1]);
                    m_Positions.push_back(positions[i * 3 + 2]);
                }

                const tinygltf::Accessor &normalsAccessor = m_Model.accessors[primitive.attributes.at("NORMAL")];
                const tinygltf::BufferView &normalsBufferView = m_Model.bufferViews[normalsAccessor.bufferView];
                const tinygltf::Buffer &normalsBuffer = m_Model.buffers[normalsBufferView.buffer];
                const auto *normals = reinterpret_cast<const float *>(&normalsBuffer.data[normalsBufferView.byteOffset +
                        normalsAccessor.byteOffset]);

                m_Normals.reserve(normalsAccessor.count * 3);
                for (size_t i = 0; i < normalsAccessor.count; ++i) {
                    m_Normals.push_back(normals[i * 3 + 0]);
                    m_Normals.push_back(normals[i * 3 + 1]);
                    m_Normals.push_back(normals[i * 3 + 2]);
                }

                if(primitive.attributes.contains("TEXCOORD_0")) {
                    const tinygltf::Accessor &texCoordAccessor = m_Model.accessors[primitive.attributes.at(
                            "TEXCOORD_0")];
                    const tinygltf::BufferView &texCoordBufferView = m_Model.bufferViews[normalsAccessor.bufferView];
                    const tinygltf::Buffer &texCoordBuffer = m_Model.buffers[normalsBufferView.buffer];
                    const auto *texCoords = reinterpret_cast<const float *>(&texCoordBuffer.data[
                            texCoordBufferView.byteOffset +
                            texCoordAccessor.byteOffset]);

                    m_TexCoords.reserve(texCoordAccessor.count * 3);
                    for (size_t i = 0; i < texCoordAccessor.count; ++i) {
                        m_TexCoords.push_back(texCoords[i * 3 + 0]);
                        m_TexCoords.push_back(texCoords[i * 3 + 1]);
                        m_TexCoords.push_back(texCoords[i * 3 + 2]);
                    }
                }

                const tinygltf::Accessor &indicesAccessor = m_Model.accessors[primitive.indices];
                const tinygltf::BufferView &indicesBufferView = m_Model.bufferViews[indicesAccessor.bufferView];
                const tinygltf::Buffer &indicesBuffer = m_Model.buffers[indicesBufferView.buffer];
                const auto *indicesData = reinterpret_cast<const uint16_t *>(&indicesBuffer.data[
                        indicesAccessor.byteOffset + indicesBufferView.byteOffset]);

                m_Indices.reserve(indicesAccessor.count);
                for (size_t i = 0; i < indicesAccessor.count / 3; i++) {
                    m_Indices.push_back(indicesData[i * 3 + 0] + 1);
                    m_Indices.push_back(indicesData[i * 3 + 1] + 1);
                    m_Indices.push_back(indicesData[i * 3 + 2] + 1);
                }
            }
        }

    }

    void GLTFModel::WriteToFile(std::filesystem::path filename) const {
        const float scale = 0.01f;
        std::ofstream out(filename.replace_extension(".obj"));

        out << "# " << m_Positions.size() / 3 << " vertices\n";

        for (int i = 0; i < m_Positions.size(); i += 3) {
            out << "v " << m_Positions[i] * scale << " " << m_Positions[i + 1] * scale << " "
                << m_Positions[i + 2] * scale << '\n';
        }

        for (int i = 0; i < m_Normals.size(); i += 3) {
            out << "vn " << m_Normals[i] * scale << " " << m_Normals[i + 1] * scale << " " << m_Normals[i + 2] * scale
                << '\n';
        }

        for (int i = 0; i < m_Normals.size(); i += 3) {
            out << "vt " << m_Normals[i] * scale << " " << m_Normals[i + 1] * scale << " " << m_Normals[i + 2] * scale
                << '\n';
        }

        for (int i = 0; i < m_Indices.size(); i += 3) {
            auto fmt = std::format("f {}/{}/{} {}/{}/{} {}/{}/{}", m_Indices[i], m_Indices[i], m_Indices[i],
                                   m_Indices[i + 1], m_Indices[i + 1], m_Indices[i + 1], m_Indices[i + 2],
                                   m_Indices[i + 2], m_Indices[i + 2]);
            out << fmt << '\n';
        }

        out.close();

        if(!m_TexCoords.empty()) {
            auto texturePath = filename.replace_extension(".bmp").string();
            stbi_write_bmp(texturePath.c_str(), m_Model.images[0].width, m_Model.images[0].height,
                           m_Model.images[0].component, m_Model.images[0].image.data());
        }
    }

} // ModelConverter