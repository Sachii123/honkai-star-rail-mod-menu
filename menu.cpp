#include "menu.h"
#include "hooks.h"

#include <windows.h>
#include <iostream>

#include "utils.h"

#include "game.h"

#include "globals.h"

using namespace globals;


namespace menu {

	static float speed = 1.f;
	static HWND window = 0;
	__int64 game_assembly = 0;
	__int64 unity_player = 0;

	void set_speed_global(float speed) {
		utils::write<float>(utils::read<__int64>(unity_player + 0x1D21D78) + 0xFC, speed);
	}

	void set_speed_battle(float speed) {
		utils::write<float>(utils::read<__int64>(utils::read<__int64>(game_assembly + 0x8CAA6A0) + 0xC0) + 0x1DC, speed);
	}

	void set_speed(float speed) {
		if (auto_dialogue) {//new 2
			set_speed_battle(1.f);
			set_speed_global(10.f);
		}
		else if (hooks::game::get_currect_phase() == RPG_BATTLE) {
			set_speed_battle(5.f);//sus
			set_speed_global(1.f);
		}
		else {
			set_speed_global(speed);
			set_speed_battle(1.f);
		}
	}

	void mega_beep(bool is) {
		if (is) {
			Beep(800, 500);
		}
		else {
			Beep(600, 500);
		}
	}


	void cheat_thread() {

		while (!game_assembly) game_assembly = reinterpret_cast<uint64_t>(GetModuleHandleA("gameassembly.dll"));
		while (!unity_player) unity_player = reinterpret_cast<uint64_t>(GetModuleHandleA("unityplayer.dll"));
		

		do
		{

			if (GetAsyncKeyState(VK_F1) && 1) {
				speedhack = !speedhack;

				mega_beep(speedhack);
			}
			else if (GetAsyncKeyState(VK_F2) && 1) {
				peeking = !peeking;

				mega_beep(peeking);
			}
			else if (GetAsyncKeyState(VK_F3) && 1) {
				fps_unlock = !fps_unlock;

				mega_beep(fps_unlock);
			}
			else if (GetAsyncKeyState(VK_F4) && 1) {
				auto_battle = !auto_battle;

				mega_beep(auto_battle);
			}
			else if (GetAsyncKeyState(VK_OEM_3) && 1) {
				blind = !blind;

				mega_beep(blind);
			}

			if (speedhack) {
				set_speed(speed);
			}
			else {
				set_speed(1.f);
			}

			if (peeking) {
				utils::write<uint8_t>(game_assembly + 0x51292C0, 0xC3);
			}
			else {
				utils::write<uint8_t>(game_assembly + 0x51292C0, 0x40);
			}

			if (fps_unlock) {
				utils::write<uint32_t>(unity_player + 0x1C4E000, 540);
			}
			else {
				utils::write<uint32_t>(unity_player + 0x1C4E000, 60);
			}

			if (blind) {
				utils::write<uint8_t>(game_assembly + 0x5800F40, 0xC3);
			}
			else {
				utils::write<uint8_t>(game_assembly + 0x5800F40, 0x40);
			}

			if (GetAsyncKeyState(VK_CAPITAL)) {

				auto_dialogue = 1;//new 2

				if (GetForegroundWindow() == window) {
					keybd_event(VK_SPACE, 0, 0, 0);
					Sleep(20);//100 new2
					keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0);

				}
			}
			else {
				auto_dialogue = 0;
			}

			Sleep(16);//sus//new 2

		} while (true);

	}

	void menu() {

		while (!window) window = FindWindowA("UnityWndClass", NULL);//new 2

		Sleep(15000);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)cheat_thread, 0, 0, 0);

		puts("enable speedhack | hotkey: F1");
		puts("enable peeking (: | hotkey: F2");
		puts("enable fps unlock | hotkey: F3");
		puts("enable auto battle unlock | hotkey: F4");
		puts("enable blind enemies | hotkey: ~");
		puts("enable auto dialog | hotkey: Capslock");

		while (true)
		{
			printf("currect speed value: %f\nchange to: ", speed);

			scanf_s("%f", &speed);
		}
	}
}
