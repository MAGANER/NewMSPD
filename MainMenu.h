#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include"FL/Fl_Input.H"
#include<filesystem>
#include<fstream>
#include"Encryption.h"
#include"DiaryManager.h"
#include"resource.h"

namespace MainMenu
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 220;

	namespace inner_data
	{
		static Fl_Input* diary_path_field = nullptr;
		static Fl_Input* key_file_field = nullptr;


		static Fl_Button* read_button = nullptr;
		static Fl_Button* generate_key_button = nullptr;
		static Fl_Button* create_diary_button = nullptr;


		static string iv = "";
		static string key = "";
		static string diary_path = "";
	};
	namespace inner
	{
		
		static void get_diary_callback(Fl_Input* input);
		static void read_callback(Fl_Widget* button_ptr,void* window);
		static void get_key_file(Fl_Input* input,void* window);

		static void create_diary_callback(Fl_Widget* button_ptr);
		static void get_new_diary_path_callback(Fl_Input* input, void* window);

		static void generate_callback(Fl_Widget* button_ptr);
		static void get_key_path_callback(Fl_Input* input, void* window);

		Fl_Window* init_window();
		void init_widgets(Fl_Window* window);
	};
};