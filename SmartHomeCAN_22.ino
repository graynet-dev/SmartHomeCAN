#define VER 22            // версия скетча   
// версия 22 - переназваны переменные типа сообщений "команды", задефайнена структура поля данных CAN фрейма каждого типа сообщений
// в отдельные файлы выделена конфигурация узлов и умного дома в целом. 
// исправлены ошибки в работе с запросами параметров


// выберем как будет устанавливаться адрес данного узла вручную или из еепром

//#define EEProm_my_ADDRESS
#define Manual_my_ADDRESS

// Выбираем аппаратную часть узла

#define ARD_ATMEGA328
//#define ARD_ATMEGA2560
//#define ARD_DUE

#include "can_struct.h"

// ниже вручную забьём адрес узла, если выше выбран дефайн  #define Manual_my_ADDRESS

#ifdef Manual_my_ADDRESS
//const byte node_address = node_1_Net_center_PC;                 //1
//const byte node_address = node_2_Net_center_oraPi1;             //2
//const byte node_address = node_3_Net_center_oraPi2;             //3 
//const byte node_address = node_4_Net_center_Due1;               //4
//const byte node_address = node_5_Net_center_Due2;               //5
//const byte node_address = node_6_Hallway_net_center;            //6
//const byte node_address = node_7_Hallway_main;                  //7
//const byte node_address = node_8_Hallway_light;                 //8
//const byte node_address = node_9_Kitchen_net_center             //9
const byte node_address = node_10_Kitchen_main;                  //10
//const byte node_address = node_11_Kitchen_light;                 //11
//const byte node_address = node_12_WC_main;                       //12
//const byte node_address = node_13_WC_waterleak;                  //13
//const byte node_address = node_14_Bathroom_main;                 //14
//const byte node_address = node_15_Boxroom_main;                  //15
//const byte node_address = node_16_Balcony_meteo;                 //16                          
//const byte node_address = node_17_Loggia_main;                   //17                             
//const byte node_address = node_18_Loggia_recuperator;            //18                             
//const byte node_address = node_19_Livingroom_main;               //19                             
//const byte node_address = node_20_Bedroom_main;                  //20                             
//const byte node_address = node_21_Cabinet_main;                  //21    
#endif

// Выбираем смысловую нагрузку узла 

//#define type_node_master
//#define type_node_slave
#define type_node_mk

// Выбираем адрес узла мониторинга

byte MonitoringAddr = node_1_Net_center_PC;

// настраиваем отладку 

#define debug                        //отладка в сериал_монитор. Закоментировать строку после отладки

#ifdef debug
#define debugStatus                        //флаг таблица статусов узлов CAN сети (для МАСТЕРА) - закоментировать если не нужна таблица
const bool statusprint =     0;            //флаг распечатки принятых/отправленных статусов.  
const bool RXparametrprint = 0;            //флаг распечатки принятых параметров.  
const bool TXparametrprint = 0;            //флаг распечатки отправленных параметров.  
const bool errors =          0;            //флаг распечатки ошибок MCP2515.  
#include "sdebug.h"
#endif 

#include "System_var.h"


void setup() 
{
   CAN_Start();
 
}

void loop() 
{
  CAN_Refresh();


}






   
