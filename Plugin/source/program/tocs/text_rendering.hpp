#pragma once

/* Text layout fixes for the English text. Port of Tfoaf/TextRendering.hpp. */

#include "common.hpp"
#include "hidden_menu.hpp"

HOOK_DEFINE_TRAMPOLINE(SetTextKerning) {
    static int Callback(void* x0, int kerning) {
        return Orig(x0, 0);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetTextWidth2) {
    static int Callback(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale) {
        int width = Orig(x0, string, width_ptr, height_ptr, x4, fontsize, X_Scale);
        temp.text = string;
        temp.width = width;
        return width;
    }
};

HOOK_DEFINE_TRAMPOLINE(GetTextWidth) {
    static int Callback(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale) {
        int width = Orig(x0, string, width_ptr, height_ptr, x4, fontsize, X_Scale);
        temp.text = string;
        temp.width = width;
        if ((strncmp("EP ", string, 3) == 0) || (strncmp("CP ", string, 3) == 0)) {
            if (temp.text.length() == 6) {
                std::string string_aft = temp.text.substr(3, 3);
                string_aft.erase(std::remove(string_aft.begin(), string_aft.end(), ' '), string_aft.end());
                CPEP_width = Orig(x0, string_aft.c_str(), width_ptr, height_ptr, x4, fontsize, X_Scale);
            }
        }
        return width;
    }
};

inline void get_UITextcase(uintptr_t LR) {
    ptrdiff_t offsetItr = returnInstructionOffset(LR);
    switch(offsetItr) {
        case 0x24FF70:
            UITextcase = 1;
            break;
        case 0x21739C:
            UITextcase = 2;
            break;
        case 0xC0BD4:
            UITextcase = 3;
            break;
        case 0xBE5DC:
            UITextcase = 4;
            break;
        case 0x2196C0:
            UITextcase = 5;
            break;
        case 0xBE61C:
        case 0xBE62C:
            UITextcase = 6;
            break;
        case 0xBE81C:
            UITextcase = 7;
            break;
        case 0x13A554:
            UITextcase = 8;
            break;
        case 0x2542BC:
            UITextcase = 9;
            break;
        case 0x245BA8:
            UITextcase = 10;
            break;
        case 0x13AAE8:
        case 0x13A728:
        case 0x13A7EC:
        case 0x16ABA8:
            UITextcase = 11;
            break;
        case 0x18403C:
            UITextcase = 12;
            break;
        default:
            UITextcase = 0;
    }
}

HOOK_DEFINE_TRAMPOLINE(VsnprintfWrapper) {
    static uint64_t Callback(char* s, size_t n, const char* format, const char* first, const char* second, const char* x5, const char* x6, const char* x7, long double q0, long double q1, long double q2, long double q3, void* x8) {
        ptrdiff_t offsetItr = returnInstructionOffset((uintptr_t)__builtin_return_address(0));
        //Fix printing Talk to button
        if (offsetItr == 0x13AA44) {
            if (strcmp(second, "Talk to") == 0)
                return Orig(s, n, format, "Talk to ", first, x5, x6, x7, q0, q1, q2, q3, x8);
        }
        return Orig(s, n, format, first, second, x5, x6, x7, q0, q1, q2, q3, x8);
    }
};

/* These only record which UI is being drawn, based on who called them. */
HOOK_DEFINE_TRAMPOLINE(RenderText) {
    static uint64_t Callback(void* unk0) {
        get_UITextcase((uintptr_t)__builtin_return_address(0));
        return Orig(unk0);
    }
};

HOOK_DEFINE_TRAMPOLINE(RenderText3) {
    static uint64_t Callback(void* unk0) {
        get_UITextcase((uintptr_t)__builtin_return_address(0));
        return Orig(unk0);
    }
};

HOOK_DEFINE_TRAMPOLINE(RenderText4) {
    static uint64_t Callback(void* unk0) {
        get_UITextcase((uintptr_t)__builtin_return_address(0));
        return Orig(unk0);
    }
};

HOOK_DEFINE_TRAMPOLINE(RenderText2) {
    static uint64_t Callback(void* x0, int X_Pos, int Y_Pos, const char* string, void* ARGB_Color, void* ARGB_Shadow, void* ARGB_Border, void* x7, float s0, float s1, float fontsize, float X_Scale, double d3, float f1, void* unk8, float* unk9, void* unk10, void* unk11, void* unk12, void* unk13, float* unk14, float* unk15, float* unk16, void* unk17) {
        get_UITextcase((uintptr_t)__builtin_return_address(0));
        switch(UITextcase) {
            //Remove "0" from printing in last line for long lists
            case 9: {
                if (strlen(string) <= 3)
                    break;
                std::string temp_str = string;
                if (temp_str.substr(temp_str.length() - 2).compare("\n0") != 0)
                    break;
                uint16_t empty = 0;
                memcpy((void*)(string+(temp_str.length() - 2)), &empty, 2);
                break;
            }
            //Reformat prompt buttons
            case 11:
                if (strlen(string) == 0)
                    break;
                if (strncmp(string, "#", 1) != 0)
                    break;
                if (strncmp(string+5, ":", 1) == 0)
                    break;
                fontsize = 32.0;
                if (X_Pos == 0)
                    break;
                X_Pos -= 0x30;
        }
        return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, x7, s0, s1, fontsize, X_Scale, d3, f1, unk8, unk9, unk10, unk11, unk12, unk13, unk14, unk15, unk16, unk17);
    }
};

HOOK_DEFINE_TRAMPOLINE(RenderTextFromAtlas) {
    static uint64_t Callback(const char* value, int x1, void* x2, float* x3, void* x4, void* x5, float X_Pos, float Y_Pos, float image_X_pos, float image_Y_pos, float elem_width, float elem_height, float kerning) {
        ptrdiff_t offsetItr = returnInstructionOffset((uintptr_t)__builtin_return_address(0));
        static float Last_X = 0;
        static bool cut_decimal = false;
        switch(offsetItr) {
            //Fix fishing timer integer position
            case 0x184EC4: {
                cut_decimal = false;
                if (Y_Pos != 766.0f && X_Pos != 544.0f)
                    break;
                int64_t intgr = strtol(value, NULL, 10);

                if (intgr == 0) {
                    elem_height = 0;
                    cut_decimal = true;
                    break;
                }
                if (intgr < 10) {
                    X_Pos += (44.0 * 2.0);
                    X_Pos -= 30.0;
                    Last_X = (44.0 * 2.0) - 30.0;
                    if (intgr == 1)
                        Last_X -= 16.0;
                }
                else {
                    X_Pos += (44.0 * 3.0);
                    X_Pos -= 20.0;
                    Last_X = (44.0 * 3.0) - 20.0;
                }
                break;
            }
            //Fix fishing timer decimal position
            case 0x184F5C:
                if (Last_X <= 0.0f)
                    break;

                if (cut_decimal == false)
                    X_Pos += Last_X;
                else cut_decimal = false;
                Last_X = 0.0;
                break;
        }
        return Orig(value, x1, x2, x3, x4, x5, X_Pos, Y_Pos, image_X_pos, image_Y_pos, elem_width, elem_height, kerning);
    }
};

int SetUIText::Callback(void* x0, int X_Pos, int Y_Pos, const char* string, int ARGB_Color, int ARGB_Shadow, int ARGB_Border, int w7, float s0, float s1, float fontsize, float X_Scale) {
    if (strlen(string) == 0)
        return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);

    ptrdiff_t offsetItr = returnInstructionOffset((uintptr_t)__builtin_return_address(0));
    switch(offsetItr) {
        case 0xC0EB4:
            UITextcase = 8;
            break;
    }

    switch(UITextcase) {
        // Achievements
        case 1:
            if (X_Pos != 0x37)
                break;
            if (temp.width <= 560)
                break;

            X_Scale = 560.0 / (float)temp.width;
            return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
        // Load/Save menu
        case 2:
            if ((X_Pos != 0x443) || (Y_Pos != 0x120))
                break;
            if (temp.width <= 720)
                break;

            X_Scale = 720.0 / (float)temp.width;
            return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
        //Battle
        case 3:
            if (X_Pos < 0x187)
                break;
            if ((Y_Pos != 0x30B) && (Y_Pos != 0x33F))
                break;
            if (temp.width <= 1160)
                break;

            X_Scale = 1160.0 / (float)(temp.width - (X_Pos - 0x187));
            return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
        //Enemy stats
        case 4:
            if ((X_Pos != 0x552) && (X_Pos != 0x60C) && (X_Pos != 0x6C6))
                break;
            if ((Y_Pos != 0x33B) && (Y_Pos != 0x370) && (Y_Pos != 0x3A5) && (Y_Pos != 0x3DA) && (Y_Pos != 0x307))
                break;

            if (temp.width > 60)
                X_Scale = 60.0 / (float)temp.width;
            return Orig(x0, X_Pos, Y_Pos+3, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize*1.2, X_Scale/1.2f);
        //Main Menu and Pause Menu
        case 5:
            if ((X_Pos == 0x64b) && (Y_Pos <= 0x56)) {
                X_Pos = 0x631;
                break;
            }

            if ((X_Pos != 0x4A5) && (X_Pos != 0x4D2) && (X_Pos != 0x4FF) && (X_Pos != 0x4FA))
                break;
            if ((Y_Pos != 0x295) && (Y_Pos != 0x2DD) && (Y_Pos != 0x325) && (Y_Pos != 0x36D))
                break;

            ReadPads();
            Orig(x0, 1040, 970, "Translation mod made by MasaGratoR & Graber", (int)0xFFFFFFF, 0x00000000, (int)0xFF000000, w7, s0, s1, fontsize, X_Scale);
            if (AnyPadHeld(nn::hid::KEY_R)) {
                BlockButtons = true;
                {
                    Orig(x0, -150, -100, &"█"[0], (int)0xFF000000, 0x00000000, 0x00000000, 0, 0, 1.0, 768.0, 2.0);

                    int base_Y = 80;
                    
                    Orig(x0, 80, base_Y+(32*0), "FPS:", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    if (Settings.FPS == 30) {
                        Orig(x0, 260, base_Y+(32*0), "[30]", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                        Orig(x0, 460, base_Y+(32*0), "60", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    }
                    else {
                        Orig(x0, 260, base_Y+(32*0), "30", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                        Orig(x0, 460, base_Y+(32*0), "[60]", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    }
                    Orig(x0, 80, base_Y+(32*1), "Rendering Resolution:", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    Orig(x0, 460, base_Y+(32*1), GetResolution(Settings.RenderingRes).label, (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

                    Orig(x0, 80, base_Y+(32*2), "Handheld GPU Boost:", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    if (Settings.GPUBoost == 0) {
                        Orig(x0, 460, base_Y+(32*2), "[Off]", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                        Orig(x0, 660, base_Y+(32*2), "On", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    }
                    else {
                        Orig(x0, 460, base_Y+(32*2), "Off", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                        Orig(x0, 660, base_Y+(32*2), "[On]", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);
                    }

                    if (indicator == 1)
                        Orig(x0, 320, base_Y+(32*11), "To apply this setting you must restart game.", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

                    Orig(x0, 60, base_Y+(32*indicator), ">", (int)0xFFFFFFF, 0x00000000, 0x00000000, 0, 0, 1.0, 32.0, 1.0);

                    static bool DDOWN_buttonHeld = false;
                    static bool DUP_buttonHeld = false;
                    static bool A_buttonHeld = false;

                    if (AnyPadHeld(nn::hid::KEY_DDOWN)) {
                        if (!DDOWN_buttonHeld) {
                            DDOWN_buttonHeld = true;
                            if (indicator+1 < options_count) indicator++;
                        }
                    }
                    else DDOWN_buttonHeld = false;

                    if (AnyPadHeld(nn::hid::KEY_DUP)) {
                        if (!DUP_buttonHeld) {
                            DUP_buttonHeld = true;
                            if (indicator-1 >= 0) indicator--;
                        }
                    }
                    else DUP_buttonHeld = false;

                    if (AnyPadHeld(nn::hid::KEY_A)) {
                        if (!A_buttonHeld) {
                            A_buttonHeld = true;
                            switch(indicator) {
                                case 0:
                                    if (Settings.FPS == 60) Settings.FPS = 30;
                                    else Settings.FPS = 60;
                                    FPSlock::Callback(nullptr, Settings.FPS);
                                    break;
                                case 1:
                                    if (Settings.RenderingRes < 0) Settings.RenderingRes = 4;
                                    else if (Settings.RenderingRes < 8) Settings.RenderingRes += 1;
                                    else Settings.RenderingRes = 0;
                                    break;
                                case 2:
                                    Settings.GPUBoost = Settings.GPUBoost == 0 ? 1 : 0;
                                    SetGpuBoost(Settings.GPUBoost != 0);
                                    break;
                            }
                        }
                    }
                    else A_buttonHeld = false;
                }
            }
            else if (BlockButtons == true) {
                SaveSettings();
                BlockButtons = false;
                indicator = 0;
            }
            break;
        //Enemy stats menu buttons
        case 6:
            if (Y_Pos == 0x40f)
                Y_Pos = 0x413;
            break;
        //Battle view buttons
        case 7:
            if (strcmp("View Specifics", string) == 0)
                Y_Pos += 2;
            break;
        //Cutted and formatted stuff like "Yes/No" and "CP/EP"
        case 8:
            if ((strcmp("Yes", string) == 0) || (strcmp("No", string) == 0))
                Y_Pos += 7;
            else if ((strncmp("EP ", string, 3) == 0) || (strncmp("CP ", string, 3) == 0)) {
                std::string temp_str = string;
                if (temp_str.length() != 6)
                    break;

                std::string string_pre = temp_str.substr(0, 2);
                std::string string_aft = temp_str.substr(3, 3);
                string_aft.erase(std::remove(string_aft.begin(), string_aft.end(), ' '), string_aft.end());
                Orig(x0, 0x473, Y_Pos, string_pre.c_str(), ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
                int new_X = 0x4F0 - CPEP_width;
                if (temp_str.substr(5, 1).compare("5") == 0)
                    new_X -= 3;
                return Orig(x0, new_X, Y_Pos, string_aft.c_str(), ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
            }
            break;
        //Character page, fixing printing club affiliations
        case 10:
            if (X_Pos != 0x483)
                break;
            X_Pos = 0x503;
            break;
        //Prompt buttons
        case 11: {
            if (Y_Pos == 0x40c)
                Y_Pos += 3;
            else if (strcmp(string,"Dismount") == 0) {
                fontsize = 32.0;
                X_Pos -= 1;
            }
            else if (strcmp(string,"Dash") == 0)
                fontsize = 32.0;
            break;
        }
        //Fix string position in Fish minigame
        case 12:
            if (strcmp(string, "Rods") == 0)
                X_Pos = 0x392;
            break;
    }

    return Orig(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
}
