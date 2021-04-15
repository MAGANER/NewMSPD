#pragma once
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_Box.H"

static void run_sub_window(const char* message,
						   const char* title,
						   Fl_Window* main_win)
{
	Fl_Window* error = new Fl_Window(300, 50, title);

	int main_win_x = main_win->x_root();
	int main_win_y = main_win->y_root();

	int window_width = main_win->w();
	int window_height = main_win->h();

	error->resize(main_win_x + (int)window_width / 2,
				  main_win_y + (int)window_height / 2,
				  300,
				  50);

	Fl_Box* def = new Fl_Box(0, 0, 300, 50, message);
	def->box(FL_FLAT_BOX);
	def->labelfont(FL_BOLD);
	def->labelsize(14);

	error->end();
	error->show();
}