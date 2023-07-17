#include <filesystem>
#include <iostream>

#include "ArgParser.h"
#include "GLTFModel.h"

int main(int argc, char* argv[]) {
    try {
        ModelConverter::ArgParser parser({{.Name = "model", .IsRequired = true}});
        parser.Parse(argc, argv);

        std::filesystem::path filepath = std::string(parser.GetValue("model"));
        if(!std::filesystem::exists(filepath)) {
            throw std::exception("File does not exist");
        }

        ModelConverter::GLTFModel model(filepath);

        auto out = filepath.replace_extension("");
        model.WriteToFile(out);
    } catch (std::exception& e) {
        std::cout << e.what();
    }
    return 0;
}
