
// В ЭТОМ ФАЙЛЕ СОЗДАЕТСЯ КАК БЫ КАРКАС УМНОГО ДОМА. А ИМЕННО - НАЗВАНИЯ (АДРЕСА) УЗЛОВ, ПАРАМЕТРОВ(ДАННЫЕ С ДАТЧИКОВ) И ДЕВАЙСОВ (ИСП. УСТРОЙСТВ). ЭТОТ ФАЙЛ БУДЕТ ЕДИНЫЙ ДЛЯ ВСЕХ УЗЛОВ.
// ПОЭТОМУ ФАЙЛ ЗАПОЛНЯЕМ ОДИН РАЗ И МОЖНО ЕГО КОПИРОВАТЬ СРАЗУ ДЛЯ ВСЕХ УЗЛОВ.

//ЗДЕСЬ ЗАДАЁМ СПИСОК НАЗВАНИЙ (АДРЕСОВ) ВСЕХ ВОЗМОЖНЫХ УЗЛОВ, ИСПОЛЬЗУЕМЫХ В СИСТЕМЕ  SmartHomeCAN

enum Node_addr_enum
{
   broadcast,                            //0
    node_1_Net_center_PC,                 //1
    node_2_Net_center_oraPi1,             //2
    node_3_Net_center_oraPi2,             //3 
    node_4_Net_center_Due1,               //4
    node_5_Net_center_Due2,               //5
    node_6_Hallway_net_center,            //6
    node_7_Hallway_main,                  //7
    node_8_Hallway_light,                 //8
    node_9_Kitchen_net_center,            //9
    node_10_Kitchen_main,                  //10
    node_11_Kitchen_light,                 //11
    node_12_WC_main,                       //12
    node_13_WC_waterleak,                  //13
    node_14_Bathroom_main,                 //14
    node_15_Boxroom_main,                  //15
    node_16_Balcony_meteo,                 //16                          
    node_17_Loggia_main,                   //17                             
    node_18_Loggia_recuperator,            //18                             
    node_19_Livingroom_main,               //19                             
    node_20_Bedroom_main,                  //20                             
    node_21_Cabinet_main,                  //21                             
    NODS_QUANTITY                          //22 size    
};               

//ЗДЕСЬ ЗАДАЁМ СПИСОК НАЗВАНИЙ (АДРЕСОВ) ВСЕХ УЗЛОВ, КАКИЕ В ДАННЫЙ МОМЕНТ ПОДКЛЮЧЕНЫ К CAN СЕТИ (КОМПЛЕКТАЦИЯ CAN СЕТИ из списка выше)
//ДАННАЯ КОНФИГУРАЦИЯ БУДЕТ ИСПОЛЬЗОВАТЬСЯ ТОЛЬКО МАСТЕРОМ

#ifdef type_node_master
const bool NodeCANpresence[NODS_QUANTITY] = 
{
  0, //  broadcast,                            //0
  0, //  node_1_Net_center_PC,                 //1
  0, //  node_2_Net_center_oraPi1,             //2
  0, //  node_3_Net_center_oraPi2,             //3 
  1, //  node_4_Net_center_Due1,               //4
  0, //  node_5_Net_center_Due2,               //5
  0, //  node_6_Hallway_net_center,            //6
  1, //  node_7_Hallway_main,                  //7
  0, //  node_8_Hallway_light,                 //8
  0, //  node_9_Kitchen_net_center,            //9
  1, //  node_10_Kitchen_main,                  //10
  0, //  node_11_Kitchen_light,                 //11
  0, //  node_12_WC_main,                       //12
  0, //  node_13_WC_waterleak,                  //13
  0, //  node_14_Bathroom_main,                 //14
  0, //  node_15_Boxroom_main,                  //15
  0, //  node_16_Balcony_meteo,                 //16                          
  0, //  node_17_Loggia_main,                   //17                             
  0, //  node_18_Loggia_recuperator,            //18                             
  0, //  node_19_Livingroom_main,               //19                             
  0, //  node_20_Bedroom_main,                  //20                             
  0, //  node_21_Cabinet_main,                  //21                             
    };
#endif 


// ЗДЕСЬ ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ НАЗВАНИЯ (АДРЕСА) ПАРАМЕТРОВ, ПРИМЕНЯЕМЫХ В УМНОМ ДОМЕ

enum Parameter_addr_enum
{
  null_param,                     //0 
  air_t                       ,   //1 //датчик температуры
  air_h                       ,   //2 //датчик влажности
  air_p                       ,   //3 //датчик давления
  mot_pir                     ,   //4 // 
  moti_mw                     ,   //5  //датчик движения
  vibrat                      ,   //6 //датчик вибрации
  noise                       ,   //7   //датчик шума
  Illum                       ,   //8 //датчик освещенности
  gas_co                      ,   //9  //датчик СО
  gas_co2                     ,   //10 //датчик СО2
  gas_met                     ,   //11 //датчик метана
  smoke                       ,   //12  //датчик курения
  fire                        ,   //13  //датчик Огня
  body_temp                   ,   //14  //Температура тела
  jal_stat                    ,   //15  //Концевик Жалюзи
  steam                       ,   //16 //Датчик Пара
  microwave                   ,   //17 //Излучение микроволновки
  fridge_temp                 ,   //18 //Холодильник
  freezer_temp                ,   //19 //Морозилка
  oven_temp                   ,   //20 //Духовка
  elec_board                  ,   //21 //Электрощит концевик
  door_lock                   ,   //22 //Замок двери
  door_close                  ,   //23 //Концевик двери
  mot_door1                   ,   //24 //Датчик прохода 1
  mot_door2                   ,   //25 //Датчик прохода 2
  wind_close                  ,   //26 //Закрыто
  water_hot_temp              ,   //27 //Температура горячей воды
  water_cold_temp             ,   //28 //Температура холодной воды
  water_hot_pres              ,   //29 //Давление горячей воды
  water_cold_pres             ,   //30 //Давление холодной воды
  leakage                     ,   //31 //Протечка
  aquar_lamp                  ,   //32 //Лампа аквариум
  aquar_temp                  ,   //33 //Температура аквариум
  aquar_pump                  ,   //34 //Насос аквариум
  aquar_clearing              ,   //35 //Очистка аквариум
  cupboard_door               ,   //36 //Дверь шкафа
  safe_door                   ,   //37 //Дверь сейфа
  balcony_door                ,   //38 //Дверь балкона
  loggia_door                 ,   //39 //Дверь лоджии
  radiator_temp_S             ,   //40 //Температура батареи
  lamp_ceiling                ,   //41 //Лампа на потолке
  lamp_wall                   ,   //42 //Ланпа настенная
  lamp_Illum                  ,   //43 //Лампа подсветки
  lamp_accident               ,   //44 //Лампа аварийная
  earth_humidity              ,   //45 //Влажность земли
  counter_gas_2               ,   //46 //Счетчик газа
  counter_gas_2_SB2           ,   //47                 сервисный байт
  counter_water_hot_2         ,   //48 //Счетчик горячей воды
  counter_water_hot_2_SB2     ,   //49                 сервисный байт
  counter_water_cold_2        ,   //50 //Счетчик холодной воды
  counter_water_cold_2_SB2    ,   //51                 сервисный байт      
  counter_elect_4             ,   //52 //Счетчик электричества
  counter_elect_4_SB2         ,   //53                 сервисный байт      
  counter_elect_4_SB3         ,   //54                 сервисный байт      
  counter_elect_4_SB4         ,   //55                 сервисный байт        
  ups_voltage_4F              ,   //56 //напряжение АКБ бесперебойника
  ups_voltage_4F_SB2          ,   //57                 сервисный байт      
  ups_voltage_4F_SB3          ,   //58                 сервисный байт      
  ups_voltage_4F_SB4          ,   //59                 сервисный байт    
  SIZE_PARAM_ARRAY                //60 size  // общее количество парамтеров, применяемых в умном доме
}; 


//ЗДЕСЬ ЗАДАЁМ ВСЕ ВОЗМОЖНЫЕ ИСПОЛНИТЕЛЬНЫЕ УСТРОЙСТВА, КОТОРЫЕ МОГУТ ПРИСУТСТВОВАТЬ В СИСТЕМЕ УМНЫЙ ДОМ 

enum Device_addr_enum
{
  null_device           ,// 0 
  lamp_ceil_onoff_d     ,// 1 //Лампа на потолке вкл/выкл
  lamp_wall_onoff_d     ,// 2 //Ланпа настенная вкл/выкл
  lamp_Illum_onoff_d    ,// 3 //Лампа подсветки вкл/выкл
  lamp_accident_onoff_d ,// 4 //Лампа аварийная вкл/выкл
  lamp_ceiling_dimm_d   ,// 5 //Лампа на потолке диммируемая
  lamp_wall_dimm_d      ,// 6 //Ланпа настенная диммируемая
  lamp_Illum_dimm_d     ,// 7 //Лампа подсветки диммируемая
  lamp_accid_dimm_d     ,// 8 //Лампа аварийная диммируемая
  fan_d                 ,// 9 
  recup_d               ,// 10
  air_cond_d            ,// 11
  valve_radiator_d      ,// 12
  jalousie_d            ,// 13
  valve_gas_d           ,// 14
  valve_hot_water_d     ,// 15
  valve_cold_water_d    ,// 16
  irda_d                ,// 17
  freshener_d           ,// 18 //Освежитель
  humidifier_d          ,// 19 //Увлажнитель
  door_lock_magnet_d    ,// 20
  door_lock_elmagnet_d  ,// 21
  door_lock_elmech_d    ,// 22
  door_lock_blink_d     ,// 23
  siren_d               ,// 24
  blinker_d             ,// 25
  music_d               ,// 26
  tv_d                  ,// 27
  radio_d               ,// 28
  power_socket_d        ,// 29 //Розетки
  
SIZE_DEVICE_ARRAY        // 30;  // КОЛИЧЕСТВО ИСПОЛНИТЕЛЬНЫХ МЕХАНИЗМОВ
};
