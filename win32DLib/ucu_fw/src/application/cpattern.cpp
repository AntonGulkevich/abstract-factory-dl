#include "cpattern.h"
#include <cstring>

#include "../driversio/charincin.h"
#include "../driversio/charincout.h"
#include "../driversio/charincwordin.h"
#include "../driversio/charincwordout.h"
#include "../driversio/chpotentiometr.h"
#include "../driversio/chanalogin.h"
#include "../driversio/chnptout.h"
#include "../driversio/chsctout.h"
#include "../driversio/chlogicalin.h"
#include "../driversio/chlogicalout.h"
#include "../driversio/chdescreetein.h"
#include "../driversio/chdescreeteout.h"
#include "../driversio/chnotconnectedio.h"
#include "../driversiohw/arincwordin.h"
#include "../driversiohw/arincwordout.h"
#include "../driversiomanager.h"
#include "cprocessor.h"

CPattern::CPattern(DriversIOManager* driversIO, CConfiguration* config, UINT lSelfNum, bool lConstant)
{
	_config = config;
	_driversIO = driversIO;
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;
	registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME, rwConstant, rtString, 0,0,0, true};
	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
	{
		if (registers_t[i].id != REGISTER_ID::NULLID)
			registers_t[i].reg = new CRegister();
		else
			registers_t[i].reg = NULL;
	}

	deviceNum = 0xFF; 
	selfNum = lSelfNum; 
    constant = lConstant;
}

CPattern::~CPattern()
{
    for(UINT i = 0; i < ins.size(); i++)
        delete ins[i];
    for(UINT i = 0; i < outs.size(); i++)
        if (outs[i])
			delete outs[i];
       
}


CPattern::ADDIO_RESULT CPattern::AddIn(IOTYPES type, BYTE num, BYTE label)
{
	if (IndexOfIns(type, num) == -1)
	{
		switch(type)
		{
		case ioDigitalIn:
			ins.push_back(new ChArincIn(this, num));
			break;
		case ioAnalog:
			ins.push_back(new ChAnalogIn(this, num));
			break;
		case ioPotentiometr:
			ins.push_back(new ChPotentiometr(this, num));
			break;
		case ioRSIn:
			//ins.push_back(new ChRSIn());
			break;
		case ioCheck:
			ins.push_back(new ChDescreeteIn(this, num));
			break;
		case ioLogicalIn:
			ins.push_back(new ChLogicalIn(this, num));
			break;
		case ioArincWordIn:
		{
			int i = IndexOfIns(ioDigitalIn, num);
			if (i != -1)
			{
				if (!((ChArincIn*)ins[i])->IsLabelExist(label))
					((ChArincIn*)ins[i])->AddLabel(label);
				else
					return ADDIO_RESULT::AlreadyExist;
			}else
				return ADDIO_RESULT::Error;
		}
			break;
		default:
			throw;
		}
		return ADDIO_RESULT::OK;
	} else
		return ADDIO_RESULT::AlreadyExist;
}

CPattern::ADDIO_RESULT CPattern::AddOut(IOTYPES type, BYTE num, BYTE label)
{
	if (IndexOfOuts(type, num) == -1)
	{
		switch(type)
		{
		case ioDigitalOut:
			outs.push_back(new ChArincOut(this, num));
			break;
		case ioNPT:
			outs.push_back(new ChNPTOut(this, num));
			break;
		case ioCommand:
			outs.push_back(new ChDescreeteOut(this, num));
			break;
		case ioSCT:
			outs.push_back(new ChSCTOut(this, num));
			break;
		case ioLogicalOut:
			outs.push_back(new ChLogicalOut(this, num));
			break;
		case ioRSOut:
			//out.push_back(new CRSOut());
			break;
		case ioArincWordOut:
		{
			int i = IndexOfOuts(ioDigitalOut, num);
			if (i != -1)
			{
				if (!((ChArincOut*)outs[i])->IsLabelExist(label))
					((ChArincOut*)outs[i])->AddLabel(label);
				else
					return ADDIO_RESULT::AlreadyExist;
			}else
				return ADDIO_RESULT::Error;
		}
			break;
		default:
			throw;
		}
		return ADDIO_RESULT::OK;
	}
	else
		return ADDIO_RESULT::AlreadyExist;
}

CPattern::ADDIO_RESULT CPattern::AddNCIO(IOTYPES type, BYTE num)
{
	if (!IsNC(type, num))
		ncio.push_back(new ChNotConnectedIO(this, type, num));
	else 
		return ADDIO_RESULT::AlreadyExist;
	return ADDIO_RESULT::OK;
}

int CPattern::IndexOfIns(IOTYPES type, BYTE num)
{
	for(UINT i = 0; i < ins.size(); i++)
		if (ins[i]->IsNumberType(type, num))
			return i;
	return -1;
}

int CPattern::IndexOfOuts(IOTYPES type, BYTE num)
{
	for(UINT i = 0; i < outs.size(); i++)
		if (outs[i]->IsNumberType(type, num))
			return i;
	return -1;
}

CProcessor* CPattern::GetChannel(IOTYPES type, BYTE num)
{
	auto out = GetOutput(type, num);
	if (out != NULL)
		return out->GetProcessor();
	return NULL;
}

CProcessor* CPattern::GetChannelLabel(BYTE num, BYTE label) // Id пока не понятно как использовать
{
	auto out = GetOutput(ioDigitalOut, num);
	if (out != NULL && ((ChArincOut*)out)->GetLabel(label) != NULL)
		return ((ChArincOut*)out)->GetLabel(label)->GetProcessor();
	return NULL;
}

IChannelIn* CPattern::GetInput(IOTYPES type, UINT num)
{
	int res = IndexOfIns(type, num);
	if (res == -1)
		return NULL;
	else
		if (res < (int)ins.size())
			return ins[res];
		else
			return NULL;
}

IChannelOut* CPattern::GetOutput(IOTYPES type, UINT num)
{
	int res = IndexOfOuts(type, num);
	if (res == -1)
		return NULL;
	else
		if (res < (int)outs.size())
			return outs[res];
		else
			return NULL;
}


bool CPattern::IsNC(IOTYPES type, BYTE num)
{
	for(UINT i = 0; i < ncio.size(); i++)
		if (ncio[i]->IsNumberType(type, num))
			return true;
	return false;
}

CPattern::VALIDIO_RESULT CPattern::ValidIO(IOTYPES type, UINT index)
{
	if (IsNC(type, index))
		return VALIDIO_RESULT::NC; // Если в(ы)ход не помечен как не подключенный, то удаление в(ы)хода
	if (IS_IN(type)) // Входы
	{
		if (GetInput(type, index) == NULL)
			return VALIDIO_RESULT::NotExist;
		else if (GetInput(type, index)->IsConfigured())	// Если уже сконфигурирован, значит повторная конфигурация - ошибка
			return VALIDIO_RESULT::AlreadyConfigured;
	}
	else
	{
		if (GetOutput(type, index) == NULL)
			return VALIDIO_RESULT::NotExist;
		else if (GetOutput(type, index)->IsConfigured())  // Если уже сконфигурирован, значит повторная конфигурация - ошибка
			return VALIDIO_RESULT::AlreadyConfigured;
	}
	return VALIDIO_RESULT::OK;
}

int CPattern::InitDevice()
{
	IOTYPES lType;
	BYTE lNum;
	BYTE lPat;
	UINT insize = ins.size();
	for(UINT i = 0; i < insize; i++)
	{
		if (_config->GetConnection(GetSelfNum(), ins[i]->GetType(), ins[i]->GetNumber(), lPat, lType, lNum) != -1)
		{
			if (ins[i]->GetType() == ioLogicalIn) // Логический соединяется с логическим выходом
				ins[i]->AssignChannel((IInputValue*)_config->GetPattern(lPat)->GetOutput(ioLogicalOut, lNum));
			else
				ins[i]->AssignChannel(_driversIO->GetInput(lType, lNum));
			if (ins[i]->GetType() == ioDigitalIn) // Для ARINC соединяем отдельно каждую метку
				for(int l = 0; l < 256; l++)
					if (((ChArincIn*)ins[i])->IsLabelExist(l))
					{
						auto word = new ArincWordIn(_driversIO->GetArincIn(lNum), l);
						_driversIO->AddArincWord(word);
						((ChArincIn*)ins[i])->GetLabel(l)->AssignChannel(word);
						((ChArincIn*)ins[i])->GetLabel(l)->InitRegisters();
						ins.push_back(((ChArincIn*)ins[i])->GetLabel(l)); // Добавление метки в общий список каналов
					}
			ins[i]->InitRegisters();

		}
	}
	UINT outsize = outs.size();
	for(UINT i = 0; i < outsize; i++)
	{
		if (_config->GetConnection(GetSelfNum(), outs[i]->GetType(), outs[i]->GetNumber(), lPat, lType, lNum) != -1)
		{
			if (outs[i]->GetType() != ioLogicalOut) // Логический выход никому ничего не должен
				outs[i]->AssignChannel(_driversIO->GetOutput(lType, lNum));
			if (outs[i]->GetType() == ioDigitalOut) // Сначала инициализируем метки
				for(int l = 0; l < 256; l++)
					if (((ChArincOut*)outs[i])->IsLabelExist(l))
					{
						auto word = new ArincWordOut(_driversIO->GetArincOut(lNum), l);
						_driversIO->AddArincWord(word);
						((ChArincOut*)outs[i])->GetLabel(l)->AssignChannel(word);
						((ChArincOut*)outs[i])->GetLabel(l)->InitRegisters();
						outs.push_back(((ChArincOut*)outs[i])->GetLabel(l));
					}
			outs[i]->InitRegisters();

		}
	}
	return 0;
}

void CPattern::CompileDevice()
{
#ifndef _NO_COMPILIER
    for(UINT i = 0; i < outs.size(); i++)
    {
		if (!constant)
		{
			outs[i]->GetProcessor()->Compile();
		}
    }
#endif
}

void CPattern::InstanceDevice()
{
    for(UINT i = 0; i < outs.size(); i++)
    {
        outs[i]->GetProcessor()->Instance();
        outs[i]->GetProcessor()->Init();
    }
    
}

void CPattern::ClearDevice()
{
	for(UINT i = 0; i < ins.size(); i++)
		delete ins[i];
	for(UINT i = 0; i < outs.size(); i++)
		delete outs[i];
	ins.clear();
	outs.clear();
	return;
}

/*
void CPattern::Save()
{
	BYTE len = (BYTE)strlen(GetRegister(rNAME)->GetValueStr());
	SaveParam(&len, sizeof(len));
	SaveParam((char*)GetRegister(rNAME)->GetValueStr(), len);
	WORD size = (WORD)ins.size();
	SaveParam(&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		SaveParam(&ins[i].num, sizeof(ins[i].num));
		SaveParam(&ins[i].type, sizeof(ins[i].type));
	}
	size = (WORD)outs.size();
	SaveParam(&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		SaveParam(&outs[i].num, sizeof(outs[i].num));
		SaveParam(&outs[i].type, sizeof(outs[i].type));
		//outs[i].proc->Save();
        //SVSET;
	}
	SaveParam(&constant, sizeof(constant));
	SaveParam(&selfNum, sizeof(selfNum));
	SaveParam(&deviceNum, sizeof(deviceNum));

}

void CPattern::Load()
{
	WORD size = 0;
	BYTE len = 0;
	LoadParam(&len, sizeof(len));
	char *strParam = new char[len+1];
	LoadParam(strParam, len);
	strParam[len] = 0;
	GetRegister(rNAME)->SetValue(strParam);
	delete[] strParam;
	LoadParam(&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		PIN pin;
		LoadParam(&pin.num, sizeof(pin.num));
		LoadParam(&pin.type, sizeof(pin.type));
		ins.push_back(pin);
	}
	LoadParam(&size, sizeof(size));
	for (int i = 0; i < size; i++)
	{
		POUT pout;
		LoadParam(&pout.num, sizeof(pout.num));
		LoadParam(&pout.type, sizeof(pout.type));
		//pout.proc = new CProcessor();
        //pout.proc->Load();
		outs.push_back(pout);
	}
	LoadParam(&constant, sizeof(constant));
	LoadParam(&selfNum, sizeof(selfNum));
	LoadParam(&deviceNum, sizeof(deviceNum));
}
*/
