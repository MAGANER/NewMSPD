#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"
#include "FL/Fl.H"
#include"FL/Fl_Button.H"
#include"FL/Fl_Window.H"
#include"FL/Fl_Text_Editor.H"
#include"FL/Fl_Text_Buffer.H"
#include"FL/Fl_Input.H"
#include"FL/Fl_Browser.h"
#include"Diary.h"
#include"Encryption.h"
#include<vector>
#include<fstream>
#include<string>
#include<ctime>
#include<iostream>
#include<tuple>

namespace DiaryManager
{
	extern void run(const string& diary_path,
					const string& key,
					const string& iv);

	static Fl_Window* window = nullptr;
	static vector<DiaryPage*> pages;

	static Fl_Text_Buffer* buff = nullptr;
	static string entered_topic;
	
	static tuple<string,string,string> coding_data;
	
	namespace inner
	{
		static void read_diary(const string& path);
		static vector<string> split_text(const string& text, const string& delimiter);

		static void save_diary();

		static void read_pages_callback(Fl_Widget* button_ptr);
		static void add_page_callback(Fl_Widget* button_ptr);
		static void save_page_callback(Fl_Widget* button_ptr);
		static void get_topic_callback(Fl_Input* input);
		static void show_page_callback(Fl_Widget* browser_ptr);
	};
};