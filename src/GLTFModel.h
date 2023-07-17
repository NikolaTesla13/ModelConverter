//
// Created by Stefan on 7/16/2023.
//

#ifndef MODELCONVERTER_GLTFMODEL_H
#define MODELCONVERTER_GLTFMODEL_H

#include <tiny_gltf.h>

#include "Model.h"

namespace ModelConverter {

    class GLTFModel : public Model {
    public:
        explicit GLTFModel(std::filesystem::path& path);
        ~GLTFModel() override;

        void WriteToFile(std::filesystem::path filename) const override;

    private:
        void BuildInternal();

    private:
        tinygltf::TinyGLTF m_Loader;
        tinygltf::Model m_Model;

        std::vector<float> m_Positions;
        std::vector<float> m_Normals;
        std::vector<float> m_TexCoords;
        std::vector<uint16_t> m_Indices;
    };

} // ModelConverter

#endif //MODELCONVERTER_GLTFMODEL_H
