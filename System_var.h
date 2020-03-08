
/////////////////////////////СИСТМЕМНЫЕ ПЕРЕМЕННЫЕ/////////////////////////////////////////


bool Setup = 0;
byte Parameter_COUNTER = 0;  // глобальный счетчик сообщения параметров
byte Status_COUNTER = 0;


//~~~~~~~~~~~~~~~~~~ниже для периодической отправки массива параметров 

uint32_t timerPeriodSendParameters = 0; // для таймера периодической отправки параметров в кан
const byte interval_sendParam = (1.5*NODS_QUANTITY) + 5;     // интервал периодической отправки в кан всех параметров

//~~~~~~~~~~~~~~~~~~ниже для организации StatusConrol

#ifdef type_node_master
byte Accident_COUNTER = 0;  // глобальный счетчик сообщения параметров
bool flag_alarmAccident = 0;                // флаг что уже можно оценивать живы ли узлы (включется через 30 сек после старта МК)
bool alarmAccident_send[NODS_QUANTITY] = {0};   // флаг что сообщение об аварии отправлено. 
bool StatusNode_OK[NODS_QUANTITY] = {0};      // массив состояние живы или нет узлы
byte countNoTAnswerNode[NODS_QUANTITY] = {0}; // массив счетчиков неответов от узлов
                                            // номер элемента массива это АДРЕС УЗЛА В СЕТИ!!!
const byte intervalStatus = 1;              // интервал таймера прибавления счетчика неответов, сек

#define MAX_NoTAnswerCOUNT 5       //число счетчика неответов, превысив которое, делаем статус узла "Мёртв"
bool printstatus = 0;

// ~~~~~~~~~~~~~~~~~~ниже выбираем узлы, которые подключены к сети CAN. 1 - подключен к CAN, 0 - НЕ подключен

const bool NodeCANpresence[NODS_QUANTITY] = 
{
  0, //  broadcast,                            //0
  0, //  node_1_Net_center_PC,                 //1
  0, //  node_2_Net_center_oraPi1,             //2
  0, //  node_3_Net_center_oraPi2,             //3 
  1, //  node_4_Net_center_Due1,               //4
  1, //  node_5_Net_center_Due2,               //5
  1, //  node_6_Hallway_net_center,            //6
  1, //  node_7_Hallway_main,                  //7
  1, //  node_8_Hallway_light,                 //8
  1, //  node_9_Kitchen_net_center,            //9
  1, //  node_10_Kitchen_main,                  //10
  1, //  node_11_Kitchen_light,                 //11
  1, //  node_12_WC_main,                       //12
  1, //  node_13_WC_waterleak,                  //13
  1, //  node_14_Bathroom_main,                 //14
  1, //  node_15_Boxroom_main,                  //15
  0, //  node_16_Balcony_meteo,                 //16                          
  0, //  node_17_Loggia_main,                   //17                             
  1, //  node_18_Loggia_recuperator,            //18                             
  1, //  node_19_Livingroom_main,               //19                             
  1, //  node_20_Bedroom_main,                  //20                             
  1, //  node_21_Cabinet_main,                  //21                             
    };

#endif 

#if defined (type_node_slave) or defined (type_node_mk) 
//byte MonitoringAddr      = node_1_Net_center_PC;
byte SendAddrStatus_master = node_4_Net_center_Due1; //адрес мастера по умолчанию, на который будет периодически отправляться статус узла
byte SendAddrStatus_slave = node_5_Net_center_Due2;  //адрес слейва по умолчанию, на который будет периодически отправляться статус узла

bool TimerStatus = 0;
bool sendparameters = 0;
#endif

unsigned long prevtimeStatus = 0; //для таймера статус
unsigned long curMillis = 0;      //снимок машинного времени

// ~~~~~~~~~~~~~~~~~~ниже для организации COMMAND 

const byte TX_repeatPeriod = 200; // периодичность повторений TX сообщений, мс,  если не приходит  отчет, но более 5 раз. 
const byte queueTX_size =     10; // максимальный размер очереди TX сообщений (количество ячеек)
const byte maxNumberRepeat_TX  = 5; // максимальное количество повторений команды, если не получен отчет о приянтии команды оппонентом

enum Name_Cells_TX_Queue
{
Command_TX_COUNTER   ,//0   // глобальный счетчик команды
Command_TX_COUNTER_2 ,//1   // глобальный счетчик команды 
Command_TX_count     ,//2   // счетчик отправленных комманд БЕЗ получения ответа от получателя для каждой ячейки очереди
CommandType          ,//3   // тип команды 
CommandValue         ,//4   // значение (байт1) команды
DEVTYPE              ,//5   // тип устройства
flagTimerCommand     ,//6   // флаг таймера ячеек  
target_Address       ,//7   // адрес получателя (отправителя) команды 
target_Address_2     ,//8   // адрес получателя (отправителя) команды 
target_Address_3     ,//9   // адрес получателя (отправителя) команды 
cellState            ,//10  // состояние занята или нет ячейка очереди
CommandValue_2       ,//11   // значение (байт2) команды 
CommandValue_3       ,//12   // значение (байт3) команды
CommandValue_4       ,//13   // значение (байт4) команды
SensNumber           ,//14   // номер девайса данного типа девайса (номер датчика данного типа параметра)
PrioritY             ,//15   // CAN приоритет команды (низкий или высокий)
queue_size_columns
};

#define Command_TX_COUNTER_3 2   // глобальный счетчик команды 

byte Command_TX_cell  [queueTX_size][queue_size_columns] = {0}; // массив ячеек очереди, в которой хранятся команды на отправку
byte Command_COUNTER = 0;
uint32_t queueTX_Timer [queueTX_size] = {0};   // таймер задержки между отправкой команд повторно в случае, если не приходит отчет, но не балее 5 раз. 

const byte IMPULSEDEVICE_QUANTITY = 5;// количество одновременно управляемых девайсов импульсом
uint32_t timerImpulse[IMPULSEDEVICE_QUANTITY] = {0};            // для таймера создания импульса управления импульсными девайсами
byte TimerImpulse_ON[IMPULSEDEVICE_QUANTITY]  = {0};            // для таймера создания импульса управления импульсными девайсами                                                         
const byte IMPULSE_DURATION = 8;   // длина импульса управления импульсными девайсами *100мс

// ~~~~~~~~~~~~~~~~~~ниже для организации COMMAND_REPORT

const byte bufferEXElongCOM_size =     10; // максимальный размер буфера исполнения долгих команд

const byte queuelongCOM_size_columns = 11;
byte bufferEXElongCOM_cell  [bufferEXElongCOM_size][queuelongCOM_size_columns] = {0}; // массив ячеек буфера, в котором хранятся долго исполняемые команды 
uint32_t bufferEXElongCOM_Timer [bufferEXElongCOM_size] = {0}; // таймер максимального ожидания выполннения долгой команды, по истечение которого нужно полюбому отправлять отчёт инициатору команды.  

byte countRepeat;
uint32_t timercountRepeat = 0;
bool timercountRepeat_ON = 0; 
