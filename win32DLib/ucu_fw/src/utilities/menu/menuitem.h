/*
 * menuitem.h
 *
 *  Created on: 22 рту. 2015 у.
 *      Author: Youbrin_A
 */

#ifndef MENUITEM_H_
#define MENUITEM_H_

class MenuItem
{
private:
	const char* _name;

protected:
	bool _isSubMenu;

public:
	MenuItem(const char* name);
	virtual ~MenuItem();

	const char* GetName() { return _name; }
	bool IsSubMenu() { return _isSubMenu; }
};

#endif /* MENUITEM_H_ */
