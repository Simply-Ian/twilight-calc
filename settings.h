#ifndef CALC_SETTINGS
#define CALC_SETTINGS
#include <string>
#include <filesystem>

struct settings {
    std::filesystem::path script_folder_path;
    std::string theme;
    int win_x, win_y, win_h, win_w;
};

#endif