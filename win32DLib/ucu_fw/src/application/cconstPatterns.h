#ifndef CONSTPATTERNS_H
#define CONSTPATTERNS_H

#include <vector>
#include "../constpatterns/cconstpatternbase.h"

class CConstPatternEngine
{
public:
	CConstPatternEngine();
	virtual ~CConstPatternEngine();
	bool CreatePattern(UINT index, bool load = false, UINT selfNum = 0xFFFF);
	void SetSelfNum(UINT index);
	void DoPatternFunction(UINT patternNum, IOTYPES type, UINT num);
	void InstanceDevice(UINT patternNum);
	DWORD OnTimer(DWORD id, DWORD param = 0xFFFF);
	void Load(UINT patternNum);
	void Save(UINT patternNum);
private:
	std::vector<CConstPatternBase*> constPatterns;
};

#endif // CONSTPATTERNS_H


