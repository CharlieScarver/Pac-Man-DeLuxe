#include <iostream>
#include "SDL.h"
#include "SDL_main.h"

/*
On Windows SDL provides WinMain(), which parses the command lineand passes
the arguments to your main function.

SDL #defines main as SDL_main and calls it internally.
The application's SDL_main is required to have C linkage.
*/

#ifdef __cplusplus
extern "C"
#endif

int main(int argc, char* argv[])
{
	std::cout << "Hi" << std::endl;

	return 0;
}
