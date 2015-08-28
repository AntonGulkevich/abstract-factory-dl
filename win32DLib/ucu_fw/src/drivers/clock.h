#ifndef CCLOCK_H
#define CCLOCK_H
#include <cstdio>

#include "../interfaces/idispatchflags.h"

#include "../types.h"

class I2C;

class Clock : IDispatchFlags
{
public:
    // Описание формата хранения данных в микросхеме
    union CHIPTIME {
        struct {
            UINT oneSec     :   4;  //Единицы секунд
            UINT decSec     :   3;  //Десядки секунд
            UINT on         :   1;  //Включение часов

            UINT oneMin     :   4;  //Единицы секунд
            UINT decMin     :   3;  //Десядки секунд
            UINT reserv0    :   1;

            UINT oneHours   :   4;  //Единицы часов
            UINT decHours   :   2;  //Десядки часов
            UINT size       :   1;  //Переключение 12 24 часа
            UINT reserv1    :   1;

            UINT day        :   3;  //Единицы дней недели
            UINT reserv2    :   5;

            UINT oneDay     :   4;  //Единицы дней
            UINT decDay     :   2;  //Десядки дней
            UINT reserv3    :   2;

            UINT oneMonth   :   4;  //Единицы месяцев
            UINT decMonth   :   1;  //Десядки месяцев
            UINT reserv4    :   3;

            UINT oneYear    :   4;  //Единицы лет
            UINT decYear    :   4;  //Десядки лет

            UINT rs         :   2;  //Управление выходм клока
            UINT reserv5    :   2;
            UINT rs_on      :   1;  //Включение клока
            UINT reserv6    :   2;
            UINT out        :   1;  //Управление выходом
        };
        BYTE byte[8];
    };

    // Структура "Время"
    struct TIME_STRUCT
    {
        BYTE sec;
        BYTE min;
        BYTE hour;
    };

    // Структура "Дата"
    struct DATE_STRUCT
    {
        BYTE day;
        BYTE month;
        WORD year;
    };


	// Конструктор, с указанием базового адреса устройства
    Clock(I2C* i2c);
    // Деструктор, чтобыбыл
    virtual ~Clock()   {}
    // Инициализация
    void Init();
    // Установка времени
    void SetTime(TIME_STRUCT& time);
    // Установка даты
    void SetDate(DATE_STRUCT& time);

    // Запись данных в микросхему часов
    void WriteData(void* buff, DWORD address, UINT count);
    // Чтение данных из микросхемы часов
    void ReadData(void* buff, DWORD address, UINT count);

    // Получение даты в текстовом формате
    void GetDateStr(char* buff, const CHIPTIME* ct = NULL);
    // Получение времени в текстовом формате
    void GetTimeStr(char* buff, const CHIPTIME* ct = NULL);
    // Получение даты и времени в текстовом формате
    void GetDateTimeStr(char* buff, const CHIPTIME* ct = NULL);

    // Получение текущих даты/времени в формате микросхемы
    const CHIPTIME* GetChipTime() { ReadChipTime(); return &chipTime; }

    // Обработчик выключения УБС-К
    void OnPowerDown();
    // Определение горячего старта
    bool IsHotStart();
    // Обработка флагов
    DISPATH_RESULT ProcessFlag(Flags id);

    UINT GetCurrentTime() { return _currentTime; }


private:
    const UINT monthSeconds[12] = { 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };

    I2C* _i2c;

    // Запись CHIPTIME в микросхему
    void WriteChipTime();
    // Чтение CHIPTIME из микросхемы
    void ReadChipTime();
    // Перевод времени в секунды
    void ChipTimeToSecond();
    // Текущее время и дата
    CHIPTIME chipTime;
    // Наличие микросхемы часов
    bool isAvaible;

    UINT _currentTime;
};

#endif
