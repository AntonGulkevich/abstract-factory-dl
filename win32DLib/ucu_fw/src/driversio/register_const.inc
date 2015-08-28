
#define MIN_FLOAT -1E+29f
#define MAX_FLOAT 1E+29f

/*CBaseIO::REGISTER_TYPE CAnalogIn::registers_t[ANALOGIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f,0.0f,0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rTYPE, rwConstant, rtDec, 0.0f, 7.0f, 0.0f, true},
	{rDELAY, rwUser, rtDec, 0.0f, 2499.9998f, 0.0f, true},
	{rSYNCNUM, rwConstant, rtDec, 0.0f, 4, 0.0f, true},
	{rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rMAXLEVEL, rwConstant, rtFloat, -100.0f, 100.0f, 1.0f, false},
	{rMINLEVEL, rwConstant, rtFloat, -100.0f, 100.0f, 1.0f, false},
	{rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true},
	{rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false},
	{rTARMIN, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rTARMAX, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rPASSPORT, rwConstant, rtArray, 4, 128, 1, false},
	{rDEADZONE, rwConstant, rtFloat, -360, 0, 0, false},
	{rCORRECTIONDELTA, rwUser, rtDec, 0, 360, 0, false},
	{rCORRECTIONSIGN, rwUser, rtDec, -1, 1, 1, false},
	{rFILTRTYPE, rwConstant, rtDec, 0, 2, 0, false},
	{rFILTRBASE, rwConstant, rtDec, 1.0f, 250.0f, 1.0f, false},
	{rRTCNUM, rwConstant, rtDec, 0, 28, 0, false},
	{rRTCVAL, rwConstant, rtDec, 0, 1, 1, false}
};

const CBaseIO::REGISTER_TYPE CAngleOut::registers_t[ANGLEOUT_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f,0.0f,0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f,  true},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true},
	{rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false},
	{rDELAY, rwUser, rtDec, 0.0f, 360.0f, 0.0f, true},
	{rTYPE, rwConstant, rtDec, 0.0f, 1.0f, 0.0f, true},
	{rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, true},
	{rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false}
};

CBaseIO::REGISTER_TYPE CArincIn::registers_t[ARINCIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rSPEED, rwConstant, rtDec, 0.0f, 3, 0.0f, true},
	{rPARITYTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, false}		
};

CBaseIO::REGISTER_TYPE CArincOut::registers_t[ARINCOUT_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rSPEED, rwConstant, rtDec, 0.0f, 3, 0.0f, true},
	{rPARITYTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, false}	
};

CBaseIO::REGISTER_TYPE CArincWord::registers_t[ARINCWORD_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rMATRIX, rwVariable, rtDec, 0.0f, 4, 0.0f, false},
	{rPERIODMAX, rwConstant, rtFloat, 0.0f, 20000.0f, 0.0f, true},
	{rPERIODMIN, rwConstant, rtFloat, 0.0f, 20000.0f, 0.0f, true},
	{rID, rwConstant, rtDec, 0.0f, 3, 0.0f, false},
	{rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true},
	{rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false},
	{rCONTROL, rwVariable, rtDec, 0.0f, 1.0f, 0.0f, false},
	{rDELTA, rwConstant, rtDec, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rBITSCOUNT, rwConstant, rtDec, 1, 22, 0.0f, false},
	{rRANGE, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},	
};

CBaseIO::REGISTER_TYPE CNPTOut::registers_t[NPTOUT_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, true},
	{rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, true},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true},
	{rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false},
	{rDELTA, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false}
};

CBaseIO::REGISTER_TYPE CChecker::registers_t[CHECKER_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false}
};

CBaseIO::REGISTER_TYPE CPotentiometrIn::registers_t[POTENTIOMETRIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rMAXLEVEL, rwConstant, rtDec, -200.0f, 200, 100.0f, false},
	{rMINLEVEL, rwConstant, rtDec, -200.0f, 200, 0.0f, false},	
	{rGROUP, rwConstant, rtDec, 0.0f, 99.0f, 0.0f, true},
	{rSUBGROUP, rwConstant, rtDec, 1.0f, 9.0f, 1.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, false},
	{rTARMIN, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rTARMAX, rwConstant, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rFILTRBASE, rwConstant, rtDec, 1, 100.0f, 1, false},
	{rPASSPORT, rwConstant, rtArray, 4, 128, 1, false},
	{rFILTRTYPE, rwConstant, rtDec, 0, 2, 0, false},
	{rRTCNUM, rwConstant, rtDec, 0, 28, 0, false},
	{rRTCVAL, rwConstant, rtDec, 0, 1, 1, false}
};

CBaseIO::REGISTER_TYPE CRSIn::registers_t[RSIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtString, 0.0f, 0.0f, 0.0f, false},
	{rSPEED, rwConstant, rtDec, 2400.0f, 115200.0f, 0.0f, true},
	{rPARITYTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, true},
	{rTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, true},
	{rSTOPBIT, rwConstant, rtDec, 1.0f, 2.0f, 1.0f, true}
	
};

CBaseIO::REGISTER_TYPE CRSOut::registers_t[RSIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtString, 0.0f, 0.0f, 0.0f, false},
	{rSPEED, rwConstant, rtDec, 2400.0f, 115200.0f, 0.0f, true},
	{rPARITYTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, true},
	{rTYPE, rwConstant, rtDec, 0.0f, 2.0f, 0.0f, true},
	{rSTOPBIT, rwConstant, rtDec, 1.0f, 2.0f, 1.0f, true}	
};

CBaseIO::REGISTER_TYPE CLogicalIn::registers_t[LOGICALIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, true},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, true},
	{rFILTRBASE, rwConstant, rtDec, 1.0f, 250.0f, 1.0f, false},
	{rFILTRTYPE, rwConstant, rtDec, 0, 2, 0, false},
	{rDATAARRAY, rwConstant, rtArray, 1, 200, 1, false},
	{rRATE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rRATEBASE, rwConstant, rtDec, 1, 50, 1, false}
};

CBaseIO::REGISTER_TYPE CLogicalOut::registers_t[LOGICALOUT_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, true},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rUNIT, rwConstant, rtDec, 0.0f, 17.0f, 0.0f, true},
	{rFILTRBASE, rwConstant, rtDec, 1.0f, 250.0f, 1.0f, false},
	{rFILTRTYPE, rwConstant, rtDec, 0, 2, 0, false},
	{rDATAARRAY, rwConstant, rtArray, 1, 200, 1, false},
	{rRATE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rRATEBASE, rwConstant, rtDec, 1, 50, 1, false}
};

CBaseIO::REGISTER_TYPE CCommand::registers_t[COMMAND_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, true},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false},
	{rCONTROL, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, true},
	{rDELAY, rwVariable, rtDec, 0.0f, 255.0f, 0.0f, false}
};

CBaseIO::REGISTER_TYPE CFrequencyIn::registers_t[FREQUENCYIN_COUNTR] = 
{
	{rNAME, rwConstant, rtString, 0.0f, 0.0f, 0.0f, false},
	{rVALUE1, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rVALUE2, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rVALUE3, rwVariable, rtFloat, MIN_FLOAT, MAX_FLOAT, 0.0f, false},
	{rINTERVAL, rwConstant, rtFloat, 0.1f, 12.9f, 0.0f, false},
	{rTYPE, rwConstant, rtDec, 0, 2, 0, false},
	{rSTATE, rwVariable, rtDec, 0.0f, 0xFFFF, 0.0f, false}
};*/
