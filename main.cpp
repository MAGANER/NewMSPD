#include"MainMenu.h"

int main(int argc, char** argv) 
{
	Fl_Window* window = MainMenu::init_window();
	MainMenu::init_widgets(window);

	window->show(argc, argv);

	return Fl::run();
}