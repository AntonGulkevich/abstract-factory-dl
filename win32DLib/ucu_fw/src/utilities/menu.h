/*
 * menu.h
 *
 *  Created on: 22 ���. 2015 �.
 *      Author: Youbrin_A
 */

#ifndef MENU_H_
#define MENU_H_
#include "menu/submenu.h"

class Keys;
class Display;

class Menu
{
private:
	bool _isActive;
	Keys* _keys;
	Display* _display;

	SubMenu* _mainMenu;

	void CreateMenu();

	// ��������� ����� � ���� ������ �������
	int _14KeysPressedTime;
	int _23KeysPressedTime;

public:
	Menu(Keys* keys, Display* display);
	virtual ~Menu() {}

	bool IsActive() { return _isActive; }
	void ProcessInput();
	void Draw();
};

#endif /* MENU_H_ */
