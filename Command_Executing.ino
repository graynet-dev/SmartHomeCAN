byte CommandExecuting (const byte &dev_Type, const byte &Command_Type, const byte &Command_Value, const byte &cell, const bool &EXE, const bool &AlreadyEx, const byte &Command_Value_2, const byte &Command_Value_3, const byte &Command_Value_4, const byte &Sensor_num) {
  
  
  // ТУТ НЕПОСРЕДСТВЕННО ВЫПОЛНЯЕМ КОМАНДУ

bool Long=0;
if (Command_Type!= PARAMETER_WRITE){Long = (device [device_addr(dev_Type)][DEVICE_VID] & B10000000)>>7;  } // определим долго ли будет девайс выполнять команду или мгновенно 

 
if ((Long && !AlreadyEx) || (!Long && EXE))
   {
    if (Command_Type !=  PARAMETER_WRITE){
                          
                          if ( ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE) || ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE_INVERT))
                                  {    
 if  (Command_Type == DIGITAL_REMOTE &&  (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],  Command_Value);
 if  (Command_Type == DIGITAL_REMOTE &&  (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == DIGITALWRITE_INVERT) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],  !Command_Value);
 if  (Command_Type == DIGITAL_INVERT) digitalWrite (device [device_addr(dev_Type)][DEVICE_PIN],!digitalRead(device [device_addr(dev_Type)][DEVICE_PIN]));
                                  }
                                                                           
                          else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == PWM_WRITE)
                                  {
     if (Command_Type == PWM_SETTING)  device [device_addr(dev_Type)][DEVICE_VALUE] = Command_Value; 
else if (Command_Type == PWM_TURN_ON)  {if (device [device_addr(dev_Type)][DEVICE_VALUE] + Command_Value <= 255 ) device [device_addr(dev_Type)][DEVICE_VALUE]+=Command_Value; else device [device_addr(dev_Type)][DEVICE_VALUE] = 255;}
else if (Command_Type == PWM_TURN_OFF) {if (device [device_addr(dev_Type)][DEVICE_VALUE] - Command_Value >= 0 )   device [device_addr(dev_Type)][DEVICE_VALUE]-=Command_Value; else device [device_addr(dev_Type)][DEVICE_VALUE] = 0;}
 
 analogWrite (device [device_addr(dev_Type)][DEVICE_PIN],  device [device_addr(dev_Type)][DEVICE_VALUE]);
                                  }

 
                           else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == PROCENT_WRITE){}
  
                           else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_GND || (device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_POWER){

byte ok = 0;

         if (Command_Type == IMPULSE_INVERT) ok = 2;
    else if (Command_Type == IMPULSE_ON  && device [device_addr(dev_Type)][DEVICE_VALUE]==0) { ok = 1;  }
    else if (Command_Type == IMPULSE_OFF && device [device_addr(dev_Type)][DEVICE_VALUE]!=0) ok = 1; 

  for (int i = 0 ; i< IMPULSEDEVICE_QUANTITY; i++) { if (TimerImpulse_ON[i]==device [device_addr(dev_Type)][DEVICE_PIN]) ok = 0;}
  
      if (ok == 1){   for (int i = 0; i< IMPULSEDEVICE_QUANTITY; i++){if (TimerImpulse_ON[i]==0) {TimerImpulse_ON[i] = device [device_addr(dev_Type)][DEVICE_PIN]; timerImpulse[i] = curMillis; ok = 2; break;} }
      
      #ifdef debug
      if (ok!=2){ Serial.println(); Serial.println (F("Пытаемся управлять неколькими импульсными девайсами одновременно! Превышенно максимальное значение девайсов одновременного управления"));}
      #endif   
      
      if (ok == 2){ 
         if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_GND)   digitalWrite (device [DEVICE_PIN][dev_Type],  LOW);
    else if ((device [device_addr(dev_Type)][DEVICE_VID] & 0xF) == IMPULSE_POWER) digitalWrite (device [DEVICE_PIN][dev_Type],  HIGH);  }
}}
    }

// НИЖЕ ЕСЛИ КОМАНДА - "УПРАВЛЕНИЕ ИЗМЕНЕНИЕМ ПАРАМЕТРА"  ТО В РАЗРЯДЕ dev-type ЗАГОЛОВКА ID  БУДЕТ ТИП ПАРАМЕТРА, А НЕ ИСПОЛНИТЕЛЬНОГО МЕХАНИЗМА

else {
   
    parameter[param_addr(dev_Type, Sensor_num)][PARAMETER_VALUE] = Command_Value;
if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) != BYTE_1 && (parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) != BYTE_1_SIGNED)  parameter[param_addr(dev_Type, Sensor_num)+1][PARAMETER_VALUE] = Command_Value_2;                         
if ((parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4 || (parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4_SIGNED ||(parameter [param_addr(dev_Type, Sensor_num)][TYPE_VAR]& 0xF) == BYTE_4_FLOAT) 
{parameter[param_addr(dev_Type, Sensor_num)+2][PARAMETER_VALUE] = Command_Value_3; parameter[param_addr(dev_Type, Sensor_num)+3][PARAMETER_VALUE] = Command_Value_4;}                                                       
                            
                            
                                                                                                   }
                              

#ifdef debug    
if (!AlreadyEx && Long){
Serial.println(); Serial.print (F("Началось выполнение долгой команды \" "));
PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
PrinT (dev_Type, TypeDEV); Serial.println();}

if (EXE && !Long) {Serial.println(); Serial.print (F("Произведено выполенение быстрой команды \" "));
PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
if (Command_Type!=PARAMETER_WRITE)   PrinT (dev_Type, TypeDEV);
else PrinT (dev_Type, PAR);
Serial.println(); }

if (!EXE && !Long){Serial.println();Serial.println (F("Получена команда в течение 1 секунды с глоб.счетчиком как в предыдущей команде. Игнорируем выполнение команды, но отчёт отправляем."));}
#endif  


   }

if (Long) return EXECUTING;
else      return COMPLETE;
  
                                    



  }
  



void LongCommandExecuting_buffer(){

// ===========================Извлечение из буфера долгих команд и ожидание выполнения


for (int i = 0; i<bufferEXElongCOM_size; i++) {
  if (bufferEXElongCOM_cell[i][cellState]==1)  {                       // если нашли занятую ячейку в очереди
        
if (bufferEXElongCOM_cell[i][flagTimerCommand]==1 && curMillis - bufferEXElongCOM_Timer[i] > ((uint32_t)timeout(bufferEXElongCOM_cell[i][DEVTYPE])*1000)-500) {      //если таймер был включен и кончился отправляем отчёт
   byte DATa[5] = {0};
DATa[3]=bufferEXElongCOM_cell[i][CommandType];
DATa[4]=bufferEXElongCOM_cell[i][CommandValue];

DATa[2]= ExecutionResult ( bufferEXElongCOM_cell[i][DEVTYPE], DATa[3], DATa[4]); 


   
   for (int j = 0; j<3; j++) {
     if (j==0) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER];
else if (j==1) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_2];
else if (j==2) DATa[0]=bufferEXElongCOM_cell[i][Command_TX_COUNTER_3];

if (bufferEXElongCOM_cell[i][queuelongCOM_size_columns-4+j]!=0)  TX (1, COMMAND_REPORT, bufferEXElongCOM_cell[i][queuelongCOM_size_columns-4+j],  bufferEXElongCOM_cell[i][DEVTYPE], EXTENDED,  DATA_CANFRAME, sizeof(DATa), DATa) ; }// отправляем отчёт в CAN    

#ifdef debug
 Serial.println();        Serial.println (F("удалил из буфера отчетов"));
#endif 
for (int j = 0; j<queuelongCOM_size_columns; j++)  bufferEXElongCOM_cell[i][j] = 0; //чистим ячейку так как отчет отправлен


}}}}



byte ExecutionResult (byte dev_Type, byte Command_Type, byte Command_Value) {
  
 
  
  if (digitalRead (device [device_addr(dev_Type)][DEVICE_VALUE]) == Command_Type) {
    
   #ifdef debug
 Serial.println(); Serial.print (F("Выполнение долгой команды \" "));
PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
PrinT (dev_Type, TypeDEV); Serial.println(F("  завершилось успешно!"));
 
 #endif   
     return COMPLETE;
  }

  else {
    #ifdef debug
 Serial.println(); Serial.print (F("Таймаут на выполнение долгой команды \" "));
PrinT (Command_Type, COM);  Serial.print (F(" \" типа устройства: "));
PrinT (dev_Type, TypeDEV); Serial.println(F("  истёк! Команда НЕ выполнена! Результат отрицательный!"));
    #endif 
    
    return NEGATIVE_RESULT; 
    
    
    }
  
}
