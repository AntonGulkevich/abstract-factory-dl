#include "cconstpatterns.h"
// Список константных устройств
/*#include "../constpatterns/ccpselector.h"
#include "../constpatterns/ccppuish.h"  
#include "../constpatterns/cpuikiev.h"  

#include "../constpatterns/ccalc_a826.h"  
#include "../constpatterns/ccprsucuout.h" 
#include "../constpatterns/ccprsucuin.h" 
#include "../constpatterns/ccprsgspin.h" 
#include "../constpatterns/ccprsgspout.h" 
*/
CConstPatternEngine::CConstPatternEngine()
{

}

CConstPatternEngine::~CConstPatternEngine()
{
	for (UINT i = 0; i < constPatterns.size(); i++)
		delete constPatterns[i];
}


bool CConstPatternEngine::CreatePattern(UINT index, bool load, UINT selfNum)
{
 
/*    try
    {    
	switch(index)
	{
	case 0: // Выбор одного из логических выходов по разовому признаку
		constPatterns.push_back(new CCPSelector(0, load));
		break;
	case 1:
		constPatterns.push_back(new CCPPuish(1, load));
		break;
	case 2:
		constPatterns.push_back(new CCPCalc_A826(2, load));
		break;
	case 3:
		constPatterns.push_back(new CCPRSUCUOut(3, load));
		break;
	case 4:
		constPatterns.push_back(new CCPRSUCUIn(4, load));
		break;
    case 5:
        constPatterns.push_back(new CPUIKiev(5, load));        
        break;
    case 6:
		constPatterns.push_back(new CCPRSGSPIn(6, load));        
        break;
    case 7:
		constPatterns.push_back(new CCPRSGSPOut(7, load));        
        break;

	default:
		return false;
	}
	if (load)
		constPatterns.back()->GetSelfNum() = selfNum;
    }
    catch(...)
    {
        TRACEUCU("Ошибка при создании константного устройства %d\r\n", index);
    }*/
	return true;
}

void CConstPatternEngine::Load(UINT patternNum)
{
	for(UINT i = 0; i < constPatterns.size(); i++)
		if (constPatterns[i]->GetSelfNum() == patternNum)
		{
			constPatterns[i]->Load();
			break;
		}
}

void CConstPatternEngine::Save(UINT patternNum)
{
	for(UINT i = 0; i < constPatterns.size(); i++)
		if (constPatterns[i]->GetSelfNum() == patternNum)
		{
			constPatterns[i]->Save();
			break;
		}
}

void CConstPatternEngine::DoPatternFunction(UINT patternNum, IOTYPES type, UINT num)
{
	for(UINT i = 0; i < constPatterns.size(); i++)
		if (constPatterns[i]->GetSelfNum() == patternNum)
		{
			constPatterns[i]->DoPatternFunction(type, num);
			break;
		}
}


void CConstPatternEngine::InstanceDevice(UINT patternNum)
{
	for(UINT i = 0; i < constPatterns.size(); i++)
		if (constPatterns[i]->GetSelfNum() == patternNum)
		{
			constPatterns[i]->InstanceDevice();
			break;
		}
}

DWORD CConstPatternEngine::OnTimer(DWORD id, DWORD param)
{
	for(UINT i = 0; i < constPatterns.size(); i++)
		if (constPatterns[i]->GetSelfNum() == id)
		{
			constPatterns[i]->OnTimer(param);
			break;
		}
	return 1;
}

