//
// Created by Stefan on 7/16/2023.
//

#include "ArgParser.h"

namespace ModelConverter {

    ArgParser::ArgParser(std::vector<ArgOption> options) {
        m_Options = std::move(options);
    }

    void ArgParser::Parse(int argc, char* argv[]) {
        for(uint32_t i = 0; i < argc; i++) {
            std::string arg = argv[i];
            for(const auto& opt: m_Options)
                if(arg.rfind(opt.Name) != std::string::npos) {
                    if(i + 1 < argc)
                        m_Values[opt.Name] = std::string(argv[i+1]);
                    else throw std::exception("Invalid argument");
                }
        }

        for(const auto& opt: m_Options)
            if(opt.IsRequired && !m_Values.count(opt.Name))
                throw std::exception("Required argument not provided");
    }

    std::string ArgParser::GetValue(const std::string &key) const {
        if(m_Values.count(key) == 0)
            throw std::exception("Key not found!");
        return m_Values.at(key);
    }

} // ModelConverter
