/*
	DiaryManager provides an ability to
	read, edit, add new pages.
*/

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
#include<filesystem>
#include"resource.h"
#include"FL/Fl_RGB_Image.H"
#include"FL/Fl_PNG_Image.H"

namespace DiaryManager
{
	extern void run(const string& diary_path,
					const string& key,
					const string& iv);


	//data bound to this module
	namespace inner_data
	{
		static Fl_Window* window = nullptr;
		static vector<DiaryPage*> pages;


		static Fl_Text_Buffer* new_page_editor_buff = nullptr;
		static string entered_topic;

		static tuple<string, string, string> coding_data;
	};

	//this namespace provides functions used inside this module
	//it can not be used somewhere else
	namespace inner
	{
		static void read_diary(const string& path);
		static vector<string> split_text(const string& text, const string& delimiter);

		static void save_diary();

		static void read_pages_callback(Fl_Widget* button_ptr);
		static void add_page_callback(Fl_Widget* button_ptr);
		static void save_page_callback(Fl_Widget* button_ptr, void* window);
		static void get_topic_callback(Fl_Input* input);
		static void show_page_callback(Fl_Widget* browser_ptr);
		static void read_whole_diary_callback(Fl_Widget* button_ptr);

		static bool already_saved(const string& body);

		namespace fs = std::filesystem;
	};
};