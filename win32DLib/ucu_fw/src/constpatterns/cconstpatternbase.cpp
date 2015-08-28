#include "cconstpatternbase.h"

#define TRACEUCU(...)

CConstPatternBase::CConstPatternBase(UINT lBaseNum, bool load) : baseNum(lBaseNum)
{
	cachedIO = NULL;
	selfNum = 0xFF;
}

CConstPatternBase::~CConstPatternBase(void)
{
	delete[] cachedIO;
}


CRegister* CConstPatternBase::GetReg(UINT name, REGISTER_ID reg)
{
    if (cachedIO[name] != NULL) 
        return cachedIO[name]->GetRegister(reg); 
    else
    { 
        TRACEUCU("Попытка чтения регистра с не кэшированого IO\r");
        return NULL;
    }
}

void CConstPatternBase::CacheIO(IChannel*& io, IOTYPES type, UINT num)
{
	if ((DWORD)io == 0xFFFFFFFF)
	{
/*		if (IS_IN(type))
			io = manager.GetConfig().GetSrcByConnection(selfNum, type, num);
		else if (IS_OUT(type))
			io = manager.GetConfig().GetDstByConnection(selfNum, type, num);
		else
		{
			io = NULL;
			TRACEUCU("Незакэшировалось!\r\n");
		}*/
	}

}

void CConstPatternBase::CacheIOLabel(IChannel*& io, IOTYPES type, UINT num, BYTE label)
{
	if ((DWORD)io == 0xFFFFFFFF)
	{
/*		if (IS_IN(type))
			io = ((CArincIn*)manager.GetConfig().GetSrcByConnection(selfNum, ioDigitalIn, num))->GetWord(label, 0);
		else if (IS_OUT(type))
			io = ((CArincOut*)manager.GetConfig().GetDstByConnection(selfNum, ioDigitalOut, num))->GetWord(label, 0);
		else
			io = NULL;*/
	}
}

CPattern* CConstPatternBase::PrepareCreatePattern(UINT countIO, bool load)
{
	CPattern* pat = NULL;
	if (!load)
	{
		/*selfNum = manager.GetConfig().AddPattern(true);
		pat = &manager.GetConfig().GetPattern(selfNum);
		pat->SetDeviceNum(baseNum);  // Этот номер должен быть равен номеру*/
	}
	// Кэширование входов
	cachedIO = new IChannel*[countIO];
	for(UINT i = 0; i < countIO; i++)
		cachedIO[i] = (IChannel*)0xFFFFFFFF;
	return pat;
}

IChannel* CConstPatternBase::AddWord(IOTYPES type, UINT num, BYTE label)
{
	/*int result;
	LABELS lab;
	lab.label = label;
	lab.num = num;
	lab.type = type;
	lab.index = manager.driversIO.AddArincWord(((IS_OUT(type)) ? CBaseIO::_OUT : CBaseIO::_IN), label, true);
	labels.push_back(lab);
	if (IS_OUT(type))
	{
		manager.GetConfig().GetPattern(selfNum).AddOut(ioArincWord, lab.index);
		result = ((CArincOut*)manager.GetConfig().GetPattern(selfNum).GetOutput(ioDigitalOut, num))->AddWord(lab.index);
		manager.GetConfig().AddConnection(selfNum, ioArincWord, lab.index, 0xFF, ioArincWord, lab.index);
		return ((CArincOut*)manager.GetConfig().GetPattern(selfNum).GetOutput(ioDigitalOut, num))->GetWordByIndex((BYTE)result);
	}
	else
	{
		manager.GetConfig().GetPattern(selfNum).AddIn(ioArincWord, lab.index);
		result = ((CArincIn*)manager.GetConfig().GetPattern(selfNum).GetInput(ioDigitalIn, num))->AddWord(lab.index);
		return ((CArincIn*)manager.GetConfig().GetPattern(selfNum).GetInput(ioDigitalIn, num))->GetWordByIndex((BYTE)result);
	}

	 
	*/
	return NULL;
}

#ifndef _NO_COMPILIER   
void CConstPatternBase::Save()
{
/*	WORD size = labels.size();
	SaveParam(&size, sizeof(size));
	for(UINT i = 0; i < size; i++)
	{
		SaveParam(&labels[i], sizeof(LABELS));
	}*/
	
}

#endif
void CConstPatternBase::Load()
{
/*	WORD size = 0;
	LoadParam(&size, sizeof(size));
	for(UINT i = 0; i < size; i++)
	{
		LABELS lab;
		LoadParam(&lab, sizeof(LABELS));
		labels.push_back(lab);
	}*/
}

UINT CConstPatternBase::GetIndexLabel(IOTYPES type, UINT num, BYTE label)
{
	for(UINT i = 0; i < labels.size(); i++)
		if (labels[i].type == type && labels[i].num == num && labels[i].label == label)
			return labels[i].index;
    return 0xFFFFFFFF;
}
