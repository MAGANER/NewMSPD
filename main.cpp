#include"MainMenu.h"
#include"rijndael.h"
#include"osrng.h"
#include"modes.h"
#include "filters.h"
#include"FL/Fl_RGB_Image.H"
#include"FL/Fl_PNG_Image.H"
using namespace CryptoPP;
using namespace std;
int main(int argc, char** argv)
{

	Fl_Window* window = MainMenu::inner::init_window();
	MainMenu::inner::init_widgets(window);


	Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");
	window->begin();
	window->icon(img);
	window->end();
	window->show();


	return Fl::run();
}