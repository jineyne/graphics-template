#include "CommandArgsParser.h"

namespace gt {
    CommandArgsParser &CommandArgsParser::Get() {
        static CommandArgsParser instance;
        return instance;
    }

    void CommandArgsParser::parse(int argc, char** argv) {
        for (int i = 1; i < argc; ++i) {
            std::string arg(argv[i]);

            if (arg[0] == '-') {
                std::string::size_type pos = arg.find('=', 2);
                if (pos == std::string::npos) {
                    std::string flag = arg.substr(arg[1] == '-' ? 2 : 1);
                    args[flag] = "";
                } else {
                    std::string key = arg.substr(2, pos - 2);
                    std::string value = arg.substr(pos + 1);
                    args[key] = value;
                }
            } else {
                params.push_back(arg);
            }
        }
    }

    int CommandArgsParser::hasArgument(const std::string &arg) const {
        return args.find(arg) != args.end();
    }

    std::string CommandArgsParser::getArgumentValue(const std::string &arg) const {
        auto it = args.find(arg);
        if (it == args.end()) {
            throw std::out_of_range("Invalid argument");
        }

        return it->second;
    }

    std::string CommandArgsParser::getArgumentValue(const std::string &arg, const std::string &defaultValue) const {
        auto it = args.find(arg);
        if (it == args.end()) {
            return defaultValue;
        }

        return it->second;
    }

    int CommandArgsParser::getParameterCount() const {
        return params.size();
    }

    std::string CommandArgsParser::getParameter(int index) const {
        if (index < 0 || index >= params.size()) {
            throw std::out_of_range("Invalid parameter index");
        }

        return params[index];
    }
}
