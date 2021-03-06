#include"DiaryManager.h"
using namespace DiaryManager::inner_data;

void DiaryManager::run(const string& diary_path,
					   const string& key,
					   const string& iv)
{
	//first obtain data from main menu and load data from encrypted diary
	coding_data = make_tuple(diary_path, key, iv);
	inner::read_diary(get<0>(coding_data));


	//icon, load and create it
	Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");

	//creates the main window, providing two buttons to 
	//open new page editor and existing pages wizard
	window = new Fl_Window(340, 100, "My personal secret diary");
	

	using namespace DiaryManager::inner;
	//when you click this button page wizard is open
	Fl_Button* read_pages = new Fl_Button(50, 15, 100, 75, "read pages");
	read_pages->callback(read_pages_callback);
	read_pages->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);


	//when you click this button editor is open
	Fl_Button* add_page   = new Fl_Button(180, 15, 100, 75, "add page");
	add_page->callback(add_page_callback);
	add_page->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	window->end();

	window->begin();
	window->icon(img);
	window->end();
	window->show();
}

void DiaryManager::inner::read_pages_callback(Fl_Widget* button_ptr)
{
	//if diary is empty, then create error window
	if (pages.empty())
	{
		run_sub_window("diary is empty!", "oops!",window);
	}
	else
	{
		//create browser window
		Fl_Window* window = new Fl_Window(720, 650, "pages");
		Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");

		//init browser with data from loaded diary
		//when you click list element
		//window with page is open
		Fl_Browser* browser = new Fl_Browser(20, 20, 600, 600);
		browser->type(FL_MULTI_BROWSER);
		browser->callback(show_page_callback);
		window->resizable(*browser);
		for (auto& p : pages)
		{
			string label = p->topic + ":" + p->date;
			browser->add(label.c_str());
		}

		//provide ability to read whole diary not separated
		Fl_Button* read_all_pages = new Fl_Button(640, 20, 60, 20, "read all");
		read_all_pages->callback(read_whole_diary_callback);
		read_all_pages->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

		window->begin();
		window->icon(img);
		window->end();
		window->show();
	}
}
void DiaryManager::inner::read_whole_diary_callback(Fl_Widget* button_ptr)
{
	Fl_Window* window = new Fl_Window(720, 650, "diary");
	Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");

	//create editor and pass to it page text
	Fl_Text_Buffer* buff = new Fl_Text_Buffer();

	string text;
	for (auto& p : pages)
	{
		text += p->body + "\n" + "topic:" + p->topic + "\n" + "date:" + p->date + "\n----------" + "\n";
	}
	buff->text(text.c_str());

	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600);

	editor->buffer(buff);
	editor->clear_visible_focus();//editor ignores keyboard
	window->resizable(editor);

	window->begin();
	window->icon(img);
	window->end();
	window->show();
}
void DiaryManager::inner::add_page_callback(Fl_Widget* button_ptr)
{
	//create editor window
	Fl_Window* window  = new Fl_Window(720, 650, "write new page");
	Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");

	//init editor widget and append to it special  buffer
	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600, "Write all you think");
	new_page_editor_buff = new Fl_Text_Buffer();
	editor->buffer(new_page_editor_buff);
	window->resizable(*editor);

	//encrypt and write down diary if clicked
	Fl_Button* save_button = new Fl_Button(640, 40, 70, 20, "save");
	save_button->callback(save_page_callback,(void*) window);
	save_button->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	//obtain page topic
	Fl_Input* topic_input  = new Fl_Input(60, 620, 300, 20, "topic:");
	topic_input->callback((Fl_Callback*)get_topic_callback);
	topic_input->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	window->end();
	window->icon(img);
	window->show();
}
void DiaryManager::inner::get_topic_callback(Fl_Input* input)
{
	entered_topic = input->value();
}
void DiaryManager::inner::save_page_callback(Fl_Widget* button_ptr, void* window)
{
	if (!entered_topic.empty())
	{
		//make page
		string text = new_page_editor_buff->text();
		//get current time
		time_t now;
		time(&now);
		string current_time = ctime(&now);

		//add new page to the end of already created
		DiaryPage* page = new DiaryPage(text, entered_topic, current_time);
		pages.push_back(page);

		save_diary();//write it down


		//close window
		static_cast<Fl_Window*>(window)->hide();
	}
}
void DiaryManager::inner::save_diary()
{
	//zero element is diary path
	ofstream file(get<0>(coding_data),ios::binary | ios::app | ios::out);

	if (file)
	{
		//it will be updated
		file.clear();

		int counter = 0;
		for (auto& page : pages)
		{
			//get special CryptoPP objects
			auto _key = Encryption::convert_to_bytes(get<1>(coding_data));
			auto _iv = Encryption::convert_to_bytes(get<2>(coding_data));

			auto key_iv = make_pair(_key, _iv);

			string text = page->body + "<del>" + page->topic+"<del>" + page->date;
			auto cipher = Encryption::encrypt(key_iv, text);


			string end = "<bor>";
			counter++;
			//file <<'('<<cipher.first<<')' << cipher.second << end;
			for (auto& ch : cipher)file << ch;
			file << end;
		}
		file.close();
	}
}

void DiaryManager::inner::read_diary(const string& path)
{
	//read file and split with special "tag"
	
	string cipher;
	if (!fs::is_empty(fs::path(path)))
	{
		ifstream file(path,ios::binary);

		std::stringstream strStream;
		strStream << file.rdbuf();
		cipher = strStream.str();
		file.close();

		vector<string> encrypted_pages = split_text(cipher,"<bor>");

		//there is empty line, maybe it's always the last one
		for (int i = 0; i < encrypted_pages.size(); ++i)
		{
			if (encrypted_pages[i].empty())
				encrypted_pages.erase(encrypted_pages.begin() + i);
		}
		for (auto& p : encrypted_pages)
		{
			//get special CryptoPP objects
			auto _key = Encryption::convert_to_bytes(get<1>(coding_data));
			auto _iv = Encryption::convert_to_bytes(get<2>(coding_data));

			//decrypt and split it to body, topic, date
			auto key_iv = make_pair(_key, _iv);

			string decrypted_text = Encryption::decrypt(key_iv, p);
			vector<string> page_data = split_text(decrypted_text, "<del>");
		
			//if it's all right, add it to all pages
			if (page_data.size() == 3)
			{
				if (!already_saved(page_data[0]))
				{
					DiaryPage* page = new DiaryPage(page_data[0], page_data[1], page_data[2]);
					pages.push_back(page);
				}
			}
		}
	}
}
vector<string> DiaryManager::inner::split_text(const string& text,const string& delimiter)
{
	//split string with passed delimiter

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
	//get browser widget
	Fl_Browser* browser = (Fl_Browser*)browser_ptr;
	
	//get choosen page it counts since 1, so get x-
	int choosen = browser->value()-1;
	DiaryPage* page = pages[choosen];


	//icon, load and create it
	Fl_RGB_Image* img = new Fl_PNG_Image("mspd.png");

	//create window
	string label = page->topic + ":" + page->date;
	Fl_Window* window = new Fl_Window(720, 650, label.c_str());


	//create editor and pass to its page text
	Fl_Text_Buffer* buff = new Fl_Text_Buffer();
	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600);
	buff->text(page->body.c_str());
	editor->buffer(buff);
	editor->clear_visible_focus();//editor ignores keyboard
	window->resizable(editor);

	window->end();

	window->begin();
	window->icon(img);
	window->end();
	window->show();

}
bool DiaryManager::inner::already_saved(const string& body)
{
	for (auto& page : pages)
	{
		if (page->body == body)return true;
	}
	return false;
}