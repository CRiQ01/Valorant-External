#pragma once
#include <Driver.h>
#include <Driver2.h>

using namespace KeyAuth;


std::string name = ""; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = ""; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = ""; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = ("https://keyauth.win/api/1.2/");
std::string sslPin = ("ssl pin key (optional)");

api KeyAuthApp(name, ownerid, secret, version, url, sslPin);


/*
using namespace KeyAuth;


std::string name = "Onlytaps"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = "I6u268bdhQ"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = "7c6d86606672da31a8e01e4b690117fd646dc38d11e72a9330376632bd9e0128"; // app secret, the blurred text on licenses tab and other tabs
std::string version = "1.0"; // leave alone unless you've changed version on website
std::string url = ("https://keyauth.win/api/1.2/");
std::string sslPin = ("ssl pin key (optional)");

api KeyAuthApp(name, ownerid, secret, version, url, sslPin);
*/


enum class ValorantAgent {
	Astra,
	Breach,
	Brimstone,
	Chamber,
	Cypher,
	Jett,
	Kay,
	Klljoy,
	Neon,
	Omen,
	Phoenix,
	Raze,
	Reyna,
	Sage,
	Skye,
	Sova,
	Viper,
	Yoru,
	Fade,
	Bot,
	MAX,
};

enum class ValorantGun {
	TacticalKnife,
	Classic,
	Shorty,
	Frenzy,
	Ghost,
	Sheriff,
	GoldenGun,
	Stinger,
	Spectre,
	Bucky,
	Judge,
	Bulldog,
	Guardian,
	Phantom,
	Vandal,
	Marshal,
	Operator,
	Ares,
	Odin,
	MAX,
};

static std::unordered_map<std::string, ValorantGun> guns = {
	{"Ability_Melee_Base_C", ValorantGun::TacticalKnife},
	{"BasePistol_C", ValorantGun::Classic},
	{"SawedOffShotgun_C", ValorantGun::Shorty},
	{"AutomaticPistol_C", ValorantGun::Frenzy},
	{"LugerPistol_C", ValorantGun::Ghost},
	{"RevolverPistol_C", ValorantGun::Sheriff},
	{"Vector_C", ValorantGun::Stinger},
	{"SubMachineGun_MP5_C", ValorantGun::Spectre},
	{"AutomaticShotgun_C", ValorantGun::Judge},
	{"PumpShotgun_C", ValorantGun::Bucky},
	{"AssaultRifle_Burst_C", ValorantGun::Bulldog},
	{"DMR_C", ValorantGun::Guardian},
	{"AssaultRifle_ACR_C", ValorantGun::Phantom},
	{"AssaultRifle_AK_C", ValorantGun::Vandal},
	{"LeverSniperRifle_C", ValorantGun::Marshal},
	{"BoltSniper_C", ValorantGun::Operator},
	{"LightMachineGun_C", ValorantGun::Ares},
	{"HeavyMachineGun_C", ValorantGun::Odin},
};

static std::unordered_map<std::string, ValorantAgent> agents = {
	{"Rift_PC_C", ValorantAgent::Astra},
	{"Rift_TargetingForm_PC_C", ValorantAgent::Astra},
	{"Breach_PC_C", ValorantAgent::Breach},
	{"Sarge_PC_C", ValorantAgent::Brimstone},
	{"Deadeye_PC_C", ValorantAgent::Chamber},
	{"Gumshoe_PC_C", ValorantAgent::Cypher},
	{"Wushu_PC_C", ValorantAgent::Jett},
	{"Grenadier_PC_C", ValorantAgent::Kay},
	{"Killjoy_PC_C", ValorantAgent::Klljoy},
	{"Sprinter_PC_C", ValorantAgent::Neon},
	{"Wraith_PC_C", ValorantAgent::Omen},
	{"Phoenix_PC_C", ValorantAgent::Phoenix},
	{"Clay_PC_C", ValorantAgent::Raze},
	{"Vampire_PC_C", ValorantAgent::Reyna},
	{"Thorne_PC_C", ValorantAgent::Sage},
	{"Guide_PC_C", ValorantAgent::Skye},
	{"Hunter_PC_C", ValorantAgent::Sova},
	{"Pandemic_PC_C", ValorantAgent::Viper},
	{"Stealth_PC_C", ValorantAgent::Yoru},
	{"BountyHunter_PC_C", ValorantAgent::Fade},
	{"TrainingBot_PC_C", ValorantAgent::Bot},
};

auto load_driver() -> void
{
	std::cout << skCrypt("Please wait...");

	auto DrvPath = "C:\\Windows\\System32\\drivers\\TECH.sys";
	Utilities::CreateFileFromMemory(DrvPath, reinterpret_cast<const char*>(Driver), sizeof(Driver));

	system(skCrypt("cd C:\\Windows\\System32\\drivers >nul 2>&1"));
	system(skCrypt("sc create TECH binpath= C:\\Windows\\System32\\drivers\\TECH.sys type=Kernel >nul 2>&1"));

	system(skCrypt("date 27/11/2017 >nul 2>&1"));
	system(skCrypt("sc start TECH >nul 2>&1"));

	system(skCrypt("w32tm /query /peers >nul 2>&1"));
	system(skCrypt("net stop w32time >nul 2>&1"));
	system(skCrypt("net start w32time >nul 2>&1"));
	system(skCrypt("w32tm /resync /nowait >nul 2>&1"));
}


auto load_driver3() -> void
{
	std::cout << skCrypt("Please wait...");

	auto DrvPath1 = "C:\\Windows\\System32\\drivers\\Nal.sys";
	Utilities::CreateFileFromMemory(DrvPath1, reinterpret_cast<const char*>(Driver2), sizeof(Driver2));

	system(skCrypt("cd C:\\Windows\\System32\\drivers >nul 2>&1"));
	system(skCrypt("sc create Nal binpath= C:\\Windows\\System32\\drivers\\Nal.sys type=Kernel >nul 2>&1"));

	system(skCrypt("date 27/11/2017 >nul 2>&1"));
	system(skCrypt("sc start Nal >nul 2>&1"));

	system(skCrypt("w32tm /query /peers >nul 2>&1"));
	system(skCrypt("net stop w32time >nul 2>&1"));
	system(skCrypt("net start w32time >nul 2>&1"));
	system(skCrypt("w32tm /resync /nowait >nul 2>&1"));
}

auto load_driver2() -> void
{
	std::cout << skCrypt("Please wait...");

	auto DrvPath = "C:\\Windows\\System32\\drivers\\TECH.sys";
	Utilities::CreateFileFromMemory(DrvPath, reinterpret_cast<const char*>(Driver), sizeof(Driver));

	system(skCrypt("cd C:\\Windows\\System32\\drivers >nul 2>&1"));
	system(skCrypt("sc create TECH binpath= C:\\Windows\\System32\\drivers\\TECH.sys type=Kernel >nul 2>&1"));

	system(skCrypt("date 11/27/2017 >nul 2>&1"));
	system(skCrypt("sc start TECH >nul 2>&1"));

	system(skCrypt("w32tm /query /peers >nul 2>&1"));
	system(skCrypt("net stop w32time >nul 2>&1"));
	system(skCrypt("net start w32time >nul 2>&1"));
	system(skCrypt("w32tm /resync /nowait >nul 2>&1"));
}

std::string tm_to_readable_time(tm ctx)
{
	char buffer[80];

	strftime(buffer, sizeof(buffer), "%m/%d/%y", &ctx);

	return std::string(buffer);
}

static std::time_t string_to_timet(std::string timestamp)
{
	auto cv = strtol(timestamp.c_str(), NULL, 10); // long

	return (time_t)cv;
}

static std::tm timet_to_tm(time_t timestamp)
{
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}

HWND init;
HWND window;
auto rndmTitle() -> void {

	constexpr int length = 15;
	const auto characters = TEXT("2s119sf3f59gga60473wxyzABCfDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
	TCHAR title[length + 1]{};

	for (int j = 0; j != length; j++)
	{
		title[j] += characters[rand() % 45 + 1];
	}

	SetConsoleTitle(title);
}

auto SetConsoleSize() -> void {

	HANDLE hOut;
	SMALL_RECT DisplayArea = { 0, 0, 0, 0 };
	int x = 80;
	int y = 20;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DisplayArea.Right = x;
	DisplayArea.Bottom = y;
	SetConsoleWindowInfo(hOut, TRUE, &DisplayArea);
}

int main() {

	rndmTitle();
	SetConsoleSize();

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);

	COORD new_screen_buffer_size;

	new_screen_buffer_size.X = screenBufferInfo.srWindow.Right -
		screenBufferInfo.srWindow.Left + 1;
	new_screen_buffer_size.Y = screenBufferInfo.srWindow.Bottom -
		screenBufferInfo.srWindow.Top + 1;

	SetConsoleScreenBufferSize(hConsole, new_screen_buffer_size);




	std::cout << skCrypt("\n\n Connecting..");
	KeyAuthApp.init();
	Beep(500, 500);
	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	Sleep(1000);
	system("cls");

	std::wcout << R"(                             
GOOD PASTE                                                  
            )" << '\n';


	std::cout << skCrypt("\n\n [1] Login\n [2] Register\n\n\n >> ");


	int option;
	std::string username;
	std::string password;
	std::string key;

	std::cin >> option;
	switch (option)
	{
	case 1:
		system("cls");

		std::wcout << R"(   
GOOD PASTE                                                                                                              
            )" << '\n';

		std::cout << skCrypt("\n\n Username: ");
		std::cin >> username;
		std::cout << skCrypt("\n Password: ");
		std::cin >> password;
		KeyAuthApp.login(username, password);
		break;
	case 2:
		system("cls");

		std::wcout << R"(
GOOD PASTE                                                                      
            )" << '\n';

		std::cout << skCrypt("\n\n Username: ");
		std::cin >> username;
		std::cout << skCrypt("\n Password: ");
		std::cin >> password;
		std::cout << skCrypt("\n License: ");
		std::cin >> key;
		KeyAuthApp.regstr(username, password, key);
		break;
	default:
		std::cout << skCrypt("\n\n Status: Failure: Invalid Selection");
		Sleep(3000);
		exit(0);
	}

	if (!KeyAuthApp.data.success)
	{
		std::cout << skCrypt("\n Status: ") << KeyAuthApp.data.message;
		Sleep(1500);
		exit(0);
	}

	KeyAuthApp.check();

	rndmTitle();
	system(skCrypt("cls"));

	std::wcout << R"(
GOOD PASTE                                                                    
        )" << '\n';

	std::cout << skCrypt("Select what your [Timezone/Date] Format is below.\n\n");

	std::cout << skCrypt("[1]: DD/MM/YYYY\n");
	std::cout << skCrypt("[2]: MM/DD/YYYY\n\n");

	std::cout << skCrypt(">> ");

	int selection = 0;
	cin >> selection;
	if (cin.fail())
	{
		cin.clear();
		std::cout << skCrypt("\n\n[!]: Invalid Selection Input!");
		Sleep(2500);
		return EXIT_SUCCESS;
	}

	switch (selection)
	{
	case 1:
		std::cout << skCrypt("\n\nPlease wait...");

		if (GlobalFindAtomA(skCrypt("UKDriverAlreadyLoadedx")) == 0)
		{
			//system(skCrypt("taskkill /F /IM RiotClientServices.exe >nul 2>&1"));
			//system(skCrypt("taskkill /F /IM VALORANT-Win64-Shipping.exe >nul 2>&1"));
			system(skCrypt("sc stop TECH >nul 2>&1"));
			system(skCrypt("sc delete TECH >nul 2>&1"));
			system(skCrypt("cls"));
			load_driver();

			system(skCrypt("cls"));
			GlobalAddAtomA(skCrypt("UKDriverAlreadyLoadedx"));
		}
		goto Start;
		break;
	case 2:
		std::cout << skCrypt("\n\nPlease wait...");

		if (GlobalFindAtomA(skCrypt("USADriverAlreadyLoadedx")) == 0)
		{
			//system(skCrypt("taskkill /F /IM RiotClientServices.exe >nul 2>&1"));
			//system(skCrypt("taskkill /F /IM VALORANT-Win64-Shipping.exe >nul 2>&1"));
			system(skCrypt("sc stop TECH >nul 2>&1"));
			system(skCrypt("sc delete TECH >nul 2>&1"));
			system(skCrypt("cls"));
			load_driver2();

			system(skCrypt("cls"));
			GlobalAddAtomA(skCrypt("USADriverAlreadyLoadedx"));
		}
		goto Start;
		break;
	default:
		std::cout << skCrypt("[!]: Invalid Selection. Please Try Again!");
		break;

	Start:
		rndmTitle();
		system(skCrypt("cls"));

		//load_driver();
		//load_driver3();

		system("cls");

		while (window == NULL)
		{

			std::wcout << R"(
GOOD PASTE                                                                                                                                    
            )" << '\n';

			cout << "\r[BV]: Waiting for [VALORANT-Win64-Shipping.exe] process";
			Sleep(500);
			for (int i = 0; i < 3; i++) {
				cout << skCrypt(". ") << flush;
				Sleep(500);
			}
			cout << skCrypt("\r                              ");
			system(skCrypt("cls"));

			window = FindWindowA(0, "Valorant  ");
		}

		system(skCrypt("cls"));

		while (!mem->Init(skCrypt("VALORANT-Win64-Shipping.exe").decrypt()))
			Sleep(1500);

		VALORANT_EXT::g_pid = mem->GetPIDByProcessName(skCrypt("VALORANT-Win64-Shipping.exe").decrypt());
		if (!VALORANT_EXT::g_pid) {
			std::cout << skCrypt("Could not find val process id.\n");
			system(skCrypt("pause"));
			return 1;
		}

		EnumWindows(VALORANT_EXT::retreiveValorantWindow, NULL);
		if (!VALORANT_EXT::valorantwindow) {
			std::cout << skCrypt("Could not find val window.\n");
			system(skCrypt("pause"));
			return 1;
		}

		VALORANT_EXT::g_baseaddress = mem->ModuleBase;
		if (!VALORANT_EXT::g_baseaddress) {
			std::cout << skCrypt("Could not get base address.\n");
			system(skCrypt("pause"));
			return 1;
		}

		VALORANT_EXT::setupWindow();
		if (!VALORANT_EXT::g_window) {
			std::cout << skCrypt("Could not setup window.\n");
			system(skCrypt("pause"));
			return 1;
		}

		rndmTitle();

		std::wcout << R"(
GOOD PASTE                                             
        )" << '\n';

		//std::cout << skCrypt("[ INFORMATION ]\n");
		//std::cout << skCrypt("Engine Expiration Date: ") << tm_to_readable_time(timet_to_tm(string_to_timet(KeyAuthApp.data.expiry)));
		//std::cout << skCrypt("\nEngine Version: V-1.5.5T");

		std::cout << skCrypt("\n\n[ CLIENT-INFO ]\n");

		std::cout << skCrypt("\n\n");

		std::cout << "[VALORANT] BASE : 0x" << std::hex << VALORANT_EXT::g_baseaddress << std::endl;
		std::cout << "[VALORANT] PID  : 0x" << std::hex << VALORANT_EXT::g_pid << std::endl;

		std::cout << skCrypt("\n\n");


		std::cout << skCrypt("[BV]: GOOD PASTE has connected!");

		_beginthread((_beginthread_proc_type)retreiveData, 0, 0);
		_beginthread((_beginthread_proc_type)Aimbot, 0, 0);

		clock_t last_update = 0;
		while (!glfwWindowShouldClose(VALORANT_EXT::g_window)) {

			handleKeyPresses();
			runRenderTick();
			if (last_update + 1000 < clock()) {
				mem->SetGuardedRegion();
				last_update = clock();
			}

			auto hwnd = FindWindowA(0, skCrypt("V.G.C").decrypt());

			static bool test = false;

			if (!test) {

				ITaskbarList* pTaskList = NULL;
				HRESULT initRet = CoInitialize(NULL);
				HRESULT createRet = CoCreateInstance(CLSID_TaskbarList,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_ITaskbarList,
					(LPVOID*)&pTaskList);

				if (createRet == S_OK) {
					pTaskList->DeleteTab(hwnd);
					pTaskList->Release();
				}
				CoUninitialize();
				test = true;
			}
		}

		VALORANT_EXT::cleanupWindow();
	}
}