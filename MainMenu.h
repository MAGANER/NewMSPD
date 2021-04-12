#pragma once
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"
#include"FL/Fl_Input.H"

namespace MainMenu
{
	const int WINDOW_WIDTH = 720;
	const int WINDOW_HEIGHT = 280;

	static Fl_Input* diary_path_field,
			       * password_field,
			       * key_field;

	Fl_Window* init_window();
	void init_widgets(Fl_Window* window);
};