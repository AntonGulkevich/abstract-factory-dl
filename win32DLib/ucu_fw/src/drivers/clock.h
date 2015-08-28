#ifndef CCLOCK_H
#define CCLOCK_H
#include <cstdio>

#include "../interfaces/idispatchflags.h"

#include "../types.h"

class I2C;

class Clock : IDispatchFlags
{
public:
    // �������� ������� �������� ������ � ����������
    union CHIPTIME {
        struct {
            UINT oneSec     :   4;  //������� ������
            UINT decSec     :   3;  //������� ������
            UINT on         :   1;  //��������� �����

            UINT oneMin     :   4;  //������� ������
            UINT decMin     :   3;  //������� ������
            UINT reserv0    :   1;

            UINT oneHours   :   4;  //������� �����
            UINT decHours   :   2;  //������� �����
            UINT size       :   1;  //������������ 12 24 ����
            UINT reserv1    :   1;

            UINT day        :   3;  //������� ���� ������
            UINT reserv2    :   5;

            UINT oneDay     :   4;  //������� ����
            UINT decDay     :   2;  //������� ����
            UINT reserv3    :   2;

            UINT oneMonth   :   4;  //������� �������
            UINT decMonth   :   1;  //������� �������
            UINT reserv4    :   3;

            UINT oneYear    :   4;  //������� ���
            UINT decYear    :   4;  //������� ���

            UINT rs         :   2;  //���������� ������ �����
            UINT reserv5    :   2;
            UINT rs_on      :   1;  //��������� �����
            UINT reserv6    :   2;
            UINT out        :   1;  //���������� �������
        };
        BYTE byte[8];
    };

    // ��������� "�����"
    struct TIME_STRUCT
    {
        BYTE sec;
        BYTE min;
        BYTE hour;
    };

    // ��������� "����"
    struct DATE_STRUCT
    {
        BYTE day;
        BYTE month;
        WORD year;
    };


	// �����������, � ��������� �������� ������ ����������
    Clock(I2C* i2c);
    // ����������, ��������
    virtual ~Clock()   {}
    // �������������
    void Init();
    // ��������� �������
    void SetTime(TIME_STRUCT& time);
    // ��������� ����
    void SetDate(DATE_STRUCT& time);

    // ������ ������ � ���������� �����
    void WriteData(void* buff, DWORD address, UINT count);
    // ������ ������ �� ���������� �����
    void ReadData(void* buff, DWORD address, UINT count);

    // ��������� ���� � ��������� �������
    void GetDateStr(char* buff, const CHIPTIME* ct = NULL);
    // ��������� ������� � ��������� �������
    void GetTimeStr(char* buff, const CHIPTIME* ct = NULL);
    // ��������� ���� � ������� � ��������� �������
    void GetDateTimeStr(char* buff, const CHIPTIME* ct = NULL);

    // ��������� ������� ����/������� � ������� ����������
    const CHIPTIME* GetChipTime() { ReadChipTime(); return &chipTime; }

    // ���������� ���������� ���-�
    void OnPowerDown();
    // ����������� �������� ������
    bool IsHotStart();
    // ��������� ������
    DISPATH_RESULT ProcessFlag(Flags id);

    UINT GetCurrentTime() { return _currentTime; }


private:
    const UINT monthSeconds[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

    I2C* _i2c;

    // ������ CHIPTIME � ����������
    void WriteChipTime();
    // ������ CHIPTIME �� ����������
    void ReadChipTime();
    // ������� ������� � �������
    void ChipTimeToSecond();
    // ������� ����� � ����
    CHIPTIME chipTime;
    // ������� ���������� �����
    bool isAvaible;

    UINT _currentTime;
};

#endif
