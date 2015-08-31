#ifndef APPLICATION_CCONFIGURATION_H_HEADER_INCLUDED_B8BBE068
#define APPLICATION_CCONFIGURATION_H_HEADER_INCLUDED_B8BBE068

#include "../types.h"
#include <vector>
#include "cconstpatterns.h"

class WorkManager;
class CPattern;
class CProcessor;
class IChannelOut;
class ChArincOut;
class ChArincIn;

class CConfiguration
{
public:

	enum class LOAD_RESULT
	{
		OK,
		Error
	};

	CConfiguration(WorkManager* workManager);
	virtual ~CConfiguration();

	LOAD_RESULT LoadConfig();
	void ProcessLogic();

	CPattern* GetPattern(BYTE index)	{	return patterns[index]; }
	CPattern* GetPatternByDeviceNum(BYTE deviceNum)	{	return deviceNumIndex[deviceNum]; }
	UINT GetPatternCount() const {	return patterns.size(); }
	UINT AddPattern(bool constant = false);
	CConstPatternEngine& GetConstPatterns()	{	return constPatterns; }

	int AddConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE dp, IOTYPES dt, BYTE dn);

	int TestConnection(BYTE sp, IOTYPES st, BYTE sn);

	int GetConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE &dp, IOTYPES &dt, BYTE &dn);
	BYTE GetChannelHWNum(IChannel* ch);
	void Instance();
	// Возвращает указатель на регистр по ID
	CRegister* GetRegister(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? registers_t[(UINT)id].reg : NULL; }
	// Возвращает указатель на параметры регистров по ID
	const IChannel::REGISTER_TYPE* GetRegisterT(REGISTER_ID id) { return (id != REGISTER_ID::NULLID) ? &registers_t[(UINT)id] : NULL; }

	bool IsInitialized() { return _isInitialized; }
	// Подготовка последовательности расчета (поиск в грубину)
	 void PrepareLogicSequence();
	void AddChannelToList(IChannel* ch);
	IO_GROUP_STATE GetState();
	std::vector<IChannel*>& GetAllChannels() { return channelList; }
	IChannel* GetArincIn(UINT number);
	IChannel* GetArincOut(UINT number);


private:

	WorkManager* _workManager;

    struct PIN
    {
        BYTE pat;  // 0xFF Вход
        IOTYPES type;
        BYTE num;
    };

    struct CONNECTION
    {
        PIN src;
        PIN dst;
    };

    IChannel::REGISTER_TYPE registers_t[(UINT)REGISTER_ID::COUNTREGISTERS];

	int IndexOfPin(PIN p); // sd = 0 src, sd = 1 dst
	int IndexOfChannelList(std::vector<IChannel*>& chs, IChannel* ch);
	void ScanChannel(std::vector<IChannel*>& scanningChannels, IChannelOut* ch);
	void CreateIndexDeviceNum();
	void CreateChannelIndex();
	void DefineChannelFailIndex(); // Создание индекса для формирования структуры state


	std::vector<CPattern*> patterns;
	CPattern* deviceNumIndex[256];
	std::vector<CONNECTION> connections;
	CConstPatternEngine constPatterns;
	std::vector<IChannel*> channelList; // Последовательность каналов для запуска вычислительной последовательности

	ChArincIn** _arincInIndex;
	ChArincOut** _arincOutIndex;

    bool _isInitialized;
    UINT _syncUse; // Использование синхронизаций в конфигурации (для формирования отказов)

};



#endif /* APPLICATION_CCONFIGURATION_H_HEADER_INCLUDED_B8BBE068 */
