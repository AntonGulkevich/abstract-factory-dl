/*
 * submenu.h
 *
 *  Created on: 22 рту. 2015 у.
 *      Author: Youbrin_A
 */

#ifndef SUBMENU_H_
#define SUBMENU_H_
#include <vector>
#include "menuitem.h"
#include "../../types.h"

class SubMenu : public MenuItem
{
private:
	std::vector<MenuItem*> _items;
	UINT _currentIndex;
	bool _isInSubMenu;

	SubMenu* GetCurrentSubMenu() { return (SubMenu*)_items[_currentIndex]; }

public:
	SubMenu(const char* name) : MenuItem(name) {  _currentIndex = 0;  _isInSubMenu = false; _isSubMenu = true;}
	virtual ~SubMenu() {}

	void AddItem(MenuItem* item) { _items.push_back(item); }
	void Next();
	void Prev();
	void Enter();
	bool Exit();

	MenuItem* GetCurrentItem() { return _currentIndex > _items.size() ? UCUNULL : _items[_currentIndex]; }

};

#endif /* SUBMENU_H_ */
