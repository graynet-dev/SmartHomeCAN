
#ifdef debug

#include <avr/pgmspace.h>

#define TypeDEV 1
#define TypeMES 2
#define ADDR    3
#define PAR     4
#define COM     5



#define  BUF_LENGTH 100
char buf[BUF_LENGTH]; // буффер для работы прогмем, если нужны строки более 100 символов, тут ставим бОльшее значение

const char stringMES_0[] PROGMEM = "NULL_C"; 
const char stringMES_1[] PROGMEM = "КОМАНДА";                 //= "COMMAND";    
const char stringMES_2[] PROGMEM = "ОТЧЁТ О КОМАНДЕ";         //= "COMMAND_REPORT";  
const char stringMES_3[] PROGMEM = "ЗАПРОС ПАРАМЕТРА";        //= "PARAMETER_REQUEST";  
const char stringMES_4[] PROGMEM = "ПАРАМЕТР";                //= "PARAMETER"; 
const char stringMES_5[] PROGMEM = "ЗАПРОС СТАТУСА";          //= "STATUS_REQUEST";    
const char stringMES_6[] PROGMEM = "СТАТУС";                  //= "STATUS";   
const char stringMES_7[] PROGMEM = "ЗАПРОС МУЛЬТИКАДРА";      //= "MULTIFRAME_REQUEST";
const char stringMES_8[] PROGMEM = "МУЛЬТИКАДР";              //= "MULTIFRAME";   
const char stringMES_9[] PROGMEM = "КОНЕЦ МУЛЬТИКАДРА";       //= "MULTIFRAME_END";
const char stringMES_10[] PROGMEM = "АВАРИЯ";                 //= "ACCIDENT";  
const char stringMES_11[] PROGMEM = "АВАРИЯ ПРИНЯТА";         //= "ACCIDENT_REPORT";
 
const char* const string_tableMES[] PROGMEM = {stringMES_0, stringMES_1, 
stringMES_2, stringMES_3, stringMES_4,  stringMES_5, stringMES_6,
stringMES_7, stringMES_8, stringMES_9, stringMES_10, stringMES_11};

const char stringADDR_0[]  PROGMEM = "ШИРОКОВЕЩАТЕЛЬНО           ";
const char stringADDR_1[]  PROGMEM = "node_1_Net_center_PC       ";
const char stringADDR_2[]  PROGMEM = "node_2_Net_center_oraPi1   ";  
const char stringADDR_3[]  PROGMEM = "node_3_Net_center_oraPi2   "; 
const char stringADDR_4[]  PROGMEM = "node_4_Net_center_Due1     ";
const char stringADDR_5[]  PROGMEM = "node_5_Net_center_Due2     ";    
const char stringADDR_6[]  PROGMEM = "node_6_Hallway_net_center  ";
const char stringADDR_7[]  PROGMEM = "node_7_Hallway_main        ";  
const char stringADDR_8[]  PROGMEM = "node_8_Hallway_light       ";
const char stringADDR_9[]  PROGMEM = "node_9_Kitchen_net_center  ";
const char stringADDR_10[] PROGMEM = "node_10_Kitchen_main       ";
const char stringADDR_11[] PROGMEM = "node_11_Kitchen_light      ";
const char stringADDR_12[] PROGMEM = "node_12_WC_main            ";
const char stringADDR_13[] PROGMEM = "node_13_WC_waterleak       ";         
const char stringADDR_14[] PROGMEM = "node_14_Bathroom_main      ";  
const char stringADDR_15[] PROGMEM = "node_15_Boxroom_main       ";
const char stringADDR_16[] PROGMEM = "node_16_Balcony_meteo      ";
const char stringADDR_17[] PROGMEM = "node_17_Loggia_main        ";
const char stringADDR_18[] PROGMEM = "node_18_Loggia_recuperator ";              
const char stringADDR_19[] PROGMEM = "node_19_Livingroom_main    ";               
const char stringADDR_20[] PROGMEM = "node_20_Bedroom_main       ";                 
const char stringADDR_21[] PROGMEM = "node_21_Cabinet_main       ";               


const char* const string_tableADDR[] PROGMEM = {stringADDR_0, stringADDR_1, stringADDR_2, stringADDR_3, stringADDR_4,   // здесь массив строк отчетов
stringADDR_5,  stringADDR_6,  stringADDR_7,  stringADDR_8,  stringADDR_9, stringADDR_10,stringADDR_11,stringADDR_12,stringADDR_13, 
stringADDR_14, stringADDR_15, stringADDR_16, stringADDR_17, stringADDR_18,stringADDR_19,stringADDR_20,stringADDR_21};

const char stringPAR_0[] PROGMEM = "NULL PARAMETER"; //0 
const char stringPAR_1[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ВОЗДУХА"; //1 air_temperature_censor
const char stringPAR_2[] PROGMEM = "ДАТЧИК ВЛАЖНОСТИ ВОЗДУХА"; //2 air_humidity_censor
const char stringPAR_3[] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ВОДУХА"; //3 air_pressure_censor
const char stringPAR_4[] PROGMEM = "ДАТЧИК ДВИЖЕНИЯ ПИРОЭЛЕКТРИЧЕСКИЙ (ИК)"; //4 motion_pir_censor
const char stringPAR_5[] PROGMEM = "ДАТЧИК ДВИЖЕНИЯ МИКРОВОЛНОВОЙ"; //5  motion_mw_censor
const char stringPAR_6[] PROGMEM = "ДАТЧИК ВИБРАЦИИ"; //6 vibration_censor
const char stringPAR_7[] PROGMEM = "ДАТЧИК ШУМА"; //7  noise_censor
const char stringPAR_8[] PROGMEM = "ДАТЧИК ОСВЕЩЁННОСТИ"; // Illumination_censor
const char stringPAR_9[] PROGMEM = "ДАТЧИК ГАЗА CO"; //9  gas_co_censor
const char stringPAR_10[] PROGMEM = "ДАТЧИК ГАЗА CO2"; //10 gas_co2_censor
const char stringPAR_11[] PROGMEM = "ДАТЧИК МЕТАНА"; //11 gas_metan_censor 
const char stringPAR_12[] PROGMEM = "ДАТЧИК КУРЕНИЯ"; //12  smoke_censor
const char stringPAR_13[] PROGMEM = "ДАТЧИК ОГНЯ"; //13  fire_censor
const char stringPAR_14[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ТЕЛА"; //14  body_temperature_censor
const char stringPAR_15[] PROGMEM = "ДАТЧИК ОТРКРЫТИЯ ЖАЛЮЗИ"; //15  jalousie_status_censor 
const char stringPAR_16[] PROGMEM = "ДАТЧИК ПАРА"; //16 steam_censor
const char stringPAR_17[] PROGMEM = "ДАТЧИК ИЗЛУЧЕНИЯ МИКРОВОЛНОВОЙ"; //17 microwave_censor
const char stringPAR_18[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ХОЛОДИЛЬНИКА"; //18 fridge_temperature_censor
const char stringPAR_19[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ МОРОЗИЛКА"; // 19 freezer_temperature_censor
const char stringPAR_20[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ДУХОВКА"; // 20 oven_temperature_censor
const char stringPAR_21[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ЭЛЕКТРОЩИТА"; // 21 electrical_board_censor
const char stringPAR_22[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ЗАМКА ДВЕРИ"; // 22 door_lock_censor
const char stringPAR_23[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ"; // 23 door_close_censor
const char stringPAR_24[] PROGMEM = "ДАТЧИК ПРОХОДА 1"; // 24 motion_door1_censor
const char stringPAR_25[] PROGMEM = "ДАТЧИК ПРОХОДА 2"; //25 motion_door2_censor
const char stringPAR_26[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ОКНА"; // 26 window_close_censor
const char stringPAR_27[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ГОРЯЧЕЙ ВОДЫ"; // 27 water_hot_temperature_censor 
const char stringPAR_28[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ ХОЛОДНОЙ ВОДЫ"; // 28 water_cold_temperature_censor
const char stringPAR_29[] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ГОРЯЧЕЙ ВОДЫ"; // 29 water_hot_pressure_censor
const char stringPAR_30[] PROGMEM = "ДАТЧИК ДАВЛЕНИЯ ХОЛОДНОЙ ВОДЫ"; // 3 0water_cold_pressure_censor
const char stringPAR_31[] PROGMEM = "ДАТЧИК ПРОТЕЧКИ"; // 31 leakage_censor
const char stringPAR_32[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ АКВАРИУМА"; //46 lamp_aquarium_censor
const char stringPAR_33[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ АКВАРИУМА"; //47 aquarium_temperature_censor
const char stringPAR_34[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ НАСОСА АКВАРИУМА"; //48 aquarium_pump_censor
const char stringPAR_35[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ФИЛЬТРА ОЧИСТКИ АКВАРИУМА"; //49 aquarium_clearing_censor
const char stringPAR_36[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ ШКАФА";  //36 cupboard_door_censor
const char stringPAR_37[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ СЕЙФА"; //37 safe_door_censor
const char stringPAR_38[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ БАЛКОНА"; //38 balcony_door_censor
const char stringPAR_39[] PROGMEM = "ДАТЧИК ЗАКРЫТИЯ ДВЕРИ ЛОДЖИИ"; // 39 loggia_door_censor
const char stringPAR_40[] PROGMEM = "ДАТЧИК ТЕМПЕРАТУРЫ БАТАРЕИ"; //40 radiator_temperature_censor
const char stringPAR_41[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ НА ПОТОЛКЕ"; //41 lamp_ceiling_censor
const char stringPAR_42[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ НАСТЕННОЙ ЛАМПЫ"; //42 lamp_wall_censor
const char stringPAR_43[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ ЛАМПЫ ПОДСВЕТКИ"; //43 lamp_Illumination_censor
const char stringPAR_44[] PROGMEM = "ДАТЧИК ВКЛЮЧЕНИЯ АВАРИЙНАЯ ЛАМПЫ"; //44 lamp_accident_censor
const char stringPAR_45[] PROGMEM = "ДАТЧИК ВЛАЖНОСТИ ЗЕМЛИ"; //45 earth_humidity_censor
const char stringPAR_46[] PROGMEM = "СЧЕТЧИК ГАЗА"; //32 counter_gas_censor
const char stringPAR_47[] PROGMEM = "";                    //47 сервис байт
const char stringPAR_48[] PROGMEM = "СЧЕТЧИК ГОРЯЧЕЙ ВОДЫ"; // 33 counter_water_hot_censor
const char stringPAR_49[] PROGMEM = "";                     //49 сервис байт
const char stringPAR_50[] PROGMEM = "СЧЕТЧИК ХОЛОДНОЙ ВОДЫ"; // 34 counter_water_hot_censor
const char stringPAR_51[] PROGMEM = "";                    //51 сервис байт
const char stringPAR_52[] PROGMEM = "СЧЕТЧИК ЭЛЕКТРОЭНЕРГИИ"; // 35 counter_electricity_censor
const char stringPAR_53[] PROGMEM = "";                    //53 сервис байт
const char stringPAR_54[] PROGMEM = "";                    //54 сервис байт
const char stringPAR_55[] PROGMEM = "";                    //55 сервис байт

const char stringPAR_56[] PROGMEM = "НАПРЯЖЕНИЕ АКБ ИБП"; // 35 counter_electricity_censor
const char stringPAR_57[] PROGMEM = "";                    //57 сервис байт
const char stringPAR_58[] PROGMEM = "";                    //58 сервис байт
const char stringPAR_59[] PROGMEM = "";                    //59 сервис байт


const char* const string_tablePAR[] PROGMEM = {stringPAR_0, stringPAR_1, stringPAR_2, stringPAR_3, stringPAR_4,   // здесь массив строк отчетов
stringPAR_5,  stringPAR_6,  stringPAR_7,  stringPAR_8,  stringPAR_9, stringPAR_10,stringPAR_11,stringPAR_12,stringPAR_13, 
stringPAR_14, stringPAR_15, stringPAR_16, stringPAR_17, stringPAR_18,stringPAR_19,stringPAR_20,stringPAR_21,stringPAR_22, 
stringPAR_23, stringPAR_24, stringPAR_25, stringPAR_26, stringPAR_27,stringPAR_28,stringPAR_29,stringPAR_30,stringPAR_31,
stringPAR_32, stringPAR_33, stringPAR_34, stringPAR_35, stringPAR_36,stringPAR_37,stringPAR_38,stringPAR_39,stringPAR_40,
stringPAR_41, stringPAR_42, stringPAR_43, stringPAR_44, stringPAR_45,stringPAR_46,stringPAR_47,stringPAR_48,stringPAR_49,
stringPAR_50, stringPAR_51, stringPAR_52, stringPAR_53, stringPAR_54,stringPAR_55,stringPAR_56,stringPAR_57,stringPAR_58,
stringPAR_59};

const char stringDEV_0[] PROGMEM = "NULL DEVICE";
const char stringDEV_1[] PROGMEM = "ЛАМПА НА ПОТОЛКЕ БУЛЕВАЯ"; //64 lamp_ceiling_onoff_device
const char stringDEV_2[] PROGMEM = "ЛАМПА НАСТЕННАЯ БУЛЕВАЯ"; //65 lamp_wall_onoff_device
const char stringDEV_3[] PROGMEM = "ЛАМПА ПОДСВЕТКИ БУЛЕВАЯ"; //66 lamp_Illumination_onoff_PARice
const char stringDEV_4[] PROGMEM = "ЛАМПА АВАРИЙНАЯ БУЛЕВАЯ"; //67 lamp_accident_onoff_device
const char stringDEV_5[] PROGMEM = "ЛАМПА НА ПОТОЛКЕ ДИММЕР"; //68 lamp_ceiling_dimm_device
const char stringDEV_6[] PROGMEM = "ЛАМПА НА СТЕНЕ ДИММЕР"; //69 lamp_wall_dimm_device 
const char stringDEV_7[] PROGMEM = "ЛАМПА ПОДСВЕТКИ ДИММЕР"; //70 lamp_Illumination_dimm_device
const char stringDEV_8[] PROGMEM = "ЛАМПА АВАРИЙНАЯ ДИММЕР"; //71 lamp_accident_dimm_device
const char stringDEV_9[] PROGMEM = "ВЕНТИЛЯТОР"; // 72 fan 
const char stringDEV_10[] PROGMEM = "РЕКУПЕРАТОР";//73 recuperator
const char stringDEV_11[] PROGMEM = "КОНДИЦИОНЕР"; // 74 air_conditioning 
const char stringDEV_12[] PROGMEM = "КЛАПАН РАДИАТОРА ОТОПЛЕНИЯ"; // 75 valve_radiator
const char stringDEV_13[] PROGMEM = "ЖАЛЮЗИ"        ; // 76 jalousie
const char stringDEV_14[] PROGMEM = "КРАН ГАЗА"      ; // 77 valve_gas 
const char stringDEV_15[] PROGMEM = "КРАН ГОРЯЧЕЙ ВОДЫ"  ; // 78 valve_hot_water
const char stringDEV_16[] PROGMEM = "КРАН ХОЛОДНОЙ ВОДЫ"   ; //79 valve_cold_water
const char stringDEV_17[] PROGMEM = "ИК ПРИЕМОПЕРЕДАТЧИК IRDA"               ; // 80 irda
const char stringDEV_18[] PROGMEM = "ОСВЕЖИТЕЛЬ ВОЗДУХА"            ; // 81 freshener
const char stringDEV_19[] PROGMEM = "УВЛАЖНИТЕЛЬ ВОЗДУХА"           ; // 82 humidifier 
const char stringDEV_20[] PROGMEM = "ЗАМОК ДВЕРИ МАГНИТНЫЙ"      ; // 83 door_lock_magnet
const char stringDEV_21[] PROGMEM = "ЗАМОК ДВЕРИ ЭЛЕКТРОМАГНИТНЫЙ"     ; //84 door_lock_electromagnet 
const char stringDEV_22[] PROGMEM = "ЗАМОК ДВЕРИ ЭЛЕКТРОМЕХАНИЧЕСКИЙ"     ;  //85 door_lock_electromechanical
const char stringDEV_23[] PROGMEM = "ЗАМОК ДВЕРИ BLINK"        ; // 86  door_lock_blink 
const char stringDEV_24[] PROGMEM = "СИРЕНА"                  ; // 87 siren
const char stringDEV_25[] PROGMEM = "СТРОБОСКОП МИГАЛКА";  // 88 blinker 
const char stringDEV_26[] PROGMEM = "МУЗЫКА";  // 89 music
const char stringDEV_27[] PROGMEM = "ТВ"; // 90 tv
const char stringDEV_28[] PROGMEM = "РАДИО"; // 91 radio 
const char stringDEV_29[] PROGMEM = "РОЗЕТКА";  // 92 power_socket 

const char* const string_tableDEV[] PROGMEM = {stringDEV_0, stringDEV_1, stringDEV_2, stringDEV_3, stringDEV_4,   // здесь массив строк отчетов
stringDEV_5,  stringDEV_6,  stringDEV_7,  stringDEV_8,  stringDEV_9, stringDEV_10,stringDEV_11,stringDEV_12,stringDEV_13, 
stringDEV_14, stringDEV_15, stringDEV_16, stringDEV_17, stringDEV_18,stringDEV_19,stringDEV_20,stringDEV_21,stringDEV_22, 
stringDEV_23, stringDEV_24, stringDEV_25, stringDEV_26, stringDEV_27,stringDEV_28,stringDEV_29};


const char stringCOM_0[]  PROGMEM = "БУЛЕВОЕ ВЫКЛ";
const char stringCOM_1[]  PROGMEM = "БУЛЕВОЕ ВКЛ";
const char stringCOM_2[]  PROGMEM = "БУЛЕВОЕ ИНВЕРТИРОВАТЬ";  
const char stringCOM_3[]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УСТАНОВИТЬ"; 
const char stringCOM_4[]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УМЕНЬШИТЬ";
const char stringCOM_5[]  PROGMEM = "ЗНАЧЕНИЕ ДИММЕРА УВЕЛИЧИТЬ";    
const char stringCOM_6[]  PROGMEM = "ИМПУЛЬСНОЕ ВКЛ";
const char stringCOM_7[]  PROGMEM = "ИМПУЛЬСНОЕ ВЫКЛ"; 
const char stringCOM_8[]  PROGMEM = "ИМПУЛЬСНОЕ ИНВЕРТИРОВАТЬ"; 
const char stringCOM_9[]  PROGMEM = "ЗНАЧЕНИЕ PWM УСТАНОВИТЬ"; 
const char stringCOM_10[]  PROGMEM = "ЗНАЧЕНИЕ PWM УМЕНЬШИТЬ";
const char stringCOM_11[]  PROGMEM = "ЗНАЧЕНИЕ PWM УВЕЛИЧИТЬ"; 
const char stringCOM_12[]  PROGMEM = "УСТАНОВИТЬ ЗНАЧЕНИЕ ПАРАМЕТРА"; 


const char* const string_tableCOM[] PROGMEM = {stringCOM_0, stringCOM_1, 
stringCOM_2, stringCOM_3, stringCOM_4,  stringCOM_5, stringCOM_6,
stringCOM_7, stringCOM_8, stringCOM_9, stringCOM_10, stringCOM_11, stringCOM_12};





void PrinT (byte Str_nomer,  byte subject) {

  #ifdef ARD_DUE
     if (subject == TypeDEV) {if (Str_nomer<SIZE_DEVICE_ARRAY) Serial.print (string_tableDEV[Str_nomer]);  else Serial.print (F("Неизвестное устройство  "));}
else if (subject == TypeMES) {if (Str_nomer<size_Message_ENUM) Serial.print (string_tableMES[Str_nomer]);   else Serial.print (F("Неизвестное сообщение  "));}
else if (subject == ADDR)    {if (Str_nomer<NODS_QUANTITY)       Serial.print (string_tableADDR[Str_nomer]);  else Serial.print (F("Неизвестный адрес  "));}
else if (subject == PAR)     {if (Str_nomer<SIZE_PARAM_ARRAY)  Serial.print (string_tablePAR[Str_nomer]);   else Serial.print (F("Неизвестный параметр  "));}
else if (subject == COM)     {if (Str_nomer<Command_enum_SIZE) Serial.print (string_tableCOM[Str_nomer]);   else Serial.print (F("Неизвестная команда  "));}
 
  #else
bool ok = 1;
     if (subject == TypeDEV) {if (Str_nomer<SIZE_DEVICE_ARRAY) strcpy_P(buf, (char*)pgm_read_word(&(string_tableDEV[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестное устройство  "));}}
else if (subject == TypeMES) {if (Str_nomer<size_Message_ENUM) strcpy_P(buf, (char*)pgm_read_word(&(string_tableMES[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестное сообщение  "));}}
else if (subject == ADDR)    {if (Str_nomer<NODS_QUANTITY)       strcpy_P(buf, (char*)pgm_read_word(&(string_tableADDR[Str_nomer])));  else {ok = 0; Serial.print (F("Неизвестный адрес  "));}}
else if (subject == PAR)     {if (Str_nomer<SIZE_PARAM_ARRAY)  strcpy_P(buf, (char*)pgm_read_word(&(string_tablePAR[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестный параметр  "));}}
else if (subject == COM)     {if (Str_nomer<Command_enum_SIZE) strcpy_P(buf, (char*)pgm_read_word(&(string_tableCOM[Str_nomer])));   else {ok = 0; Serial.print (F("Неизвестная команда  "));}}
if (ok) Serial.print (buf);
#endif
}
      

    


// ниже функция контроля настройки таймаутов ддя долгих команд

void timeoutsConfigControl (){
  
  bool alarmTimeout = 0;
  bool alarmDEV = 0;
  bool alarm = 0;
for (int i = 0; i<device_quantity; i++) {   
  if ((device [i][DEVICE_VID] & B10000000)>>7){
    
    alarmTimeout = 1; alarmDEV = 1; 
    for (int j = 0 ; j < TIMEOUTS_NUMBER; j++) {if (timeoutsArray [j][DEV_TYPE] == device [i][0]) {alarmTimeout = 0; } }
    
    if (alarmDEV && alarmTimeout) alarm = 1;
    alarmTimeout = 0; alarmDEV = 0;
    }
  
  
  }

  Serial.println();
 if  (!alarm) Serial.println (F("Таймауты для долгих устройств сконфигурированы успешно!"));
 else Serial.println (F("Неправильно сконфигурированы таймауты для долгих устройств!!!!!!"));
  }

void parametersConfigControl (){
  bool alarm = 0;
  for (int i = 0; i < parameters_quantity; i++ ){
byte compare_type = parameter[i][PARAMETER_TYPE];
byte compare_sens = parameter[i][PARAMETER_SENS];
    
for (int k = 0 ; k < parameters_quantity; k++){if (k!=i && parameter[k][PARAMETER_TYPE]==compare_type && parameter[k][PARAMETER_SENS]==compare_sens) alarm = 1; }
   
   }
    
    Serial.println();
 if  (!alarm) {Serial.print (F("Массив параметров сконфигурирован успешно! Количество параметров: ")); Serial.println(parameters_quantity);}
 else Serial.println (F("Неправильно сконфигурирован массив параметров!!!!!!  На одном типе параметров найдены датчики с одинаковым номером!"));
    
    
    
    }
  
 
#endif
