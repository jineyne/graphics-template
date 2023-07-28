#pragma once

#include "pch.h"

namespace gt {
    class CommandArgsParser {
    private:
        std::map<std::string, std::string> args;
        std::vector<std::string> params;

    public:
        CommandArgsParser() = default;

    public:
        static CommandArgsParser &Get();

    public:
        void parse(int argc, char** argv);

        int hasArgument(const std::string &arg) const;
        std::string getArgumentValue(const std::string &arg) const;
        std::string getArgumentValue(const std::string &arg, const std::string &defaultValue) const;

        int getParameterCount() const;
        std::string getParameter(int index) const;
    };
}
