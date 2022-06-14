#include "gui.h"

Textbox::Textbox() {
	sourceX = 0;
	sourceY = 0;
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	inputText = "";
	disabledText = "";
	length = 32;
	selected = false;
	isPassword = false;
}

Textbox::Textbox(ALLEGRO_BITMAP*& bitmap, int textboxType, ALLEGRO_FONT*& font, int& charPressed, int& mouseX, int& mouseY, int& mouseB, int givenX, int givenY, int givenW, int givenH, std::string givenDisabledText, int givenLength, bool givenIsPassword) {
	this->bitmap = bitmap;
	this->font = font;
	this->charPressed = &charPressed;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->textboxType = textboxType;
	this->x = givenX;
	this->y = givenY;
	this->w = givenW;
	this->h = givenH;
	sourceX = 0;
	sourceY = 0;
	inputText = "";
	disabledText = givenDisabledText;
	length = givenLength;
	selected = false;
	isPassword = givenIsPassword;
}

void Textbox::Process() {

	if (*mouseB & 1)
	{
		if (mouseOver())
		{
			selected = true;
		}
		else
		{
			selected = false;
		}
	}

	if (mouseOver()) {
		sourceX = w;
	}
	else {
		sourceX = 0;
	}

	if (selected)
	{
		if (*charPressed == 8)
		{
			if (inputText.length() > 0)
			{
				inputText.erase(inputText.length() - 1);
			}
		}
		else if (inputText.length() < (unsigned int)length)
		{
			if (*charPressed >= 65 && *charPressed <= 122)
			{
				inputText += (char)*charPressed;
			}
		}
	}
}

void Textbox::Render() {
	std::string drawText = "";
	if (isPassword)
	{
		for (unsigned int i = 0; i < inputText.length(); i++)
			drawText += '*';
	}
	else
		drawText = inputText;

	if (h == 0)
	{
		h = al_get_font_line_height(font) + 2;
	}

	if (textboxType != TEXTBOX_REGULAR) {
		if (selected)
			al_draw_bitmap_region(bitmap, sourceX, textboxType * 31, w, h, x, y, 0);
		else
			al_draw_bitmap_region(bitmap, sourceX, textboxType * 31, w, h, x, y, 0);
	}
	else {
		al_draw_bitmap_region(bitmap, sourceX, 0, w, h, x, y, 0);
	}

	if (inputText == "" && !selected)
	{
		if (textboxType != TEXTBOX_REGULAR)
			al_draw_text(font, al_map_rgba_f(0, 0, 0, 0.5), x + 35, y + 5, 0, disabledText.c_str());
		else
			al_draw_text(font, al_map_rgba_f(0, 0, 0, 0.5), x + 5, y + 5, 0, disabledText.c_str());
	}
	else
	{
		if (textboxType != TEXTBOX_REGULAR) {
			if (selected)
				al_draw_textf(font, al_map_rgba_f(0, 0, 0, 1), x + 35, y + 5, 0, "%s|", drawText.c_str());
			else
				al_draw_text(font, al_map_rgba_f(0.1, 0.1, 0.1, 0.5), x + 35, y + 5, 0, drawText.c_str());
		}
		else {
			if (selected)
				al_draw_textf(font, al_map_rgba_f(0, 0, 0, 1), x + 5, y + 5, 0, "%s|", drawText.c_str());
			else
				al_draw_text(font, al_map_rgba_f(0.1, 0.1, 0.1, 0.5), x + 5, y + 5, 0, drawText.c_str());
		}
	}
}

Button::Button() {
	this->bitmap = NULL;
	this->mouseX = 0;
	this->mouseY = 0;
	this->mouseB = 0;
	this->h = 0;
	this->w = 0;
	this->sourceX = 0;
	this->sourceY = 0;
	this->x = 0;
	this->y = 0;
}

Button::Button(ALLEGRO_BITMAP*& bitmap, int type, int x, int y, int sw, int sh, int sourceY, int& mouseX, int& mouseY, int& mouseB) {
	this->bitmap = bitmap;
	this->buttonType = type;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->x = x;
	this->y = y;
	this->w = sw;
	this->h = sh;
	this->sourceX = 0;
	this->sourceY = sourceY;
}

void Button::Process() {
	if (!mouseOver()) {
		sourceX = 0;
	}
	else
		sourceX = al_get_bitmap_width(bitmap) / 2;
}

bool Button::clicked() {
	if (mouseOver()) {
		if (*mouseB & 1) {
			*mouseB = 0;
			return true;
		}
	}

	return false;
}

void Button::Render() {
	al_draw_bitmap_region(bitmap, sourceX, sourceY, w, h, x, y, 0);
}

void Button::RenderScaled() {
	al_draw_scaled_bitmap(bitmap, sourceX, sourceY, w, h, x, y, x * 1.3, y * 1.3, 0);
}

Box::Box() {
	this->bitmap = NULL;
	this->mouseX = 0;
	this->mouseY = 0;
	this->mouseB = 0;
	this->w = 0;
	this->h = 0;
	this->sourceX = 0;
	this->sourceY = 0;
	this->x = 0;
	this->y = 0;
	this->selected = false;
}

Box::Box(ALLEGRO_BITMAP*& bitmap, int x, int y, int& mouseX, int& mouseY, int& mouseB) {
	this->bitmap = bitmap;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->x = x;
	this->y = y;
	this->sourceX = 0;
	this->sourceY = 0;
	this->selected = false;

	h = al_get_bitmap_height(bitmap);
	w = al_get_bitmap_width(bitmap) / 2;
}

void Box::Process() {
	if (mouseOver()) {
		sourceX = al_get_bitmap_width(bitmap) / 2;
	}
	else {
		sourceX = 0;
		this->selected == false ? sourceX = 0 : sourceX = al_get_bitmap_width(bitmap) / 2;
	}

	if (*mouseB & 1)
	{
		if (mouseOver())
		{
			selected = true;
		}
		else
		{
			selected = false;
		}
	}
}

bool Box::clicked() {
	if (*mouseB & 1) {
		if (mouseOver()) {
			*mouseB = 0;

			return true;
		}
	}

	return false;
}

void Box::Render() {
	al_draw_bitmap_region(bitmap, sourceX, 0, w, h, x, y, 0);
}

Checkbox::Checkbox() {
	this->bitmap = NULL;
	this->mouseX = 0;
	this->mouseY = 0;
	this->mouseB = 0;
	this->h = 0;
	this->w = 0;
	sourceX = 0;
	sourceY = 0;
	x = 0;
	y = 0;
	selected = false;
	checked = false;
}

Checkbox::Checkbox(ALLEGRO_BITMAP*& bitmap, int givenX, int givenY, bool givenChecked, int& mouseX, int& mouseY, int& mouseB) {
	this->bitmap = bitmap;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	sourceX = 0;
	sourceY = 0;
	x = givenX;
	y = givenY;
	selected = false;
	checked = givenChecked;

	w = al_get_bitmap_width(bitmap) / 2;
	h = al_get_bitmap_height(bitmap);
}

void Checkbox::Process() {
	if (mouseOver())
	{
		sourceX = 16;
		if (*mouseB & 1)
		{
			*mouseB = 0;
			checked ? checked = false : checked = true;
		}
	}
	else {
		sourceX = 0;
	}
}

void Checkbox::Render() {
	if (!checked) {
		al_draw_bitmap_region(bitmap, sourceX, 0, al_get_bitmap_width(bitmap) / 3, al_get_bitmap_height(bitmap), x, y, 0);
	}
	else if (checked) {
		al_draw_bitmap_region(bitmap, 32, 0, al_get_bitmap_width(bitmap) / 3, al_get_bitmap_height(bitmap), x, y, 0);
	}
}

Dialogue::Dialogue() {
	this->bitmap = {};
	this->mouseX = 0;
	this->mouseY = 0;
	this->mouseB = 0;
	this->x = 0;
	this->y = 0;
	this->h = 0;
	this->w = 0;
}

Dialogue::Dialogue(std::vector<ALLEGRO_BITMAP*> bitmap, int givenX, int givenY, int givenSizeX, int givenSizeY, int& mouseX, int& mouseY, int& mouseB) {
	this->bitmap = bitmap;
	this->mouseX = &mouseX;
	this->mouseY = &mouseY;
	this->mouseB = &mouseB;
	this->x = givenX;
	this->y = givenY;
	this->w = givenSizeX;
	this->h = givenSizeY;
}

void Dialogue::Process() {

}

void Dialogue::Render(int option) {
	if (option == CHAR_CREATION) {
		al_draw_rounded_rectangle(x, y, x + w, y + h, 2, 2, al_map_rgb(212, 179, 132), 2);

		//Fill out with bg
		for (int i = this->x + 1; i < this->x + this->w - 50; i += 54) {
			for (int u = this->y + 1; u < this->y + this->h - 50; u += 54) {
				al_draw_bitmap(bitmap[8], i, u, 0);
			}
		}

		//Fill out remaining side
		for (int u = this->y + 1; u <= this->y + this->h - 50; u += 54) {
			al_draw_bitmap(bitmap[8], x + 245, u, 0);
		}

		//Fill out remaining bottom
		for (int u = this->x + 1; u <= this->x + this->w - 50; u += 54) {
			al_draw_bitmap(bitmap[8], u, y + 195, 0);
		}
	}
}