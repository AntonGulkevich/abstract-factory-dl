/*
 * submenu.cpp
 *
 *  Created on: 22 рту. 2015 у.
 *      Author: Youbrin_A
 */

#include "submenu.h"

void SubMenu::Next()
{
	if (_isInSubMenu)
		GetCurrentSubMenu()->Next();
	else
		_currentIndex = (_currentIndex + 1) % _items.size();

}

void SubMenu::Prev()
{
	if (_isInSubMenu)
		GetCurrentSubMenu()->Prev();
	else
	{
		if (_currentIndex  == 0)
			_currentIndex= _items.size()-1;
		else
			_currentIndex--;
	}
}


void SubMenu::Enter()
{
	if (_isInSubMenu)
		GetCurrentSubMenu()->Enter();
	else
	{
		if (_items[_currentIndex]->IsSubMenu())
			_isInSubMenu = true;
	}
}


bool SubMenu::Exit()
{
	if (_isInSubMenu)
	{
		if (GetCurrentSubMenu()->Exit())
			_isInSubMenu = false;
		return false;
	}
	else
		return true;
}

