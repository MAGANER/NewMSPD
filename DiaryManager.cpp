#include"DiaryManager.h"

void DiaryManager::run()
{
	window = new Fl_Window(340, 100, "My personal secret diary");
	

	using namespace DiaryManager::inner;
	Fl_Button* read_pages = new Fl_Button(50, 15, 100, 75, "read pages");
	read_pages->callback(read_pages_callback, (void*)&pages);
	read_pages->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	Fl_Button* add_page   = new Fl_Button(180, 15, 100, 75, "add page");
	add_page->callback(add_page_callback, (void*)&pages);
	add_page->when(FL_WHEN_RELEASE | FL_WHEN_ENTER_KEY);

	window->end();
	window->show();
}

void DiaryManager::inner::read_pages_callback(Fl_Widget* button_ptr, void* pages)
{
	auto diary = static_cast<vector<DiaryPage*>*>(pages);
	if (diary->empty())
	{
		run_sub_window("diary is empty!", "oops!",window);
	}
}
void DiaryManager::inner::add_page_callback(Fl_Widget* button_ptr, void* pages)
{
	Fl_Window* window      = new Fl_Window(720, 640, "write new page");
	Fl_Text_Buffer* buff   = new Fl_Text_Buffer();
	Fl_Text_Editor* editor = new Fl_Text_Editor(20, 20, 600, 600, "Write all you think");
	editor->buffer(buff);
	window->resizable(*editor);

	Fl_Button* save_button = new Fl_Button(640, 40, 70, 20, "save");

	window->end();
	window->show();
}