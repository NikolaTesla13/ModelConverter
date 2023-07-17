//
// Created by Stefan on 7/16/2023.
//

#ifndef MODELCONVERTER_MODEL_H
#define MODELCONVERTER_MODEL_H

#include <filesystem>
#include <string>

namespace ModelConverter {

    class Model {
    public:
        explicit Model(std::filesystem::path& path) {};
        virtual ~Model() = default;

        virtual void WriteToFile(std::filesystem::path filename) const = 0;
    };

} // ModelConverter

#endif //MODELCONVERTER_MODEL_H
