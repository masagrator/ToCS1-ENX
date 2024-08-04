int (*set_TextKerning_original)(void* x0, int kerning);
int set_TextKerning_hook(void* x0, int kerning) {
	return set_TextKerning_original(x0, 0);
}

int (*get_TextWidth2_original)(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale);
int get_TextWidth2_hook(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale) {
	int width = get_TextWidth2_original(x0, string, width_ptr, height_ptr, x4, fontsize, X_Scale);
	temp.text = string;
	temp.width = width;
	return width;
}

int (*get_TextWidth_original)(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale);
int get_TextWidth_hook(void* x0, const char* string, void* width_ptr, void* height_ptr, void* x4, float fontsize, float X_Scale) {
	int width = get_TextWidth_original(x0, string, width_ptr, height_ptr, x4, fontsize, X_Scale);
	temp.text = string;
	temp.width = width;
	if ((strncmp("EP ", string, 3) == 0) || (strncmp("CP ", string, 3) == 0)) {
		if (temp.text.length() == 6) {
			std::string string_aft = temp.text.substr(3, 3);
			string_aft.erase(std::remove(string_aft.begin(), string_aft.end(), ' '), string_aft.end());
			CPEP_width = get_TextWidth_original(x0, string_aft.c_str(), width_ptr, height_ptr, x4, fontsize, X_Scale);
		}
	}
	return width;
}

void get_UITextcase(uintptr_t LR) {
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
	return;
}

uint64_t (*vsnprintfWrapper_original)(char* s, size_t n, const char* format, const char* first, const char* second, const char* x5, const char* x6, const char* x7, long double q0, long double q1, long double q2, long double q3, void* x8);
uint64_t vsnprintfWrapper_hook(char* s, size_t n, const char* format, const char* first, const char* second, const char* x5, const char* x6, const char* x7, long double q0, long double q1, long double q2, long double q3, void* x8) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	ptrdiff_t offsetItr = returnInstructionOffset(LR);
	//Fix printing Talk to button
	if (offsetItr == 0x13AA44) {
		if (strcmp(second, "Talk to") == 0) 
			return vsnprintfWrapper_original(s, n, format, "Talk to ", first, x5, x6, x7, q0, q1, q2, q3, x8);
	}

	return vsnprintfWrapper_original(s, n, format, first, second, x5, x6, x7, q0, q1, q2, q3, x8);
}

uint64_t (*RenderText_original)(void* unk0);
uint64_t RenderText_hook(void* unk0) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	get_UITextcase(LR);
	return RenderText_original(unk0);
}

uint64_t (*RenderText2_original)(void* x0, int X_Pos, int Y_Pos, const char* string, void* ARGB_Color, void* ARGB_Shadow, void* ARGB_Border, void* x7, float s0, float s1, float fontsize, float X_Scale, double d3, float f1, void* unk8, float* unk9, void* unk10, void* unk11, void* unk12, void* unk13, float* unk14, float* unk15, float* unk16, void* unk17);
uint64_t RenderText2_hook(void* x0, int X_Pos, int Y_Pos, const char* string, void* ARGB_Color, void* ARGB_Shadow, void* ARGB_Border, void* x7, float s0, float s1, float fontsize, float X_Scale, double d3, float f1, void* unk8, float* unk9, void* unk10, void* unk11, void* unk12, void* unk13, float* unk14, float* unk15, float* unk16, void* unk17) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	get_UITextcase(LR);
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
	return RenderText2_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, x7, s0, s1, fontsize, X_Scale, d3, f1, unk8, unk9, unk10, unk11, unk12, unk13, unk14, unk15, unk16, unk17);
}

uint64_t (*RenderText3_original)(void* unk0);
uint64_t RenderText3_hook(void* unk0) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	get_UITextcase(LR);
	return RenderText3_original(unk0);
}

uint64_t (*RenderText4_original)(void* unk0);
uint64_t RenderText4_hook(void* unk0) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	get_UITextcase(LR);
	return RenderText4_original(unk0);
}


uint64_t (*RenderTextFromAtlas_original)(const char* value, int x1, void* x2, float* x3, void* x4, void* x5, float X_Pos, float Y_Pos, float image_X_pos, float image_Y_pos, float elem_width, float elem_height, float kerning);
uint64_t RenderTextFromAtlas_hook(const char* value, int x1, void* x2, float* x3, void* x4, void* x5, float X_Pos, float Y_Pos, float image_X_pos, float image_Y_pos, float elem_width, float elem_height, float kerning) {
	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	ptrdiff_t offsetItr = returnInstructionOffset(LR);
	static float Last_X = 0;
	static bool cut_decimal = false;
	switch(offsetItr) {
		//Fix fishing timer integer position
		case 0x184EC4: {
			cut_decimal = false;
			if (Y_Pos != 766.0 && X_Pos != 544.0)
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
			if (Last_X <= 0.0) 
				break;

			if (cut_decimal == false)
				X_Pos += Last_X;
			else cut_decimal = false;
			Last_X = 0.0;
			break;

	}
	return RenderTextFromAtlas_original(value, x1, x2, x3, x4, x5, X_Pos, Y_Pos, image_X_pos, image_Y_pos, elem_width, elem_height, kerning);
}

int set_UIText_hook(void* x0, int X_Pos, int Y_Pos, const char* string, int ARGB_Color, int ARGB_Shadow, int ARGB_Border, int w7, float s0, float s1, float fontsize, float X_Scale) {
	if (strlen(string) == 0)
		return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);

	uintptr_t LR = (uintptr_t)__builtin_return_address(0);
	ptrdiff_t offsetItr = returnInstructionOffset(LR);
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
			return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
		// Load/Save menu
		case 2:
			if ((X_Pos != 0x443) || (Y_Pos != 0x120)) 
				break;
			if (temp.width <= 720)
				break;
			
			X_Scale = 720.0 / (float)temp.width;
			return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
		//Battle
		case 3:
			if (X_Pos < 0x187) 
				break;
			if ((Y_Pos != 0x30B) && (Y_Pos != 0x33F)) 
				break;
			if (temp.width <= 1160)
				break;

			X_Scale = 1160.0 / (float)(temp.width - (X_Pos - 0x187));
			return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
		//Enemy stats
		case 4:
			if ((X_Pos != 0x552) && (X_Pos != 0x60C) && (X_Pos != 0x6C6))
				break;
			if ((Y_Pos != 0x33B) && (Y_Pos != 0x370) && (Y_Pos != 0x3A5) && (Y_Pos != 0x3DA) && (Y_Pos != 0x307))
				break;

			if (temp.width > 60)
				X_Scale = 60.0 / (float)temp.width;
			return set_UIText_original(x0, X_Pos, Y_Pos+3, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize*1.2, X_Scale/1.2);
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

			nn::hid::GetNpadState(&out, nn::hid::CONTROLLER_HANDHELD);
			nn::hid::GetNpadState(&out2, nn::hid::CONTROLLER_PLAYER_1);
			nn::hid::GetNpadState(&out3, nn::hid::CONTROLLER_PLAYER_1);
			set_UIText_original(x0, 1040, 970, "Translation mod made by MasaGratoR & Graber", 0xFFFFFFFF, 0x00000000, 0xFF000000, w7, s0, s1, fontsize, X_Scale);
			if ((out.Buttons & nn::hid::KEY_R) || (out2.Buttons & nn::hid::KEY_R) || (out.Buttons & nn::hid::KEY_R)) {
				BlockButtons = true;
				RenderHiddenMenu(x0);
			}
			else if (BlockButtons == true) {
				nn::fs::FileHandle handle;
				if (!nn::fs::OpenFile(&handle, "sd:/config/ToCS1/subsdk9.save", nn::fs::OpenMode_Write)) {
					nn::fs::WriteFile(handle, 0, &magic, 4, nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));
					nn::fs::WriteFile(handle, 4, &Settings, sizeof(Settings), nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush));
					nn::fs::CloseFile(handle);
				}
				BlockButtons = false;
				indicator = 0;
			}
			break;
		//Enemy stats menu buttons
		case 6:
			if (Y_Pos == 0x40f)
				Y_Pos = 0x413;
			break;
		case 7:
		//Battle view buttons
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
				set_UIText_original(x0, 0x473, Y_Pos, string_pre.c_str(), ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
				int new_X = 0x4F0 - CPEP_width;
				if (temp_str.substr(5, 1).compare("5") == 0)
					new_X -= 3;
				return set_UIText_original(x0, new_X, Y_Pos, string_aft.c_str(), ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
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
		default:
			return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
	}
	
	return set_UIText_original(x0, X_Pos, Y_Pos, string, ARGB_Color, ARGB_Shadow, ARGB_Border, w7, s0, s1, fontsize, X_Scale);
}