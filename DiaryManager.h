#pragma once
#include"Common.h"
#include "FL/Fl.H"
#include"FL/Fl_Button.H"
#include"FL/Fl_Window.H"
#include"FL/Fl_Text_Editor.H"
#include"FL/Fl_Text_Buffer.H"
#include"Diary.h"
#include<vector>
#include<fstream>

namespace DiaryManager
{
	void run();

	static Fl_Window* window = nullptr;
	static vector<DiaryPage*> pages;
	namespace inner
	{
		static void read_diary(const string& path);
		static void read_pages_callback(Fl_Widget* button_ptr,void* pages);
		static void add_page_callback(Fl_Widget* button_ptr,void* pages);
	};
};