/*
 * ToCS1-ENX translation plugin (Trails of Cold Steel Kai, Switch 1.0.3).
 * Port of the former Skyline plugin (Tfoaf/main_patch.cpp) to exlaunch.
 */

#include "lib.hpp"
#include "tocs/common.hpp"
#include "tocs/hidden_menu.hpp"
#include "tocs/text_rendering.hpp"

/* The first file the game opens is used to load the plugin settings from the SD card. */
HOOK_DEFINE_TRAMPOLINE(FsOpenFile) {
    static Result Callback(nn::fs::FileHandle* handle, const char* filepath, int mode) {
        static bool initialized = false;
        if (!initialized) {
            nn::fs::MountSdCardForDebug("sd");
            nn::fs::FileHandle save;
            long filesize = 0;
            if (!Orig(&save, SAVE_PATH, nn::fs::OpenMode_Read)) {
                nn::fs::GetFileSize(&filesize, save);
                char magic_check[5] = "";
                nn::fs::ReadFile(save, 0, &magic_check, 4);
                if (strcmp(magic, magic_check) == 0 && filesize > 4)
                    nn::fs::ReadFile(save, 4, &Settings, std::min<long>(filesize - 4, sizeof(Settings)));
                nn::fs::CloseFile(save);
            }
            else {
                nn::fs::CreateDirectory("sd:/config");
                nn::fs::CreateDirectory(SAVE_DIR);
                nn::fs::CreateFile(SAVE_PATH, 4 + sizeof(Settings));
            }
            SetGpuBoost(Settings.GPUBoost != 0);
            initialized = true;
        }
        return Orig(handle, filepath, mode);
    }
};

/* While the settings menu is open, the game only sees R held (so the menu stays up) and no stick input. */
template<typename State, State* Own>
struct BlockedPad {
    static void Apply(State* KeyState) {
        if (BlockButtons && KeyState != Own) {
            KeyState->Buttons = nn::hid::KEY_R;
            KeyState->LStickY = 0;
        }
    }
};

HOOK_DEFINE_TRAMPOLINE(GetNpadStatePro) {
    static void Callback(nn::hid::NpadFullKeyState* KeyState, u32 const& NpadID) {
        Orig(KeyState, NpadID);
        BlockedPad<nn::hid::NpadFullKeyState, &out3>::Apply(KeyState);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetNpadStateJoyCons) {
    static void Callback(nn::hid::NpadJoyDualState* KeyState, u32 const& NpadID) {
        Orig(KeyState, NpadID);
        BlockedPad<nn::hid::NpadJoyDualState, &out2>::Apply(KeyState);
    }
};

HOOK_DEFINE_TRAMPOLINE(GetNpadStateHandheld) {
    static void Callback(nn::hid::NpadHandheldState* KeyState, u32 const& NpadID) {
        Orig(KeyState, NpadID);
        BlockedPad<nn::hid::NpadHandheldState, &out>::Apply(KeyState);
    }
};

extern "C" void exl_main(void* x0, void* x1) {
    exl::hook::Initialize();

    NSO_main_start = exl::util::modules::GetTargetStart();

    //Hook Function calling UI Text Render
    RenderText::InstallAtOffset(0x219410);
    RenderText2::InstallAtOffset(0x168010);
    RenderText3::InstallAtOffset(0x2573F0);
    RenderText4::InstallAtOffset(0x1844B0);
    RenderTextFromAtlas::InstallAtOffset(0x21F9C0);

    //Hook UI Text Render
    SetUIText::InstallAtOffset(0x167BC0);

    //Hook Text Width Calcs
    GetTextWidth::InstallAtOffset(0x113810);
    GetTextWidth2::InstallAtOffset(0x1131E0);

    //Hook Text Kerning
    SetTextKerning::InstallAtOffset(0x112F40);

    //Hook vsnprintf wrapper
    VsnprintfWrapper::InstallAtOffset(0x222D0);

    //Hook FS
    FsOpenFile::InstallAtFuncPtr(&nn::fs::OpenFile);

    //Hook FPS lock
    FPSlock::InstallAtOffset(0x22CA0);

    //Hook buttons
    GetNpadStatePro::InstallAtFuncPtr(static_cast<void (*)(nn::hid::NpadFullKeyState*, u32 const&)>(&nn::hid::GetNpadState));
    GetNpadStateJoyCons::InstallAtFuncPtr(static_cast<void (*)(nn::hid::NpadJoyDualState*, u32 const&)>(&nn::hid::GetNpadState));
    GetNpadStateHandheld::InstallAtFuncPtr(static_cast<void (*)(nn::hid::NpadHandheldState*, u32 const&)>(&nn::hid::GetNpadState));

    //Hook 3D world rendering res
    RenderingRes::InstallAtOffset(0x151550);
    TextureMaker::InstallAtOffset(0x15A60);

    //Patch save description to not add character names + their levels.
    //Formatting included in game is breaking under longer names like "Instructor Sara"
    //which locks game when trying to format it
    exl::patch::CodePatcher(0x150928).Write<u32>(0xB5000E08); //cbnz x8, #0x1c0

    //Force game to use nvnSamplerBuilderSetMaxAnisotropy
    exl::patch::CodePatcher(0x4D13DC).Write<u32>(0xD503201F); //nop
    //Force nvnSamplerBuilderSetMaxAnisotropy to set anisotropy to 16.0
    exl::patch::CodePatcher(0x4D13E4).Write<u32>(0x1E261000); //fmov s0, #16.0
}

extern "C" NORETURN void exl_exception_entry() {
    /* Only used for applets/sysmodules. */
    EXL_ABORT("Default exception handler called!");
}
