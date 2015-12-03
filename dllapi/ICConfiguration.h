#ifndef ICCONFIGURATION
#define  ICCONFIGURATION

//#include "../types.h"
struct CProcessor;
struct IChannelOut;


struct ICConfiguration
{
	enum class LOAD_RESULT
	{
		OK,
		Error
	};

	virtual ~ICConfiguration() {};
	virtual LOAD_RESULT LoadConfig() = 0;
	virtual void ProcessLogic() = 0;
	virtual CPattern* GetPattern(BYTE index) = 0;
	virtual CPattern* GetPatternByDeviceNum(BYTE deviceNum) const = 0;
	virtual UINT GetPatternCount()const  = 0;
	virtual UINT AddPattern(bool constant = false) = 0;
	virtual CConstPatternEngine& GetConstPatterns() = 0;
	virtual int AddConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE dp, IOTYPES dt, BYTE dn) = 0;
	virtual int TestConnection(BYTE sp, IOTYPES st, BYTE sn) = 0;
	virtual int GetConnection(BYTE sp, IOTYPES st, BYTE sn, BYTE &dp, IOTYPES &dt, BYTE &dn) = 0;
	virtual UINT GetChannelHWNum(IChannel* ch) = 0;
	virtual void Instance() = 0;
	virtual CRegister* GetRegister(REGISTER_ID id) = 0;
	virtual const IChannel::REGISTER_TYPE* GetRegisterT(REGISTER_ID id) = 0;
	virtual bool IsInitialized() const = 0;
	virtual void PrepareLogicSequence() = 0;
	virtual void AddChannelToList(IChannel* ch) = 0;
	virtual IO_GROUP_STATE GetState() = 0;
	virtual std::vector<IChannel*>& GetAllChannels() = 0;
	virtual IChannel* GetArincIn(UINT number) = 0;
	virtual IChannel* GetArincOut(UINT number) = 0;
	virtual IChannel* GetCommandOut(UINT number) = 0;
}; 
#endif

