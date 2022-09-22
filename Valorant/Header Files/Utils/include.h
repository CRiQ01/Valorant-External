#pragma once
#define GLFW_EXPOSE_NATIVE_WIN32
#pragma warning(disable : 4996)

/* Windows SDK */
#include <filesystem>
#include <ShObjIdl.h>
#include <Wininet.h>
#include <algorithm>
#include <Windows.h>
#include <intrin.h>
#include <fstream>
#include <tchar.h>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <vector>
#include <thread>
#include <random>
#include <string>

#include <wincrypt.h>
#define IMGUI_DEFINE_MATH_OPERATORS

/* Overlay */
#include "GL/glew.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

/* Kernel (RPM & WPM) */
#include "MemoryManagement.h"

/* Misc */
#include "Defs.h"
#include "Offsets.h"
#include "Vector3.h"

/* SDK */
#include "Overlay.h"
#include "Globals.h"
#include "cheat.h"

/* String-Encrypt */
#include "Xor.h"

/* Other */
#include "auth.hpp"