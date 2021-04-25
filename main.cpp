#include"MainMenu.h"
#include"rijndael.h"
#include"osrng.h"
#include"modes.h"
#include "filters.h"
using namespace CryptoPP;
using namespace std;
int main() 
{

	Fl_Window* window = MainMenu::inner::init_window();
	MainMenu::inner::init_widgets(window);

	window->show();

	return Fl::run();
}