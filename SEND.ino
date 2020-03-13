void SendCommand(bool Priority, byte Command_Type, byte Command_Value, byte Command_Value_2, byte Command_Value_3, byte Command_Value_4, byte Target_Address,  byte Device_Type, byte Sensor_numb){

bool empty_cells = 0; // наличие свободной ячейки в очереди
Command_COUNTER++;
//-----------------------------------------запись в  очередь 
for (int i = 0; i<queueTX_size; i++) {
  if (Command_TX_cell[i][cellState]==0) 
        {empty_cells = 1;
         Command_TX_cell[i][Command_TX_COUNTER] = Command_COUNTER; // запись значения глобального счетчика 
         Command_TX_cell[i][CommandType] =        Command_Type;
         Command_TX_cell[i][CommandValue] =       Command_Value;
         Command_TX_cell[i][CommandValue_2] =     Command_Value_2;
         Command_TX_cell[i][CommandValue_3] =     Command_Value_3;
         Command_TX_cell[i][CommandValue_4] =     Command_Value_4;
         Command_TX_cell[i][target_Address] =     Target_Address;
         Command_TX_cell[i][cellState] = 1; 
         Command_TX_cell[i][PrioritY] =           Priority;
         Command_TX_cell[i][DEVTYPE] =            Device_Type; 
         Command_TX_cell[i][SensNumber] =         Sensor_numb; 
         
         break;
         }
}
#ifdef debug //если при постановке в очередь нет места, пожалуемся в отладку
if (!empty_cells) Serial.println (F("Внимание!!! отсутствует свободная ячейка в очереди на отправку CAN сообщения. Сообщение НЕ отправлено!!!"));
#endif 
  
//------------------------------------------  

}


void SendCommand_queue(){

// ======================================Извлечение из очереди отправки сообщений TX


for (int i = 0; i<queueTX_size; i++) {
  if (Command_TX_cell[i][cellState]==1)  {                       // если нашли занятую ячейку в очереди и статус, что ещё НЕ получили отчёт "команда принята"
        
if (Command_TX_cell[i][flagTimerCommand]==0) {      //если таймер не включен отправляем TX сообщение
   byte DATa[8] = {0};
DATa[CAN_COUNTER]=Command_TX_cell[i][Command_TX_COUNTER];
DATa[CAN_CommandType]   = Command_TX_cell[i][CommandType];
DATa[CAN_CommandValue]  = Command_TX_cell[i][CommandValue];
DATa[CAN_CommandValue_2]= Command_TX_cell[i][CommandValue_2];
DATa[CAN_CommandValue_3]= Command_TX_cell[i][CommandValue_3];
DATa[CAN_CommandValue_4]= Command_TX_cell[i][CommandValue_4];
DATa[CAN_SENSOR_NUM]    = Command_TX_cell[i][SensNumber];
   
  TX  (Command_TX_cell[i][PrioritY], COMMAND, Command_TX_cell[i][target_Address],  Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ; // отправляем команду в CAN    
//  вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, DATa)

Command_TX_cell[i][flagTimerCommand] = 1;     // вклчаем таймер
queueTX_Timer [i] = curMillis; // обнуляем таймер

Command_TX_cell[i][Command_TX_count]++; // прибавляем счетчик отправленных сообщений

} // конец скобки "если таймер не включен отправляем TX сообщение"


else if (Command_TX_cell[i][flagTimerCommand]==1 && (curMillis - queueTX_Timer[i]) > TX_repeatPeriod){  // если таймер былл включен и мы все ещё не получили отчет
  
   byte DATa[5] = {0};
DATa[0]=Command_TX_cell[i][Command_TX_COUNTER];
DATa[3]=Command_TX_cell[i][CommandType];
DATa[4]=Command_TX_cell[i][CommandValue];
  
   TX  (1, COMMAND, Command_TX_cell[i][target_Address], Command_TX_cell[i][DEVTYPE], BIT_29, DATA_CANFRAME, sizeof(DATa), DATa ) ;  // отправляем команду в CAN
  Command_TX_cell[i][Command_TX_count]++;   // прибавляем счетчик отправленных сообщений

  
// если через 5 попыток отправки команды так не пришёл отчет, бросаем это дело -  освобождаем ячейку в очереди
if (Command_TX_cell[i][Command_TX_count]>=maxNumberRepeat_TX) {
// и пожалуемся в отладчик
#ifdef debug
 Serial.println(); Serial.print (F("Ошибка команды  \" ")); PrinT (Command_TX_cell[i][CommandType], COM);
 Serial.print (F(" \"")); Serial.print (F(" на тип устройства ")); PrinT(Command_TX_cell[i][DEVTYPE], TypeDEV);
 Serial.println (F(" Отчёт о принятии команды от оппонента не получен!!!"));
#endif 
for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0; // освобождаем ячейку в очереди
}
else    queueTX_Timer [i] = curMillis;              // иначе обнуляем таймер


                                                              }   
//    delay (4); // эта задержка нужна чтобы сообщения очереди не подряд сыпались, но можно и без неё попробовать   
       
         }

else if (Command_TX_cell[i][cellState]==0x02)  {                       // если нашли занятую ячейку в очереди и статус, что получили отчёт "команда принята, выполняется"

if (Command_TX_cell[i][flagTimerCommand]==0) { // если таймаут ожидания выполнения команды кончился то:

 
  // ТУТ ЧТО-НИБУДЬ ДЕЛАЕМ, ЕСЛИ ОППОНЕНТ КОМАНДУ ПРИНЯЛ, НО ОТЧЁТА ОБ УСПЕЛНОМ ВЫПОЛНЕНИИ КОМАНДЫ ОТ ОППОНЕНТА МЫ ТАК И НЕ ДОЖДАЛИСЬ


#ifdef debug

 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM);
 Serial.print (F(" \"")); Serial.print (F(" на тип устройства ")); PrinT(Command_TX_cell[i][DEVTYPE], TypeDEV); 
 Serial.println (F(" Закончился таймаут, отведённый на выполнение команды. Отчёт об успешном выполнении не получен!!! ")); 
#endif
for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0;

  }                  
else if (Command_TX_cell[i][flagTimerCommand]==1 && curMillis - queueTX_Timer[i] > (uint32_t)timeout(Command_TX_cell[i][DEVTYPE])*1000) // если таймер таймаута был включен и он кончился
{
  Command_TX_cell[i][flagTimerCommand]=0; // выключаем таймер, значит таймаут кончился 
  
  
  }
 


}





}

 
}



void StatusTX (byte addr) 
{
  byte Data[8] = {0};
  #ifdef type_node_master
  Data[0] = Status_COUNTER;
  Data[1] = ss;
  Data[2] = mm;
  Data[3] = hh;
  Data[4] = dd;
  #endif 
  TX (1,STATUS,addr,00,EXTENDED, DATA_CANFRAME, 8, Data);
  // вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, data)
   Status_COUNTER++;
}



#ifdef  type_node_master

void SendAccident (byte AlarmAddr) {
if (NodeCANpresence [AlarmAddr] == 0 || AlarmAddr == node_address) return;
Accident_COUNTER++;
byte Data[8] = {Accident_COUNTER,0,0,0,0,0,0,0};

if (flag_alarmAccident && !alarmAccident_send[AlarmAddr]) { TX (1  , ACCIDENT , MonitoringAddr  , AlarmAddr , EXTENDED ,DATA_CANFRAME , 1, Data); alarmAccident_send[AlarmAddr]=1;}
    }
#endif


#if defined (type_node_slave) or defined (type_node_mk) 



void DataStreamSend(){
 bool Periodical_timer = 0;  

// if (curMillis - timerPeriodSendParameters > (uint32_t)interval_sendParam *1500ul) {Periodical_timer = 1; timerPeriodSendParameters = curMillis; }
if (ss == node_address && !sendparameters) {sendparameters = 1; Periodical_timer = 1;}
if (ss!=node_address) sendparameters = 0;
#ifdef debug
if (Periodical_timer) PrintSystemTime();
#endif

 for (int i = 0; i < parameters_quantity; i++) {
  for (byte p = PERIODIC_CANADDR; p<(PERIODIC_CANADDR+second_dimension-5); p++){
  if ( (parameter [i][TYPE_VAR]& 0xF)!=SERVICE_BYTE && parameter [i][p]!=0xFF){  // если  это не сервисный байт
 byte DLc; byte ok = 0;
       if ((parameter [i][TYPE_VAR] & 0xF)== BYTE_1||(parameter [i][TYPE_VAR] & 0xF)== BYTE_1_SIGNED) {if (ok==0) {ok = 1;} DLc = 5;}
  else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_2||(parameter [i][TYPE_VAR]& 0xF) == BYTE_2_SIGNED) {if (ok==0) {ok = 2;} DLc = 6;}
  else if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4||(parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) {if (ok==0) {ok = 4;} DLc = 8;}
   
     // ниже если параметр поменял своё значение
      if (parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] && ok == 1) ok = CHANGEVALUE;          
else if ((parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER_VALUE]!= parameter [i+1][PARAMETER_LAST]) && ok == 2) ok = CHANGEVALUE;          
else if ((parameter [i][PARAMETER_VALUE]!= parameter [i][PARAMETER_LAST] || parameter [i+1][PARAMETER_VALUE]!= parameter [i+1][PARAMETER_LAST] || parameter [i+2][PARAMETER_VALUE]!= parameter [i+2][PARAMETER_LAST]||
parameter [i+3][PARAMETER_VALUE]!= parameter [i+3][PARAMETER_LAST])&& ok == 4) ok = CHANGEVALUE;           
       
 if (Periodical_timer) ok = PERIODICALLY;
if (ok == PERIODICALLY || ok == CHANGEVALUE){   
   Parameter_COUNTER++;  
   
  byte Data[8]= {Parameter_COUNTER, parameter [i][PARAMETER_SENS], ok, parameter [i][TYPE_VAR], parameter [i][PARAMETER_VALUE], parameter [i+1][PARAMETER_VALUE], parameter [i+2][PARAMETER_VALUE], parameter [i+3][PARAMETER_VALUE]};    
      
      TX(1,PARAMETER, parameter[i][p], parameter [i][PARAMETER_TYPE], EXTENDED, DATA_CANFRAME, DLc, Data);
delay (5);

      
    }}
    
    }
   
//ниже  обновляем прошлое значение параметра

parameter [i][PARAMETER_LAST]= parameter [i][PARAMETER_VALUE];  
if ((parameter [i][TYPE_VAR]& 0xF) != BYTE_1&&(parameter [i][TYPE_VAR]& 0xF) != BYTE_1_SIGNED) parameter [i+1][PARAMETER_LAST]= parameter [i+1][PARAMETER_VALUE]; 
if ((parameter [i][TYPE_VAR]& 0xF) == BYTE_4 || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_SIGNED || (parameter [i][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) 
{parameter [i+2][PARAMETER_LAST]= parameter [i+2][PARAMETER_VALUE]; parameter [i+3][PARAMETER_LAST]= parameter [i+3][PARAMETER_VALUE]; } 
    
                 }
    
    
    }
#endif  



void SendRequestParam (bool Priority, byte Target_Address,  byte Param_Type, const size_t siZE, byte *Sensor_numb) 
{
    byte sensors_quantity;
    if (siZE<8) sensors_quantity = siZE; else sensors_quantity = 7;
    if (Sensor_numb[0]==0) sensors_quantity = 1;
    byte daTa[sensors_quantity+1]; 
    Parameter_COUNTER++; daTa[0]=Parameter_COUNTER;
    for (byte i = 1 ; i<sensors_quantity+1; i++) daTa [i] = Sensor_numb[i-1];
    TX (Priority, PARAMETER_REQUEST, Target_Address, Param_Type, BIT_29, DATA_CANFRAME, sizeof(daTa), daTa);
}
 
 
