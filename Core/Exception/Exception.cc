#include "Exception.h"

void exception_hinding()
{
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", "A fatal error occurred, please refer to the log", nullptr);
}
