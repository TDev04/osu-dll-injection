#include "system.h"

// argument count
// argument vector
int main(int argc, char* argv[])
{
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS); // get the highest possible elevation in usermode
	while (argc--)
	{
		char* v, * p = *argv++;
		if (*p == '-' && p[1])
		{
			p++;
			while (*p)
			{
				switch (tolower(*p))
				{
					// no idea to be honest if it is the proper way but it works
				case 'd':
					v = p[2] ? p + 2 : --argc ? *++argv : NULL; // hell.
					std::cout << "[i] Compatibility enabled info: osudx9.dll is being loaded" << std::endl;
					InjectLibrary(FindProcessIdByName(L"osu!.exe"), "path"); // path is supposed to be the dll
					return 1;

				case 'o':
					v = p[2] ? p + 2 : --argc ? *++argv : NULL;
					std::cout << "[i] Compatibility disabled info: osugl.dll is being loaded" << std::endl;
					//InjectLibrary(FindProcessIdByName(L"osu!.exe"), "path"); // gotta develop osugl.dll
					return 1;

				case 'h':
					v = p[2] ? p + 2 : --argc ? *++argv : NULL;
					std::cout << "[i] Argument -d will load the DirectX module for osu, -o, is the same, but instead uses OpenGL" << std::endl;
					return 1;
				}
				std::cout << "The argument " << *p << " is invalid, type -h in order to get a list of available commands" << std::endl;
				return 0;
			}
		}
	}

	std::cout << "No arguments were given. Refer for 'inject -h' for more info." << std::endl;
	return EXIT_SUCCESS;
}