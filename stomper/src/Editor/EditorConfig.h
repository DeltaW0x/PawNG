#pragma once
#include <string>
#include <nlohmann/json.hpp>
struct EditorConfig {
    std::string project = "";

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(EditorConfig,project)
};