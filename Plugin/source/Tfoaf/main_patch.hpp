#pragma once

#include "skyline/inlinehook/And64InlineHook.hpp"
#include "skyline/utils/cpputils.hpp"
#include "skyline/inlinehook/memcpy_controlled.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include "nn/fs.h"
#include "nn/hid.hpp"
#include "nn/oe.h"
#define LINKABLE __attribute__ ((weak))

extern "C" {
	extern void _ZN2nn3hid12GetNpadStateEPNS0_16NpadFullKeyStateERKj(nn::hid::NpadFullKeyState * KeyState, unsigned int const& NpadID) LINKABLE;
	extern void _ZN2nn3hid12GetNpadStateEPNS0_16NpadJoyDualStateERKj(nn::hid::NpadJoyDualState * KeyState, unsigned int const& NpadID) LINKABLE;
	extern void _ZN2nn3hid12GetNpadStateEPNS0_17NpadHandheldStateERKj(nn::hid::NpadHandheldState * KeyState, unsigned int const& NpadID) LINKABLE;
}

struct TextWidth {
	std::string text;
	int width;
} temp;

int CPEP_width = 0;

int UITextcase = 0;

uintptr_t NSO_main_start = 0;

ptrdiff_t returnInstructionOffset(uintptr_t LR) {
	return LR - NSO_main_start;
}

int8_t EngCheck = 0;
bool enSelected = false;
bool BlockButtons = false;

nn::hid::NpadHandheldState out;
nn::hid::NpadJoyDualState out2;
nn::hid::NpadFullKeyState out3;

int (*set_UIText_original)(void* x0, int X_Pos, int Y_Pos, const char* string, int ARGB_Color, int ARGB_Shadow, int ARGB_Border, int w7, float s0, float s1, float fontsize, float X_Scale);

const char magic[5] = "MS00";

int PerformanceConfig_GPU307mhz = 0x00020003;
int PerformanceConfig_GPU460mhz = 0x92220008;