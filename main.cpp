#include"MainMenu.h"

int main(int argc, char** argv) 
{
	Fl_Window* window = MainMenu::inner::init_window();
	MainMenu::inner::init_widgets(window);

	window->show(argc, argv);

	return Fl::run();
}