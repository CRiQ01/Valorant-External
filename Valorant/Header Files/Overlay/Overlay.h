#pragma once
#include "font.h"
#include "icons.h"
#include "imguipp.h"
#include "Globals.h"

namespace VALORANT_EXT {

	inline static void glfwErrorCallback(int error, const char* description)
	{
		fprintf(stderr, skCrypt("Glfw Error %d: %s\n"), error, description);
	}

	inline void setupWindow()
	{
		glfwSetErrorCallback(glfwErrorCallback);
		if (!glfwInit())
		{
			std::cout << skCrypt("glfwInit didnt work.\n");
			return;
		}

		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor) 
		{
			fprintf(stderr, skCrypt("Failed to get primary monitor!\n"));
			return;
		}

		g_width = glfwGetVideoMode(monitor)->width;
		g_height = glfwGetVideoMode(monitor)->height;

		glfwWindowHint(GLFW_FLOATING, true);
		glfwWindowHint(GLFW_RESIZABLE, false);
		glfwWindowHint(GLFW_MAXIMIZED, true);
		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);

		g_window = glfwCreateWindow(g_width, g_height, skCrypt("V.G.C"), NULL, NULL);
		if (g_window == NULL) 
		{
			std::cout << skCrypt("Could not create window.\n");
			return;
		}

		glfwSetWindowAttrib(g_window, GLFW_DECORATED, false);
		glfwSetWindowMonitor(g_window, NULL, 0, 0, g_width, g_height + 1, 0);
		glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, true);

		glfwMakeContextCurrent(g_window);
		glfwSwapInterval(1); // Enable vsync

		if (glewInit() != GLEW_OK)
		{
			fprintf(stderr, skCrypt("Failed to initialize OpenGL loader!\n"));
			return;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
		ImFontConfig icons_config;

		io.IniFilename = nullptr;
		io.LogFilename = nullptr;

		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.OversampleH = 3;
		icons_config.OversampleV = 3;

		ImFontConfig CustomFont;
		CustomFont.FontDataOwnedByAtlas = false;

		io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 16, &CustomFont);
		io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 30, &icons_config, icons_ranges);
		Large = io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(Custom), sizeof(Custom), 40, &CustomFont);
		Proggy = io.Fonts->AddFontDefault();
		io.Fonts->AddFontDefault();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(g_window, true);
		ImGui_ImplOpenGL3_Init(skCrypt("#version 130"));

		ImFont* font = io.Fonts->AddFontFromFileTTF(skCrypt("Font.ttf"), 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
		IM_ASSERT(font != NULL);
	}

	inline void cleanupWindow() 
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwDestroyWindow(g_window);
		glfwTerminate();
	}

	inline BOOL CALLBACK retreiveValorantWindow(HWND hwnd, LPARAM lparam) 
	{
		DWORD process_id;
		GetWindowThreadProcessId(hwnd, &process_id);
		if (process_id == g_pid) {
			valorantwindow = hwnd;
		}
		return TRUE;
	}

	inline void activateValorantWindow()
	{
		SetForegroundWindow(valorantwindow);
	}
}

inline void handleKeyPresses()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		overlay = !overlay;
		glfwSetWindowAttrib(g_window, GLFW_MOUSE_PASSTHROUGH, !overlay);
		if (overlay)
		{
			glClearColor(0.0, 0.0, 0.0, 0.8);
			HWND overlay_window = glfwGetWin32Window(g_window);
			SetForegroundWindow(overlay_window);
		}
		else
		{
			glClearColor(0.0, 0.0, 0.0, 0.0);
			activateValorantWindow();
		}
	}
}