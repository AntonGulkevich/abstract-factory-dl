#include "cprocessor.h"

#include "../application/cpattern.h"

#include "../utilities/console.h"
#include "../utilities/m_math.h"
#include "../compilier/CParser.h"

//#define _TRACE_CHANNEL
//#define COMPILIER_TRACE

#define ftoi__ (int)ftou__

#ifdef COMPILIER_TRACE
char* ActionName(UINT act)
{
   const char* name;
   switch((CProcessor::ACTIONS)act)
   {
        case CProcessor::acAdd:
            name = "Add";
            break;
        case CProcessor::acSub:
            name = "Sub";
            break;
        case CProcessor::acMul:
            name = "Mul";
            break;        
        case CProcessor::acIntMul:
            name = "IntMul";
            break;        
        case CProcessor::acDiv:
            name = "Div";
            break;
        case CProcessor::acMinus:
            name = "Minus";
            break;        
        case CProcessor::acNot:
            name = "Not";
            break;
        case CProcessor::acGtEq:
            name = "GtEq";
            break;
        case CProcessor::acGt:
            name = "Gt";
            break;
        case CProcessor::acLtEq:
            name = "LtEq";
            break;
        case CProcessor::acLt:
            name = "Lt";
            break;
        case CProcessor::acEqEq:
            name = "EqEq";
            break;
        case CProcessor::acNotEq:
            name = "NotEq";
            break;
        case CProcessor::acAmp:
            name = "Amp";
            break;
        case CProcessor::acCaret:
            name = "Caret";
            break;
        case CProcessor::acPipe:
            name = "Pipe";
            break;
        case CProcessor::acAmpAmp:
            name = "AmpAmp";
            break;
        case CProcessor::acPipePipe:
            name = "PipePipe";
            break;
        case CProcessor::acAssign:
            name = "Assign";
            break;        
        case CProcessor::acAssignConst:
            name = "AssignConst";
            break;        
        case CProcessor::acAssignRegister:
            name = "AssignRegister";
            break;        
        case CProcessor::acAssignRegisterConst:
            name = "AssignRegisterConst";
            break;        
        case CProcessor::acAddStack:
            name = "AddStack";
            break;        
        case CProcessor::acJz:
            name = "Jz";
            break;        
        case CProcessor::acJmp:
            name = "Jmp";
            break;        
        case CProcessor::acFunction:
            name = "Function";
            break;        
        case CProcessor::acReadRegister:
            name = "ReadRegister";
            break;        
        case CProcessor::acReadRegisterLabel:
            name = "ReadRegisterLabel";
            break;        
        case CProcessor::acReadSelfRegister:
            name = "ReadSelfRegister";
            break;        
        case CProcessor::acLoadBinParamVarVar:
            name = "LoadBinParamVarVar";
            break;                
        case CProcessor::acLoadBinParamVarConst:
            name = "LoadBinParamVarConst";
            break;                
        case CProcessor::acLoadBinParamVarStack:
            name = "LoadBinParamVarStack";
            break;                
        case CProcessor::acLoadBinParamConstVar:
            name = "LoadBinParamConstVar";
            break;                
        case CProcessor::acLoadBinParamConstConst:
            name = "LoadBinParamConstConst";
            break;                
        case CProcessor::acLoadBinParamConstStack:
            name = "LoadBinParamConstStack";
            break;                
        case CProcessor::acLoadBinParamStackVar:
            name = "LoadBinParamStackVar";
            break;                
        case CProcessor::acLoadBinParamStackConst:
            name = "LoadBinParamStackConst";
            break;                
        case CProcessor::acLoadBinParamStackStack:
            name = "LoadBinParamStackStack";
            break;                
        case CProcessor::acLoadParamStack:
            name = "LoadParamStack";
            break;                
        case CProcessor::acLoadParamVar:
            name = "LoadParamVar";
            break;                
        case CProcessor::acSaveParamStack:
            name = "SaveParamStack";
            break;                
        case CProcessor::acSaveParamVar:
            name = "SaveParamVar";
            break;                
        case CProcessor::acAddStackVar:
            name = "AddStackVar";
            break;                
        case CProcessor::acAddStackConst:
            name = "AddStackConst";
            break;                
        default:
            name = "Неопознано";
            break;                        
    };
    return (char*)name;
}    
#endif    


CProcessor::CProcessor(IChannelOut* channel)
{
	_channel = channel;
	exprSize = 0;
    expr = NULL;
    actionPtr = 0;
    calculated = false;
	constant = false;
	cleared = false;
}


CProcessor::~CProcessor()
{ 
	if (expr) 
	{
		delete[] expr;
		expr = NULL;
	}
}


void CProcessor::Init()
{
	if (exprSize > 0 && exprSize < 1000)
		expr = new UCU_FLOAT[exprSize];
	else
		expr = NULL;
	exprPtr = 0;
}

void CProcessor::Instance()
{
	for(actionPtr = 0; actionPtr < actionsTr.size(); actionPtr++)
	{
		switch(actionsTr[actionPtr].act)
		{
		case acAssignRegisterConst:
		case acAssignRegister:
		{
			actionsTr[actionPtr].ptrs.ptr2 = reinterpret_cast<DWORD>(_channel->GetRegister((REGISTER_ID)actionsTr[actionPtr].ptrs.ptr2));
		}
			break;
		case acReadSelfRegister:
		case acReadRegister:
		{
			IChannel* io = NULL;
			IOTYPES ct = (IOTYPES)((actionsTr[actionPtr].ptrs.ptr1 >> 8) & 0xFF);
			BYTE cn = (IOTYPES)((actionsTr[actionPtr].ptrs.ptr1) & 0xFF);
			io = _channel->GetPatternChannel(ct, cn);
			if (io!=NULL)
			{
				actionsTr[actionPtr].ptrs.ptr1 = reinterpret_cast<DWORD>(io);
				actionsTr[actionPtr].ptrs.ptr2 = reinterpret_cast<DWORD>(io->GetRegister((REGISTER_ID)actionsTr[actionPtr].ptrs.ptr2));
			} else
			{
                Console::TraceLine("Ошибка: Неудалось связь с выходом (%s, %d) %X", CParser::GetTypeStr(ct), cn, actionsTr[actionPtr].ptrs.ptr1 >> 16);
				throw;
			}
		}
			break;
		case acReadSelfRegisterLabel:
        case acReadRegisterLabel:
		{
			IChannel* io = NULL;
			IOTYPES ct = (IOTYPES)((actionsTr[actionPtr].ptrs.ptr1 >> 8) & 0xFF);
			BYTE cn = (IOTYPES)((actionsTr[actionPtr].ptrs.ptr1) & 0xFF);
			io = _channel->GetPatternChannelLabel(ct, cn, (BYTE)(actionsTr[actionPtr].ptrs.ptr1 >> 16), (BYTE)(actionsTr[actionPtr].ptrs.ptr1 >> 24));
			if (io!=NULL)
			{
				if (actionsTr[actionPtr].act == acReadSelfRegisterLabel)
					actionsTr[actionPtr].act = acReadSelfRegister;
				else
					actionsTr[actionPtr].act = acReadRegister;
				actionsTr[actionPtr].ptrs.ptr1 = reinterpret_cast<DWORD>(io);
				actionsTr[actionPtr].ptrs.ptr2 = reinterpret_cast<DWORD>(io->GetRegister((REGISTER_ID)actionsTr[actionPtr].ptrs.ptr2));
			} else
                Console::TraceLine("Ошибка: Неудалось найти связь с меткой (%s, %d) %X", CParser::GetTypeStr(ct), cn, actionsTr[actionPtr].ptrs.ptr1 >> 16);
								
		}
			break;
		default:
			break;
		}
	}
	cleared = false;
	//TRACEUCU("%s, %d, количество %d, %s\r\n", outNum, CParser::GetTypeStr(outType), actionsTr.size(), (cleared ? "Очищен" : ""));
}

void CProcessor::Convert()
{
	if (cleared)
		return;
    UCU_FLOAT reg1 = 0;  // Регистр виртуального процессора
    UCU_FLOAT reg2 = 0;  // Регистр виртуального процессора
    UCU_FLOAT reg3 = 0;  // Регистр виртуального процессора

	//TRACEUCU("%d, %s, %d, %s\r\n", outNum, CParser::GetTypeStr(outType), patternNum, (constant ? "Константный" : ""));
    if (!constant)
	{
		if (expr == NULL)
			return;
		exprPtr = 0;
		actionPtr = 0;
		for (UINT i = 0; i < varsV.size(); i++)
			varsV[i] = 1.0E-10f;
		for(actionPtr = 0; actionPtr < actionsTr.size(); actionPtr++)
        {
            ACTION_TRIADA id = actionsTr[actionPtr];
            switch(id.act)
            {
            case acAdd:
                reg3 = reg1 + reg2;
                break;
            case acSub:
                reg3 = reg1 - reg2;
                break;
            case acMul:
                reg3 = reg1 * reg2;
                break;
            case acIntMul:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) % ftoi__(reg2));
                break;
            case acDiv:
                reg3 = reg1 / reg2;
                break;
            case acMinus:
                reg3 = -reg1;
                break;
            case acNot:
                reg3 = (UCU_FLOAT)(!ftoi__(reg1));
                break;
            case acGtEq:
                reg3 = reg1 >= reg2;
                break;
            case acGt:
                reg3 = reg1 > reg2;
                break;
            case acLtEq:
                reg3 = reg1 <= reg2;
                break;
            case acLt:
                reg3 = reg1 < reg2;
                break;
            case acEqEq:
                reg3 = reg1 == reg2;
                break;
            case acNotEq:
                reg3 = reg1 != reg2;
                break;
            case acAmp:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) & ftoi__(reg2));
                break;
            case acCaret:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) ^ ftoi__(reg2));
                break;
            case acPipe:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) | ftoi__(reg2));
                break;
            case acAmpAmp:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) && ftoi__(reg2));
                break;
            case acPipePipe:
                reg3 = (UCU_FLOAT)(ftoi__(reg1) || ftoi__(reg2));
                break;
            case acAssign:
                varsV[ftoi__(id.param1)] = reg3;
                break;
            case acAssignConst:
                varsV[ftoi__(id.param1)] = id.param2;
                break;
            case acAssignRegister:
                (reinterpret_cast<CRegister*>(id.ptrs.ptr2))->SetValue(reg3);
                break;
            case acAssignRegisterConst:
                (reinterpret_cast<CRegister*>(id.ptrs.ptr2))->SetValue(id.param1);
                break;
            case acAddStack:
                break;
            case acJz:
                if (reg3 == 0)
                    actionPtr = ftoi__(id.param1) - 1;
                break;
            case acJmp:
                actionPtr = ftoi__(id.param1) - 1;
                break;
                
                ///// Начало выполнения фунций /////////////////////////////////
            case acFunction:
            {
            #ifdef _TRACE_CHANNEL           
                char text[50];
            #endif
                // Получение параметров функций из стека
                UCU_FLOAT funcParams[10];
                for(int i = ftoi__(id.param2 - 1); (i >= 0 && exprPtr); i--)
                {
                    funcParams[i] = exprPop();
                }
                switch((FUNCTION_ID)(ftou__(id.param1)))
                {
                case fLINEAR:  // 3 параметра
                {
                	// funcParams[0] - значение
                	// funcParams[1] - тип входа
                	// funcParams[2] - номер входа
                	auto io = _channel->GetPatternChannel((IOTYPES)ftou__(funcParams[1]), (UINT)funcParams[2]);
                    if (io != NULL && io->IsHasCalibration())
                        reg3 = io->GetChCalibration()->GetValueByKoeff((float)funcParams[0]);
                    else
                        reg3 = 0;
                }
                    break;
                case fRESETFILTR:
                {
                	// funcParams[0] - тип входа
                	// funcParams[1] - номер входа
                	auto io = _channel->GetPatternChannel((IOTYPES)ftou__(funcParams[1]), (UINT)funcParams[0]);
                    if (io != NULL && io->IsValueFiltered())
                        io->GetChFilter()->ResetFiltr();
                    reg3 = 0;
                }
                    break;
				case fISFILTRREADY:
					{
	                	auto io = _channel;
	                    if (io != NULL && io->IsValueFiltered())
	                    	reg3 = io->GetChFilter()->IsFilterReady();
					}
					break;
        		case fRESETRATE:
        		{
                	auto io = _channel;
                    if (io != NULL && io->IsRateCalculating())
                        io->GetChRate()->ResetRate();
                    reg3 = 0;
        		}
                    break;
        		case fRESETRATECHANNEL:  // 2 параметра
        		{
                	auto io = _channel->GetPatternChannel((IOTYPES)ftou__(funcParams[1]), (UINT)funcParams[0]);
                    if (io != NULL && io->IsRateCalculating())
                        io->GetChRate()->ResetRate();
                    reg3 = 0;
        		}
        			break;
                case fISRATEREADY:
				{
                	auto io = _channel;
                    if (io != NULL && io->IsRateCalculating())
                    	reg3 = io->GetChRate()->IsRateReady();
				}
                    break;
/*
                case fAERODYNAMICCORR:
                    if (outType != ioLogicalOut)
                    {
                        reg3 = 0;
                        break;
                    }
                    IO_CACHE ioc;
                    ioc.index = ((patternNum & 0xFF) << 16) + ((BYTE)outType << 8) + outNum;
                    ioc.io = IndexOfIO(ioc.index);
                    if (ioc.io == NULL)
                    {
                        if (IS_IN(outType))
                            ioc.io = manager.GetConfig().GetSrcByConnection(patternNum, outType, outNum);
                        else if (IS_OUT(outType))
                            ioc.io = manager.GetConfig().GetDstByConnection(patternNum, outType, outNum);
                        if (ioc.io != NULL)
                            ios.push_back(ioc);
                    }
                    if (ioc.io!=NULL)
                        reg3 = reinterpret_cast<CLogicalOut*>(ioc.io)->AeroDynamicCorr(funcParams[0], funcParams[1]);
                    break;
                    */
                case fTOBNR:  // 3 параметра
                    {
                    	// 1 - упаковываемое значение
                    	// 2 - количество значащих бит
                    	// 3 - Диапазон
                        reg3 = (UCU_FLOAT)ToBNR(funcParams[0], ftou__(funcParams[1]), funcParams[2]);				
        			}
                    break;
                case fFROMBNR: // 3 параметра
                    {
                    	// 1 - распаковываемое значение
                    	// 2 - количество значащих бит
                    	// 3 - Диапазон

                        reg3 = FromBNR(ftoi__(funcParams[0]), ftou__(funcParams[1]), funcParams[2]);
                     // Вставить проверку корректности параметров        
                    }
                    break;
                case fTORTM:  // 3 параметра
                    {
                    	// 1 - упаковываемое значение
                    	// 2 - количество значащих бит
                    	// 3 - Диапазон

                        reg3 = (UCU_FLOAT)ToRTM(funcParams[0], ftou__(funcParams[1]), funcParams[2]);				
                    }
                    break;
                case fFROMRTM: // 3 параметра
                    {
                    	// 1 - распаковываемое значение
                    	// 2 - количество значащих бит
                    	// 3 - Диапазон
                         reg3 = FromRTM(ftoi__(funcParams[0]), ftou__(funcParams[1]), funcParams[2]);
                     // Вставить проверку корректности параметров        
                    }
                    break;
                case fTOBCD:
                	// 1 - Значение
                	// 2 - Заполнитель затемнения
                	reg3 = ToBCD(ftoi__(funcParams[0]), ftou__(funcParams[1]));
                    break;
                case fFROMBCD:
                    break;
                case fSHOWALARM: // 3 параметра
                    {
                    	auto io = _channel->GetPatternChannel((IOTYPES)ftou__(funcParams[1]), (UINT)funcParams[2]);
						if (io != NULL)
							io->SetCheckAlarmCode(ftou__(funcParams[0]));
						reg3 = 0;
                    }
                    break;
                case fSETBIT:
                    reg3 = (UCU_FLOAT)SetBit(ftou__(funcParams[0]), ftou__(funcParams[1]), ftou__(funcParams[2]));
                    break;
                case fGETBIT:
                    reg3 = (UCU_FLOAT)GetBit(ftou__(funcParams[0]), ftou__(funcParams[1]));
                    break;
                case fSETFIELD:
                    reg3 = (UCU_FLOAT)SetField(ftou__(funcParams[0]), ftou__(funcParams[1]), ftou__(funcParams[2]), ftou__(funcParams[3]));
                    break;
                case fGETFIELD:
                    reg3 = (UCU_FLOAT)GetField(ftou__(funcParams[0]), ftou__(funcParams[1]), ftou__(funcParams[2]));
                    break;
                case fGETUCUSTATE:
                    reg3 = (UCU_FLOAT)_channel->GetPattern()->GetConfig()->GetState().dword;
                    break;
                case fSUPPRESSALARM:
                    {
                    	auto io = _channel->GetPatternChannel((IOTYPES)ftou__(funcParams[1]), ftou__(funcParams[2]));
						if (io != NULL)
							io->SetSuppressAlarm((funcParams[0] != 0));
                    reg3 = 0;
                    }
				case fSIN:
					reg3 = (UCU_FLOAT)m_sin(funcParams[0]);
					break;
				case fCOS:
					reg3 = (UCU_FLOAT)m_cos(funcParams[0]);
					break;
				case fTOBNRS: // 1 паарметр
					// funcParams[0] - упаковываемое значение
					if (_channel->GetType() == ioArincWordOut)
					{													
						reg3 = ToBNR(funcParams[0], _channel->GetRegister(REGISTER_ID::rBITSCOUNT)->GetValueUInt(), _channel->GetRegister(REGISTER_ID::rRANGE)->GetValueFloat());
					} else
						reg3 = 0;
					break;
				case fFROMBNRS:
					{
						auto io = _channel->GetPatternChannelLabel(ioDigitalIn, ftou__(funcParams[0]), REVERSEOCT(ftou__(funcParams[1])), 0); // TODO добавить ID
						if (io != NULL)
						{
							reg3 = FromBNR(ftou__(io->GetRegister(REGISTER_ID::rVALUE)->GetValueFloat()), io->GetRegister(REGISTER_ID::rBITSCOUNT)->GetValueUInt(), io->GetRegister(REGISTER_ID::rRANGE)->GetValueFloat());
						} else
							reg3 = 0;
					}
					break;
				case fTORTMS:
					if (_channel->GetType() == ioArincWordOut)
					{													
						reg3 = ToRTM(funcParams[0], _channel->GetRegister(REGISTER_ID::rBITSCOUNT)->GetValueUInt(), _channel->GetRegister(REGISTER_ID::rRANGE)->GetValueFloat());
					} else
						reg3 = 0;
					break; 
				case fFROMRTMS:
					{
						auto io = _channel->GetPatternChannelLabel(ioDigitalIn, ftou__(funcParams[0]), REVERSEOCT(ftou__(funcParams[1])), 0); // TODO добавить ID
						if (io != NULL)
						{
							reg3 = FromRTM(ftou__(io->GetRegister(REGISTER_ID::rVALUE)->GetValueFloat()), io->GetRegister(REGISTER_ID::rBITSCOUNT)->GetValueUInt(), io->GetRegister(REGISTER_ID::rRANGE)->GetValueFloat());
						} else
							reg3 = 0;
					}
					break;
				case fTRUNC:
					reg3 = truncf(funcParams[0]);
					break;
				case fFRAC:
					reg3 = funcParams[0] - truncf(funcParams[0]);
					break;
                default:
            #ifdef _TRACE_CHANNEL       
                    sprintf(text, "!!!! %d Неизвестная функция", paramsV[paramPtr]);
                    manager.utils.GetTerminal().WriteLine(text);
            #endif
                    break;
                } // Switch FUNCTION
                
            }    
                //DoFunction(id);
                /////////////////////// Конец функций ////////////////////////////////////////
                break;
            case acReadRegister:
                reg3 = (reinterpret_cast<CRegister*>(id.ptrs.ptr2))->GetValueUCUFloat();
                break;
            case acReadSelfRegister:
                reg3 = (reinterpret_cast<CRegister*>(id.ptrs.ptr2))->GetValueUCUFloat();
                break;
            case acLoadBinParamVarVar:
                reg1 = varsV[ftoi__(id.param1)];
                reg2 = varsV[ftoi__(id.param2)];
                break;
            case acLoadBinParamVarConst:
                reg1 = varsV[ftoi__(id.param1)];
                reg2 = id.param2;
                break;
            case acLoadBinParamVarStack:
                reg1 = varsV[ftoi__(id.param1)];
                reg2 = exprPop();
                break;
            case acLoadBinParamConstVar:
                reg1 = id.param1;
                reg2 = varsV[ftoi__(id.param2)];
                break;
            case acLoadBinParamConstConst:
                reg1 = id.param1;
                reg2 = id.param2;
                break;
            case acLoadBinParamConstStack:
                reg1 = id.param1;
                reg2 = exprPop();
                break;
            case acLoadBinParamStackVar:
                reg1 = exprPop();
                reg2 = varsV[ftoi__(id.param2)];
                break;
            case acLoadBinParamStackConst:
                reg1 = exprPop();
                reg2 = id.param2;
                break;
            case acLoadBinParamStackStack:
                reg2 = exprPop();
                reg1 = exprPop();
                break;
            case acLoadParamStack:
                reg3 = exprPop();
                break;
            case acLoadParamVar:
                reg3 = varsV[ftoi__(id.param1)];
                break;
            case acSaveParamStack:
                exprPush(reg3);
                break;
            case acSaveParamVar:
                varsV[ftoi__(id.param1)] = reg3;
                break;
            case acAddStackVar:
                exprPush(varsV[ftoi__(id.param1)]);
                break;
            case acAddStackConst:
                exprPush(id.param1);
                break;
            default:
                Console::TraceLine("Неизвесная команда процессора");
                throw;
            } // switch
        }
	} else
		;//manager.GetConfig().GetConstPatterns().DoPatternFunction(patternNum, outType, outNum);
    calculated = true;
}

void CProcessor::Clear()
{
    cleared = true;
	varsV.clear();
    exprPtr = 0;
#ifndef _NO_COMPILIER
    actionsV.clear();
    constsV.clear();
    paramsV.clear();
    labelsV.clear();
#endif    
}


#ifndef _NO_COMPILIER

void CProcessor::GetDependentChannels(std::vector<IChannel*>& channels)
{
	for(actionPtr = 0; actionPtr < actionsTr.size(); actionPtr++)
	{
		switch(actionsTr[actionPtr].act)
		{
		case acReadRegister:
        case acReadRegisterLabel:
        {
			IChannel* io = reinterpret_cast<IChannel*>(actionsTr[actionPtr].ptrs.ptr1);
			channels.push_back(io);
		}
			break;
		default:
			break;
		}
	}
}

void CProcessor::Analys()
{

}

void CProcessor::Compile()
{
	if (_channel->GetType() == ioCommand && _channel->GetNumber() == 2)
		Console::TraceLine("Command");

	if ((actionsV.size() == 0))
		return;
	exprPtr = 0;
	paramPtr = 0;
	actionPtr = 0;
	actionsTr.clear();
    UINT* actToAct = new UINT[actionsV.size()+1]; 
	for(actionPtr = 0; actionPtr < actionsV.size(); actionPtr++)
	{      
		actToAct[actionPtr] = actionsTr.size();
		ACTION_TRIADA acTr;
		switch((ACTIONS)actionsV[actionPtr])
		{
		case acMinus:  // Унарные операции
		case acNot:
#ifdef COMPILIER_TRACE
            Console::TraceLine("Матвыражение %s, %d, %d", ActionName(actionsV[actionPtr]), paramsV[paramPtr], paramsV[paramPtr+1]);
#endif            
			if (paramsV[paramPtr] == STACKPTR)
				acTr.act = acLoadBinParamStackConst;
			else if (paramsV[paramPtr] > 0)
			{
				acTr.act = acLoadBinParamVarConst;
				acTr.param1 = (float)(paramsV[paramPtr]-1);
			}
			else 
			{
				acTr.act = acLoadBinParamConstConst;
				acTr.param1 = constsV[-paramsV[paramPtr]-1];
			}
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back());
#endif                        
			acTr.act = (ACTIONS)actionsV[actionPtr];
            acTr.param1 = acTr.param2 = -1; 
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back());
#endif                        
			if (paramsV[paramPtr+1] == STACKPTR)
			{
				acTr.act = acSaveParamStack;
			} else
			{
				acTr.act = acSaveParamVar;
				acTr.param1 = (float)(paramsV[paramPtr+1]-1);
			}
			actionsTr.push_back(acTr);
			paramPtr += 2;
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back());
#endif                        
			break;
		case acAdd: // Бинарные операции
		case acSub:
		case acMul:
		case acIntMul:
		case acDiv:
		case acGtEq:
		case acGt:
		case acLtEq:
		case acLt:
		case acEqEq:
		case acNotEq:
		case acAmp:
		case acCaret:
		case acPipe:
		case acAmpAmp:
		case acPipePipe:
#ifdef COMPILIER_TRACE
        Console::Trace("Матвыражение %d, %d, %d, %d\r\n", actionsV[actionPtr], paramsV[paramPtr], paramsV[paramPtr+1], paramsV[paramPtr+2]);
#endif            
			if (paramsV[paramPtr] < 0)
			{
				acTr.param1 = constsV[-paramsV[paramPtr]-1];
				if (paramsV[paramPtr + 1] == STACKPTR)
					acTr.act = acLoadBinParamConstStack;
				else if (paramsV[paramPtr + 1] > 0)
				{
					acTr.act = acLoadBinParamConstVar;
					acTr.param2 = (float)(paramsV[paramPtr+1]-1);
				}
				else 
				{
					acTr.act = acLoadBinParamConstConst;
					acTr.param2 = constsV[-paramsV[paramPtr+1]-1];
				}
			} 
			else if (paramsV[paramPtr] == STACKPTR)
			{
				if (paramsV[paramPtr + 1] == STACKPTR)
					acTr.act = acLoadBinParamStackStack;
				else if (paramsV[paramPtr + 1] > 0)
				{
					acTr.act = acLoadBinParamStackVar;
					acTr.param2 = (float)(paramsV[paramPtr+1]-1);
				}
				else 
				{
					acTr.act = acLoadBinParamStackConst;
					acTr.param2 = constsV[-paramsV[paramPtr+1]-1];
				}
			} else
			{
				acTr.param1 = (float)(paramsV[paramPtr]-1);
				if (paramsV[paramPtr + 1] == STACKPTR)
					acTr.act = acLoadBinParamVarStack;
				else if (paramsV[paramPtr + 1] > 0)
				{
					acTr.act = acLoadBinParamVarVar;
					acTr.param2 = (float)(paramsV[paramPtr+1]-1);
				}
				else 
				{
					acTr.act = acLoadBinParamVarConst;
					acTr.param2 = constsV[-paramsV[paramPtr+1]-1];
				}
			}
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                        
			acTr.act = (ACTIONS)(actionsV[actionPtr]);
            acTr.param1 = acTr.param2 = -1;
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                        
			if (paramsV[paramPtr+2] == STACKPTR)
			{
				acTr.act = acSaveParamStack;
			} else
			{
				acTr.act = acSaveParamVar;
				acTr.param1 = (float)(paramsV[paramPtr+2]-1);
			}
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                        
			paramPtr += 3;
			break;
		case acAssign:
#ifdef COMPILIER_TRACE
			Console::TraceLine("Присвоение %d, %d", paramsV[paramPtr], paramsV[paramPtr+1]);
#endif            
			if (paramsV[paramPtr] == STACKPTR)
			{
				if (!actionsTr.empty() && (actionsTr.back().act == acSaveParamStack))
					actionsTr.pop_back();
				else
				{
					acTr.act = acLoadParamStack;
					actionsTr.push_back(acTr);                    
                    
				}
				acTr.act = acAssign;
				acTr.param1 = (float)(paramsV[paramPtr+1]-1);
				actionsTr.push_back(acTr);
			} 
			else if (paramsV[paramPtr] < 0)
			{
				acTr.act = acAssignConst;
				acTr.param1 = (float)(paramsV[paramPtr+1]-1);
				acTr.param2 = constsV[-paramsV[paramPtr]-1];
				actionsTr.push_back(acTr);
			}
			else 
			{
				acTr.act = acLoadParamVar;
				acTr.param1 = (float)(paramsV[paramPtr]-1);
				actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
				Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                        
				
				acTr.act = acAssign;
				acTr.param1 = (float)(paramsV[paramPtr+1]-1);
				actionsTr.push_back(acTr);
			}	
			paramPtr+=2;
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                   
			break;
		case acAssignRegister:
			{
#ifdef COMPILIER_TRACE
				Console::TraceLine("Присвоение регистра %d, %d", paramsV[paramPtr], paramsV[paramPtr+1]);
#endif            
            	acTr.act = acAssignRegister;
				if (paramsV[paramPtr] == STACKPTR)
				{
					if (!actionsTr.empty() && (actionsTr.back().act == acSaveParamStack))
						actionsTr.pop_back();
					else
					{
						acTr.act = acLoadParamStack;
						actionsTr.push_back(acTr);
					}
				}
				else if (paramsV[paramPtr] > 0)
				{
					acTr.act = acLoadParamVar;
					acTr.param1 = (float)(paramsV[paramPtr]-1);
					actionsTr.push_back(acTr);
				} else
				{
					acTr.act = acAssignRegisterConst;
					acTr.ptrs.ptr2 = paramsV[paramPtr+1];
					acTr.param1 = constsV[-paramsV[paramPtr] - 1];
					actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
					Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                                   
					paramPtr+=2;
					break;
				}
#ifdef COMPILIER_TRACE
				Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                           
				acTr.act = acAssignRegister;
				acTr.ptrs.ptr2 = paramsV[paramPtr+1];
				actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
				Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                            
				paramPtr+=2;
			}
			break;
		case acAddStack:
#ifdef COMPILIER_TRACE
			Console::TraceLine("Добавить в стек %d", paramsV[paramPtr]);
#endif            
        
			if (paramsV[paramPtr] > 0 && (paramsV[paramPtr] != STACKPTR))
			{
				acTr.act = acAddStackVar;
				acTr.param1 = (float)(paramsV[paramPtr]-1);
				actionsTr.push_back(acTr);
			}
			else if (paramsV[paramPtr] < 0)
			{
				acTr.act = acAddStackConst;
				acTr.param1 = constsV[-paramsV[paramPtr]-1];
				actionsTr.push_back(acTr);
			}			
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                            
			paramPtr++;
			break;
		case acJz:	
#ifdef COMPILIER_TRACE
			Console::TraceLine("Jz %d, %d", paramsV[paramPtr], paramsV[paramPtr+1]);
#endif 
			if (paramsV[paramPtr] < 0)
			{
				if (constsV[-paramsV[paramPtr]-1] == 0)
				{
					acTr.act = acJmp;
					acTr.param1 = (float)paramsV[paramPtr+1];
					actionsTr.push_back(acTr);
				}
				paramPtr+=2;
				break;
			} else if (paramsV[paramPtr] == STACKPTR)
			{
				if (!actionsTr.empty() && (actionsTr.back().act == acSaveParamStack))
					actionsTr.pop_back();
				else
				{
					acTr.act = acLoadParamStack;
					actionsTr.push_back(acTr);                           
				}
			} else
			{
				acTr.act = acLoadParamVar;
				acTr.param1 = (float)(paramsV[paramPtr]-1);
				actionsTr.push_back(acTr);
			}
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                                    
			acTr.act = acJz;
			acTr.param1 = (float)paramsV[paramPtr+1];
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                        
			paramPtr+=2;
			break;
		case acJmp:
#ifdef COMPILIER_TRACE
			Console::TraceLine("Jmp %d", paramsV[paramPtr]);
#endif                    
			acTr.act = acJmp;
			acTr.param1 = (float)paramsV[paramPtr];
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                                    
			paramPtr++;
			break;
		case acReadRegister:
			{
#ifdef COMPILIER_TRACE
				Console::TraceLine("Чтение регистра %d, %d, %d, %d", paramsV[paramPtr], paramsV[paramPtr+1], paramsV[paramPtr+2], paramsV[paramPtr+3]);
#endif            
               
				IOTYPES ct = (IOTYPES)paramsV[paramPtr];
				BYTE cn = paramsV[paramPtr + 1];
				if (_channel->GetPatternChannel(ct, cn) !=NULL)
				{
					acTr.ptrs.ptr1 = (ct << 8) + cn;
					acTr.ptrs.ptr2 = paramsV[paramPtr + 2];
					if (_channel->IsNumberType(ct, cn))
						acTr.act = acReadSelfRegister;
					else
						acTr.act = acReadRegister;
					actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
					Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                                            
					if (paramsV[paramPtr+3] == STACKPTR)
						acTr.act = acSaveParamStack;
					else
					{
						acTr.act = acSaveParamVar;
						acTr.param1 = (float)(paramsV[paramPtr+3]-1);
					}
					actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
					Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                        
                    
				} else
					Console::TraceLine("Ошибка: Неудалось найти связь с %s (%s, %d)", (IS_OUT(ct) ? "выходом" : "входом"), CParser::GetTypeStr(ct), cn);
				paramPtr += 4;
			}
			break;
		case acReadRegisterLabel:
			{
#ifdef COMPILIER_TRACE
				Console::TraceLine("Чтение регистра метки %d, %d, %d, %d", paramsV[paramPtr], paramsV[paramPtr+1], paramsV[paramPtr+2], paramsV[paramPtr+3], paramsV[paramPtr+4]);
#endif            
				IOTYPES ct = (IOTYPES)paramsV[paramPtr+4];
				BYTE cn = paramsV[paramPtr];
				auto io = _channel->GetPatternChannelLabel(ct, cn, (BYTE)(paramsV[paramPtr+1]), (BYTE)(paramsV[paramPtr+3]));
				if (io != NULL)
				{

					acTr.ptrs.ptr1 = (paramsV[paramPtr+3]<<24) + (paramsV[paramPtr+1] << 16) + (ct << 8) + cn;
					acTr.ptrs.ptr2 = paramsV[paramPtr + 2];
					if (ct == ioDigitalOut && _channel->IsNumberType(ioArincWordOut, io->GetNumber()))
						acTr.act = acReadSelfRegisterLabel;
					else
						acTr.act = acReadRegisterLabel;
					actionsTr.push_back(acTr);
	#ifdef COMPILIER_TRACE
					Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
	#endif                                                        
	                    
					if (paramsV[paramPtr+5] == STACKPTR)
						acTr.act = acSaveParamStack;
					else
					{
						acTr.act = acSaveParamVar;
						acTr.param1 = (float)(paramsV[paramPtr+5]-1);
					}
					actionsTr.push_back(acTr);
	#ifdef COMPILIER_TRACE
					Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
	#endif                                                        
				} else
					Console::TraceLine("Ошбика: Неудалось связь с меткой %s (%s, %d, %o)", (IS_OUT(ct) ? "выходом" : "входом"), CParser::GetTypeStr(ct), cn, REVERSE(paramsV[paramPtr+3]));

				paramPtr += 6;
			}
			break;
		case acFunction:
#ifdef COMPILIER_TRACE
			Console::TraceLine("Функция %d, %d, %d", paramsV[paramPtr], paramsV[paramPtr+1], paramsV[paramPtr+2]);
#endif            
			acTr.act = (ACTIONS)actionsV[actionPtr];
			acTr.param1 = (float)paramsV[paramPtr];
			acTr.param2 = (float)paramsV[paramPtr+1];
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                        
            
			if (paramsV[paramPtr+2] == STACKPTR)
				acTr.act = acSaveParamStack;
			else
			{
				acTr.act = acSaveParamVar;
				acTr.param1 = (float)(paramsV[paramPtr+2]-1);
			}
			actionsTr.push_back(acTr);
#ifdef COMPILIER_TRACE
			Console::TraceLine("%d %s, %g, %g, %08X, %08X", actionsTr.size(), ActionName(actionsTr.back().act), actionsTr.back().param1, actionsTr.back().param2, &actionsTr.back(), &actionsTr[0]);
#endif                                                        
            
			paramPtr += 3;

			break;
		default:
			Console::TraceLine("Непонятная команда %d, %d", actionPtr, paramPtr);
			throw;          
		}
	}
	// Добавление команды end   
	actToAct[actionsV.size()] = actionsTr.size()+1;   
    // Сопоставление меток и вычисление стека
	WORD stackSize = 0;
	WORD maxStackSize = 0;
	for(actionPtr = 0; actionPtr < actionsTr.size(); actionPtr++)
    {
#ifdef COMPILIER_TRACE
   const char* name;
   switch(actionsTr[actionPtr].act)
   {
        case acAdd:
            name = "Add";
            break;
        case acSub:
            name = "Sub";
            break;
        case acMul:
            name = "Mul";
            break;        
        case acIntMul:
            name = "IntMul";
            break;        
        case acDiv:
            name = "Div";
            break;
        case acMinus:
            name = "Minus";
            break;        
        case acNot:
            name = "Not";
            break;
        case acGtEq:
            name = "GtEq";
            break;
        case acGt:
            name = "Gt";
            break;
        case acLtEq:
            name = "LtEq";
            break;
        case acLt:
            name = "Lt";
            break;
        case acEqEq:
            name = "EqEq";
            break;
        case acNotEq:
            name = "NotEq";
            break;
        case acAmp:
            name = "Amp";
            break;
        case acCaret:
            name = "Caret";
            break;
        case acPipe:
            name = "Pipe";
            break;
        case acAmpAmp:
            name = "AmpAmp";
            break;
        case acPipePipe:
            name = "PipePipe";
            break;
        case acAssign:
            name = "Assign";
            break;        
        case acAssignConst:
            name = "AssignConst";
            break;        
        case acAssignRegister:
            name = "AssignRegister";
            break;        
        case acAssignRegisterConst:
            name = "Assign";
            break;        
        case acAddStack:
            name = "AddStack";
            break;        
        case acJz:
            name = "Jz";
            break;        
        case acJmp:
            name = "Jmp";
            break;        
        case acFunction:
            name = "Function";
            break;        
        case acReadRegister:
            name = "ReadRegister";
            break;        
        case acReadRegisterLabel:
            name = "ReadRegisterLabel";
            break;        
        case acReadSelfRegister:
            name = "ReadSelfRegister";
            break;        
        case acLoadBinParamVarVar:
            name = "LoadBinParamVarVar";
            break;                
        case acLoadBinParamVarConst:
            name = "LoadBinParamVarConst";
            break;                
        case acLoadBinParamVarStack:
            name = "LoadBinParamVarStack";
            break;                
        case acLoadBinParamConstVar:
            name = "LoadBinParamConstVar";
            break;                
        case acLoadBinParamConstConst:
            name = "LoadBinParamConstConst";
            break;                
        case acLoadBinParamConstStack:
            name = "LoadBinParamConstStack";
            break;                
        case acLoadBinParamStackVar:
            name = "LoadBinParamStackVar";
            break;                
        case acLoadBinParamStackConst:
            name = "LoadBinParamStackConst";
            break;                
        case acLoadBinParamStackStack:
            name = "LoadBinParamStackStack";
            break;                
        case acLoadParamStack:
            name = "LoadParamStack";
            break;                
        case acLoadParamVar:
            name = "LoadParamVar";
            break;                
        case acSaveParamStack:
            name = "SaveParamStack";
            break;                
        case acSaveParamVar:
            name = "SaveParamVar";
            break;                
        case acAddStackVar:
            name = "AddStackVar";
            break;                
        case acAddStackConst:
            name = "AddStackConst";
            break;                   
        default:
            name = " ";
            Console::TraceLine("%d. неопознанная команда %d ", actionPtr, actionsTr[actionPtr].act);
            break;                        
    };    
   Console::TraceLine("%d. %s %f %f %X %X", actionPtr, name, actionsTr[actionPtr].param1,  actionsTr[actionPtr].param2, actionsTr[actionPtr].ptrs.ptr1, actionsTr[actionPtr].ptrs.ptr2);
#endif    
		switch(actionsTr[actionPtr].act)
		{
		case acSaveParamStack:
		case acAddStackVar:
		case acAddStackConst:
			stackSize++;
			if (stackSize > maxStackSize)
				maxStackSize = stackSize;
			break;
		case acLoadBinParamStackStack:
			stackSize--;
			// break  не надо
		case acLoadBinParamVarStack:
		case acLoadBinParamConstStack:
		case acLoadBinParamStackVar:
		case acLoadParamStack:
			stackSize--;
			if (stackSize > 1000)
            {
				Console::TraceLine("Переполнение стека при компиляции %d", stackSize);
				for(;;);
                throw;
            }
			break;
		case acJz:	
		case acJmp:
			actionsTr[actionPtr].param1 = (float)actToAct[(UINT)labelsV[ftou__(actionsTr[actionPtr].param1)] & 0xFFFF];
			break;
		default:
			break;
		}

    }
	delete[] actToAct;
	exprSize = maxStackSize + 1;
	actionsV.clear();
	constsV.clear();
	paramsV.clear();
	labelsV.clear();

}

BYTE CProcessor::AddVar(BYTE id)
{
	if (id != 0xFF)
	{
		int res = ExistVar(id);
		if (res != -1)
			return res;
	}
	varsV.push_back(id);
	return (BYTE)varsV.size();
}

short CProcessor::ExistVar(BYTE id)
{
	for(UINT i = 0; i < varsV.size(); i++)
		if (varsV[i] == id)
			return i+1;
	return -1;
}

BYTE CProcessor::AddConst(float num)
{
	for(UINT i = 0; i < constsV.size(); i++)
		if (constsV[i] == num)
			return i + 1;
	constsV.push_back(num);
	return (BYTE)constsV.size();
}

int CProcessor::AddAction(CProcessor::ACTIONS id)
{
	actionsV.push_back(id);
	switch(id)
	{
	case acAddStack:
	case acAssign:
	case acAssignRegister:
	case acJz:
	case acJmp:
		return 0;
	default:
		paramsV.push_back(STACKPTR);
	}
	return paramsV.back();
}

BYTE CProcessor::AddParam(short id)
{
	paramsV.push_back(id);
	return (BYTE)(paramsV.size() - 1);
}

void CProcessor::SetLabel(int labelNum)	
{ 
	labelsV[labelNum] = ((int)actionsV.size() &0xFFFF) + (((int)paramsV.size() << 16) & 0xFFF0000);
} 

BYTE CProcessor::TestFunctionParam(int fId)
{
	switch(FUNCTION_ID(fId))
	{
	case fGETUCUSTATE:
	case fISRATEREADY:
	case fRESETRATE:
    case fISFILTRREADY:
		return 0;
	case fSIN:
	case fCOS:
	case fTOBNRS:
	case fTORTMS:
	case fTRUNC:
	case fFRAC:
		return 1;
	case fRESETFILTR:
	case fGETBIT:
	case fAERODYNAMICCORR:
	case fFROMBNRS:
	case fFROMRTMS:
	case fRESETRATECHANNEL:
	case fTOBCD:
		return 2;
	case fSHOWALARM:  // 3 параметра
	case fSUPPRESSALARM:  // 3 параметра
	case fLINEAR:  // 3 параметра
	case fTOBNR:  // 3 параметра
	case fFROMBNR:
	case fTORTM:  // 3 параметра
	case fFROMRTM:
	case fSETBIT:
	case fGETFIELD:
		return 3;
	case fSETFIELD:
		return 4;
	case fFROMBCD:
		return 5;
				
	default:
		return 0xFF;
		break;
	}
	return 0xFF;
}

#endif    


/*void CProcessor::Save()
{
	SaveParam(&outType, sizeof(outType)); 
	SaveParam(&outNum, sizeof(outNum)); 
	SaveParam(&patternNum, sizeof(patternNum)); 
	SaveParam(&exprSize, sizeof(exprSize)); 
	SaveParam(&constant, sizeof(constant)); 

	WORD size = (WORD)actionsTr.size();
	SaveParam(&size, sizeof(size));
	for (UINT i = 0; i < size; i++)
		SaveParam(&actionsTr[i], sizeof(ACTION_TRIADA));
	size = (WORD)varsV.size();
	SaveParam(&size, sizeof(size));
	for (UINT i = 0; i < size; i++)
		SaveParam(&varsV[i], sizeof(float));
}

void CProcessor::Load()
{
	LoadParam(&outType, sizeof(outType)); 
	LoadParam(&outNum, sizeof(outNum)); 
	LoadParam(&patternNum, sizeof(patternNum)); 
	LoadParam(&exprSize, sizeof(exprSize)); 
	LoadParam(&constant, sizeof(constant)); 

	WORD size;
	LoadParam(&size, sizeof(size));
	for (UINT i = 0; i < size; i++)
	{
		ACTION_TRIADA act;
		LoadParam(&act, sizeof(ACTION_TRIADA));
		actionsTr.push_back(act);
	}

	LoadParam(&size, sizeof(size));
	for (UINT i = 0; i < size; i++)
	{
		float p;
		LoadParam(&p, sizeof(p));
		varsV.push_back(p);
	}

}*/


DWORD CProcessor::GetBit(DWORD val, UINT bit)
{
    return ((val >> (bit-1)) & 1);
}

DWORD CProcessor::SetBit(DWORD val, UINT bit, UINT bitval)
{
    UINT shiftSize = (bit - 1);
    return ((val & ~(1 << shiftSize)) |  ((bitval & 1) << shiftSize));    
}

DWORD CProcessor::GetField(DWORD val, UINT startbit, UINT count)
{
    return ((val >> (startbit-1)) & ((1 << count) - 1));
}

DWORD CProcessor::SetField(DWORD val, UINT startbit, UINT count, DWORD valfield)
{
    DWORD mask = (((1 << count) - 1) << (startbit-1));
    return ((val & ~mask) | ((valfield << (startbit-1)) & mask));
}

UCU_FLOAT CProcessor::FromBNR(UINT val, UINT count, UCU_FLOAT range)
{
	int tmp = ((((int)val & 0x1FFFFF00) << 3) >> 11) ;
	tmp = tmp >> (int)(20 - count);
	return (UCU_FLOAT)tmp * (range  / (UCU_FLOAT)(1 << count));
      
}

UCU_FLOAT CProcessor::FromRTM(UINT val, UINT count, UCU_FLOAT range)
{
	int tmp = ((((int)val & 0x3FFFFF00) << 2) >> 10) ;
	tmp = tmp >> (int)(21 - count);
	return (UCU_FLOAT)tmp * (range  / (UCU_FLOAT)(1 << count)); 
}

int CProcessor::ToBNR(UCU_FLOAT val, UINT count, UCU_FLOAT range)
{
	int tmp = ROUND(val * (1 << count) / range) << (20 - count);
    return (tmp << 8);
}

int CProcessor::ToRTM(UCU_FLOAT val, UINT count, UCU_FLOAT range)
{
	int tmp = ROUND(val * (1 << count) / range) << (21 - count);
    return (tmp << 8);
}

UINT CProcessor::ToBCD(UCU_FLOAT val, UINT fillBits)
{
	UINT tmp = 0;
	while (val >= 1000000)
		val = val / 10;
	for(int i = 0; i < 24; i+=4)
	{
		if(ftou__(val) > 0 || i == 0) // Если первый 0 то его надо установить
		{
			tmp |= (ftou__(val) % 10) << i;
			val /= 10;
		}
		else
			tmp |= (fillBits &0xF) << i;
	}
    return tmp;
}

