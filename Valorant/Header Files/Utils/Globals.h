#pragma once

inline RGBA espcol;

inline ImFont* Large = nullptr;
inline ImFont* Proggy = nullptr;

static ULONG Width = GetSystemMetrics(SM_CXSCREEN);
static ULONG Height = GetSystemMetrics(SM_CYSCREEN);

inline int HlfWidth = GetSystemMetrics(SM_CXSCREEN) - 600;
inline int HlfHeight = GetSystemMetrics(SM_CYSCREEN) - 400;

static ImVec4 active = ImGuiPP::ToVec4(37, 150, 190, 255);
static ImVec4 inactive = ImGuiPP::ToVec4(0, 70, 110, 255);

inline int aimkey = VK_SHIFT;

inline auto DrvPath = "C:\\Windows\\System32\\drivers\\TECH.sys";
inline auto DrvPath1 = "C:\\Windows\\System32\\drivers\\Nal.sys";
inline int selection = 0;
static int MenuTab = 1;

using namespace VALORANT_EXT;

#define FINLINE __forceinline

namespace VALORANT_EXT {

	inline int g_width{};
	inline int g_height{};

	inline bool overlay{ false };

	namespace aimbot {

		inline bool aimbot{ true };
		inline bool visible{ false };
		inline bool boneselect{ true };
		inline bool drawfov{ true };
		inline bool smooth{ true };
		inline float smoothness = 1.0f;
		inline float fov = 100.0f;
		inline int aimbone = 8;
	}

	namespace visuals {

		inline bool box{ false };
		inline bool fillbox{ false };
		inline bool headbone{ false };
		inline bool headbox{ false };
		inline bool skeleton{ false };
		inline bool snapline{ false };
		inline bool infobar{ false };
		inline bool dormant{ false };
		inline bool health{ false };
		inline bool glow{ false };
		inline bool ammo{ false };
		inline bool action{ false };
		inline bool ammowarning{ false };
		inline int customwarning = 4;
		inline float thickness = 1.0f;
		inline int maxdist = 300;
		inline bool watermark = true;
		inline bool hitsound = false;
	}

	namespace misc {
		inline bool autoreload{ false };
		inline int customwarningreload = 0;

		inline bool radar{ false };

	}

	namespace colors  {

		inline float headbonecolor[] = { 1.f,1.f,1.f,1.f };
		inline float skeletoncolor[] = { 1.f,1.f,1.f,1.f };
		inline float fovcrclecolor[] = { 1.f,1.f,1.f,1.f };
	}

	inline int aimboneselect = 0;
	inline int lineselect = 0;
	inline int boxselect = 1;
	inline int headcircleselect = 0;

	static const char* aimboneitems[]{ "Head", "Neck", "Pelvis" };
	static const char* lineitems[]{ "Top", "Middle","Bottom" };
	static const char* boxitems[]{ "3D Box", "2D Box", "Cornered Box" };
	static const char* circleitems[]{ "Box", "Circle"};
}

inline auto mem = std::make_unique<MemoryManagement>();

namespace VALORANT_EXT {

	template<typename T>
	inline BOOL Read(DWORD_PTR Address, T* pBuffer, DWORD Size)
	{
		BOOL bRead = FALSE;
		DWORD_PTR Rsh_x24 = (Address >> 0x24);
		if (Rsh_x24 == 0x8 || Rsh_x24 == 0x10)
			bRead = mem->ReadGuardedRegion(DWORD_PTR(Address & 0xFFFFFF), (PVOID)pBuffer, sizeof(T));
		else
			bRead = mem->Read_Buf(Address, (PVOID)pBuffer, sizeof(T));
		return bRead;
	}
	template <typename T>
	inline T Read(DWORD_PTR Address)
	{
		T Buffer{};
		Read(Address, &Buffer, sizeof(T));
		return Buffer;
	}



	typedef struct {
		uintptr_t actor_ptr;
		uintptr_t damagehandler_ptr;
		uintptr_t playerstate_ptr;
		uintptr_t rootcomponent_ptr;
		uintptr_t mesh_ptr;
		uintptr_t bonearray_ptr;
		int bonecount;
		bool isvisible;
	} Enemy;
	struct FText
	{
		char _padding_[0x28];
		PWCHAR Name;
		DWORD Length;
	};

	inline int g_pid{};
	inline uintptr_t g_baseaddress{};
	inline uintptr_t g_localplayercontroller;
	inline uintptr_t g_localplayerpawn;
	inline uintptr_t g_localdamagehandler;
	inline uintptr_t g_cameramanager;
	inline int g_localteamid;

	// Window / Process values
	inline HWND valorantwindow{};
	inline GLFWwindow* g_window{};

	__forceinline __int64 decrypt_uworld(const uint32_t key, const uintptr_t* state) {

		unsigned __int64 v19; // r11
		unsigned __int64 v20; // r8
		unsigned __int64 v21; // r9
		unsigned int v22; // er10
		unsigned __int64 v23; // rcx
		unsigned __int64 v24; // rdx
		unsigned __int64 v25; // rcx
		int v26; // ebx
		unsigned int v27; // ecx
		__int64 v28; // rax
		unsigned __int64 v29; // r8
		unsigned __int64 v30; // r8
		unsigned __int64 v31; // rcx
		unsigned __int64 v32; // rdx
		unsigned __int64 v33; // rcx

		v19 = 2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))
			% 7;
		v20 = state[v19];
		v21 = (2685821657736338717i64
			* ((unsigned int)key ^ (unsigned int)(key << 25) ^ (((unsigned int)key ^ ((unsigned __int64)(unsigned int)key >> 15)) >> 12))) >> 32;
		v22 = (unsigned int)v19 % 7;
		if (!((unsigned int)v19 % 7))
		{
			v23 = (2 * (v20 - (unsigned int)(v21 - 1))) ^ ((2 * (v20 - (unsigned int)(v21 - 1))) ^ ((v20
				- (unsigned int)(v21 - 1)) >> 1)) & 0x5555555555555555i64;
			v24 = (4 * v23) ^ ((4 * v23) ^ (v23 >> 2)) & 0x3333333333333333i64;
			v25 = (16 * v24) ^ ((16 * v24) ^ (v24 >> 4)) & 0xF0F0F0F0F0F0F0Fi64;
			v20 = __ROL8__((v25 << 8) ^ ((v25 << 8) ^ (v25 >> 8)) & 0xFF00FF00FF00FFi64, 32);
		LABEL_26:
			v26 = 2 * v19;
			goto LABEL_27;
		}
		if (v22 != 1)
			goto LABEL_26;
		v26 = 2 * v19;
		v20 = __ROL8__(v20 - (unsigned int)(2 * v19 + v21), (unsigned __int8)(((int)v21 + (int)v19) % 0x3Fu) + 1);
	LABEL_27:
		v27 = v26 + v21;
		if (v22 == 2)
			v20 = ~(v20 - v27);
		switch (v22)
		{
		case 3u:
			v28 = 2 * ((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64);
			v20 = v28 ^ (v28 ^ (((2 * v20) ^ ((2 * v20) ^ (v20 >> 1)) & 0x5555555555555555i64) >> 1)) & 0x5555555555555555i64;
			break;
		case 4u:
			v29 = __ROR8__(v20, (unsigned __int8)(v27 % 0x3F) + 1);
			v20 = (2 * v29) ^ ((2 * v29) ^ (v29 >> 1)) & 0x5555555555555555i64;
			break;
		case 5u:
			v30 = __ROR8__(v20, (unsigned __int8)(v27 % 0x3F) + 1);
			v31 = (2 * v30) ^ ((2 * v30) ^ (v30 >> 1)) & 0x5555555555555555i64;
			v32 = (4 * v31) ^ ((4 * v31) ^ (v31 >> 2)) & 0x3333333333333333i64;
			v33 = (16 * v32) ^ ((16 * v32) ^ (v32 >> 4)) & 0xF0F0F0F0F0F0F0Fi64;
			v20 = __ROL8__((v33 << 8) ^ ((v33 << 8) ^ (v33 >> 8)) & 0xFF00FF00FF00FFi64, 32);
			break;
		case 6u:
			v20 = ~v20 - (unsigned int)(v21 + v19);
			break;
		}
		return v20 ^ (unsigned int)key;
	}

	inline auto decryptWorld(uintptr_t base_address) -> uintptr_t {
		auto key = Read<uintptr_t>(base_address + offsets::uworld_key);
#pragma pack(push, 1)
		struct State
		{
			uint64_t Keys[7];
		};
#pragma pack(pop)
		auto state = Read<State>(base_address + offsets::uworld_state);
		auto decrypt = decrypt_uworld(key, (uintptr_t*)&state);
		return Read<uintptr_t>(decrypt);
	}

	inline auto getBonePosition(Enemy enemy, int index) -> Vector3 {

		FTransform Transform = Read<FTransform>(enemy.bonearray_ptr + (0x30 * index));
		FMatrix Matrix = Transform.ToMatrixWithScale() * Read<FTransform>(enemy.mesh_ptr + offsets::component_to_world).ToMatrixWithScale();
		return Vector3(Matrix.WPlane.X, Matrix.WPlane.Y, Matrix.WPlane.Z);
	}





	inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2) {

		D3DMATRIX pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}

#define M_PI 3.1415926535

	inline D3DMATRIX toMatrix(Vector3 Rotation, Vector3 origin = Vector3(0, 0, 0));
	inline D3DMATRIX toMatrix(Vector3 Rotation, Vector3 origin) {

		float Pitch = (Rotation.x * float(M_PI) / 180.f);
		float Yaw = (Rotation.y * float(M_PI) / 180.f);
		float Roll = (Rotation.z * float(M_PI) / 180.f);

		float SP = sinf(Pitch);
		float CP = cosf(Pitch);
		float SY = sinf(Yaw);
		float CY = cosf(Yaw);
		float SR = sinf(Roll);
		float CR = cosf(Roll);

		D3DMATRIX Matrix;
		Matrix._11 = CP * CY;
		Matrix._12 = CP * SY;
		Matrix._13 = SP;
		Matrix._14 = 0.f;

		Matrix._21 = SR * SP * CY - CR * SY;
		Matrix._22 = SR * SP * SY + CR * CY;
		Matrix._23 = -SR * CP;
		Matrix._24 = 0.f;

		Matrix._31 = -(CR * SP * CY + SR * SY);
		Matrix._32 = CY * SR - CR * SP * SY;
		Matrix._33 = CR * CP;
		Matrix._34 = 0.f;

		Matrix._41 = origin.x;
		Matrix._42 = origin.y;
		Matrix._43 = origin.z;
		Matrix._44 = 1.f;

		return Matrix;
	}

	inline auto worldToScreen(Vector3 world_location, Vector3 position, Vector3 rotation, float fov) -> Vector2 {

		Vector2 screen_location = Vector2(0, 0);

		D3DMATRIX tempMatrix = toMatrix(rotation);

		Vector3 vAxisX, vAxisY, vAxisZ;

		vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = world_location - position;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 0.001f)
			vTransformed.z = 0.001f;

		float FovAngle = fov;
		float ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
		float ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;

		screen_location.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		screen_location.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return screen_location;
	}

	inline float FovAngle;

	inline auto ProjectWorldToScreen(Vector3 WorldLocation) -> Vector3
	{
		Vector3 Screenlocation = Vector3(0, 0, 0);

		Vector3 CameraLocation = Read<Vector3>(g_cameramanager + offsets::camera_position);
		Vector3 CameraRotation = Read<Vector3>(g_cameramanager + offsets::camera_rotation);
		float camera_fov = Read<float>(g_cameramanager + offsets::camera_fov);

		D3DMATRIX tempMatrix = toMatrix(CameraRotation, Vector3(0, 0, 0));

		Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]),
			vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]),
			vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		Vector3 vDelta = WorldLocation - CameraLocation;
		Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.z < 1.f) vTransformed.z = 1.f;

		FovAngle = camera_fov;

		float ScreenCenterX = GetSystemMetrics(SM_CXSCREEN) / 2.0f;
		float ScreenCenterY = GetSystemMetrics(SM_CYSCREEN) / 2.0f;

		Screenlocation.x = ScreenCenterX + vTransformed.x * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;
		Screenlocation.y = ScreenCenterY - vTransformed.y * (ScreenCenterX / tanf(FovAngle * (float)M_PI / 360.f)) / vTransformed.z;

		return Screenlocation;
	}
}

namespace VALORANT_EXT { // RENDER

	inline auto renderBoneLine(Vector3 first_bone_position, Vector3 second_bone_position, Vector3 position, Vector3 rotation, float fov) -> void {

		Vector2 first_bone_screen_position = worldToScreen(first_bone_position, position, rotation, fov);
		ImVec2 fist_screen_position = ImVec2(first_bone_screen_position.x, first_bone_screen_position.y);
		Vector2 second_bone_screen_position = worldToScreen(second_bone_position, position, rotation, fov);
		ImVec2 second_screen_position = ImVec2(second_bone_screen_position.x, second_bone_screen_position.y);
		ImGui::GetOverlayDrawList()->AddLine(fist_screen_position, second_screen_position, ImColor(colors::skeletoncolor[0] * 255, colors::skeletoncolor[1] * 255, colors::skeletoncolor[2] * 255, colors::skeletoncolor[3] * 255));
	}

	inline auto renderBones(Enemy enemy, Vector3 position, Vector3 rotation, float fov)  -> void {

		Vector3 head_position = getBonePosition(enemy, 8);
		Vector3 neck_position;
		Vector3 chest_position = getBonePosition(enemy, 6);
		Vector3 l_upper_arm_position;
		Vector3 l_fore_arm_position;
		Vector3 l_hand_position;
		Vector3 r_upper_arm_position;
		Vector3 r_fore_arm_position;
		Vector3 r_hand_position;
		Vector3 stomach_position = getBonePosition(enemy, 4);
		Vector3 pelvis_position = getBonePosition(enemy, 3);
		Vector3 l_thigh_position;
		Vector3 l_knee_position;
		Vector3 l_foot_position;
		Vector3 r_thigh_position;
		Vector3 r_knee_position;
		Vector3 r_foot_position;

		if (enemy.bonecount == 104) { // MALE & FEMALE

			neck_position = getBonePosition(enemy, 19);

			l_upper_arm_position = getBonePosition(enemy, 21);
			l_fore_arm_position = getBonePosition(enemy, 22);
			l_hand_position = getBonePosition(enemy, 23);

			r_upper_arm_position = getBonePosition(enemy, 47);
			r_fore_arm_position = getBonePosition(enemy, 48);
			r_hand_position = getBonePosition(enemy, 49);

			l_thigh_position = getBonePosition(enemy, 75);
			l_knee_position = getBonePosition(enemy, 76);
			l_foot_position = getBonePosition(enemy, 78);

			r_thigh_position = getBonePosition(enemy, 82);
			r_knee_position = getBonePosition(enemy, 83);
			r_foot_position = getBonePosition(enemy, 85);
		}

		else if (enemy.bonecount == 101) {

			neck_position = getBonePosition(enemy, 21);

			l_upper_arm_position = getBonePosition(enemy, 45);
			l_fore_arm_position = getBonePosition(enemy, 42);
			l_hand_position = getBonePosition(enemy, 44);

			r_upper_arm_position = getBonePosition(enemy, 48);
			r_fore_arm_position = getBonePosition(enemy, 67);
			r_hand_position = getBonePosition(enemy, 69);

			l_thigh_position = getBonePosition(enemy, 75);
			l_knee_position = getBonePosition(enemy, 76);
			l_foot_position = getBonePosition(enemy, 78);

			r_thigh_position = getBonePosition(enemy, 82);
			r_knee_position = getBonePosition(enemy, 84);
			r_foot_position = getBonePosition(enemy, 85);
		}

		else if (enemy.bonecount == 103) { // BOT

			neck_position = getBonePosition(enemy, 9);

			l_upper_arm_position = getBonePosition(enemy, 33);
			l_fore_arm_position = getBonePosition(enemy, 30);
			l_hand_position = getBonePosition(enemy, 32);

			r_upper_arm_position = getBonePosition(enemy, 58);
			r_fore_arm_position = getBonePosition(enemy, 55);
			r_hand_position = getBonePosition(enemy, 57);

			l_thigh_position = getBonePosition(enemy, 63);
			l_knee_position = getBonePosition(enemy, 65);
			l_foot_position = getBonePosition(enemy, 69);

			r_thigh_position = getBonePosition(enemy, 77);
			r_knee_position = getBonePosition(enemy, 79);
			r_foot_position = getBonePosition(enemy, 83);
		}
		else {
			return;
		}

		renderBoneLine(head_position, neck_position, position, rotation, fov);
		renderBoneLine(neck_position, chest_position, position, rotation, fov);
		renderBoneLine(neck_position, l_upper_arm_position, position, rotation, fov);
		renderBoneLine(l_upper_arm_position, l_fore_arm_position, position, rotation, fov);
		renderBoneLine(l_fore_arm_position, l_hand_position, position, rotation, fov);
		renderBoneLine(neck_position, r_upper_arm_position, position, rotation, fov);
		renderBoneLine(r_upper_arm_position, r_fore_arm_position, position, rotation, fov);
		renderBoneLine(r_fore_arm_position, r_hand_position, position, rotation, fov);
		renderBoneLine(chest_position, stomach_position, position, rotation, fov);
		renderBoneLine(stomach_position, pelvis_position, position, rotation, fov);
		renderBoneLine(pelvis_position, l_thigh_position, position, rotation, fov);
		renderBoneLine(l_thigh_position, l_knee_position, position, rotation, fov);
		renderBoneLine(l_knee_position, l_foot_position, position, rotation, fov);
		renderBoneLine(pelvis_position, r_thigh_position, position, rotation, fov);
		renderBoneLine(r_thigh_position, r_knee_position, position, rotation, fov);
		renderBoneLine(r_knee_position, r_foot_position, position, rotation, fov);
	}

	static auto DrawRect(int x, int y, int w, int h, RGBA* color, int thickness) -> void
	{
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0, thickness);
	}

	inline auto DrawFilledRect2(float x, float y, float w, float h, ImColor color) -> void
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, NULL, NULL);
	}

	inline auto DrawFilledRect3(int x, int y, int w, int h, RGBA* color) -> void
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), 0, 0);
	}

	inline auto DrawFilledRect4(float x, float y, float w, float h, ImColor color) -> void
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, NULL, NULL);
	}

	inline auto DrawNormalBox(int x, int y, int w, int h, int borderPx, RGBA* color) -> void
	{
		DrawFilledRect4(x + borderPx, y, w, borderPx, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x + w - w + borderPx, y, w, borderPx, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x, y, borderPx, h, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x, y + h - h + borderPx * 2, borderPx, h, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x + borderPx, y + h + borderPx, w, borderPx, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x + w - w + borderPx, y + h + borderPx, w, borderPx, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x + w + borderPx, y, borderPx, h, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
		DrawFilledRect4(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	}

	inline auto DrawCornerBox(int x, int y, int w, int h, int borderPx, RGBA* color) -> void
	{
		DrawFilledRect3(x + borderPx, y, w / 3, borderPx, color); //top 
		DrawFilledRect3(x + w - w / 3 + borderPx, y, w / 3, borderPx, color); //top 
		DrawFilledRect3(x, y, borderPx, h / 3, color); //left 
		DrawFilledRect3(x, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color); //left 
		DrawFilledRect3(x + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
		DrawFilledRect3(x + w - w / 3 + borderPx, y + h + borderPx, w / 3, borderPx, color); //bottom 
		DrawFilledRect3(x + w + borderPx, y, borderPx, h / 3, color);//right 
		DrawFilledRect3(x + w + borderPx, y + h - h / 3 + borderPx * 2, borderPx, h / 3, color);//right 
	}

	inline auto RectFilled(float x, float y, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags) -> void
	{
		ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
	}

	inline auto DrawHealthBar(float x, float y, float w, float h, int phealth, bool Outlined) -> void
	{
		auto vList = ImGui::GetOverlayDrawList();

		int healthValue = max(0, min(phealth, 100));

		ImColor barColor = ImColor
		(
			min(510 * (100 - healthValue) / 100, 255), min(510 * healthValue / 100, 255),
			25,
			255
		);
		if (Outlined)
			vList->AddRect(ImVec2(x - 1, y - 1), ImVec2(x + w + 1, y + h + 1), ImColor(0.f, 0.f, 0.f), 0.0f, 0, 1.0f);

		RectFilled(x, y, x + w, y + (int)(((float)h / 100.0f) * (float)phealth), barColor, 0.0f, 0);
	}

	inline static auto DrawLine(int x1, int y1, int x2, int y2, ImColor color, int thickness) -> void
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(color), thickness);
	}

	inline auto DrawLine3(ImVec2 CirclePos, ImVec2 PlayerPos, float radius, RGBA* color, float width) -> void
	{
		ImVec2 Direction = PlayerPos - CirclePos;
		float Multiplier = radius / Direction.Length();

		CirclePos += Direction * Multiplier;
		ImGui::GetBackgroundDrawList()->AddLine(CirclePos, PlayerPos, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), width);
	}

	inline auto DrawLineNew(int x1, int y1, int x2, int y2, RGBA* color, int thickness) -> void
	{
		ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), thickness);
	}

	inline auto DrawBorder(float x, float y, float w, float h, float px, RGBA* BorderColor) -> void
	{
		DrawRect(x, (y + h - px), w, px, BorderColor, 1 / 2);
		DrawRect(x, y, px, h, BorderColor, 1 / 2);
		DrawRect(x, y, w, px, BorderColor, 1 / 2);
		DrawRect((x + w - px), y, px, h, BorderColor, 1 / 2);
	}

	inline auto DegreeToRadian(float degree) -> float
	{
		return degree * (M_PI / 180);
	}

	inline FVector DegreeToRadian(FVector degrees)
	{
		FVector radians;
		radians.X = degrees.X * (M_PI / 180);
		radians.Y = degrees.Y * (M_PI / 180);
		radians.Z = degrees.Z * (M_PI / 180);
		return radians;
	}

	inline FVector2D WorldRadar(FVector srcPos, FVector distPos, float yaw, float radarX, float radarY, float size)
	{
		auto cosYaw = cos(DegreeToRadian(yaw));
		auto sinYaw = sin(DegreeToRadian(yaw));

		auto deltaX = srcPos.X - distPos.X;
		auto deltaY = srcPos.Y - distPos.Y;

		auto locationX = (float)(deltaY * cosYaw - deltaX * sinYaw) / 45.f;
		auto locationY = (float)(deltaX * cosYaw + deltaY * sinYaw) / 45.f;

		if (locationX > (size - 2.f))
			locationX = (size - 2.f);
		else if (locationX < -(size - 2.f))
			locationX = -(size - 2.f);

		if (locationY > (size - 6.f))
			locationY = (size - 6.f);
		else if (locationY < -(size - 6.f))
			locationY = -(size - 6.f);

		return FVector2D((int)(-locationX + radarX), (int)(locationY + radarY));
	}

	static FVector pRadar;

	static int enemy_size = 3;

	inline auto DrawCircleFilled2(int x, int y, int radius, RGBA* color) -> void
	{
		ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	}

	inline auto DrawRadar(FVector EntityPos) -> void
	{
		auto radar_posX = pRadar.X + 135;
		auto radar_posY = pRadar.Y + 135;
		if (g_localplayerpawn == 0) return;
		uint64_t LocalRootComp = Read<uint64_t>(g_localplayerpawn + offsets::root_component);
		uint64_t rot = Read<uint64_t>(g_localplayercontroller + offsets::camera_rotation);
		FVector LocalPos = Read<FVector>(LocalRootComp + offsets::root_position);
		auto Radar2D = WorldRadar(LocalPos, EntityPos, rot, radar_posX, radar_posY, 135.f);
		DrawCircleFilled2(Radar2D.x, Radar2D.y, 4, &espcol);
	}
}

FINLINE auto GetCharacterName(int id) -> string {

	switch (id)
	{
	case 14222827: return "ASTRA"; // ok 14223910
		break;
	case 14230510: return "JETT"; // ok
		break;
	case 14211973: return "CHAMBER"; // ok
		break;
	case 14219418: return "KILLJOY"; // ok
		break;
	case 14209948: return "RAZE"; // ok
		break;
	case 14228884: return "REYNA"; // ok
		break;
	case 14228027: return "SAGE"; // ok
		break;
	case 14215643: return "SKYE"; // ok
		break;
	case 14225108: return "NEON"; // ok
		break;
	case 14220733: return "VIPER"; // ok
		break;
	case 14209373: return "BREACH"; // ok
		break;
	case 14224190: return "BRIMSTONE"; // ok
		break;
	case 14216852: return "CYPHER"; // ok
		break;
	case 14229675: return "OMEN"; // ok
		break;
	case 14221852: return "PHOENIX"; // ok
		break;
	case 14218064: return "SOVA"; // ok 
		break;
	case 14226956: return "YORU"; // ok
		break;
	case 14214549: return "KAY/O"; // ok
		break;
	case 14228837: return  "BOT"; // ok
		break;
	case 14207796: return  "FADE"; // ok
		break;
	default:
		return ("");
		break;
	}
}

FINLINE auto GetWeaponName(int id) -> string {

	switch (id)
	{
	case 14391538:  return "Knife";
		break;
	case 14228826:  return "Classic";
		break;
	case 14344465:  return "Shorty";
		break;
	case 14337836:  return "Sheriff";
		break;
	case 14318008:  return "Frenzy";
		break;
	case 14332155:  return "Ghost";
		break;
	case 14381675:  return "Stinger";
		break;
	case 14372761:  return "Spectre";
		break;
	case 14311523:  return "Bucky";
		break;
	case 14307093:  return "Judge";
		break;
	case 14291226:  return "Bulldog";
		break;
	case 14359498:  return "Guardian";
		break;
	case 14296107:  return "Phantom";
		break;
	case 14282114:  return "Vandal";
		break;
	case 14367503:  return "Marshal";
		break;
	case 14348192:  return "Operator";
		break;
	case 14277573:  return "Ares";
		break;
	case 14273931:  return "Odin";
		break;
	case 14246916: return "Spike";
		break;
	case 14246939: return "Defuser";
		break;
	case 14231466: return "Orb";
		break;
		//omen
	case 14229727: return "Dark Cover";
		break;
	case 14230055: return "Paranoia";
		break;
	case 14229906: return "Shrouded Step";
		break;
	case 14230204: return "Omen Ult";
		break;
		//PH
	case 14222191: return "Curveball";
		break;
	case 14222400: return "Blaze";
		break;
	case 14222142: return "Hot Hands";
		break;
		//raze
	case 14210204: return "Boom Bot";
		break;
	case 14210462: return "Blast Pack";
		break;
	case 14210020: return "Paint Shells";
		break;
	case 14210631: return "Raze Ult";
		break;
		//Rey
	case 14228942: return "Leer";
		break;
		//sage
	case 14228246: return "Barrier Orb";
		break;
	case 14228091: return "Slow Orb";
		break;
	case 14228481: return "Healing Orb";
		break;
	case 14228592: return "Sage Revive";
		break;
		//sky
	case 14215701: return "Regrowth";
		break;
	case 14216133: return "Trailblazer";
		break;
	case 14215825: return "Guiding Light";
		break;
	case 14216541: return "Skye Ult";
		break;
		//sova
	case 14218662: return "Owl Drone";
		break;
	case 14218154: return "Shock Bolt";
		break;
	case 14218787: return "Recon Bolt";
		break;
	case 14219051: return "Sova Ult";
		break;
		//viper
	case 14221364: return "Snake Bite";
		break;
	case 14220902: return "Poison Cloud";
		break;
	case 14221113: return "Toxic Screen";
		break;
	case 14221516: return "Viper Ult";
		break;
		//yuro
	case 14227009: return "Fakeout";
		break;
	case 14227646: return "Blindside";
		break;
	case 14227289: return "Gatecrash";
		break;
	case 14227750: return "Yuro Ult";
		break;
		//Astra
	case 14223982: return "Astra Form";
		break;
	case 14223161: return "Nebula";
		break;
	case 14223358: return "Gravity Well";
		break;
	case 14223036: return "Nova Pulse";
		break;
		//Breach
	case 14209448: return "Aftershock";
		break;
	case 14209703: return "Flashpoint";
		break;
	case 14209591: return "Fault Line";
		break;
	case 14209806: return "Breach Ult";
		break;
		//Brim
	case 14224718: return "Stim Beacon";
		break;
	case 14224457: return "Incendiary";
		break;
	case 14224248: return "Sky Smoke";
		break;
	case 14224597: return "Brim Ult";
		break;
		//Chamb
	case 14212087: return "Trademark";
		break;
	case 14212948: return "Headhunter";
		break;
	case 14212492: return "Redezvous";
		break;
	case 14213404: return "TOUR DE FORCE";
		break;
		//Cy
	case 14217018: return "Trapwire";
		break;
	case 14217913: return "Cyber Cage";
		break;
	case 14217398: return "Spycam";
		break;
	case 14217406: return "Spycam (Active)";
		break;
		//fade
	case 14207896: return "PROWLER";
		break;
	case 14208676: return "SEIZE";
		break;
	case 14208386: return "HAUNT";
		break;
	case 14209041: return "FADE ULT";
		break;
		//Jett
	case 14230583: return "Cloudburst";
		break;
	case 14230842: return "Updraft";
		break;
	case 14230880: return "Jett Ult";
		break;
		//ko
	case 14215013: return "Fragment";
		break;
	case 14214617: return "Flashdrive";
		break;
	case 14214765: return "Zeropoint";
		break;
		//KJ
	case 14219574: return "Nanoswarm";
		break;
	case 14220186: return "Alarmbot";
		break;
	case 14219779: return "Turret";
		break;
	case 14220454: return "KJ Ult";
		break;
		//neon
	case 14225174: return "Fast Lane";
		break;
	case 14225484: return "Relay Bolt";
		break;
	case 14225329: return "High Gear";
		break;
	case 14225817: return "Neon Ult";
		break;


	case 0: return "AFK";
		break;
	}
}

namespace ImGui {

	static auto Hotkey(int* k, const ImVec2& size_arg = ImVec2(125, 0)) -> void {

		static bool waitingforkey = false;
		if (waitingforkey == false) {
			if (ImGui::Button(KeyNames[*(int*)k], size_arg))
				waitingforkey = true;
		}
		else if (waitingforkey == true) {
			ImGui::Button("...", size_arg);

			for (auto& Key : KeyCodes)
			{
				if (GetAsyncKeyState(Key)) {
					*(int*)k = Key;
					waitingforkey = false;
				}
			}
		}
	}


	inline auto InitVGTheme() -> void
	{
		/*ImGuiStyle& Style = ImGui::GetStyle();
		auto Color = Style.Colors;

		Style.WindowMinSize = ImVec2(600, 400);
		Style.WindowBorderSize = 0;

		Style.ChildRounding = 0;
		Style.FrameRounding = 0;
		Style.ScrollbarRounding = 0;
		Style.GrabRounding = 0;
		Style.PopupRounding = 0;
		Style.WindowRounding = 0;

		Color[ImGuiCol_WindowBg] = ImColor(18, 18, 18, 255);

		Color[ImGuiCol_FrameBg] = ImColor(31, 31, 31, 255);
		Color[ImGuiCol_FrameBgActive] = ImColor(41, 41, 41, 255);
		Color[ImGuiCol_FrameBgHovered] = ImColor(41, 41, 41, 255);

		Color[ImGuiCol_Button] = ImColor(168, 50, 157, 150);
		Color[ImGuiCol_ButtonActive] = ImColor(168, 50, 157, 255);
		Color[ImGuiCol_ButtonHovered] = ImColor(168, 50, 157, 50);

		Color[ImGuiCol_Border] = ImColor(168, 50, 157, 255);
		Color[ImGuiCol_Separator] = ImColor(36, 36, 36, 255);

		Color[ImGuiCol_ResizeGrip] = ImColor(30, 30, 30, 255);
		Color[ImGuiCol_ResizeGripActive] = ImColor(30, 30, 30, 255);
		Color[ImGuiCol_ResizeGripHovered] = ImColor(30, 30, 30, 255);

		Color[ImGuiCol_ChildBg] = ImColor(26, 26, 26, 255);

		Color[ImGuiCol_ScrollbarBg] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);
		Color[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);

		Color[ImGuiCol_Header] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_HeaderActive] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_HeaderHovered] = ImColor(39, 39, 39, 255);
		Color[ImGuiCol_CheckMark] = ImColor(168, 50, 157, 255);

		Color[ImGuiCol_Text] = ImColor(255, 255, 255, 255);

		Color[ImGuiCol_SliderGrab] = ImColor(168, 50, 157, 255);
		Color[ImGuiCol_SliderGrabActive] = ImColor(168, 50, 157, 255);*/

		/*
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
		colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(8.00f, 8.00f);
		style.FramePadding = ImVec2(5.00f, 2.00f);
		style.CellPadding = ImVec2(6.00f, 6.00f);
		style.ItemSpacing = ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing = ImVec2(6.00f, 6.00f);
		style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style.IndentSpacing = 25;
		style.ScrollbarSize = 15;
		style.GrabMinSize = 10;
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 1;
		style.TabBorderSize = 1;
		style.WindowRounding = 7;
		style.ChildRounding = 4;
		style.FrameRounding = 3;
		style.PopupRounding = 4;
		style.ScrollbarRounding = 9;
		style.GrabRounding = 3;
		style.LogSliderDeadzone = 4;
		style.TabRounding = 4;
		*/

ImGuiStyle* style = &ImGui::GetStyle();

ImVec4* colors = ImGui::GetStyle().Colors;
colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
colors[ImGuiCol_Border] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
colors[ImGuiCol_FrameBgHovered] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_FrameBgActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_TitleBg] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_TitleBgActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_SliderGrabActive] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_Button] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_ButtonHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_ButtonActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_Header] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_HeaderHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_HeaderActive] = ImVec4(0.93f, 0.65f, 0.14f, 1.00f);
colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
colors[ImGuiCol_SeparatorHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_SeparatorActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_ResizeGripActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_Tab] = ImVec4(0.51f, 0.36f, 0.15f, 1.00f);
colors[ImGuiCol_TabHovered] = ImVec4(0.91f, 0.64f, 0.13f, 1.00f);
colors[ImGuiCol_TabActive] = ImVec4(0.78f, 0.55f, 0.21f, 1.00f);
colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}
}

namespace VALORANT_EXT {

	class dot
	{
	public:
		dot(Vector3 p, Vector3 v) {
			m_vel = v;
			m_pos = p;
		}

		void update();
		void draw();
		void draw2();

		Vector3 m_pos, m_vel;
	};
    inline std::vector<dot*> dots = { };

	inline auto dot::update() -> void {

		auto opacity = 240 / 255.0f;

		m_pos += m_vel * (opacity);
	}

	inline auto dot::draw() -> void {

		int opacity = 55.0f * (240 / 255.0f);

		ImGui::GetWindowDrawList()->AddRectFilled({ m_pos.x - 2, m_pos.y - 2 }, { m_pos.x + 2, m_pos.y + 2 }, ImColor(168, 50, 157, 150));

		auto t = std::find(dots.begin(), dots.end(), this);
		if (t == dots.end()) {
			return;
		}

		for (auto i = t; i != dots.end(); i++) {
			if ((*i) == this) continue;

			auto dist = (m_pos - (*i)->m_pos).Length2D();

			if (dist < 128) {
				int alpha = opacity * (dist / 128);
				ImGui::GetWindowDrawList()->AddLine({ m_pos.x - 1, m_pos.y - 2 }, { (*i)->m_pos.x - 2, (*i)->m_pos.y - 1 }, ImColor(168, 50, 157, 150));
			}
		}
	}

	inline auto dot::draw2() -> void {

		int opacity = 55.0f * (240 / 255.0f);

		ImGui::GetWindowDrawList()->AddRectFilled({ m_pos.x - 2, m_pos.y - 2 }, { m_pos.x + 2, m_pos.y + 2 }, ImColor(168, 50, 157, 150));

		auto t = std::find(dots.begin(), dots.end(), this);
		if (t == dots.end()) {
			return;
		}

		for (auto i = t; i != dots.end(); i++) {
			if ((*i) == this) continue;

			auto dist = (m_pos - (*i)->m_pos).Length2D();

			if (dist < 128) {
				int alpha = opacity * (dist / 128);
				ImGui::GetWindowDrawList()->AddLine({ m_pos.x - 1, m_pos.y - 2 }, { (*i)->m_pos.x - 2, (*i)->m_pos.y - 1 }, ImColor(168, 50, 157, 150));
			}
		}
	}

	inline auto dot_draw() -> void {

		struct screen_size {
			int x, y;
		}; screen_size sc;

		sc.x = Width, sc.y = Height;

		int s = rand() % 9;

		if (s == 0) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, -16, 0), Vector3((rand() % 7) - 3, rand() % 3 + 1, 0)));
		}
		else if (s == 1) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, (int)sc.y + 16, 0), Vector3((rand() % 7) - 3, -1 * (rand() % 3 + 1), 0)));
		}
		else if (s == 2) {
			dots.push_back(new dot(Vector3(-16, rand() % (int)sc.y, 0), Vector3(rand() % 3 + 1, (rand() % 7) - 3, 0)));
		}
		else if (s == 3) {
			dots.push_back(new dot(Vector3((int)sc.x + 16, rand() % (int)sc.y, 0), Vector3(-1 * (rand() % 3 + 1), (rand() % 7) - 3, 0)));
		}

		auto alph = 255.0f * (255 / 255.0f);
		auto a_int = (int)(alph);

		ImGui::GetWindowDrawList()->AddRectFilled({ 0, 0 }, { (float)sc.x, (float)sc.y }, ImColor(a_int, 0, 0, 0));

		for (auto i = dots.begin(); i < dots.end();) {
			if ((*i)->m_pos.y < -20 || (*i)->m_pos.y > sc.y + 20 || (*i)->m_pos.x < -20 || (*i)->m_pos.x > sc.x + 20) {
				delete (*i);
				i = dots.erase(i);
			}
			else {
				(*i)->update();
				i++;
			}
		}

		for (auto i = dots.begin(); i < dots.end(); i++) {
			(*i)->draw();
		}
	}



	inline auto dot_draw2() -> void {

		struct screen_size {
			int x, y;
		}; screen_size sc;

		sc.x = Width, sc.y = Height;

		int s = rand() % 9;

		if (s == 0) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, -16, 0), Vector3((rand() % 7) - 3, rand() % 3 + 1, 0)));
		}
		else if (s == 1) {
			dots.push_back(new dot(Vector3(rand() % (int)sc.x, (int)sc.y + 16, 0), Vector3((rand() % 7) - 3, -1 * (rand() % 3 + 1), 0)));
		}
		else if (s == 2) {
			dots.push_back(new dot(Vector3(-16, rand() % (int)sc.y, 0), Vector3(rand() % 3 + 1, (rand() % 7) - 3, 0)));
		}
		else if (s == 3) {
			dots.push_back(new dot(Vector3((int)sc.x + 16, rand() % (int)sc.y, 0), Vector3(-1 * (rand() % 3 + 1), (rand() % 7) - 3, 0)));
		}

		auto alph = 135.0f * (240 / 255.0f);
		auto a_int = (int)(alph);

		ImGui::GetWindowDrawList()->AddRectFilled({ 0, 0 }, { (float)sc.x, (float)sc.y }, ImColor(a_int, 0, 0, 0));

		for (auto i = dots.begin(); i < dots.end();) {
			if ((*i)->m_pos.y < -20 || (*i)->m_pos.y > sc.y + 20 || (*i)->m_pos.x < -20 || (*i)->m_pos.x > sc.x + 20) {
				delete (*i);
				i = dots.erase(i);
			}
			else {
				(*i)->update();
				i++;
			}
		}

		for (auto i = dots.begin(); i < dots.end(); i++) {
			(*i)->draw2();
		}
	}
}