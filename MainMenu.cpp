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

	using namespace MainMenu;
	using namespace MainMenu::inner;
	diary_path_field = new Fl_Input(120, 80, 500, 30,  "diary path:");
	diary_path_field->callback((Fl_Callback*)input_callback,(void*)InputType::Diary);
	diary_path_field->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	key_field		 = new Fl_Input(120, 130, 500, 30, "AES key:");
	key_field->callback((Fl_Callback*)input_callback, (void*)InputType::Key);
	key_field->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	password_field   = new Fl_Input(120, 170, 500, 30, "password:");
	password_field->callback((Fl_Callback*)input_callback, (void*)InputType::Password);
	password_field->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	read_button = new Fl_Button(320, 210, 60, 30, "read");
	read_button->callback(read_callback,(void*)window);

	window->end();
}
void MainMenu::inner::input_callback(Fl_Input* input, void* type)
{
	switch ((int)type)
	{
	case InputType::Diary:
		diary_path = (char*)input->value();
		break;
	case InputType::Password:
		password = (char*)input->value();
		break;
	case InputType::Key:
		key = (char*)input->value();
		break;
	}
}
void MainMenu::inner::read_callback(Fl_Widget* button_ptr, void* window)
{
	read = true;
	
	bool empty_diary    = diary_path == nullptr || strlen(diary_path) == 0;
	bool empty_password = password   == nullptr || strlen(password)   == 0;
	bool empty_key		= key		 == nullptr || strlen(key)		  == 0;
	bool empty = empty_diary || empty_password || empty_key;

	if (!empty)
	{

	}
	else
	{
		Fl_Window* error = new Fl_Window(300, 50, "error!");

		int main_win_x = static_cast<Fl_Window*>(window)->x_root();
		int main_win_y = static_cast<Fl_Window*>(window)->y_root();

		error->resize(main_win_x + (int)WINDOW_WIDTH / 2,
					  main_win_y + (int)WINDOW_HEIGHT / 2,
					  300,
					  50);

		Fl_Box* def = new Fl_Box(0, 0, 300, 50, "enter all information!");
		def->box(FL_FLAT_BOX);
		def->labelfont(FL_BOLD);
		def->labelsize(14);

		error->end();
		error->show();
	}
	
}