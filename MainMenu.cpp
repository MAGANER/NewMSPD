#include"MainMenu.h"

Fl_Window* MainMenu::init_window()
{
	Fl_Window* window = new Fl_Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	window->label("My personal secret diary");
	window->resize(200, 10, WINDOW_WIDTH, WINDOW_HEIGHT);

	return window;
}
void MainMenu::init_widgets(Fl_Window* window)
{
	//init main label
	Fl_Box* label = new Fl_Box(200, 0, 300, 50, "My personal secret diary");
	label->box(FL_FLAT_BOX);
	label->labelfont(FL_BOLD + FL_ITALIC);
	label->labelsize(24);

	const char* definition = "use power of AES encryption to keep your secrets!";
	Fl_Box* def = new Fl_Box(150, 40, 400, 20, definition);
	def->box(FL_FLAT_BOX);
	def->labelfont(FL_BOLD);
	def->labelsize(14);
	def->color(FL_RED);

	MainMenu::diary_path_field = new Fl_Input(120, 80, 500, 30,  "diary path:");
	MainMenu::key_field		   = new Fl_Input(120, 130, 500, 30, "AES key:");
	MainMenu::password_field   = new Fl_Input(120, 170, 500, 30, "password:");

	window->end();
}