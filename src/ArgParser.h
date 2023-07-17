//
// Created by Stefan on 7/16/2023.
//

#pragma once

#include <initializer_list>
#include <unordered_map>
#include <string>
#include <vector>

namespace ModelConverter {

    struct ArgOption {
        std::string Name;
        bool IsRequired;
    };

    class ArgParser {
    public:
        explicit ArgParser(std::vector<ArgOption> options);

        void Parse(int argc, char* argv[]);
        [[nodiscard]] std::string GetValue(const std::string& key) const;

    private:
        std::vector<ArgOption> m_Options;
        std::unordered_map<std::string, std::string> m_Values;
    };

} // ModelConverter
