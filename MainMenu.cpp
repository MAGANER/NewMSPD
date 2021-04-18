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

	iv_field   = new Fl_Input(120, 170, 500, 30, "IV:");
	iv_field->callback((Fl_Callback*)input_callback, (void*)InputType::Password);
	iv_field->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	read_button = new Fl_Button(140, 210, 60, 30, "read");
	read_button->callback(read_callback,(void*)window);

	generate_key_button = new Fl_Button(280, 210, 90, 30, "generate key");
	generate_key_button->callback(generate_callback);

	create_diary_button = new Fl_Button(480, 210, 90, 30,"create diary");
	create_diary_button->callback(create_diary_callback);

	window->end();
}
void MainMenu::inner::input_callback(Fl_Input* input, void* type)
{
	switch ((int)type)
	{
	case InputType::Diary:
		diary_path = string(input->value());
		break;
	case InputType::Password:
		iv = string(input->value());
		break;
	case InputType::Key:
		key = string(input->value());
		break;
	}
}
void MainMenu::inner::read_callback(Fl_Widget* button_ptr, void* window)
{
	bool empty_diary    = diary_path.empty();
	bool empty_iv		= iv.empty();
	bool empty_key		= key.empty();
	bool empty = empty_diary && empty_iv && empty_key;


	if (!empty)
	{
		namespace fs = std::filesystem;
		fs::path diary{ diary_path };
		bool does_diary_exist = fs::exists(diary);
		if (does_diary_exist)
		{
			Fl_Window* win = static_cast<Fl_Window*>(window);
			win->hide();

	
			DiaryManager::run(diary_path,key,iv);
		}
		else run_sub_window(strcat((char*)diary_path.c_str(), " doesn't exist!"),"error!", static_cast<Fl_Window*>(window));
	}
	else run_sub_window("enter all required information!","error!", static_cast<Fl_Window*>(window));
	
}
void MainMenu::inner::create_diary_callback(Fl_Widget* button_ptr)
{
	Fl_Window* diary_creation_window = new Fl_Window(600, 150, "diary creation");
	Fl_Input* input = new Fl_Input(120, 50, 450, 30, "enter diary path:");
	input->callback((Fl_Callback*)get_new_diary_path_callback, (void*)diary_creation_window);
	input->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	diary_creation_window->end();
	diary_creation_window->show();
}
void MainMenu::inner::get_new_diary_path_callback(Fl_Input* input, void* window)
{
	const char* path = input->value();

	std::ofstream diary_file(path);
	if (diary_file)
	{
		//just create empty file
		diary_file.close();
		static_cast<Fl_Window*>(window)->hide();
	}
}

void MainMenu::inner::generate_callback(Fl_Widget* button_ptr)
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
	string key  = convert_bytes(data.first);
	string iv   = convert_bytes(data.second);
	ofstream key_file(generated_key_path);

	if (key_file) 
	{
		key_file << "key:"+key
				 << endl
				 <<"IV:"+iv;
		key_file.close();
		static_cast<Fl_Window*>(window)->hide();
	}
}
