#include"DiaryManager.h"

void DiaryManager::run(const string& diary_path,
					   const string& key,
					   const string& iv)
{

	coding_data = make_tuple(diary_path, key, iv);
	inner::read_diary(get<0>(coding_data));

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
	else
	{
		Fl_Window* window = new Fl_Window(720, 650, "pages");
		Fl_Browser* browser = new Fl_Browser(20, 20, 600, 600);
		browser->type(FL_MULTI_BROWSER);
		browser->callback(show_page_callback);
		window->resizable(*browser);
		for (auto& p : pages)
		{
			string label = p->topic + ":" + p->date;
			browser->add(label.c_str());
		}

		window->end();
		window->show();
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
			auto _key = Encryption::convert_to_bytes(get<1>(coding_data));
			auto _iv = Encryption::convert_to_bytes(get<2>(coding_data));

			auto key_iv = make_pair(_key, _iv);

			string text = page->body + "<del>" + page->topic+"<del>" + page->date;
			string cipher = Encryption::encrypt(key_iv, text);
			string end = "<bor>";

			file << cipher << end;
		}
		file.close();
	}
}
void DiaryManager::inner::read_diary(const string& path)
{
	ifstream file(path);
	string cipher;
	file >> cipher;
	vector<string> encrypted_pages = split_text(cipher,"<bor>");
	
	for (auto& p : encrypted_pages)
	{
		auto key = Encryption::convert_to_bytes(get<1>(coding_data));
		auto iv = Encryption::convert_to_bytes(get<2>(coding_data));

		auto key_iv = make_pair(key, iv);
		string decrypted_text = Encryption::decrypt(key_iv, p);
		vector<string> page_data = split_text(decrypted_text, "<del>");
		if (page_data.size() == 3)
		{
			DiaryPage* page = new DiaryPage(page_data[0], page_data[1], page_data[2]);
			pages.push_back(page);
		}
	}
}
vector<string> DiaryManager::inner::split_text(const string& text,const string& delimiter)
{
	string s = text;

	vector<string> pages;

	size_t pos = 0;
	string page;
	while ((pos = s.find(delimiter)) != std::string::npos)
	{
		page = s.substr(0, pos);
		pages.push_back(page);
		s.erase(0, pos + delimiter.length());
	}
	pages.push_back(s);


	return pages;
}
void DiaryManager::inner::show_page_callback(Fl_Widget* browser_ptr)
{
	Fl_Browser* browser = (Fl_Browser*)browser_ptr;
	
	int choosen = browser->value()-1;
	DiaryPage* page = pages[choosen];

	string label = page->topic + ":" + page->date;
	Fl_Window* window = new Fl_Window(720, 650, label.c_str());


	Fl_Text_Buffer* buff = new Fl_Text_Buffer();
	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600);
	buff->text(page->body.c_str());
	editor->buffer(buff);
	editor->clear_visible_focus();
	window->resizable(editor);

	window->end();
	window->show();

}