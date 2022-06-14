#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <iostream>
#include <vector>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>

enum TextboxType {
	TEXTBOX_USERNAME,
	TEXTBOX_PASSWORD,
	TEXTBOX_REGULAR
};

enum ButtonType {
	REGULAR_BUTTON,
	ARROW_BUTTON
};

enum DialogueType {
	CHAR_CREATION
};

class Textbox 
{
	private:
		ALLEGRO_BITMAP* bitmap;
		ALLEGRO_FONT* font;
		int* charPressed, * mouseX, * mouseY, * mouseB;
		int textboxType;
		int sourceX, sourceY;
		int x, y;
		int w, h;
		std::string inputText;
		std::string disabledText;
		int length;
		bool selected;
		bool isPassword;

	public:
		Textbox();
		Textbox(ALLEGRO_BITMAP*& bitmap, int textboxType, ALLEGRO_FONT*& font, int& charPressed, int& mouseX, int& mouseY, int& mouseB, int givenX, int givenY, int givenW, int givenH, std::string givenDisabledText, int givenLength, bool givenDrawString);
		~Textbox() {
		}

	public:
		bool Selected() { return(selected); }
		void Select() { selected = true; }
		void Deselect() { selected = false; }

		bool mouseOver() { return(*mouseX >= x && *mouseY >= y && *mouseX < x + w && *mouseY < y + h); }

		void Process();
		void Render();

		std::string getText() { return inputText.c_str(); }
};


class Button 
{
	private:
		ALLEGRO_BITMAP* bitmap;
		int* mouseX, * mouseY, * mouseB;
		int buttonType;
		int sourceX, sourceY;
		int x, y;
		int w, h;

	public:
		Button();
		Button(ALLEGRO_BITMAP*& bitmap, int type, int x, int y, int sw, int sh, int sourceY, int& mouseX, int& mouseY, int& mouseB);
		~Button() {}; 

	public:
		void Process();

		bool clicked();
		bool mouseOver() { return(*mouseX >= x && *mouseY >= y && *mouseX < x + w && *mouseY < y + h); }

		void Render();
		void RenderScaled();
};


class Box 
{
	public:
		Box();
		Box(ALLEGRO_BITMAP*& bitmap, int x, int y, int& mouseX, int& mouseY, int& mouseB);
		~Box() {
		}

	public:
		void Process();

		bool clicked();
		bool mouseOver() { return(*mouseX >= x && *mouseY >= y && *mouseX < x + w && *mouseY < y + h); }
		bool Selected() { return selected; }
		void Deselect() { selected = false;  }

		void Render();

	private:
		ALLEGRO_BITMAP* bitmap;
		int* mouseX, * mouseY, * mouseB;
		int sourceX, sourceY;
		int x, y;
		int w, h;
		bool selected;
};
	

class Checkbox 
{
	public:
		Checkbox();
		Checkbox(ALLEGRO_BITMAP*& bitmap, int givenX, int givenY, bool givenChecked, int& mouseX, int& mouseY, int& mouseB);
		~Checkbox() {
		}

	public:
		bool isChecked() { return checked; }
		bool Selected() { return(selected); }
		void Select() { selected = true; }
		void Deselect() { selected = false; }

		bool mouseOver() { return(*mouseX >= x && *mouseY >= y && *mouseX < x + w && *mouseY < y + h); }

		void Process();
		void Render();

	private:
		ALLEGRO_BITMAP* bitmap;
		int* mouseX, * mouseY, * mouseB;
		int sourceX, sourceY;
		int x, y;
		int w, h;
		bool checked, selected;
};


class Dialogue {
	public:
		Dialogue();
		Dialogue(std::vector<ALLEGRO_BITMAP*> gui, int givenX, int givenY, int givenSizeX, int givenSizeY, int& mouseX, int& mouseY, int& mouseB);
		~Dialogue() {
		}

	public:
		void Process();
		void Render(int option);

	private:
		std::vector<ALLEGRO_BITMAP*> bitmap;
		int* mouseX, * mouseY, * mouseB;
		int x, y;
		int w, h;
};

#endif