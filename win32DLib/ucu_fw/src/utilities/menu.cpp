/*
 * menu.cpp
 *
 *  Created on: 22 авг. 2015 г.
 *      Author: Youbrin_A
 */

#include "menu.h"
#include "../drivers/keys.h"
#include "../drivers/display.h"
#include "../utilities/console.h"

Menu::Menu(Keys* keys, Display* display) : _keys(keys), _display(display)
{

	_isActive = false;
	_14KeysPressedTime = 0;
	_23KeysPressedTime = 0;
	CreateMenu();
}

void Menu::CreateMenu()
{
	_mainMenu = new SubMenu("MAIN");
	_mainMenu->AddItem(new MenuItem("ПРП_"));
	auto ubs = new SubMenu("УБС_");
	_mainMenu->AddItem(ubs);
	ubs->AddItem(new MenuItem("АППР"));
	ubs->AddItem(new MenuItem("ПО__"));
	ubs->AddItem(new MenuItem("CFr"));
	ubs->AddItem(new MenuItem("CFUr"));
}

void Menu::ProcessInput()
{
	if (_isActive)
	{
		if (_keys->GetPrevKeyState().menu.Up == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Up == Keys::KEY_STATE::Pressed)
			_mainMenu->Prev();
		if (_keys->GetPrevKeyState().menu.Down == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Down == Keys::KEY_STATE::Pressed)
			_mainMenu->Next();
		if (_keys->GetPrevKeyState().menu.Enter == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Enter == Keys::KEY_STATE::Pressed)
			_mainMenu->Enter();
		if (_keys->GetPrevKeyState().menu.Esc == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Esc == Keys::KEY_STATE::Pressed)
			if (_mainMenu->Exit())
				_isActive = false;

	}else
	{
		if (_14KeysPressedTime > 0)
			_14KeysPressedTime+=80;
		if (_23KeysPressedTime > 0)
			_23KeysPressedTime+=80;
		// Отпущена комбинация 1 4
		if (_keys->GetPrevKeyState().menu.Enter == Keys::KEY_STATE::Pressed && _keys->GetKeyState().menu.Enter == Keys::KEY_STATE::Released
			&&_keys->GetPrevKeyState().menu.Esc == Keys::KEY_STATE::Pressed && _keys->GetKeyState().menu.Esc == Keys::KEY_STATE::Released)
		{
			_14KeysPressedTime = 80;
		}
		// Нажата комбинация 2 3
		else if (_keys->GetPrevKeyState().menu.Up == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Up == Keys::KEY_STATE::Pressed
		 && _keys->GetPrevKeyState().menu.Down == Keys::KEY_STATE::Released && _keys->GetKeyState().menu.Down == Keys::KEY_STATE::Pressed
		 && _14KeysPressedTime > 0 && _14KeysPressedTime < 12000)
			_23KeysPressedTime = 80;
		// Отпущена комбинация 2 3
		else  if (_keys->GetPrevKeyState().menu.Up == Keys::KEY_STATE::Pressed && _keys->GetKeyState().menu.Up == Keys::KEY_STATE::Released
				 && _keys->GetPrevKeyState().menu.Down == Keys::KEY_STATE::Pressed && _keys->GetKeyState().menu.Down == Keys::KEY_STATE::Released
				 && _23KeysPressedTime > 2000 && _14KeysPressedTime < 12000)
			_isActive= true;
		else if (_keys->GetPrevKeyState().Value != _keys->GetKeyState().Value)
			_23KeysPressedTime = 0;
		//Console::TraceLine("prev %X, cur %X, 14 %d, 23 %d", _keys->GetPrevKeyState().Value, _keys->GetKeyState().Value, _14KeysPressedTime, _23KeysPressedTime);
	}

}

void Menu::Draw()
{
	if (_isActive)
		_display->SetTextByText(_mainMenu->GetCurrentItem()->GetName(), true);
}
