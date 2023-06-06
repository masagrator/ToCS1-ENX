#pragma once

struct GameSettings {
    int Audio = 0;
    int FPS = 30;
    int RenderingRes = 4;
    int GPUBoost = 0;
} Settings;

int8_t indicator = 0;
uint8_t options_count = 3;

uint64_t (*TextureMaker_original)(void* x0, void* x1, uint32_t width, uint32_t height, void* x4, void* x5, void* x6, void* x7);
uint64_t TextureMaker_hook(void* x0, void* x1, uint32_t width, uint32_t height, void* x4, void* x5, void* x6, void* x7) {
    uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	ptrdiff_t offsetItr = returnInstructionOffset(LR);
    if (offsetItr == 0x14D808) {
        switch(Settings.RenderingRes) {
            case 0:
                return TextureMaker_original(x0, x1, 640, 360, x4, x5, x6, x7);
            case 1:
                return TextureMaker_original(x0, x1, 853, 480, x4, x5, x6, x7);
            case 2:
                return TextureMaker_original(x0, x1, 960, 540, x4, x5, x6, x7);
            case 3:
                return TextureMaker_original(x0, x1, 1120, 630, x4, x5, x6, x7);
            case 5:
                return TextureMaker_original(x0, x1, 1440, 810, x4, x5, x6, x7);
            case 6:
                return TextureMaker_original(x0, x1, 1600, 900, x4, x5, x6, x7);
            case 7:
                return TextureMaker_original(x0, x1, 1760, 990, x4, x5, x6, x7);
            case 8:
                return TextureMaker_original(x0, x1, 1920, 1080, x4, x5, x6, x7);
            default:
                return TextureMaker_original(x0, x1, 1280, 720, x4, x5, x6, x7);
        }
    }
    else return TextureMaker_original(x0, x1, width, height, x4, x5, x6, x7);
}

uint64_t (*RenderingRes_original)(void* unk, uint32_t width, uint32_t height);
uint64_t RenderingRes_hook(void* unk, uint32_t width, uint32_t height) {
    switch(Settings.RenderingRes) {
        case 0:
            return RenderingRes_original(unk, 640, 360);
        case 1:
            return RenderingRes_original(unk, 853, 480);
        case 2:
            return RenderingRes_original(unk, 960, 540);
        case 3:
            return RenderingRes_original(unk, 1120, 630);
        case 5:
            return RenderingRes_original(unk, 1440, 810);
        case 6:
            return RenderingRes_original(unk, 1600, 900);
        case 7:
            return RenderingRes_original(unk, 1760, 990);
        case 8:
            return RenderingRes_original(unk, 1920, 1080);
        default:
            return RenderingRes_original(unk, 1280, 720);
    }
}

uint64_t (*FPSlock_original)(void* unk, uint32_t FPStarget);
uint64_t FPSlock_hook(void* unk, uint32_t FPStarget) {
    static void* unk_holder = 0;
    if (unk != nullptr)
        unk_holder = unk;
    else 
        unk = unk_holder;
    if (Settings.FPS == 30)
        return FPSlock_original(unk, FPStarget);
    else
        return FPSlock_original(unk, Settings.FPS);
}

void RenderHiddenMenu(void* x0) {
    static std::string block = "█";
    set_UIText_original(x0, -150, -100, block.c_str(), 0xFF000000, 0x00000000, 0x00000000, 0, 0, 1.0, 768.0, 2.0);

    int base_Y = 80;
    
    set_UIText_original(x0, 80, base_Y+(32*0), "FPS:", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    if (Settings.FPS == 30) {
        set_UIText_original(x0, 260, base_Y+(32*0), "[30]", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
        set_UIText_original(x0, 460, base_Y+(32*0), "60", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    }
    else {
        set_UIText_original(x0, 260, base_Y+(32*0), "30", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
        set_UIText_original(x0, 460, base_Y+(32*0), "[60]", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    }
    set_UIText_original(x0, 80, base_Y+(32*1), "Rendering Resolution:", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    char* RenderRes = 0;
    switch(Settings.RenderingRes) {
        case 0:
            RenderRes = (char*)&"[640x360]";
            break;
        case 1:
            RenderRes = (char*)&"[853x480]";
            break;
        case 2:
            RenderRes = (char*)&"[960x540]";
            break;
        case 3:
            RenderRes = (char*)&"[1120x630]";
            break;
        case 5:
            RenderRes = (char*)&"[1440x810]";
            break;
        case 6:
            RenderRes = (char*)&"[1600x900]";
            break;
        case 7:
            RenderRes = (char*)&"[1760x990]";
            break;
        case 8:
            RenderRes = (char*)&"[1920x1080]";
            break;
        default:
            RenderRes = (char*)&"[1280x720] (Default)";
            break;
    }
    set_UIText_original(x0, 460, base_Y+(32*1), RenderRes, 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

    set_UIText_original(x0, 80, base_Y+(32*2), "Handheld GPU Boost:", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    if (Settings.GPUBoost == 0) {
        set_UIText_original(x0, 460, base_Y+(32*2), "[Off]", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
        set_UIText_original(x0, 660, base_Y+(32*2), "On", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    }
    else {
        set_UIText_original(x0, 460, base_Y+(32*2), "Off", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
        set_UIText_original(x0, 660, base_Y+(32*2), "[On]", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
    }

    if (indicator == 2)
        set_UIText_original(x0, 320, base_Y+(32*11), "To apply this setting you must restart game.", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

    set_UIText_original(x0, 60, base_Y+(32*indicator), ">", 0xFFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

    nn::hid::GetNpadState(&out, nn::hid::CONTROLLER_HANDHELD);
    nn::hid::GetNpadState(&out2, nn::hid::CONTROLLER_PLAYER_1);
    nn::hid::GetNpadState(&out3, nn::hid::CONTROLLER_PLAYER_1);
    
    static bool DDOWN_buttonHeld = false;
    static bool DUP_buttonHeld = false;
    static bool A_buttonHeld = false;

    if ((out.Buttons & nn::hid::KEY_DDOWN) || (out2.Buttons & nn::hid::KEY_DDOWN) || (out3.Buttons & nn::hid::KEY_DDOWN)) {
        if (!DDOWN_buttonHeld) {
            DDOWN_buttonHeld = true;
            if (indicator+1 < options_count) indicator++;
        }
    }
    else DDOWN_buttonHeld = false;
    
    if ((out.Buttons & nn::hid::KEY_DUP) || (out2.Buttons & nn::hid::KEY_DUP) || (out3.Buttons & nn::hid::KEY_DUP)) {
        if (!DUP_buttonHeld) {
            DUP_buttonHeld = true;
            if (indicator-1 >= 0) indicator--;
        }
    }
    else DUP_buttonHeld = false;
    if ((out.Buttons & nn::hid::KEY_A) || (out2.Buttons & nn::hid::KEY_A) || (out3.Buttons & nn::hid::KEY_A)) {
        if (!A_buttonHeld) {
            A_buttonHeld = true;
            switch(indicator) {
                case 0:
                    if (Settings.FPS == 60) Settings.FPS = 30;
                    else Settings.FPS = 60;
                    FPSlock_hook(nullptr, Settings.FPS);
                    break;
                case 1:
                    if (Settings.RenderingRes < 0) Settings.RenderingRes = 4;
                    else if (Settings.RenderingRes < 8) Settings.RenderingRes += 1;
                    else Settings.RenderingRes = 0;
                    break;
                case 2:
                    if (Settings.GPUBoost == 0) {
                        Settings.GPUBoost = 1;
                        nn::oe::SetPerformanceConfiguration(nn::oe::Normal, PerformanceConfig_GPU460mhz);
                    }
                    else {
                        Settings.GPUBoost = 0;
                        nn::oe::SetPerformanceConfiguration(nn::oe::Normal, PerformanceConfig_GPU307mhz);
                    }
            }
        }
    }
    else A_buttonHeld = false;
}