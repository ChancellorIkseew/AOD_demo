#pragma once
#include <string>
#include <unordered_map>
#include "binding.hpp"

class Controls {
    static std::unordered_map<std::string, Binding> bindings;
public:
    static const std::unordered_map<std::string, Binding>& getBindings() { return bindings; }
    static void addBinding(const std::string& bindName, int code, InputType type);
    static void initDefaults();
};
