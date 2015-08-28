#ifndef CCONSTPATTERNBASE_H
#define CCONSTPATTERNBASE_H

#include <vector>

#include "../types.h"
#include "../driversio/ichannel.h"

#define GETIOREG(reg) io->GetRegister(reg)
#define GETREG(name, reg) GetReg(name, reg)
#define CONVERT(name) { if (cachedIO[name] != NULL) cachedIO[name]->Convert(); else TRACEUCU("Конвертируется не кэшированый IO\r"); }
#define IS_CONNECTED(name) (cachedIO[name] != NULL)
#define ADDIN(type, num) 	{pat->AddIn(type, num-1); io = pat->GetInput(type, num-1);}
#define ADDOUT(type, num)	{pat->AddOut(type, num-1); io = pat->GetOutput(type, num-1);}
#define CACHEIO(name, type, num) CacheIO(cachedIO[name], type, num-1)
#define CACHEIOLABEL(name, type, num, label) CacheIOLabel(cachedIO[name], type, num-1, REVERSE(label))
#define SETTIMER(interval, id) 	manager.GetConfig().GetConstPatterns().SetTimer(interval, selfNum, id)
#define SETONCETIMER(interval, id)  manager.GetConfig().GetConstPatterns().SetSuspendTimer(interval, selfNum, id)
#define KILLTIMER 	manager.GetConfig().GetConstPatterns().KillTimer(selfNum)
#define ADDWORD(type, num, label) io = AddWord(type, num-1, REVERSE(label))
#define GETINDEXLABEL(type, num, label) GetIndexLabel(type, num-1, REVERSE(label))



class CPattern;

class CConstPatternBase
{
public:
	CConstPatternBase(UINT lBaseNum, bool load);
	virtual ~CConstPatternBase(void);
	
	virtual void InstanceDevice() = 0;
	virtual void DoPatternFunction(IOTYPES type, UINT num) = 0;
	virtual void OnTimer(DWORD timer_id)	{}
	UINT& GetSelfNum()	{ return selfNum; }
#ifndef _NO_COMPILIER   
    void Save();
#endif
	void Load();
    
    CRegister* GetReg(UINT name, REGISTER_ID reg);

protected:
	struct LABELS
	{
		BYTE label;
		IOTYPES type;
		BYTE num;
		BYTE index;
	};
	CPattern* PrepareCreatePattern(UINT countIO, bool load = false);
	void CacheIO(IChannel*& io, IOTYPES type, UINT num);
	void CacheIOLabel(IChannel*& io, IOTYPES type, UINT num, BYTE label);
	IChannel* AddWord(IOTYPES type, UINT num, BYTE label);
	UINT GetIndexLabel(IOTYPES type, UINT num, BYTE label);

	IChannel** cachedIO;
	std::vector<LABELS> labels;
	UINT baseNum;
	UINT selfNum;
};

#endif

