#include <mcp_can.h>
#include <SPI.h>
#ifdef EEProm_my_ADDRESS
#include <EEPROM.h>
#endif
// Set and phisical connect! CS to pin 10 for ATMEGA328, 53 for Mega 2560, 52 for DUE

#ifdef ARD_ATMEGA328 
#define can CAN0
#define csPIN 10
#endif
#ifdef ARD_ATMEGA2560
#define can CAN0
#define csPIN 53
#endif
#ifdef ARD_DUE
#define can CAN3                             
#define csPIN 52
#endif

MCP_CAN can(csPIN); 

#define BIT_29 1
#define BIT_11 0

#define EXTENDED 1
#define STANDART 0

#define HIGH_PRIORITY 0
#define LOW_PRIORITY  1

#define REMOTE_CANFRAME 1
#define DATA_CANFRAME   0

#define END_ MULTIFRAME      1
#define STREAM_ MULTIFRAME   0

#define MY_ADDRESS   50     // номер ячейки еепром, в которой лежит адрес данного узла

// ниже пин INT CAN модуля
#define CAN0_INT 2                              // Set INT to pin 2


// НИЖЕ ТИП ОТЧЕТОВ НА ПОЛУЧЕННЫЕ КОМАНДЫ

#define COMPLETE           0x01   // команда выполнена успешно 
#define EXECUTING          0x02   // команда принята, началось выполнение 
#define FAIL_UNKNOWN       0x03   // ошибка выполнения команды, причины неизвестны
#define UNKNOWN_COMMAND    0x04   // ошибка выполнения команды, неизвестная команда
#define UNKNOWN_TYPEDEV    0x05   // ошибка выполнения команды, неизвестный тип устройства, посланный в команде. 
#define NOT_INCLUDED       0x06   // ошибка выполнения команды, тип устройства, посланный в команде, отсутствует на узле
#define NOT_SUITABLE       0x07   // ошибка выполнения команды, команда не подходит для данного вида устройства.
#define NEGATIVE_RESULT    0x08   // ошибка выполнения команды, желаемый результат воздействия на устройство не достигнут (желаемое состояние устройства после выполнения команды не соответствует значению , полученному при контроле состояния устройства)

// НИЖЕ ПРИЧИНА ОСТЫЛКИ В CAN ПАРАМЕТРА

#define PERIODICALLY       0x09   // параметр отправлен периодически
#define CHANGEVALUE        0x0A   // параметр отправлен по факту изменения его значения на узле
#define ON_REQUEST         0x0B   // параметр отправлен по запросу от другого узла

// ЕСЛИ ПРИХОДИТ ЗАПРОС НА ПАРАМЕТР, НЕ УКОМПЛЕКТОВАННЫЙ НА УЗЛЕ - ОТЧЁТ БУДЕТ ТАКОЙ:    #define NOT_INCLUDED       0x06



// НИЖЕ ДЛЯ МАССИВА "ПАРАМЕТРЫ"
#define PARAMETER_TYPE    0
#define PARAMETER_SENS    1
#define TYPE_VAR          2
#define PARAMETER_VALUE   3
#define PARAMETER_LAST    4
#define PERIODIC_CANADDR  5

#define BYTE_1        1
#define BYTE_2        2
#define BYTE_4        4
#define SERVICE_BYTE  5
#define BYTE_4_SIGNED 6
#define BYTE_2_SIGNED 7
#define BYTE_1_SIGNED 8
#define BYTE_4_FLOAT  9

#define NOT_SENT 0xFF

// НИЖЕ ДЛЯ МАССИВА "ИСПОЛНИТЕЛЬНЫЕ МЕХАНИЗМЫ"

#define DEVICE_TYPE           0
#define DEVICE_VALUE          1
#define DEVICE_PIN            2
#define DEVICE_STATUS_PIN     3
#define DEVICE_VID            4


// НИЖЕ ДЛЯ МАССИВА ТАЙМАУТОВ

#define TIMEOUTS_NUMBER sizeof(timeoutsArray)/2 
#define DEV_TYPE 0
#define TIMEOUT  1



// НИЖЕ ВИД УСТРОЙСТВА 

#define LONG                  |0x80
#define DIGITALWRITE           1   // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино
#define DIGITALWRITE_INVERT    2   // булевое устройство, имеющее два состояния вкл и выкл, подключено к цифровому выходу ардуино, у которого инвертирована логика (например, как у релейных модулей)
#define PWMWRITE               3   // устройство управляемое ШИМ сигналом, подключено к PWM выходу ардуино
#define PROCENTWRITE           4   // устройство управляемое градацией от 0 до 100%, подключено к ардуино может быть по разному. 
#define IMPULSE_GND            5   // устройство, подавая на которое импульс LOW,  меняет своё состояние на противоположное
#define IMPULSE_POWER          6   // устройство, подавая на которое импульс HIGH, меняет своё состояние на противоположное


// Типы команд (CommandType)

enum Command_enum
{
 RESERVE,              //  0x00   // Резерв
 DIGITAL_REMOTE,       //  0x01   // Управление (вкл/выкл) булевыми устройствами
 DIGITAL_INVERT,       //  0x02   // Инвертирование состояния булевых устройств 

 DIMMER_SETTING,       //  0x03   // Установка процента включения диммируемых устройств (значение в CommandValue)
 DIMMER_TURN_OFF,      //  0x04   // Увеличене  включенности PWM устройств (значение увеличения в CommandValue)
 DIMMER_TURN_ON,       //  0x05   // Уменьшение включенности PWM устройств (значение уменьшения в CommandValue)

 IMPULSE_ON,           //  0x06   // Включение устройств, запуск которых осуществляется 1 сек импульсом GND 
 IMPULSE_OFF,          //  0x07   // Включение устройств, запуск которых осуществляется 1 сек импульсом GND 
 IMPULSE_INVERT,       //  0x08   // Инвертирование состояния устройств, запуск которых осуществляется 1 сек импульсом GND 

 PWM_SETTING,          //  0x09   // Установка величины включения диммируемых устройств управляемых PWM (значение в CommandValue)
 PWM_TURN_OFF,         //  0x0A   // Увеличене  включенности PWM устройств (значение увеличения в CommandValue)
 PWM_TURN_ON,          //  0x0B   // Уменьшение включенности PWM устройств (значение уменьшения в CommandValue)

 PARAMETER_WRITE,      //  0x0C    // Изменение значения выбранного параметра (массив parameter)

 Command_enum_SIZE     // размер списка команд 
  } ;


// структуры полей данных CAN фреймов (за что отвечает какой байт)

#define CAN_COUNTER             0  // для всех типов сообщений

// структура поля данных типа сообщения "COMMAND" и "COMMAND_REPORT"
#define CAN_SENSOR_NUM          1
#define CAN_REPORT              2
#define CAN_CommandType         3
#define CAN_CommandValue        4
#define CAN_CommandValue_2      5
#define CAN_CommandValue_3      6
#define CAN_CommandValue_4      7

// структура поля данных типа сообщения "STATUS"

#define CAN_SEC                 1
#define CAN_MIN                 2
#define CAN_HOURS               3
#define CAN_DAYS                4


// Типы сообщений в системе SmartHomeCAN
enum Message_enum
{
    NULL_C,             //0
    COMMAND,            //1
    COMMAND_REPORT,     //2
    PARAMETER_REQUEST,  //3 
    PARAMETER,          //4
    STATUS_REQUEST,     //5
    STATUS,             //6
    MULTIFRAME_REQUEST, //7
    MULTIFRAME,         //8
    MULTIFRAME_END,     //9  
    ACCIDENT,           //10
    ACCIDENT_REPORT,    //11
    size_Message_ENUM   //12 size
} ;   


// ЗАДАЁМ СПИСОК НАЗВАНИЙ (АДРЕСОВ) ВСЕХ УЗЛОВ, ПАРАМЕТРОВ И ДЕВАЙСОВ СИСТЕМЫ  SmartHomeCAN

#include "name_addresses.h"

// ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ НАЗВАНИЯ (АДРЕСА) ПАРАМЕТРОВ, ПРИМЕНЯЕМЫХ В УМНОМ ДОМЕ

#include "param_conf.h"

//ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ ИСПОЛНИТЕЛЬНЫЕ УСТРОЙСТВА, КОТОРЫЕ МОГУТ ПРИСУТСТВОВАТЬ В СИСТЕМЕ УМНЫЙ ДОМ 

#include "devices_conf.h"

// системные часы на миллис
uint8_t ss = 0;
uint8_t mm = 0;
uint8_t hh = 0;
uint8_t dd = 0;
uint32_t prevSystemtime = 0;

//ниже прочитаем из еепром адрес данного узла
#ifdef EEProm_my_ADDRESS
const byte node_address = EEPROM.read(MY_ADDRESS);
#endif

//ЗАДАЁМ ВСЕ СЛУЖЕБНЫЕ СИСТЕМНЫЕ ПЕРЕМЕННЫЕ CAN СЕТИ

#include "System_var.h"
