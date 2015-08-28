#include "cconfiguration.h"
#include <cstring>
#include "../compilier/cparserconfig.h"
#include "../compilier/cparserdevice.h"
#include "../drivers/gpio.h"
#include "../driversio/charincout.h"
#include "../driversio/charincin.h"
#include "../driversio/charincwordin.h"
#include "../driversio/charincwordout.h"
#include "../driversio/chsctout.h"
#include "../driversiohw/analogin.h"
#include "../types.h"
#include "../workmanager.h"
#include "../driversiomanager.h"
#include "../utilities/console.h"
#include "cpattern.h"

CConfiguration::CConfiguration(WorkManager* workManager) : _workManager(workManager)
{
    _isInitialized = false;
    _arincInIndex = NULL;
    _arincOutIndex = NULL;
    _syncUse = 0;

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
		registers_t[i].id = REGISTER_ID::NULLID;

    registers_t[(UINT)REGISTER_ID::rVERSION] = {REGISTER_ID::rVERSION, rwConstant, rtString, 0,0,0, true};
    registers_t[(UINT)REGISTER_ID::rNAME] = {REGISTER_ID::rNAME,    rwConstant, rtString, 0,0,0, true};
	registers_t[(UINT)REGISTER_ID::rDATABASENUM] = {REGISTER_ID::rDATABASENUM, rwConstant, rtDec, 0,9999,0, true};
	registers_t[static_cast<UINT>(REGISTER_ID::rSETTINGSNUM)] = {REGISTER_ID::rSETTINGSNUM, rwConstant, rtDec, 0,99,0, false};

	for(UINT i = 0; i < (UINT)REGISTER_ID::COUNTREGISTERS; i++)
	{
		if (registers_t[i].id != REGISTER_ID::NULLID)
			registers_t[i].reg = new CRegister();
		else
			registers_t[i].reg = NULL;
	}
}

CConfiguration::~CConfiguration()
{
    for (UINT i = 0; i < patterns.size(); i++)
        delete patterns[i];
}

void CConfiguration::ProcessLogic()
{
	if (_isInitialized)
	{
		// Контейнерные каналы входов
		for(UINT i = 0; i < DriversIOManager::arincInCount; i++)
			if (_arincInIndex[i] != NULL)
				_arincInIndex[i]->UpdateHWToData();
		// Остальные каналы
		for(UINT i = 0; i < channelList.size(); i++)
		{
			if (IS_IN(channelList[i]->GetType()))
				channelList[i]->UpdateHWToData();
			else if (IS_OUT(channelList[i]->GetType()))
			{
				channelList[i]->UpdateHWToData();
				((IChannelOut*)channelList[i])->GetProcessor()->Convert();
				channelList[i]->UpdateDataToHW();
			}
		}
		// Контейнерные каналы выходов
		for(UINT i = 0; i < DriversIOManager::arincOutCount; i++)
			if (_arincOutIndex[i] != NULL)
				_arincOutIndex[i]->UpdateDataToHW();
	}
}
IO_GROUP_STATE CConfiguration::GetState()
{
	auto state = _workManager->GetDriversIO()->GetState();
	if (IsInitialized())
	{
		state.iostArincIn1Fail = _arincInIndex[0] != NULL && _arincInIndex[0]->IsDataExists() ? state.iostArincIn1Fail : 0;
		state.iostArincIn2Fail = _arincInIndex[1] != NULL && _arincInIndex[1]->IsDataExists() ? state.iostArincIn2Fail : 0;
		state.iostArincIn3Fail = _arincInIndex[2] != NULL && _arincInIndex[2]->IsDataExists() ? state.iostArincIn3Fail : 0;
		state.iostArincIn4Fail = _arincInIndex[3] != NULL && _arincInIndex[3]->IsDataExists() ? state.iostArincIn4Fail : 0;
		state.iostArincIn5Fail = _arincInIndex[4] != NULL && _arincInIndex[4]->IsDataExists() ? state.iostArincIn5Fail : 0;
		state.iostArincIn6Fail = _arincInIndex[5] != NULL && _arincInIndex[5]->IsDataExists() ? state.iostArincIn6Fail : 0;

		state.iostArincOut1Fail = _arincOutIndex[0] != NULL && _arincOutIndex[0]->IsDataExists() ? state.iostArincOut1Fail : 0;
		state.iostArincOut2Fail = _arincOutIndex[1] != NULL && _arincOutIndex[1]->IsDataExists() ? state.iostArincOut2Fail : 0;
		state.iostArincOut3Fail = _arincOutIndex[2] != NULL && _arincOutIndex[2]->IsDataExists() ? state.iostArincOut3Fail : 0;
		state.iostArincOut4Fail = _arincOutIndex[3] != NULL && _arincOutIndex[3]->IsDataExists() ? state.iostArincOut4Fail : 0;

		state.iostSync1Fail = (_syncUse & 1) ? state.iostSync1Fail : 0;
		state.iostSync2Fail = (_syncUse & 2) ? state.iostSync2Fail : 0;
		state.iostSync3Fail = (_syncUse & 4) ? state.iostSync3Fail : 0;
		state.iostSync4Fail = (_syncUse & 8) ? state.iostSync4Fail : 0;
		state.iostSync5Fail = (_syncUse & (16+128)) ? state.iostSync5Fail : 0;
		state.iostSync6Fail = (_syncUse & (32+256)) ? state.iostSync6Fail : 0;
		state.iostSync7Fail = (_syncUse & (64+512)) ? state.iostSync7Fail : 0;
	}
	return state;
}



CConfiguration::LOAD_RESULT CConfiguration::LoadConfig()
{
#ifdef _NO_COMPILIER
    //manager.utils.GetTerminal().WriteLine("Загрузка конфигурации не поддерживается, загрузите соответствующее ПО");
    //manager.drivers.GetComMod().SetErrorLoad();
#else
	LOAD_RESULT lresult = LOAD_RESULT::Error;
	CParserConfig* parserC = new CParserConfig(this, _workManager->GetConsole(), _workManager->GetCommod(), _workManager->GetDriversIO());
	int result = parserC->LoadConfiguration();
	delete parserC;
	for(UINT i = 0; i < patterns.size(); i++)
	{
		if (!GetPattern(i)->IsConstant())
		{
			CParserDevice* parser = new CParserDevice(GetPattern(i)->GetDeviceNum(), this, *patterns[i], _workManager->GetConsole(), _workManager->GetCommod());
			result += parser->LoadConfiguration();
			delete parser;
		}
		Gpio::SVSet();
	}
	if (result == 0)
	{
		// Проверка логики КМ
		{
			// Проверка совместимости
			// Проверка табличных выходных аринк
			// Проверка использования функций

		}

		for(UINT i = 0; i < patterns.size(); i++)
        {
			_workManager->GetConsole()->Format("Инициализация устройства %s(%d)\r",
                GetPattern(i)->GetRegister(REGISTER_ID::rNAME)->GetValueStr(), GetPattern(i)->GetDeviceNum());
			GetPattern(i)->InitDevice();
            Gpio::SVSet();
        }
		_workManager->GetConsole()->WriteLine("Инициализация устройств завершена                                         ");
		for(UINT i = 0; i < patterns.size(); i++)
		{
			_workManager->GetConsole()->Format("Подготовка исполняемой части устройства %d\r", i);
			patterns[i]->CompileDevice();
			Gpio::SVSet();
		}
		_workManager->GetConsole()->WriteLine("Подготовка исполняемой части устройств успешна                            ");


		//_workManager->GetConsole()->Write("Конфигурация для быстрого старта подготавливается...\r");
		//manager.utils.GetArchive().Save();
		//_workManager->GetConsole()->WriteLine("Конфигурация для быстрого старта подготовлена        ");
		lresult = LOAD_RESULT::OK;
	}
	if (lresult == LOAD_RESULT::OK)
	{
		Instance(); // Преобразование номеров в указатели на входы
		PrepareLogicSequence(); // Подготовка последовательности расчета
		CreateIndexDeviceNum(); // Создание индекса по номерам устройств
		DefineChannelFailIndex();
	}
	_isInitialized = lresult == LOAD_RESULT::OK;
    return lresult;
#endif
}

void CConfiguration::CreateIndexDeviceNum()
{
	memset(deviceNumIndex, 0, sizeof(deviceNumIndex));
	for(UINT i = 0; i < patterns.size(); i++)
		deviceNumIndex[patterns[i]->GetDeviceNum()] = patterns[i];
}

int CConfiguration::IndexOfChannelList(std::vector<IChannel*>& chs, IChannel* ch)
{
	for(UINT i = 0; i < chs.size(); i++)
	{
		if (chs[i] == ch)
			return i;
	}
	return -1;
}

void CConfiguration::AddChannelToList(IChannel* ch)
{
	for(UINT i = 0; i < channelList.size(); i++)
		if (channelList[i] == ch)
			return;
	Console::TraceLine("%d, %s, %d", ch->GetPatternIndex(), CParser::GetTypeStr(ch->GetType()), ch->GetNumber());
	channelList.push_back(ch);

}

void CConfiguration::ScanChannel(std::vector<IChannel*>& scanningChannels, IChannelOut* ch)
{
	if (ch != NULL && IndexOfChannelList(channelList, ch) == -1 && IndexOfChannelList(scanningChannels, ch) == -1)
	{
		scanningChannels.push_back(ch);
		std::vector<IChannel*> chs;
		ch->GetProcessor()->GetDependentChannels(chs);
		for(UINT i = 0; i < chs.size(); i++)
		{
			if (IS_OUT(chs[i]->GetType()))
			{
				ScanChannel(scanningChannels, (IChannelOut*)chs[i]);
			}
			else if (chs[i]->GetType() == ioLogicalIn)
			{
				BYTE ptn;
				IOTYPES type;
				BYTE num;
				int res = GetConnection(chs[i]->GetPatternIndex(), chs[i]->GetType(), chs[i]->GetNumber(), ptn, type, num);
				if (res >= 0)
				{
					//Console::TraceLine(">> %d", ptn);
					ScanChannel(scanningChannels, patterns[ptn]->GetOutput(type, num));
					AddChannelToList(chs[i]);
					//Console::TraceLine("%d, %s, %d", chs[i]->GetPatternIndex(), CParser::GetTypeStr(chs[i]->GetType()), chs[i]->GetNumber());
					//Console::TraceLine("<< %d", ptn);
				}
			}
			else
			{
				AddChannelToList(chs[i]);
				//Console::TraceLine("%d, %s, %d", chs[i]->GetPatternIndex(), CParser::GetTypeStr(chs[i]->GetType()), chs[i]->GetNumber());
			}
		}
		AddChannelToList(ch);
		//Console::TraceLine("%d, %s, %d", ch->GetPatternIndex(), CParser::GetTypeStr(ch->GetType()), ch->GetNumber());
		scanningChannels.erase(scanningChannels.begin()+IndexOfChannelList(scanningChannels, ch));
	}
}

void CConfiguration::PrepareLogicSequence()
{
	std::vector<IChannel*> scanningChannels;
	for(UINT i = 0; i < patterns.size(); i++)
	{
		for(UINT o = 0; o < patterns[i]->outs.size(); o++)
		{
			ScanChannel(scanningChannels, patterns[i]->outs[o]);
		}
	}
}

UINT CConfiguration::AddPattern(bool constant)
{
	patterns.push_back(new CPattern(_workManager->GetDriversIO(), this, (UINT)patterns.size(), constant));
	return (UINT)(patterns.size() - 1);
}

int CConfiguration::AddConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE dp, IOTYPES dt, BYTE dn)
{
	PIN src;
	PIN dst;
	src.num = sn;
	src.pat = sp;
	src.type = st;
	dst.num = dn;
	dst.pat = dp;
	dst.type = dt;
	CONNECTION c;
	c.dst = dst;
	c.src = src;
	if ((IndexOfPin(src) == -1) && (IndexOfPin(dst) == -1))
		connections.push_back(c);
	else
		return -1;
	return 0;
}

int CConfiguration::IndexOfPin(PIN p)
{
	if (IS_OUT(p.type))
	{
		for (UINT i = 0; i < connections.size(); i++)
			if (p.num == connections[i].src.num &&
				p.pat == connections[i].src.pat &&
				p.type == connections[i].src.type)
					return i;
	}
	else if (IS_IN(p.type))
	{
		for (UINT i = 0; i < connections.size(); i++)
			if (p.num == connections[i].dst.num &&
				p.pat == connections[i].dst.pat &&
				((p.type == connections[i].dst.type) || (IS_LOGICAL(p.type) && IS_LOGICAL(connections[i].dst.type))))
					return i;
	}
	return -1;
}

int CConfiguration::TestConnection(BYTE sp, IOTYPES st, BYTE sn)
{
	PIN pin;
	pin.num = sn;
	pin.pat = sp;
	pin.type = st;
	return IndexOfPin(pin);
}

int CConfiguration::GetConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE &dp, IOTYPES &dt, BYTE &dn)
{
	PIN pin;
	pin.num = sn;
	pin.pat = sp;
	pin.type = st;
	int res = IndexOfPin(pin);
	if (res == -1)
		return -1;
	if (IS_OUT(st))
	{
		dp = connections[res].dst.pat;
		dt = connections[res].dst.type;
		dn = connections[res].dst.num;
	} else
	{
		dp = connections[res].src.pat;
		dt = connections[res].src.type;
		dn = connections[res].src.num;
	}
    return res;
}

BYTE CConfiguration::GetChannelHWNum(IChannel* ch)
{
	if (ch->GetType() == ioArincWordIn)
		return ((ChArincWordIn*)ch)->GetChannel()->GetNumber();
	else if (ch->GetType() == ioArincWordOut)
		return ((ChArincWordOut*)ch)->GetChannel()->GetNumber();
	else
	{
		BYTE chNum;
		BYTE ptNum;
		IOTYPES type;
		if (GetConnection(ch->GetPatternIndex(), ch->GetType(), ch->GetNumber(), ptNum, type, chNum) == -1)
			return 0xFF;
		else
			return chNum;
	}
}

void CConfiguration::Instance()
{
	for(UINT i = 0; i < patterns.size(); i++)
	{
		if (patterns[i]->IsConstant())
			constPatterns.InstanceDevice(i);
		else
			patterns[i]->InstanceDevice();
	}
}

void CConfiguration::DefineChannelFailIndex()
{
	_syncUse = 0;
	for(UINT i = 0; i < DriversIOManager::analogInCount; i++)
	{
		 if (_workManager->GetDriversIO()->GetAnalogIn(i)->GetSyncNum() != 0)
			 _syncUse = _syncUse | (1 << (_workManager->GetDriversIO()->GetAnalogIn(i)->GetSyncNum() - 1));
	}
	for(UINT i = 0; i < DriversIOManager::SCTOutCount; i++)
	{
		_arincInIndex[i] = NULL;
		for (UINT c = 0; c < connections.size(); c++)
			if (connections[c].dst.num == i &&
				connections[c].dst.pat == 0xFF &&
				connections[c].dst.type == ioSCT)
			{
				_syncUse = _syncUse | (1 << ((connections[c].src.num) + 4));
				break;
			}
	}

	if (_arincInIndex != NULL)
		delete[] _arincInIndex;
	if (_arincOutIndex != NULL)
		delete[] _arincOutIndex;
	_arincInIndex = new ChArincIn*[DriversIOManager::arincInCount];
	_arincOutIndex = new ChArincOut*[DriversIOManager::arincOutCount];

	for(UINT i = 0; i < DriversIOManager::arincInCount; i++)
	{
		_arincInIndex[i] = NULL;
		for (UINT c = 0; c < connections.size(); c++)
			if (connections[c].src.num == i &&
				connections[c].src.pat == 0xFF &&
				connections[c].src.type == ioDigitalIn)
			{
				_arincInIndex[i] = (ChArincIn*)GetPattern(connections[c].dst.pat)->GetInput(ioDigitalIn, connections[c].dst.num);
				break;
			}
	}
	for(UINT i = 0; i < DriversIOManager::arincOutCount; i++)
	{
		_arincOutIndex[i] = NULL;
		for (UINT c = 0; c < connections.size(); c++)
			if (connections[c].dst.num == i &&
				connections[c].dst.pat == 0xFF &&
				connections[c].dst.type == ioDigitalOut)
			{
				_arincOutIndex[i] = (ChArincOut*)GetPattern(connections[c].src.pat)->GetOutput(ioDigitalOut, connections[c].src.num);
				break;
			}
	}
}

IChannel* CConfiguration::GetArincIn(UINT number)
{
	return number < DriversIOManager::arincInCount ? (IChannel*)_arincInIndex[number] : NULL;
}

IChannel* CConfiguration::GetArincOut(UINT number)
{
	return number < DriversIOManager::arincOutCount? (IChannel*)_arincOutIndex[number] : NULL;
}


