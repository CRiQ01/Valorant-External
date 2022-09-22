#pragma once
#include "icons.h"
#include "imguipp.h"
#include "custom.hpp"
#include <Windows.h>

inline std::vector<Enemy> enemy_collection{};


struct Target {

	Enemy entity;
	Vector3 headPos;
	float fovDist;

	Target(Enemy e, Vector3 hp, float fd) : entity(e), headPos(hp), fovDist(fd) {}
};

inline auto IsMale(Enemy enemy) -> bool {

	if (enemy.bonecount == 104) return true;
	return false;
}

inline auto IsFemale(Enemy enemy) -> bool {

	if (enemy.bonecount == 99 || enemy.bonecount == 101) return true;
	return false;
}

inline auto IsBot(Enemy enemy) -> bool {

	if (enemy.bonecount == 103)
		return true;

	return false;
}

inline auto isVisible(DWORD_PTR mesh) -> bool {

	float fLastSubmitTime = Read<float>(mesh + 0x350);
	float fLastRenderTimeOnScreen = Read<float>(mesh + 0x358);

	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;

	return bVisible;
}

inline auto IsDormant(Enemy enemy) -> bool {

	bool dormant = Read<bool>(enemy.actor_ptr + offsets::dormant);
	if (!dormant) {
		return false;
	}
	return true;
}

inline auto GetCharacter(Enemy enemy) -> string {

	int charObj = Read<int>(enemy.actor_ptr + offsets::inventory);
	auto characterName = GetCharacterName(charObj);
	return characterName;
}

inline auto GetCurrentWeapon(Enemy enemy) -> string {

	auto Inventory = Read<intptr_t>(enemy.actor_ptr + offsets::inventory);
	intptr_t CurrentEquip = Read<intptr_t>(Inventory + offsets::current_equipable);
	int weaponObj = Read<int>(CurrentEquip + offsets::ObjID);
	return GetWeaponName(weaponObj);
}

inline bool AimSortCrossHair(const Target& pf1, const Target& pf2) {

	return pf1.fovDist <= pf2.fovDist;
}

inline auto SmoothAim(Vector3 CameraRotation, Vector3 Target, float SmoothFactor) -> Vector3 {

	Vector3 delta = (Target - CameraRotation).Clamp();

	float smooth = powf(SmoothFactor, 0.4f);
	smooth = min(0.99f, smooth);

	float coeff = (1.0f - smooth) / delta.Length2D() * 0.5f;
	coeff = min(0.99f, coeff);

	return delta * coeff;
}


inline auto Aimbot() -> void {

	for (;;) {

		if (aimbot::aimbot) {

			while (GetAsyncKeyState(aimkey)) {

				std::vector<Target> possible;

				Vector3 Location = Read<Vector3>(g_cameramanager + offsets::camera_position);
				Vector3 Rocation = Read<Vector3>(g_cameramanager + offsets::camera_rotation);
				float Fov = Read<float>(g_cameramanager + offsets::camera_fov);

				for (int i = 0; i < enemy_collection.size(); ++i) {

					Enemy enemy = enemy_collection[i];

					float Health = Read<float>(enemy.damagehandler_ptr + offsets::health);
					if (enemy.actor_ptr == g_localplayerpawn || Health <= 0 || !enemy.mesh_ptr) {
						continue;
					}

					uintptr_t team_component = Read<uintptr_t>(enemy.playerstate_ptr + offsets::team_component);
					int team_id = Read<int>(team_component + offsets::team_id);

					if (team_id == g_localteamid) {
						continue;
					}

					bool Dormant = Read<bool>(enemy.actor_ptr + offsets::dormant);
					auto MeshComponent = Read<uint64_t>(enemy.actor_ptr + offsets::mesh_component);
					auto IsVisible = isVisible(MeshComponent);

					if (aimbot::visible) {
						if (!Dormant || !IsVisible) {
							continue;
						}
					}
					else {
						if (!Dormant) {
							continue;
						}
					}

					Vector3 head_position = getBonePosition(enemy, aimbot::aimbone);
					Vector2 outPos = worldToScreen(head_position, Location, Rocation, Fov);
					Vector3 mPos = { outPos.x - GetSystemMetrics(SM_CXSCREEN) / 2, outPos.y - GetSystemMetrics(SM_CYSCREEN) / 2, 0 };

					float fovDist = mPos.Length();

					if (fovDist <= aimbot::fov) {
						possible.emplace_back(enemy, head_position, fovDist);
					}
				}

				if (possible.size()) {

					std::sort(possible.begin(), possible.end(), AimSortCrossHair);

					Target currentTarget = possible[0];

					Vector3 TargetPos = currentTarget.headPos;
					Vector3 TargetRotation = (Location - TargetPos).ToRotator().Clamp();

					Vector3 ConvertRotation = Rocation.Clamp();

					Vector3 ControlRotation = Read<Vector3>(g_localplayercontroller + offsets::control_rotation);
					Vector3 DeltaRotation = (ConvertRotation - ControlRotation).Clamp();

					DeltaRotation.y /= 4.0f;
					ConvertRotation = (TargetRotation - (DeltaRotation)).Clamp();

					Vector3 Smoothed = SmoothAim(Rocation, ConvertRotation, aimbot::smoothness);

					{
						READ_GUARDED_REGION_REQUEST Request = {};
						DWORD cbReturned = 0;
						BOOL status = TRUE;

						ULONGLONG Displacement = (g_localplayercontroller + 0x05E4 - 0xC) & 0xFFFFFF;

						if (Displacement < 0x200000) {

							uint64_t tmp;

							Request.Displacement = Displacement;
							Request.Buffer = &tmp;
							Request.Size = 8;
							Request.X = Smoothed.x;
							Request.Y = Smoothed.y;

							status = DeviceIoControl(
								mem->hDevice,
								IOCTL_READ_GUARDED_REGION,
								&Request,
								sizeof(Request),
								&Request,
								sizeof(Request),
								&cbReturned,
								NULL);
						}
					}
				}
				if (aimbot::smooth) {
					std::this_thread::sleep_for(std::chrono::nanoseconds(1));
				} else {
					aimbot::smoothness = 0.0f;
				}
			}
		}
	}
}

inline std::vector<Enemy> retreiveValidEnemies(uintptr_t actor_array, int actor_count) {

	std::vector<Enemy> temp_enemy_collection{};

	for (int i = 0; i < actor_count; i++) {

		uintptr_t actor = Read<uintptr_t>(actor_array + (i * 0x8));
		if (actor == NULL) {
			continue;
		}

		uintptr_t unique_id = Read<uintptr_t>(actor + offsets::unique_id);
		if (unique_id != 0x11e0101) {
			continue;
		}

		uintptr_t mesh = Read<uintptr_t>(actor + offsets::mesh_component);
		if (!mesh) {
			continue;
		}

		uintptr_t playerstate = Read<uintptr_t>(actor + offsets::player_state);
		uintptr_t teamcomponent = Read<uintptr_t>(playerstate + offsets::team_component);
		uintptr_t teamid = Read<int>(teamcomponent + offsets::team_id);
		uintptr_t bonecount = Read<int>(mesh + offsets::bone_count);

		if (teamid == g_localteamid) {
			continue;
		}

		uintptr_t damage_handler = Read<uintptr_t>(actor + offsets::damage_handler);
		uintptr_t root_component = Read<uintptr_t>(actor + offsets::root_component);

		uintptr_t bonearray = Read<uintptr_t>(mesh + offsets::bone_array);
		uintptr_t bonearraycount = Read<uint32_t>(mesh + offsets::bone_array + 8);
		if (!bonearraycount) {
			bonearray = Read<uintptr_t>(mesh + offsets::bone_array);
		}

		Enemy enemy {
			actor,
			damage_handler,
			playerstate,
			root_component,
			mesh,
			bonearray,
			bonecount,
			true
		};
		temp_enemy_collection.push_back(enemy);
	}

	return temp_enemy_collection;
}

inline auto retreiveData() -> void {

	while (true) {

		uintptr_t world = decryptWorld(g_baseaddress);
		auto gameinstance = Read<uint64_t>(world + offsets::game_instance);

		auto localplayerarray = Read<uintptr_t>(gameinstance + offsets::local_player_array);
		auto localplayer = Read<uintptr_t>(localplayerarray);

		auto playercontroller = Read<uintptr_t>(localplayer + offsets::local_player_controller);
		auto localpawn = Read<uintptr_t>(playercontroller + offsets::local_player_pawn);


		auto cameramanager = Read<uintptr_t>(playercontroller + offsets::camera_manager);

		auto damagehandler = Read<uintptr_t>(localpawn + offsets::damage_handler);
		auto localplayerstate = Read<uintptr_t>(localpawn + offsets::player_state);

		auto teamcomponent = Read<uintptr_t>(localplayerstate + offsets::team_component);
		auto localteamid = Read<int>(teamcomponent + offsets::team_id);

		auto persistlevel = Read<uintptr_t>(world + offsets::persistent_level);

		auto actor_array = Read<uintptr_t>(persistlevel + offsets::actor_array);
		auto actor_count = Read<int>(persistlevel + offsets::actor_count);

		/*
		std::cout << "[VALORANT]        UWORLD       : 0x" << std::hex << world << std::endl;
		std::cout << "[VALORANT]    GAME_INSTANCE    : 0x" << std::hex << gameinstance << std::endl;

		std::cout << "[VALORANT] LOCAL_PLAYER ARRAY  : 0x" << std::hex << localplayerarray << std::endl;
		std::cout << "[VALORANT]    LOCAL_PLAYER     : 0x" << std::hex << localplayer << std::endl;

		std::cout << "[VALORANT]  PLAYER_CONTROLLER  : 0x" << std::hex << playercontroller << std::endl;
		std::cout << "[VALORANT]     LOCAL_PAWN      : 0x" << std::hex << localpawn << std::endl;

		std::cout << "[VALORANT]   CAMERA_MANAGER    : 0x" << std::hex << cameramanager << std::endl;

		std::cout << "[VALORANT]   DAMAGE_HANDLER    : 0x" << std::hex << damagehandler << std::endl;
		std::cout << "[VALORANT] LOCAL_PLAYER_STATE  : 0x" << std::hex << localplayerstate << std::endl;

		std::cout << "[VALORANT]   TEAM_COMPONENT    : 0x" << std::hex << teamcomponent << std::endl;
		std::cout << "[VALORANT]   LOCAL_TEAM_I.D    : 0x" << std::hex << localteamid << std::endl;

		std::cout << "[VALORANT]  PERSISTANCE_LEVEL  : 0x" << std::hex << persistlevel << std::endl;

		std::cout << "[VALORANT]     ACTOR_ARRAY     : 0x" << std::hex << actor_array << std::endl;
		std::cout << "[VALORANT]     ACTOR_COUNT     : 0x" << std::hex << actor_count << std::endl;
		*/

		g_localplayercontroller = playercontroller;
		g_localplayerpawn = localpawn;
		g_cameramanager = cameramanager;
		g_localdamagehandler = damagehandler;
		g_localteamid = localteamid;

		enemy_collection = retreiveValidEnemies(actor_array, actor_count);
		std::this_thread::sleep_for(std::chrono::nanoseconds(1));
	}
}

/*
enum class EAresEquippableState : uint8_t
{
	Unknown = 0,
	Idle = 1,
	Equipping = 2,
	Inspecting = 3,
	Attacking = 4,
	Firing = 5,
	Reloading = 6,
	ADS_Idle = 7,
	ADS_Firing = 8,
	Count = 9,
	EAresEquippableState_MAX = 10,
};

// Enum ShooterGame.EAresMovementType
enum class EAresMovementType : uint8 {
	Walking = 0,
	Running = 1,
	Jumping = 2,
	Crouching = 3,
	OnAscender = 4,
	Flying = 5,
	Count = 6,
	Invalid = 254,
	EAresMovementType_MAX = 255
};

std::string GetEquippableVFXState1(EAresEquippableState STATE) {
	switch (STATE) {
	case EAresEquippableState::Reloading:
		return "Reloading..";
		break;
	case EAresEquippableState::Inspecting:
		return "Inspecting";
		break;
	case EAresEquippableState::Firing:
		return "Shooting";
		break;
	case EAresEquippableState::ADS_Firing:
		return "Shooting";
		break;
	case EAresEquippableState::Attacking:
		return "Shooting";
		break;
	default:
		return "No Action";
	}
}
*/

inline auto renderEsp() -> void {

	std::vector<Enemy> local_enemy_collection = enemy_collection;

	if (local_enemy_collection.empty()) {
		return;
	}

	Vector3 cameraposition = Read<Vector3>(g_cameramanager + offsets::camera_position);
	Vector3 camerarotation = Read<Vector3>(g_cameramanager + offsets::camera_rotation);
	float camerafov = Read<float>(g_cameramanager + offsets::camera_fov);

	for (int i = 0; i < local_enemy_collection.size(); i++) {

		int maxdistance = 300;


		Enemy enemy = local_enemy_collection[i];

		float health = Read<float>(enemy.damagehandler_ptr + offsets::health);

		auto ActorRootComponent = Read<uintptr_t>(enemy.actor_ptr + offsets::root_component);
		auto RelativeLocation = Read<Vector3>(ActorRootComponent + offsets::last_render_time);
		auto LocalRootComponent = Read<uintptr_t>(g_localplayerpawn + offsets::root_component);

		auto MeshComponent = Read<uint64_t>(enemy.actor_ptr + offsets::mesh_component);
		auto LocalRelativeLocation = Read<Vector3>(LocalRootComponent + offsets::last_render_time);

		bool Dormant = Read<bool>(enemy.actor_ptr + offsets::dormant);
		auto IsVisible = isVisible(MeshComponent);



		ImColor ESPColor;
		if (visuals::dormant) {
			if (enemy.actor_ptr == g_localplayerpawn || health <= 0 || !enemy.mesh_ptr || !IsDormant(enemy)) {
				continue;
			}

			if (IsVisible) {
				ESPColor = ImColor(0, 255, 0, 255);
				espcol = Col.green;
			}
			else {
				ESPColor = ImColor(255, 0, 0, 255);
				espcol = Col.red;
			}
		}
		else {
			if (enemy.actor_ptr == g_localplayerpawn || health <= 0 || !enemy.mesh_ptr) {
				continue;
			}

			if (IsVisible) {
				ESPColor = ImColor(0, 255, 0, 255);
				espcol = Col.green;
			} else {
				ESPColor = ImColor(255, 0, 0, 255);
				espcol = Col.red;
			}
			if (Dormant)
			{
				ESPColor = ImColor(0, 255, 0, 255);
				espcol = Col.green;
			} else {
				ESPColor = ImColor(255, 128, 0, 255);
				espcol = Col.orange;
			}
		}

		Vector3 head_position = getBonePosition(enemy, 8);
		Vector3 root_position = getBonePosition(enemy, 0);

		if (head_position.z <= root_position.z) {
			continue;
		}

		Vector3 vBaseBone = getBonePosition(enemy, 0);
		Vector3 vBaseBoneOut = ProjectWorldToScreen(vBaseBone);

		Vector3 vHeadBone = getBonePosition(enemy, 8);
		Vector3 vHeadBoneOut = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z));
		Vector3 vHeadBoneOut2 = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z + 15));
		Vector3 vBaseBoneOut2 = ProjectWorldToScreen(Vector3(vBaseBone.x, vBaseBone.y, vBaseBone.z - 15));

		Vector2 head_at_screen_vec = worldToScreen(head_position, cameraposition, camerarotation, camerafov);
		ImVec2 head_at_screen = ImVec2(head_at_screen_vec.x, head_at_screen_vec.y);

		Vector2 root_at_screen_vec = worldToScreen(root_position, cameraposition, camerarotation, camerafov);
		ImVec2 root_at_screen = ImVec2(root_at_screen_vec.x, root_at_screen_vec.y);

		float BoxHeight = abs(vHeadBoneOut2.y - vBaseBoneOut.y);
		float BoxWidth = BoxHeight * 0.55;

		float width = BoxWidth / 10; if (width < 2.f) width = 2.; if (width > 3) width = 3.;

		float DistanceModifier = cameraposition.Distance(head_position) * 0.001F;
		float Distance = cameraposition.Distance(head_position) * 0.01F;

		auto Inventory = Read<intptr_t>(enemy.actor_ptr + offsets::inventory);
		intptr_t CurrentEquip = Read<intptr_t>(Inventory + offsets::current_equipable);
		uintptr_t MagazineAmmo = Read<uintptr_t>(CurrentEquip + 0xFB0);
		int32_t AuthResourceAmount = Read<int32_t>(MagazineAmmo + 0x120);
		int32_t MaxAmmo = Read<int32_t>(MagazineAmmo + 0x11c);
		std::string DisplayAmmoA = "A: " + std::to_string(AuthResourceAmount) + " / " + std::to_string(MaxAmmo);

		
		auto Inventory1 = Read<uintptr_t>(g_localplayerpawn + offsets::inventory);
		intptr_t CurrentEquip1 = Read<uintptr_t>(Inventory1 + offsets::current_equipable);
		uintptr_t MagazineAmmo1 = Read<uintptr_t>(CurrentEquip1 + 0xFB0);
		int32_t AmmoMax = Read<int32_t>(MagazineAmmo1 + 0x11C);
		int32_t CurrentAmmo = Read<int32_t>(MagazineAmmo1 + 0x100);

		if (visuals::ammowarning)
		{
			if (CurrentAmmo < visuals::customwarning)
			{
				ImGui::PushFont(Proggy);
				ImColor Red = { 255, 0, 0, 255 };
				std::string AmmoWarning = "WARNING! Low Ammo";
				ImGui::GetOverlayDrawList()->AddText(ImVec2(Width / 2 - 60, Height / 2 - 350), Red, AmmoWarning.c_str());
			}
		}

		
		if (visuals::hitsound)
		{
			if (health - 1)
			{
				Beep(1000, 100);
			}
		}

			if (misc::autoreload)
			{
				if (CurrentAmmo < misc::customwarningreload)
				{
					INPUT ip;

					ip.type = INPUT_KEYBOARD;
					ip.ki.wScan = 0;
					ip.ki.time = 0;
					ip.ki.dwExtraInfo = 0;

					ip.ki.wVk = 0x52;
					ip.ki.dwFlags = 0;
					SendInput(1, &ip, sizeof(INPUT));

					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
				}
			}

		if (visuals::skeleton) renderBones(enemy, cameraposition, camerarotation, camerafov);



		if (visuals::box) {
			switch (boxselect) {
			case 0: if (vHeadBoneOut.x != 0 || vHeadBoneOut.y != 0 || vHeadBoneOut.z != 0) {
					Vector3 bottom1 = ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y - 40, vBaseBone.z));
					Vector3 bottom2 = ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y - 40, vBaseBone.z));
					Vector3 bottom3 = ProjectWorldToScreen(Vector3(vBaseBone.x - 40, vBaseBone.y + 40, vBaseBone.z));
					Vector3 bottom4 = ProjectWorldToScreen(Vector3(vBaseBone.x + 40, vBaseBone.y + 40, vBaseBone.z));
					Vector3 top1 = ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y - 40, vHeadBone.z + 15));
					Vector3 top2 = ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y - 40, vHeadBone.z + 15));
					Vector3 top3 = ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y + 40, vHeadBone.z + 15));
					Vector3 top4 = ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y + 40, vHeadBone.z + 15));
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ESPColor, 0.1f);
					ImGui::GetOverlayDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ESPColor, 0.1f); } break;
			case 1: DrawNormalBox(vBaseBoneOut.x - BoxWidth / 2 + 1, vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &Col.black); 
				    DrawNormalBox(vBaseBoneOut.x - BoxWidth / 2 - 1, vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawNormalBox(vBaseBoneOut.x - BoxWidth / 2, vHeadBoneOut2.y + 1, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawNormalBox(vBaseBoneOut.x - BoxWidth / 2, vHeadBoneOut2.y - 1, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawNormalBox(vBaseBoneOut.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &espcol); break;
			case 2: DrawCornerBox(vHeadBoneOut2.x - BoxWidth / 2 + 1, vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &Col.black); 
				    DrawCornerBox(vHeadBoneOut2.x - BoxWidth / 2 - 1, vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawCornerBox(vHeadBoneOut2.x - BoxWidth / 2, vHeadBoneOut2.y + 1, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawCornerBox(vHeadBoneOut2.x - BoxWidth / 2, vHeadBoneOut2.y - 1, BoxWidth, BoxHeight, 1, &Col.black);
				    DrawCornerBox(vHeadBoneOut2.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, 1, &espcol); break;
			}
		}

		if (visuals::fillbox) {
			DrawFilledRect3(vHeadBoneOut2.x - (BoxWidth / 2), vHeadBoneOut2.y, BoxWidth, BoxHeight, &Col.glassblack);
		}

		if (visuals::glow)
		{
			//Write<float>(enemy.actor_ptr + offsets::fresnel_intensity, 100);
		}

		if (visuals::headbox)
		{

		}

		if (visuals::headbone)
		{
			switch (headcircleselect)
			{
			case 0: 			if (vHeadBoneOut.x != 0 || vHeadBoneOut.y != 0 || vHeadBoneOut.z != 0) {

				Vector3 bottom1 = ProjectWorldToScreen(Vector3(vBaseBone.x + 20, vBaseBone.y - 20, vBaseBone.z));
				Vector3 bottom2 = ProjectWorldToScreen(Vector3(vBaseBone.x - 20, vBaseBone.y - 20, vBaseBone.z));
				Vector3 bottom3 = ProjectWorldToScreen(Vector3(vBaseBone.x - 20, vBaseBone.y + 20, vBaseBone.z));
				Vector3 bottom4 = ProjectWorldToScreen(Vector3(vBaseBone.x + 20, vBaseBone.y + 20, vBaseBone.z));
				Vector3 top1 = ProjectWorldToScreen(Vector3(vHeadBone.x + 20, vHeadBone.y - 20, vHeadBone.z + 5));
				Vector3 top2 = ProjectWorldToScreen(Vector3(vHeadBone.x - 20, vHeadBone.y - 20, vHeadBone.z + 5));
				Vector3 top3 = ProjectWorldToScreen(Vector3(vHeadBone.x - 20, vHeadBone.y + 20, vHeadBone.z + 5));
				Vector3 top4 = ProjectWorldToScreen(Vector3(vHeadBone.x + 20, vHeadBone.y + 20, vHeadBone.z + 5));

				//others
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ESPColor, 0.1f);

				//bottom square
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), ESPColor, 0.1f);


				//top square
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ESPColor, 0.1f);
				ImGui::GetOverlayDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ESPColor, 0.1f);
			} break;

			case 1: ImGui::GetOverlayDrawList()->AddCircle(head_at_screen, 7 / DistanceModifier, ImColor(colors::headbonecolor[0] * 255, colors::headbonecolor[1] * 255, colors::headbonecolor[2] * 255, colors::headbonecolor[3] * 255), 0, 3);
			}

		}

		if (visuals::snapline) {
			switch (lineselect) {
			case 0: DrawLine(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(2500), root_at_screen.x, root_at_screen_vec.y, ImColor(255, 255, 255, 150), visuals::thickness); break;
			case 1: DrawLine3(ImVec2(Width / 2, Height / 2), ImVec2(vHeadBoneOut2.x, vHeadBoneOut2.y),aimbot::fov, &espcol, visuals::thickness); break;
			case 2: DrawLine(GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN), root_at_screen.x, root_at_screen_vec.y, ImColor(255, 255, 255, 150), visuals::thickness); break;
			}
		}

		if (visuals::health) DrawHealthBar(vHeadBoneOut.x - (BoxWidth / 2) - 8, vHeadBoneOut.y, width, vBaseBoneOut.y - vHeadBoneOut.y, health, true);

		if (misc::radar) {

			int customX = +950;
			int customY = +300;

			pRadar.X = (Width / 2) - customX;
			pRadar.Y = Width / 2 - Height / 2 + customY;
			DrawRect(pRadar.X, pRadar.Y, 270, 270, &Col.black, 1);
			DrawBorder(pRadar.X, pRadar.Y, 270, 270, 1, &Col.black);
			auto radar_posX = pRadar.X + 135;
			auto radar_posY = pRadar.Y + 135;
			DrawLineNew(radar_posX, radar_posY, radar_posX, radar_posY + 135, &Col.black, 1);
			DrawLineNew(radar_posX, radar_posY, radar_posX, radar_posY - 135, &Col.black, 1);
			DrawLineNew(radar_posX, radar_posY, radar_posX + 135, radar_posY, &Col.black, 1);
			DrawLineNew(radar_posX, radar_posY, radar_posX - 135, radar_posY, &Col.black, 1);
			DrawCircleFilled2(radar_posX, radar_posY, 3, &Col.green);
			FVector pos = Read<FVector>(enemy.rootcomponent_ptr + offsets::root_position);
			DrawRadar(pos);
		}

		if (visuals::ammo)
		{
			ImVec2 TextSize = ImGui::CalcTextSize(DisplayAmmoA.c_str());
			ImGui::PushFont(Proggy);
			ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x + 20 - TextSize.x / 2, vBaseBoneOut2.y - 35 - TextSize.y / 2), ImColor({ 0, 255, 205, 255 }), DisplayAmmoA.c_str());
		}
		
		/*
		if (visuals::action)
		{
			uintptr_t InventoryE = Read<uintptr_t>(enemy.actor_ptr + 0x948);
			if (InventoryE) {
				uintptr_t CurrentEqippableE = Read<uintptr_t>(InventoryE + 0x228);
				if (CurrentEqippableE) {
					EAresEquippableState1 CurrentEquippableVFXState = Read<EAresEquippableState1>(CurrentEqippableE + 0xCC0);
					std::string Out1 = GetEquippableVFXState1(CurrentEquippableVFXState);
					ImVec2 TextSize = ImGui::CalcTextSize(Out1.c_str());
					ImGui::GetOverlayDrawList()->AddText(ImVec2(vBaseBoneOut2.x - TextSize.x / 2, vBaseBoneOut2.y - TextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out1.c_str());
				}
			}
		}
		*/
		if (visuals::infobar) {

			char dist[64];
			std::string Out = "[ " + std::to_string(static_cast<int32_t>(Distance)) + " meters ]";
			//sprintf_s(dist, "%.f meteres", Distance);
			//std::string Out = dist;
			ImVec2 TextSize = ImGui::CalcTextSize(Out.c_str());

			std::string healthValue = (std::to_string((int)health) + " Health").c_str();
			ImVec2 HealthTextSize = ImGui::CalcTextSize(healthValue.c_str());

			ImGui::PushFont(Proggy);
			ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - HealthTextSize.x / 2, (vHeadBoneOut2.y - 5) - HealthTextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), healthValue.c_str()); //TOPBOX
			ImGui::GetOverlayDrawList()->AddText(ImVec2(vHeadBoneOut2.x - HealthTextSize.x / 2, (vHeadBoneOut2.y - 20) - HealthTextSize.y / 2), ImGui::GetColorU32({ 255, 255, 255, 255 }), Out.c_str()); //BOTTOMBOX
			ImGui::PopFont();
		}

		if (Distance < visuals::maxdist)
		{
			continue;
		}
	}
}
constexpr int WndWidth = 600;
constexpr int WndHeight = 400;
inline bool isRunning = true;
static int x, y;
static bool animation_menu = true;
static int plus2 = 1;
inline auto runRenderTick() -> void {

	if (animation_menu)
	{
		plus2 += 10;
		if (plus2 >= 480)
		{
			plus2 = 480;
			animation_menu = false;
		}
	}

	glfwPollEvents();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	//renderEsp(); //uncomment

	if (aimbot::drawfov) ImGui::GetOverlayDrawList()->AddCircle(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2.0f, GetSystemMetrics(SM_CYSCREEN) / 2.0f), aimbot::fov, ImColor(colors::fovcrclecolor[0] * 255, colors::fovcrclecolor[1] * 255, colors::fovcrclecolor[2] * 255, colors::fovcrclecolor[3] * 255), 100000, 1);

	if (visuals::watermark)
	{
		ImGui::PushFont(Proggy);
		ImColor Red = { 255, 255, 255, 255 };
		std::string Watermark = "[GOOD PASTE]";
		ImGui::GetOverlayDrawList()->AddText(ImVec2(Width / 2 - 45, Height / 2 - 400), Red, Watermark.c_str());
	}

	if (overlay) {
		/*
		ImGui::InitVGTheme();
		ImGui::SetNextWindowSize({ WndWidth, WndHeight });
		ImGui::SetNextWindowPos(ImVec2(HlfWidth / 2, HlfHeight / 2));
		if (ImGui::Begin(skCrypt("Valorant"), &isRunning, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse)) {

			if (overlay == true)
				//dot_draw2();

			ImGui::BeginChild(skCrypt("##TabsBar"), ImVec2(100, ImGui::GetContentRegionAvail().y), TRUE);
			{
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 50);

				ImGui::PushFont(Large);
				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("VGC")).x) / 2);
				ImGui::Text(skCrypt("  BE"));
				ImGui::PopFont();

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
				ImGuiPP::Line(1);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 25);

				ImColor orange = { 255, 150, 0, 0 };

				ImGui::PushStyleColor(orange, MenuTab == 1 ? active : inactive);
				ImGuiPP::CenterTextEx("Aimbot", 105, 0, 0);
				if (ImGui::IsItemClicked()) MenuTab = 1;

				ImGui::NewLine();
				ImGui::PushStyleColor(orange, MenuTab == 2 ? active : inactive);
				ImGuiPP::CenterTextEx("Visuals", 105, 0, 0);
				if (ImGui::IsItemClicked()) MenuTab = 2;

				ImGui::NewLine();
				ImGui::Spacing();
				ImGui::PushStyleColor(orange, MenuTab == 3 ? active : inactive);
				ImGuiPP::CenterTextEx("Misc", 105, 0, 0);
				if (ImGui::IsItemClicked()) MenuTab = 3;

				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);
				ImGuiPP::Line(2);
				ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 15);

				ImGui::PopStyleColor(3);

				ImGui::SetWindowFontScale(0.75);
				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("Menu key : Insert")).x) / 2);
				ImGui::TextDisabled(skCrypt("Menu key : Insert"));
				ImGui::SetWindowFontScale(1);

				ImGui::SetWindowFontScale(0.75);
				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("GOOD PASTE")).x) / 2);
				ImGui::TextDisabled(skCrypt("GOOD PASTE"));
				ImGui::SetWindowFontScale(1);

				ImGui::SetWindowFontScale(0.75);
				ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("Current Valorant build (5.03)")).x) / 2);
				ImGui::TextDisabled(skCrypt("Build (5.03)"));
				ImGui::SetWindowFontScale(1);
			}

			ImGui::EndChild();
			ImGuiPP::Linevertical();

			{
				ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(0, 0, 0, 0));
				ImGui::BeginChild(skCrypt("##RightSide"), ImVec2(ImGuiPP::GetX(), ImGuiPP::GetY()), TRUE);
				ImGui::PopStyleColor();
				{
					switch (MenuTab)
					{
					case 1:

						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);
						ImGui::BeginChild(skCrypt("##AimbotBar"), ImVec2(ImGui::GetContentRegionAvail().x, 40), TRUE);
						ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::CalcTextSize(skCrypt("Aimbot")).y) / 2);
						ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("Aimbot")).x) / 2);
						ImGui::Text(skCrypt("Aimbot"));
						ImGui::EndChild();

						ImGui::Spacing();
						ImGuiPP::Line(1);
						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Memory Aimbot"), &aimbot::aimbot);
						ImGui::SameLine(); ImGui::SetCursorPosX(335); ImGui::Hotkey(&aimkey);

						ImGui::Spacing();
						ImGuiPP::Line(2);
						ImGui::Spacing();

						if (aimbot::aimbot)
						{
							ImGui::Checkbox(skCrypt("Visibility Check"), &aimbot::visible);
							ImGui::SameLine(); ImGui::SetCursorPosX(230);
							ImGui::Checkbox(skCrypt("Enable Smoothing"), &aimbot::smooth);

							ImGui::Spacing();

							ImGui::Checkbox(skCrypt("Draw FOV Circle"), &aimbot::drawfov);
							ImGui::SameLine(); ImGui::SetCursorPosX(230);
							ImGui::Checkbox(skCrypt("Enable Aim Bone"), &aimbot::boneselect);

							ImGui::Spacing();
							ImGuiPP::Line(3);
							ImGui::Spacing();

							ImGui::SliderFloat(skCrypt(" FOV Value"), &aimbot::fov, 1, 1000);

							if (aimbot::smooth)
							{
								ImGui::Spacing();
								ImGui::SliderFloat(skCrypt(" Smoothness"), &aimbot::smoothness, 0, 1, skCrypt("%0.1f"));
							}
							if (aimbot::boneselect)
							{
								ImGui::Spacing();
								ImGui::Combo(skCrypt(" Aim Bone"), &aimboneselect, aimboneitems, sizeof(aimboneitems) / sizeof(*aimboneitems));

								if (aimboneselect == 0) aimbot::aimbone = 8;
								if (aimboneselect == 1) aimbot::aimbone = 19;
								if (aimboneselect == 2) aimbot::aimbone = 6;
							}
							else aimboneselect == 0;
						}
						break;

					case 2:
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);
						ImGui::BeginChild(skCrypt("##VisualsBar"), ImVec2(ImGui::GetContentRegionAvail().x, 40), TRUE);
						ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::CalcTextSize(skCrypt("Visuals")).y) / 2);
						ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("Visuals")).x) / 2);
						ImGui::Text(skCrypt("Visuals"));
						ImGui::EndChild();

						ImGui::Spacing();
						ImGuiPP::Line(1);
						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Box ESP"), &visuals::box);
						ImGui::SameLine(); ImGui::SetCursorPosX(230);
						ImGui::Checkbox(skCrypt("Skeleton ESP"), &visuals::skeleton);

						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Snapline ESP"), &visuals::snapline);
						ImGui::SameLine(); ImGui::SetCursorPosX(230);
						ImGui::Checkbox(skCrypt("Head ESP"), &visuals::headbone);

						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Info ESP"), &visuals::infobar);
						ImGui::SameLine(); ImGui::SetCursorPosX(230);
						ImGui::Checkbox(skCrypt("Health ESP"), &visuals::health);

						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Enable Dormant Check"), &visuals::dormant);
						ImGui::SameLine(); ImGui::SetCursorPosX(230);
						ImGui::Checkbox(skCrypt("Draw Filled Box ESP"), &visuals::fillbox);

						if (visuals::box == false) {
							visuals::fillbox == false;
						}

						if (visuals::box)
						{
							ImGui::Spacing();
							ImGuiPP::Line(2);
							ImGui::Spacing();

							ImGui::Combo(skCrypt(" Box Type"), &boxselect, boxitems, 3);
						}

						if (visuals::headbone)
						{
							ImGui::Spacing();
							ImGui::Combo(skCrypt(" Head Type"), &headcircleselect, circleitems, 2);
						}

						if (visuals::snapline)
						{
							ImGui::Spacing();
							ImGui::Combo(skCrypt(" Line Position"), &lineselect , lineitems, 3);

							ImGui::Spacing();
							ImGui::SliderFloat(skCrypt(" Line Thickness"), &visuals::thickness, 1, 5, skCrypt("%1.f"));
						}
						break;

					case 3:
						ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11);
						ImGui::BeginChild(skCrypt("##MiscBar"), ImVec2(ImGui::GetContentRegionAvail().x, 40), TRUE);
						ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::CalcTextSize(skCrypt("Miscellaneous")).y) / 2);
						ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(skCrypt("Miscellaneous")).x) / 2);
						ImGui::Text(skCrypt("Miscellaneous"));
						ImGui::EndChild();

						ImGui::Spacing();
						ImGuiPP::Line(1);
						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Radar"), &misc::radar);

						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Ammo Warning"), &visuals::ammowarning);

						ImGui::Spacing();

						ImGui::Checkbox(skCrypt("Auto Reload (Beta)"), &misc::autoreload);

						ImGui::Spacing();
						ImGui::Separator();
						ImGui::Spacing();

						if (visuals::ammowarning)
						{
							ImGui::Spacing();
							ImGui::SliderInt(skCrypt("Custom Warning"), &visuals::customwarning, 0, 30, skCrypt("%0.1f (Amount of Ammo)"));
							ImGui::Spacing();
						}



						/*
						if (misc::radar)
						{
							ImGui::Spacing();
							ImGui::SliderInt(skCrypt("Radar X POS"), &customX, -300, 300, skCrypt("%0.1f"));
							ImGui::Spacing();
							ImGui::SliderInt(skCrypt("Radar Y POS"), &customY, -300, 300, skCrypt("%0.1f"));
							ImGui::Spacing();
						}
						*/

/*

						if (misc::autoreload)
						{
							ImGui::Spacing();
							ImGui::SliderInt(skCrypt("Custom Reload"), &misc::customwarningreload, 0, 30, skCrypt("%0.1f (Amount of Ammo)"));
							ImGui::Spacing();
						}

						bool SkeletonColor_open = ImGui::ColorButton(skCrypt("##skeletoncolor"), *(ImVec4*)&colors::skeletoncolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions);
						if (SkeletonColor_open)
						{
							ImGui::OpenPopup(skCrypt("boxcolor5"));
						}
						if (ImGui::BeginPopup(skCrypt("boxcolor5")))
						{
							ImGui::ColorPicker3(skCrypt("##boxcolor5"), (float*)&colors::skeletoncolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text(skCrypt("ESP Skeleton Color"));

						ImGui::SameLine(); ImGui::SetCursorPosX(230);

						bool HeadCircleColor_open = ImGui::ColorButton(("##headbonecolor"), *(ImVec4*)&colors::headbonecolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions);
						if (HeadCircleColor_open)
						{
							ImGui::OpenPopup(skCrypt("boxcolor4"));
						}
						if (ImGui::BeginPopup(skCrypt("boxcolor4")))
						{
							ImGui::ColorPicker3(skCrypt("##boxcolor4"), (float*)&colors::headbonecolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text(skCrypt("ESP Head Circle Color"));

						ImGui::Spacing();

						bool FOVColor_open = ImGui::ColorButton(skCrypt("##fovcolor"), *(ImVec4*)&colors::fovcrclecolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions);
						if (FOVColor_open)
						{
							ImGui::OpenPopup(skCrypt("boxcolor3"));
						}
						if (ImGui::BeginPopup(skCrypt("boxcolor3")))
						{
							ImGui::ColorPicker3(skCrypt("##boxcolor3"), (float*)&colors::fovcrclecolor, ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);
							ImGui::EndPopup();
						}
						ImGui::SameLine();
						ImGui::Text(skCrypt("FOV Circle Color"));
						break;
					}
				}

				ImGui::EndChild();
			}
		}

		ImGui::End();
		
		*/
		//new menu 
ImGui::InitVGTheme();
ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { NULL, NULL });

auto display_size = ImGui::GetIO().DisplaySize;

ImGui::SetNextWindowSize({ 720, 420 /*length y*/ });
ImGui::SetNextWindowPos({ display_size.x / 2 - (700 / 2),  display_size.y / 2 - (530 / 2) }, ImGuiCond_Once);

ImGui::Begin("##MENU", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar);
{

	auto window_current = ImGui::GetCurrentWindow();
	auto window_draw = ImGui::GetWindowDrawList();
	auto window_pos = ImGui::GetWindowPos();

	window_draw->AddRectFilled(window_pos, { window_pos.x + window_current->Size.x, window_pos.y + 65 }, ImColor(35, 35, 42), window_current->WindowRounding, ImDrawCornerFlags_Top);

	static int tab = NULL;

	ImGui::SetCursorPos({ 110, 65 / 2 - ImGui::CalcTextSize("cheat name").y / 2 });
	ImGui::BeginGroup();
	{
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 65, 0 });

		if (Custom1337::tab("Aimbot", tab == 0)) tab = 0;
		ImGui::SameLine();
		if (Custom1337::tab("Visuals", tab == 1)) tab = 1;
		ImGui::SameLine();
		if (Custom1337::tab("Misc", tab == 2)) tab = 2;

		ImGui::PopStyleVar();
	}
	ImGui::EndGroup();

	if (tab == 0)
	{
		ImGui::SetCursorPos({ 20, 85 });
		Custom1337::beginChild("Aimbot", { 700 / 2 - 15, 325 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{
				ImGui::Checkbox("Memory Aimbot", &aimbot::aimbot);

				ImGui::Checkbox(skCrypt("Visibility Check"), &aimbot::visible);

				ImGui::Checkbox(skCrypt("Enable Smoothing"), &aimbot::smooth);

				ImGui::Checkbox(skCrypt("Aim at Bone"), &aimbot::boneselect);

				ImGui::Checkbox("Render FOV", &aimbot::drawfov);


			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();

		ImGui::SetCursorPos({ 365, 85 });
		Custom1337::beginChild("Aimbot Settings", { 700 / 2 - 15, 265 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{

					ImGui::Spacing();
					ImGui::Text("Smoothing Factor (0.1 - 0.6 = Rage, 0.7 - 1 = Legit)");
					ImGui::SliderFloat(skCrypt("##Smoothness"), &aimbot::smoothness, 0, 1, skCrypt("%0.1f"));

					ImGui::Spacing();
					ImGui::Text("Aim Key (EU Keyboard Layout)");
					ImGui::Spacing();
					ImGui::Hotkey(&aimkey);

					ImGui::Spacing();
					ImGui::Text("Aimbone");
					ImGui::Combo(skCrypt("##Aim Bone"), &aimboneselect, aimboneitems, sizeof(aimboneitems) / sizeof(*aimboneitems));

					if (aimboneselect == 0) aimbot::aimbone = 8;
					if (aimboneselect == 1) aimbot::aimbone = 19;
					if (aimboneselect == 2) aimbot::aimbone = 6;
				else aimboneselect == 0;

					ImGui::Spacing();
					ImGui::Text("Field of View Value");
					ImGui::SliderFloat(skCrypt("##FOV Value"), &aimbot::fov, 1, 1000);
			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();
	}
	if (tab == 1)
	{
		ImGui::SetCursorPos({ 20, 85 });
		Custom1337::beginChild("Visuals", { 700 / 2 - 15, 325 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{
				ImGui::Checkbox("Box ESP", &visuals::box);

				ImGui::Checkbox(skCrypt("Skeleton ESP"), &visuals::skeleton);

				ImGui::Checkbox(skCrypt("Snaplines"), &visuals::snapline);

				ImGui::Checkbox(skCrypt("Health ESP"), &visuals::health);

				ImGui::Checkbox("Numerical Info", &visuals::infobar);

				ImGui::Checkbox("Dormant", &visuals::dormant);

				ImGui::Checkbox("Head Circle", &visuals::headbone);

			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();

		ImGui::SetCursorPos({ 365, 85 });
		Custom1337::beginChild("Visual Settings", { 700 / 2 - 15, 305 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{
				ImGui::Text("Box Type");
				ImGui::Spacing();
				ImGui::Combo(skCrypt("##Box Type"), &boxselect, boxitems, 3);

				ImGui::Text("Head Type");
				ImGui::Spacing();
				ImGui::Combo(skCrypt("##Head Type"), &headcircleselect, circleitems, 2);

				ImGui::Text("Snapline Position");
				ImGui::Spacing();
				ImGui::Combo(skCrypt("##Snapline Position"), &lineselect, lineitems, 3);

				ImGui::Text("Snapline Thickness");
				ImGui::Spacing();
				ImGui::SliderFloat(skCrypt("##Snapline Thickness"), &visuals::thickness, 1, 5, skCrypt("%1.f"));

				ImGui::Text("Max ESP Distance");
				ImGui::Spacing();
				ImGui::SliderInt(skCrypt("##ESPDistance"), &visuals::maxdist, 100, 500, skCrypt("%1.f"));

			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();
	}
	if (tab == 2)
	{
		ImGui::SetCursorPos({ 20, 85 });
		Custom1337::beginChild("Miscellaneous", { 700 / 2 - 15, 325 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{
				ImGui::Checkbox("Radar", &misc::radar);

				ImGui::Checkbox(skCrypt("Ammo Alert"), &visuals::ammowarning);

				ImGui::Checkbox(skCrypt("Auto Reload (dont use)"), &misc::autoreload);

				ImGui::Checkbox(skCrypt("Hitsound (global)"), &visuals::hitsound);

			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();

		ImGui::SetCursorPos({ 365, 85 });
		Custom1337::beginChild("Miscellaneous Settings", { 700 / 2 - 15, 265 });
		{
			ImGui::SetCursorPos({ 20, 20 });
			ImGui::BeginGroup();
			{
				ImGui::Spacing();
				ImGui::SliderInt(skCrypt("Custom Warning"), &visuals::customwarning, 0, 30, skCrypt("%0.1f (Amount of Ammo)"));
				ImGui::Spacing();

				ImGui::Spacing();
				ImGui::SliderInt(skCrypt("Custom Reload"), &misc::customwarningreload, 0, 30, skCrypt("%0.1f (Amount of Ammo)"));
				ImGui::Spacing();


			}
			ImGui::EndGroup();
		}
		Custom1337::endChild();

	}
}
ImGui::End();
ImGui::PopStyleVar();

	}


	ImGui::Render();
	int display_w, display_h;
	glfwGetFramebufferSize(g_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(g_window);
}