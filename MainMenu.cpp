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

	read_button = new Fl_Button(100, 210, 60, 30, "read");
	read_button->callback(read_callback,(void*)window);

	generate_key_button = new Fl_Button(240, 210, 90, 30, "generate key");
	generate_key_button->callback(generate_callback, (void*)window);

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
	bool empty_diary    = diary_path == nullptr || strlen(diary_path) == 0;
	bool empty_password = password   == nullptr || strlen(password)   == 0;
	bool empty_key		= key		 == nullptr || strlen(key)		  == 0;
	bool empty = empty_diary || empty_password || empty_key;


	if (!empty)
	{
		namespace fs = std::filesystem;
		fs::path diary{ diary_path };
		bool does_diary_exist = fs::exists(diary);
		if (does_diary_exist)
		{
			//try to read and decrypt

			Fl_Window* win = static_cast<Fl_Window*>(window);
			win->hide();

		}
		else run_sub_window(strcat(diary_path, " doesn't exist!"),"error!", static_cast<Fl_Window*>(window));
	}
	else run_sub_window("enter all required information!","error!", static_cast<Fl_Window*>(window));
	
}
void MainMenu::inner::generate_callback(Fl_Widget* button_ptr,void* window)
{
	Fl_Window* key_generation_window = new Fl_Window(600, 150, "random key generation");
	Fl_Input* input = new Fl_Input(100, 50, 450, 30, "enter key path:");
	input->callback((Fl_Callback*)get_key_path_callback, (void*)key_generation_window);
	input->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);
	
	key_generation_window->end();
	key_generation_window->show();
}
void MainMenu::inner::get_key_path_callback(Fl_Input* input, void* window)
{
	const char* generated_key_path = (char*)input->value();

	using namespace Encryption;
	key_iv data = get_random_key();
	string key = convert_bytes(data.first);

	ofstream key_file(generated_key_path);

	if (key_file) 
	{
		key_file << key;
		key_file.close();
		static_cast<Fl_Window*>(window)->hide();
	}
}
void MainMenu::inner::run_sub_window(const char* message,
									 const char* title,
									 Fl_Window* main_win)
{
	Fl_Window* error = new Fl_Window(300, 50, title);

	int main_win_x = main_win->x_root();
	int main_win_y = main_win->y_root();

	error->resize(main_win_x + (int)WINDOW_WIDTH / 2,
		main_win_y + (int)WINDOW_HEIGHT / 2,
		300,
		50);

	Fl_Box* def = new Fl_Box(0, 0, 300, 50,message);
	def->box(FL_FLAT_BOX);
	def->labelfont(FL_BOLD);
	def->labelsize(14);

	error->end();
	error->show();
}