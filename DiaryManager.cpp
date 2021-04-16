#include"DiaryManager.h"

void DiaryManager::run(const string& diary_path,
					   const string& key,
					   const string& iv)
{

	coding_data = make_tuple(diary_path, key, iv);

	window = new Fl_Window(340, 100, "My personal secret diary");
	

	using namespace DiaryManager::inner;
	Fl_Button* read_pages = new Fl_Button(50, 15, 100, 75, "read pages");
	read_pages->callback(read_pages_callback);
	read_pages->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	Fl_Button* add_page   = new Fl_Button(180, 15, 100, 75, "add page");


	add_page->callback(add_page_callback);
	add_page->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	window->end();
	window->show();
}

void DiaryManager::inner::read_pages_callback(Fl_Widget* button_ptr)
{
	if (pages.empty())
	{
		run_sub_window("diary is empty!", "oops!",window);
	}
}
void DiaryManager::inner::add_page_callback(Fl_Widget* button_ptr)
{
	Fl_Window* window      = new Fl_Window(720, 650, "write new page");
	buff   = new Fl_Text_Buffer();
	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600, "Write all you think");
	editor->buffer(buff);
	window->resizable(*editor);

	Fl_Button* save_button = new Fl_Button(640, 40, 70, 20, "save");
	save_button->callback(save_page_callback);
	save_button->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	Fl_Input* topic_input  = new Fl_Input(60, 620, 300, 20, "topic:");
	topic_input->callback((Fl_Callback*)get_topic_callback);
	topic_input->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	window->end();
	window->show();
}
void DiaryManager::inner::get_topic_callback(Fl_Input* input)
{
	entered_topic = input->value();
}
void DiaryManager::inner::save_page_callback(Fl_Widget* button_ptr)
{
	if (!entered_topic.empty())
	{
		//make page
		string text = buff->text();

		time_t now;
		time(&now);
		string current_time = ctime(&now);

		DiaryPage* page = new DiaryPage(text, entered_topic, current_time);

		pages.push_back(page);

		save_diary();
	}
}

void DiaryManager::inner::save_diary()
{
	ofstream file(get<0>(coding_data));
	if (file)
	{
		file.clear();

		for (auto& page : pages)
		{
			auto _key = Encryption::convert_to_bytes(string(get<1>(coding_data)));
			auto _iv = Encryption::convert_to_bytes(string(get<2>(coding_data)));
			auto key_iv = make_pair(_key, _iv);

			string body = Encryption::encrypt(key_iv, page->body);
			string topic = Encryption::encrypt(key_iv, "topic:" + page->topic);
			string date = Encryption::encrypt(key_iv, "date:" + page->date);
			string next_string = Encryption::encrypt(key_iv, "\n");
			string end = "<bor>";
		
			file << body << next_string << topic << next_string << date << end;
		}
		file.close();
	}
}