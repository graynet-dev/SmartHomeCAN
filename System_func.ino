

void CAN_Start ()                  // это setup
{
  MCP2515_Init ();
  pinmode();                       // настройка портов (в зависимости от конфигурации массива device)
  Setup  = 1;
}


void CAN_Refresh ()                // это loop
{              
  curMillis = millis();            // снимок системного времени

  RX();                            // чтение шины CAN

  #ifdef type_node_master
    StatusControl();               // контроль статусов node_mk - заполнение массива StatusNode_OK[NODS_QUANTITY] (только для мастера)
  #endif 
  #if defined (type_node_slave) or defined (type_node_mk)

    DataStreamSend();              // отправка параметров в CAN по факту изменения параметра и периодически (только для подчинённых)
  #endif

  SendCommand_queue();             // отправка команд из очереди

  LongCommandExecuting_buffer();   // отправка отчётов о долгих командах из очереди

  timers();                        // таймеры 

  test ();                         // это так для теста инициирование отправки CAN команд через сериал монитор, потом убрать можно

}



void MCP2515_Init () 
{

byte SpeedCan = CAN_125KBPS;

#ifdef debug
if (!Setup) {Serial.begin(115200);
Serial.println(); Serial.print(F("Version ")); Serial.print(VER); Serial.print(F("  ")); 
PrinT (node_address, ADDR); 
#ifdef type_node_master
Serial.print (F("   MASTER!  "));
#endif
Serial.print(F("  ")); SpeedCanPrint (SpeedCan); Serial.println();}
#endif
    SPI.begin();
    SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));
    digitalWrite(csPIN, LOW);
    SPI.transfer(0xC0);
    digitalWrite(csPIN, HIGH);
    SPI.endTransaction();
    delay (200);
  bool can_ok = 0;
  
  if(can.begin(MCP_STDEXT, SpeedCan, MCP_8MHZ) == CAN_OK) can_ok = 1;
  can.init_Mask(0,1,0x1000FF00);                       // Init first mask...
  can.init_Filt(0,1,((node_address & 0xFFFFFFFF)<<8)|0x10000000);   // Init first filter...
  can.init_Filt(1,1,0x10000000);                       // Init second filter...
  can.init_Mask(1,1,0x1000FF00);                       // Init second mask...   
  can.init_Filt(2,1,0x10000000);                       // Init 3 filter...
  can.init_Filt(3,1,0x10000000);                       // Init 4 filter...
  can.init_Filt(4,1,0x10000000);                       // Init 5 filter...
  can.init_Filt(5,1,0x10000000);                       // Init 6 filter...
#ifdef debug

      if (can_ok) Serial.println(F("MCP2515 Initialized Successfully!"));
    else
                  Serial.println(F("Error Initializing MCP2515..."));
                  
Serial.println(); Serial.print(F("РЕСТАРТ MCP2515 ")); 

Serial.println();
if (!Setup) {
  Serial.println(F("РЕБУТ АРДУИНО")); 
  Serial.println();  
  timeoutsConfigControl ();
  parametersConfigControl ();
  }

#endif 
 can.setMode(MCP_NORMAL);    
}



//ниже функция конфигурирования портов ардуино , в случае укомплетованности узла тем или иным устройством и факта подсоединения к пину ардуино
void pinmode() 
{
  timerPeriodSendParameters = 0-((uint32_t)interval_sendParam-node_address)*1500ul; // для таймера периодической отправки параметров в кан
  pinMode(CAN0_INT, INPUT);               
  for (int i = 0 ; i < device_quantity; i++) 
 { 
    pinMode (device[i][DEVICE_PIN], OUTPUT);
    if ((device[i][DEVICE_VID] & 0xF) == IMPULSE_GND || (device[i][DEVICE_VID]& 0xF) == IMPULSE_POWER) 
      {
        pinMode ( device[i][DEVICE_STATUS_PIN], INPUT_PULLUP); 
      }
 }
}



#ifdef type_node_master
void StatusControl () {

//создаем тикающий таймер раз в интервал
//по окончании которого, будут прибавляться счетчики неответов от узлов
//PS эти счетники будут сбрасываться в "0", если в фунции RX() принят ответ от узла
  
 if  (curMillis -prevtimeStatus > intervalStatus*1000) {


StatusTX(broadcast);
//#ifdef debugStatus
//Serial.println(); Serial.println (F("  NODES CAN COMMUNICATION:")) ; 
//#endif 

 // ниже прибавляем счетчики неответов от узлов, если они ещё живы конечно
 for (int i = 0; i<NODS_QUANTITY; i++) {if (StatusNode_OK[i] && NodeCANpresence [i]!=0) countNoTAnswerNode[i]++; 
//#ifdef debug
//#ifdef debugStatus
//if (NodeCANpresence [i]!=0 && i!= node_address){
//PrintADDR (i);
//if (!StatusNode_OK[i]) Serial.println (F("  FAIL!!!")) ;
//else Serial.println (F("  OK!!!")) ;}
//#endif
//#endif                                                  
  }
    prevtimeStatus = curMillis;}

//ниже если счетчик превысил 5 
for (int i = 0; i<NODS_QUANTITY; i++) {if (NodeCANpresence [i]!=0 && i!= node_address && StatusNode_OK[i] && countNoTAnswerNode[i] > MAX_NoTAnswerCOUNT) {StatusNode_OK[i] = false; SendAccident(i);}}   // статус узла - мёртв

}
  #endif 


void timers() 
{

#ifdef type_node_master
if (curMillis - prevSystemtime > 1000) {ss++; if (ss >=60) {ss = 0; mm++; if (mm >=60) {mm = 0; hh++; if (hh>=24) {dd++; hh=0;}}} prevSystemtime = curMillis;} // это тикание системных часов на миллис 

if (curMillis >= 30000 && !flag_alarmAccident) {flag_alarmAccident = 1;

for (int i = 0; i<NODS_QUANTITY; i++) if (!StatusNode_OK[i] && i!= node_address && NodeCANpresence [i]!=0) SendAccident(i);}

#ifdef debug
if ((ss == 30 || ss == 0) && !printstatus) {PrintSystemTime(); 
if (errors){readErrorFlags_MCP2515 ();} 

#ifdef debugStatus
PrintStatus(); 
#endif
printstatus = 1;}

if (ss != 30 && ss != 0) printstatus = 0;
#endif

#endif 
#if defined (type_node_slave) or defined (type_node_mk) 

//ниже если это подчинённый , то шлём наш статус мастеру, через 30мс*адрес_узла после того как получили статус от мастера 

if  (TimerStatus && curMillis - prevtimeStatus > node_address*30) {
StatusTX(SendAddrStatus_master);   
TimerStatus = 0;}
#endif 

// ниже таймер, в течение которого происходит игнорирование поступающих одинаковых команд с одинаковым глобальным счетчиком
if (timercountRepeat_ON && curMillis - timercountRepeat > TX_repeatPeriod*(maxNumberRepeat_TX+1)) timercountRepeat_ON = 0; 

// ниже таймер создания импульса управления импульсными девайсами
for (int i = 0; i< IMPULSEDEVICE_QUANTITY; i++){
if (TimerImpulse_ON[i]>0 && (curMillis - timerImpulse[i] > IMPULSE_DURATION * 100)){
digitalWrite(TimerImpulse_ON[i], !digitalRead(TimerImpulse_ON[i]));
  TimerImpulse_ON[i] = 0;
  }}

//ниже для функционирования управления импульсными устройствами. Это мониторинг статуса устройства при помощи входов: ВКЛ оно или ВЫКЛ
  for (int i = 0; i < device_quantity; i++) 
  { 
      if ((device[i][DEVICE_VID] & 0xF) == IMPULSE_GND || (device[i][DEVICE_VID] & 0xF) == IMPULSE_POWER) 
        {
            device [i][DEVICE_VALUE] = !digitalRead ( device[i][DEVICE_STATUS_PIN]);
        }
  }

}


//ниже значения таймаутов на выполнение команды , сек
  
byte timeout(byte dev_Type) {
  
for (int i=0; i<TIMEOUTS_NUMBER; i++){ if (dev_Type == timeoutsArray[i][DEV_TYPE]) return timeoutsArray[i][TIMEOUT];}

return 15; // если таймаут не найден в базе, делаем его по умолчанию 15 секунд
#ifdef debug
Serial.println(); Serial.println(F("При попытке воспользоваться таймаутом ожидания выполнения долгой команды, он НЕ НАЙДЕН в базе (в массиве)!!! Таймаут установлен по умолчанию 15 секунд"));
#endif
                            }


// ниже функция по адресу параметра находит его порядковый номер в массиве
byte param_addr (byte AdDr, byte SensNum){for (int h = 0 ; h < parameters_quantity; h++) {if (AdDr == parameter [h][PARAMETER_TYPE]&& SensNum == parameter [h][PARAMETER_SENS]) return h; } return 0xFF;}

// ниже функция по адресу девайса находит его порядковый номер в массиве
byte device_addr (byte AdDr){for (int h = 0 ; h < device_quantity; h++) {if (AdDr == device [h][DEVICE_TYPE]) return h; } return 0xFF;}




void test ()
{
if (Serial.available()>0){
    byte inByte;
    inByte = Serial.read () - '0';

     if (inByte == 1) SendCommand (1, PWM_TURN_ON,  20, 0,0,0,node_10_Kitchen_main, lamp_wall_onoff_d , 1);
else if (inByte == 0) SendCommand (1, PWM_TURN_OFF, 20, 0,0,0,node_10_Kitchen_main, lamp_wall_onoff_d, 1);

else if (inByte == 2) SendCommand (1, DIGITAL_REMOTE,  1, 0,0,0, node_10_Kitchen_main, lamp_ceil_onoff_d , 1);
else if (inByte == 3) SendCommand (1, DIGITAL_REMOTE,  0, 0,0,0, node_10_Kitchen_main, lamp_ceil_onoff_d , 1), Serial.println("TRI");
else if (inByte == 4) device [device_addr (lamp_ceiling_dimm_d)][DEVICE_VALUE] = 0;
else if (inByte == 5) SendCommand (1, PARAMETER_WRITE, 45, 0,0,0, node_10_Kitchen_main, air_t, 1);
else if (inByte == 9) MCP2515_Init ();
                           }
}

#ifdef debug
void PrintSystemTime (){
  
  Serial.println(); 
  if (dd<10)  Serial. print (F("0")); 
 Serial. print (dd);  
 Serial. print (F(":")); 
  if (hh<10)  Serial. print (F("0")); 
 Serial. print (hh);  
 Serial. print (F(":")); 
 if (mm<10)  Serial. print (F("0")); 
 Serial. print (mm);  
 Serial. print (F(":")); 
 if (ss<10)  Serial. print (F("0")); 
 Serial. print (ss);  
 Serial. print (F("       "));  
  
  
  
  }

void readErrorFlags_MCP2515 () {
  
SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));  // начало связи по SPI ( скорость , очередность, режим SPI)
digitalWrite(csPIN, LOW);                // выбираем устройство на шине SPI (пин CS нужного устройства в лоу)
SPI.transfer(0x03);                   // команда прочитать регистры MCP2515 в BIN это 00000011
SPI.transfer(0x2D);                   // адрес регистра 6-3 EFGL - Error Flag
byte inByteSPI = SPI.transfer(0x00);  // получение данных из шины SPI
digitalWrite(csPIN, HIGH);               // отпускаем пин CS 
SPI.endTransaction();                 // конец связи по SPI
// ниже разборка полученного байта на биты
    if (inByteSPI != 0) {for (int i=0; i<8; i++) {
           if (i==0) { if (bitRead (inByteSPI, i)) Serial.print (F(" RX or TX errors >= 96   "));}
      else if (i==1) { if (bitRead (inByteSPI, i)) Serial.print (F(" RX errors >= 96   "));}
      else if (i==2) { if (bitRead (inByteSPI, i)) Serial.print (F(" TX errors >= 96   "));}
      else if (i==3) { if (bitRead (inByteSPI, i)) Serial.print (F(" RX errors >= 128. Mode ErrorPassive is ON  "));}
      else if (i==4) { if (bitRead (inByteSPI, i)) Serial.print (F(" TX errors >= 128. Mode ErrorPassive is ON  "));           }
      else if (i==5) { if (bitRead (inByteSPI, i)) Serial.print (F(" TX errors == 255. Mode BusOff is ON  "));           }
      else if (i==6) { if (bitRead (inByteSPI, i)) Serial.print (F(" Переполнен буфер приёма RX0   "));           }
      else if (i==7) { if (bitRead (inByteSPI, i)) Serial.print (F(" Переполнен буфер приёма RX1   ")); Serial.println();     }
       }
    
  }
   else Serial.println (F("NO ErrorFlags"));
Serial.print (F("TX Errors = "));      Serial.print   (readErrors_MCP2515(1)); 
Serial.print (F("     RX Errors = ")); Serial.println (readErrors_MCP2515(0)); 

}


byte readErrors_MCP2515 (bool TXRX) {
SPI.beginTransaction(SPISettings(10000000, MSBFIRST, SPI_MODE0));  // начало связи по SPI ( скорость , очередность, режим SPI)
digitalWrite(csPIN, LOW);                // выбираем устройство на шине SPI (пин CS нужного устройства в лоу)
SPI.transfer(0x03);                   // команда прочитать регистры MCP2515 в BIN это 00000011
if  (TXRX) SPI.transfer(0x1C);                   // адрес регистра 6-1 Счетчик ошибок передачи
else SPI.transfer(0x1D);                         // адрес регистра 6-2 Счетчик ошибок приёма
byte inByteSPI = SPI.transfer(0x00);  // получение данных из шины SPI
digitalWrite(csPIN, HIGH);               // отпускаем пин CS 
SPI.endTransaction();                 // конец связи по SPI
return inByteSPI;
  
  
  }
  


  #ifdef debugStatus
  #ifdef type_node_master
  void PrintStatus (){
  

Serial.println(); Serial.println (F("  NODES CAN COMMUNICATION:")) ; 

 for (int i = 0; i<NODS_QUANTITY; i++) {


if (NodeCANpresence [i]!=0 && i!= node_address){
PrinT (i, ADDR);
if (!StatusNode_OK[i]) Serial.println (F("  FAIL!!!")) ;
else Serial.println (F("  OK!!!")) ;}}}
   #endif 
   #endif  

void SpeedCanPrint (byte SpeedCAN_){
  
  switch (SpeedCAN_){
    case CAN_4K096BPS: 
    Serial.println (F("CAN Speed 4,096 kbps")); break;
    case CAN_5KBPS: 
    Serial.println (F("CAN Speed 5 kbps")); break; 
    case CAN_10KBPS: 
    Serial.println (F("CAN Speed 10 kbps")); break; 
    case CAN_20KBPS: 
    Serial.println (F("CAN Speed 20 kbps")); break; 
    case CAN_31K25BPS: 
    Serial.println (F("CAN Speed 31.25 kbps")); break; 
    case CAN_33K3BPS: 
    Serial.println (F("CAN Speed 33.3 kbps")); break; 
    case  CAN_40KBPS: 
    Serial.println (F("CAN Speed 40 kbps")); break; 
    case CAN_50KBPS: 
    Serial.println (F("CAN Speed 50 kbps")); break; 
    case CAN_80KBPS: 
    Serial.println (F("CAN Speed 80 kbps")); break; 
    case CAN_100KBPS: 
    Serial.println (F("CAN Speed 100 kbps")); break; 
    case CAN_125KBPS: 
    Serial.println (F("CAN Speed 125 kbps")); break; 
    case CAN_200KBPS: 
    Serial.println (F("CAN Speed 200 kbps")); break; 
    case CAN_250KBPS: 
    Serial.println (F("CAN Speed 250 kbps")); break; 
    case CAN_500KBPS: 
    Serial.println (F("CAN Speed 500 kbps")); break; 
    case CAN_1000KBPS: 
    Serial.println (F("CAN Speed 1000 kbps")); break;  }}
   

#endif
