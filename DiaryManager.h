#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"Common.h"
#include "FL/Fl.H"
#include"FL/Fl_Button.H"
#include"FL/Fl_Window.H"
#include"FL/Fl_Text_Editor.H"
#include"FL/Fl_Text_Buffer.H"
#include"FL/Fl_Input.H"
#include"Diary.h"
#include<vector>
#include<fstream>
#include<string>
#include<ctime>
#include<iostream>

namespace DiaryManager
{
	void run();

	static Fl_Window* window = nullptr;
	static vector<DiaryPage*> pages;

	static Fl_Text_Buffer* buff = nullptr;
	static string entered_topic;

	namespace inner
	{
		static void read_diary(const string& path);
		static void read_pages_callback(Fl_Widget* button_ptr,void* pages);
		static void add_page_callback(Fl_Widget* button_ptr,void* pages);
		static void save_page_callback(Fl_Widget* button_ptr, void* pages);
		static void get_topic_callback(Fl_Input* input);
	};
};