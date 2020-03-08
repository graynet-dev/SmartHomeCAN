


// ЗАДАЁМ ТАБЛИЦУ (МАССИВ) ПАРАМЕТРОВ, КОТОРЫЕ БУДУТ ПРИСУТСТВОВАТЬ НА ДАННОМ УЗЛЕ ,т.е. это уже конфигурация - эта информация на каждом узле будет своя
// названия (типы) параметров берём из файла name_addresses.h

#define AUTOSENT_NODES_QUANTITY  2    // тут указываем максимальное количество узлов, на которые автоматически отсылаться один и тот же параметр

const byte second_dimension = 5 + AUTOSENT_NODES_QUANTITY ;

#define parameters_quantity sizeof(parameter)/second_dimension
byte parameter [][second_dimension] =   
                                                                     
//       тип             № датчика     тип данных            значение       посл.значение   таргет адрес 1-го узла          таргет адрес 2-го узла 
//    параметра                                              параметра        параметра     автоматической отправки         автоматической отправки
{
{       air_t,              1,         BYTE_1_SIGNED,            200,             0,              broadcast,                node_19_Livingroom_main,}, //1 //датчик температуры
{       air_t,              2,         BYTE_1_SIGNED,            100,             0,              node_7_Hallway_main,      node_19_Livingroom_main,}, //1 //датчик температуры
{       air_t,              3,         BYTE_1_SIGNED,            50,              0,              node_7_Hallway_main,      node_19_Livingroom_main,}, //1 //датчик температуры
{       air_h,              1,         BYTE_1,                   0,               0,              node_4_Net_center_Due1,   node_19_Livingroom_main,}, //2 //датчик влажности
{       counter_elect_4,    1,         BYTE_4,                   7,               7,              node_4_Net_center_Due1,   NOT_SENT,               }, //52 //Счетчик электричества
{counter_elect_4_SB2,       1,         SERVICE_BYTE,             8,               8,              NOT_SENT,                 NOT_SENT,               }, //53   сервисный байт
{counter_elect_4_SB3,       1,         SERVICE_BYTE,             9,               9,              NOT_SENT,                 NOT_SENT,               }, //54   сервисный байт
{counter_elect_4_SB4,       1,         SERVICE_BYTE,             10,              10,             NOT_SENT,                 NOT_SENT,               }, //55   сервисный байт


};

//тип данных это :
   // SIGNED ЭТО ЗНАЧИТ ПЕРЕМЕННАЯ ИМЕЕТ ЗНАК. 
   // ЕСЛИ ПЕРЕМЕННАЯ BYTE_2 ТО ЗА НЕЙ ДОЛЖЕН СЛЕДОВАТЬ ОДИН SERVICE_BYTE
   // ЕСЛИ ПЕРЕМЕННАЯ BYTE_4 ТО ПОСЛЕ НЕЁ ДОЛЖНЫ СЛЕДОВАТЬ ТРИ SERVICE_BYTE
   // ПЕРЕМЕННАЯ FLOAT МОЖЕТ БЫТЬ ВЫБРАНА ТОЛЬКО ДЛЯ BYTE_4 БЕЗ SIGNED И С ПОСЛЕДУЮЩИМИ ТРЕМЯ SERVICE_BYTE

//возможные варианты типов данных 

//   BYTE_1                   беззнаковый 1байтный (0...255)
//   BYTE_2                   беззнаковый 2байтный (0...65535)
//   BYTE_4                   беззнаковый 4байтный (0...4294967295)
//   SERVICE_BYTE             сервис байт (добавляется, если тип данных более одного байта)- Соответсвенно для 2 байтных добавляется один сервисбайт,для 4 байтных - три сервисбайта
//   BYTE_1_SIGNED            знаковый 1байтный (-128...+127)
//   BYTE_2_SIGNED            знаковый 2байтный (-32768...+23767)
//   BYTE_4_SIGNED            знаковый 4байтный (-2147483648...2147483647)
//   BYTE_4_FLOAT             4байтный флоат , с плавающей точкой
