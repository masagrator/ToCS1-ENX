#pragma once

/* Shared state of the ToCS1 translation plugin (port of Tfoaf/main_patch.hpp). */

#include "lib.hpp"
#include <nn/fs.hpp>
#include <nn/hid.hpp>
#include <nn/oe.hpp>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>

#define SAVE_DIR  "sd:/config/ToCS1"
#define SAVE_PATH SAVE_DIR "/subsdk9.save"

struct TextWidth {
    std::string text;
    int width;
};
inline TextWidth temp;

inline int CPEP_width = 0;
inline int UITextcase = 0;
inline uintptr_t NSO_main_start = 0;

inline ptrdiff_t returnInstructionOffset(uintptr_t LR) {
    return LR - NSO_main_start;
}

inline bool BlockButtons = false;

inline nn::hid::NpadHandheldState out;
inline nn::hid::NpadJoyDualState out2;
inline nn::hid::NpadFullKeyState out3;

inline const char magic[5] = "MS00";

inline constexpr int PerformanceConfig_GPU307mhz = 0x00020003;
inline constexpr int PerformanceConfig_GPU460mhz = 0x92220008;

struct GameSettings {
    int Audio = 0;
    int FPS = 30;
    int RenderingRes = 4;
    int GPUBoost = 0;
};
inline GameSettings Settings;

/* UI text draw call. Declared here because the hidden menu draws with the original function. */
HOOK_DEFINE_TRAMPOLINE(SetUIText) {
    static int Callback(void* x0, int X_Pos, int Y_Pos, const char* string, int ARGB_Color, int ARGB_Shadow, int ARGB_Border, int w7, float s0, float s1, float fontsize, float X_Scale);
};

/* Read the held buttons of every controller kind the menu reacts to. */
inline void ReadPads() {
    nn::hid::GetNpadState(&out, nn::hid::CONTROLLER_HANDHELD);
    nn::hid::GetNpadState(&out2, nn::hid::CONTROLLER_PLAYER_1);
    nn::hid::GetNpadState(&out3, nn::hid::CONTROLLER_PLAYER_1);
}

inline bool AnyPadHeld(u64 key) {
    return (out.Buttons & key) || (out2.Buttons & key) || (out3.Buttons & key);
}

inline void SetGpuBoost(bool on) {
    nn::oe::SetPerformanceConfiguration(nn::oe::Normal, on ? PerformanceConfig_GPU460mhz : PerformanceConfig_GPU307mhz);
}

inline void SaveSettings() {
    nn::fs::FileHandle handle;
    if (!nn::fs::OpenFile(&handle, SAVE_PATH, nn::fs::OpenMode_Write)) {
        nn::fs::WriteFile(handle, 0, &magic, 4, nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));
        nn::fs::WriteFile(handle, 4, &Settings, sizeof(Settings), nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));
        nn::fs::CloseFile(handle);
    }
}
