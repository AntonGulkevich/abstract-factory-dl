/*
 * arinctypes.h
 *
 *  Created on: 04 мая 2015 г.
 *      Author: Youbrin_A
 */

#ifndef ARINCTYPES_H_
#define ARINCTYPES_H_
#include "../types.h"


class Arinc
{
public:
	struct WORD429
	{
		DWORD time;
		DWORD value;
	};

	enum class Rate : UINT
	{
		R12_5,
		R50,
		R100,
	};



	enum class ParityIn : UINT
	{
		NoControl = 0,
		OddControl = 2,
		EvenControl = 3
	};

	enum class ParityOut : UINT
	{
		NoChange = 8,
		EvenControl = 9,
		OddControl = 10,
		Always0 = 11,
		Always1 = 12,
	};

	enum class FifoStatus
	{
		FifoEmpty = 1,
		FifoFull = 2,

	};

    union ARINC_STRUCT {
        struct ARINC_DEFAULT {
            UINT label  : 8;
            UINT data   : 21;
            UINT matrix : 2;
            UINT parity : 1;
        } arincDef;
        struct ARINC_ID {
            UINT label  : 8;
            UINT id     : 2;
            UINT data   : 19;
            UINT matrix : 2;
            UINT parity : 1;
        } arincId;
        struct ARINC_INDEX {
            UINT index  : 10;
            UINT data   : 19;
            UINT matrix : 2;
            UINT parity : 1;
        } arincIndex;
        DWORD dword;
    };

};


#endif /* ARINCTYPES_H_ */
