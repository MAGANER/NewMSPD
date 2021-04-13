#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include"FL/Fl_Input.H"
#include"FL/Fl_Button.H"
#include<filesystem>
namespace MainMenu
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 280;


	static char* diary_path = nullptr; 
	static char* password   = nullptr;
	static char* key        = nullptr;

	static Fl_Input* diary_path_field = nullptr;
	static Fl_Input* password_field   = nullptr;
	static Fl_Input* key_field		  = nullptr;

	static bool read = false;
	static Fl_Button* read_button = nullptr;

	Fl_Window* init_window();
	void init_widgets(Fl_Window* window);

	namespace inner
	{
		
		enum InputType
		{
			Diary,
			Password,
			Key,
		};
		static void input_callback(Fl_Input* input,void* type);
		static void read_callback(Fl_Widget* button_ptr,void* window);
		static void run_error_window(const char* _error, Fl_Window* main_win);
	};
};