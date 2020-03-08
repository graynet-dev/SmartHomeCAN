void RX()
{
  uint32_t rxId_can;
  byte len_can = 0;
  byte rxBuf_can[8];
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    can.readMsgBuf(&rxId_can, &len_can, rxBuf_can);      // Read data: len = data length, buf = data byte(s)
       
uint8_t  msg_type, dev_addr, rem_addr, dev_type;
bool priority, can_frame_type, can_ID_type, RTR_11BIT;


can_ID_type    = (rxId_can & 0x80000000)>>31;  //извлекаем тип ID        (0 - 11bit, 1 - 29 bit)
can_frame_type = (rxId_can & 0x40000000)>>30;  //извлекаем тип СAN FRAME (0 - Data,  1 - Remote)   

if (can_frame_type && ((rxId_can & 0x20000000)>>29)) RTR_11BIT = 1;
else RTR_11BIT = 0;                            //если 29 бит единичка, то это RTR 11 бит

priority  = (rxId_can & 0x10000000)>>28;  //  извлекаем из ID 1-битный флаг важности (приоритет) сообщения. (0 - высокий приоритет, 1 - низкий )
msg_type = (rxId_can &   0xF000000)>>24;  //  извлекаем из ID 4-битный идентификатор типа сообщения   
dev_addr  = (rxId_can &  0xFF0000)>>16;   //  извлекаем из ID 8-битный адрес отправителя 
rem_addr = (rxId_can &   0xFF00)>>8;      //  извлекаем из ID 8-битный адрес получателя 
dev_type = rxId_can;                      //  извлекаем из ID 8-битный тип устройства у получателя 



//ниже отладка заголовка ID

#ifdef debug
#if defined (type_node_slave) or defined (type_node_mk)
if (errors) readErrorFlags_MCP2515 ();
#endif

#ifdef type_node_master
if (statusprint && errors) readErrorFlags_MCP2515 ();
#endif
/*
if (ID_Print){
Serial.println();
if (rem_addr == node_address) Serial.println(F("-------CAN MESSAGE RX ADRRESSED TO ME-------"));
else if (rem_addr == 0)       Serial.println(F("----------CAN BROADCAST MESSAGE RX----------"));
else                          Serial.println(F("----------------CAN MESSAGE RX--------------"));
Serial.print (F(" ID  ")); Serial.println(rxId_can, HEX);

Serial.print(F(" type_msg  - "));  PrinT (msg_type, TypeMES); Serial.println ();
Serial.print(F(" node_addr - ")); PrinT (dev_addr, ADDR); Serial.println ();
Serial.print(F(" Rem_addr  - ")); PrinT (rem_addr, ADDR); Serial.println ();
Serial.print(F(" Dev_type  - ")); PrinT (dev_type, TypeDEV); Serial.println ();
Serial.print(F("DATA FRAME  "));  Serial.print ("    "); 
if (len_can!=0) {for (int i=0; i<len_can; i++) {
if (rxBuf_can[i]<=0x0F) Serial.print (F("0"));
Serial.print (rxBuf_can[i], HEX); Serial.print (F(" ")); }}
else Serial.println (F("NOT DATA, BECAUSE REMOTE FRAME"));
Serial.println(); Serial.println(F("--------------------------------------------"));
Serial.println();}
#endif   */





bool prin = 1;
if (msg_type == STATUS && !statusprint) prin = 0;
if (msg_type == PARAMETER && !RXparametrprint) prin = 0;
  
  
  if (prin) {
    PrintSystemTime();
   //Serial.println(); 
  
   Serial.print(F("              Принято  из  CAN:  "));
   

   PrinT (msg_type, TypeMES);  
  
   if (msg_type==COMMAND || msg_type==COMMAND_REPORT) {
   Serial.print(F("  \" ")); PrinT (rxBuf_can[CAN_CommandType], COM);  Serial.print (F(" \" ")); Serial.print(F("На тип устройства:  "));
if (rxBuf_can[CAN_CommandType]!=PARAMETER_WRITE)   PrinT (dev_type, TypeDEV);
else PrinT (dev_type, PAR);
Serial.print ("  ");  Serial.print (F("   Глобальный счетчик:  "));  Serial.print(rxBuf_can[CAN_COUNTER], HEX);
  
                                                                                     }
 Serial.print (F("  От Кого:  "));  PrinT (dev_addr, ADDR); Serial.print ("  ");
                                                                                     
                               
 
Serial.print(F("  - ID ")); 
Serial.print(rxId_can, HEX);
  
Serial.print ("    "); if (len_can!=0) {for (int i=0; i<len_can; i++) {
if (rxBuf_can[i]<=0x0F) Serial.print ("0");
Serial.print (rxBuf_can[i], HEX); Serial.print (" "); }}
Serial.println();
              }
#endif   

if (priority ==0 && msg_type ==0 && dev_addr == 0 && rem_addr == 0 && dev_type == 0) { MCP2515_Init (); return;}
if (rem_addr!=node_address && rem_addr!=0) return;   // если сообщение не нам или не широковещательное - выходим из функции

      
//=------------------------------------Начинаем разбор принятого       P A R S I N G ----------------------------------
      switch (msg_type){              
             
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
case COMMAND: //выполняется, когда мне прислали команду от удаленного контроллера


{

byte Command_NOActual = 0;

if ((rxBuf_can[CAN_CommandType]!=PARAMETER_WRITE && dev_type>=SIZE_DEVICE_ARRAY) || (rxBuf_can[CAN_CommandType]==PARAMETER_WRITE && dev_type>=SIZE_PARAM_ARRAY)) {
 #ifdef debug
 Serial.println(); Serial.println (F("Ошибка выполнения команды. Неизвестный тип устройства, посланный в команде!"));
 #endif 
 rxBuf_can[CAN_REPORT] = UNKNOWN_TYPEDEV ;
  Command_NOActual = 1;
  }

else if ((rxBuf_can[CAN_CommandType]!=PARAMETER_WRITE && device_addr(dev_type)==0xFF) || (rxBuf_can[CAN_CommandType]==PARAMETER_WRITE && param_addr(dev_type, rxBuf_can[CAN_SENSOR_NUM])==0xFF) ) {
 #ifdef debug
 Serial.println(); Serial.println (F("Ошибка выполнения команды. Устройство или параметр, которым пытаемся управлять данной командой, не укомплектованы на узле!"));
 #endif 
 rxBuf_can[CAN_REPORT] = NOT_INCLUDED ;
  Command_NOActual = 1;
  }

else if ( rxBuf_can[CAN_CommandType]>=Command_enum_SIZE) {
 #ifdef debug
 Serial.println(); Serial.println (F("Ошибка выполнения команды. Неизвестная команда!"));
 #endif 
 rxBuf_can[CAN_REPORT] = UNKNOWN_COMMAND ;
  Command_NOActual = 1;
  }

if (Command_NOActual == 0 && rxBuf_can[CAN_CommandType]!=PARAMETER_WRITE){
switch (device [device_addr(dev_type)][DEVICE_VID] & 0xF){
case DIGITALWRITE:
  if (rxBuf_can[CAN_CommandType]>DIGITAL_INVERT) Command_NOActual = 2; break;
case DIGITALWRITE_INVERT:
  if (rxBuf_can[CAN_CommandType]>DIGITAL_INVERT) Command_NOActual = 2; break;
case PWM_WRITE:
  if (rxBuf_can[CAN_CommandType]<PWM_SETTING || rxBuf_can[CAN_CommandType]>PWM_TURN_ON) Command_NOActual = 2; break;
case PROCENT_WRITE:
  if (rxBuf_can[CAN_CommandType]<DIMMER_SETTING || rxBuf_can[CAN_CommandType]>DIMMER_TURN_ON) Command_NOActual = 2; break;
case IMPULSE_GND:
  if (rxBuf_can[CAN_CommandType]<IMPULSE_ON || rxBuf_can[CAN_CommandType]>IMPULSE_INVERT) Command_NOActual = 2; break;
case PARAMETER_WRITE:
  break;  
  }}  

if (Command_NOActual == 2) { 
  #ifdef debug
 Serial.println(); Serial.println (F("Ошибка выполнения команды. Команда не подходит для данного вида устройства!"));
 #endif 
 rxBuf_can[CAN_REPORT] = NOT_SUITABLE ;
  }

if (Command_NOActual==0){


  
bool AlreadyExecuting = 0; 
byte Cell;
if ((device [device_addr(dev_type)][DEVICE_VID] & B10000000)>>7) {  //если вид устройства долгий (LONG) значит это долгая команда

//-----------------------------------------Проверка. нет ли уже в исполнении тайкой же команды (даже если глобальный счетчик отличается)
for (int i = 0; i<bufferEXElongCOM_size; i++){
  
  if (bufferEXElongCOM_cell[i][CommandType] == rxBuf_can[CAN_CommandType] &&
      bufferEXElongCOM_cell[i][CommandValue] == rxBuf_can[CAN_CommandValue] &&
      bufferEXElongCOM_cell[i][DEVTYPE] == dev_type){
  
  AlreadyExecuting = 1;

      // если команда уже выполняется и прилетела такая же команда, но с другого МК, запишем в очередь отчёт ему:

      
     if (bufferEXElongCOM_cell[i][target_Address]!= dev_addr){     
    #ifdef debug
Serial.println(); Serial.print (F("В момент выполнения команды получена точно такая же команда, но от ДРУГОГО Узла. Команда продолжает выполняться. Этому узлу отправлен отчёт EXECUTING. "));
    #endif 
     if (bufferEXElongCOM_cell[i][target_Address_2]==0) {bufferEXElongCOM_cell[i][target_Address_2] = dev_addr; bufferEXElongCOM_cell[i][Command_TX_COUNTER_2] = rxBuf_can[CAN_COUNTER];
#ifdef debug
Serial.println (F("Также, по завершении выполнения команды, будет отправлен отчёт о результате выполнения!"));
#endif   
     
     }
else if (bufferEXElongCOM_cell[i][target_Address_3]==0) {bufferEXElongCOM_cell[i][target_Address_3] = dev_addr; bufferEXElongCOM_cell[i][Command_TX_COUNTER_3] = rxBuf_can[CAN_COUNTER];
#ifdef debug
Serial.println (F("Также, по завершении выполнения команды, будет отправлен отчёт о результате выполнения!"));
#endif   
}

else  
{
#ifdef debug
Serial.println (F("Не хватает места в буфере. Отчёт о результатах выполнения НЕ будет отправлен этому узлу!"));
#endif

}}
else {
#ifdef debug
Serial.println(); Serial.println (F("В момент выполнения команды получена точно такая же команда от ЭТОГО ЖЕ узла. Команда продолжает выполняться. Узлу отправлен отчёт EXECUTING"));
#endif
if (bufferEXElongCOM_cell[i][Command_TX_COUNTER]!= rxBuf_can[CAN_COUNTER]){ 

if (bufferEXElongCOM_cell[i][target_Address_2]==0) {bufferEXElongCOM_cell[i][target_Address_2] = dev_addr; bufferEXElongCOM_cell[i][Command_TX_COUNTER_2] = rxBuf_can[CAN_COUNTER];
#ifdef debug
Serial.println (F("Счётчик команды НЕ СОВПАДАЕТ с предыдущим. По завершении выполнения команды, на НОВЫЙ счётчик ТАКЖЕ будет отправлен отчёт о результате выполнения команды!"));
#endif   
     
     }

else if (bufferEXElongCOM_cell[i][target_Address_3]==0) {bufferEXElongCOM_cell[i][target_Address_3] = dev_addr; bufferEXElongCOM_cell[i][Command_TX_COUNTER_3] = rxBuf_can[CAN_COUNTER];
#ifdef debug
Serial.println (F("Счётчик команды НЕ СОВПАДАЕТ с предыдущим. По завершении выполнения команды, на НОВЫЙ счётчик ТАКЖЕ будет отправлен отчёт о результате выполнения команды!"));
#endif   
}

}
else {
  #ifdef debug
Serial.println (F("Счётчик команды СОВПАДАЕТ с предыдущим. По завершении выполнения команды, будет отправлен отчёт о результате выполнения команды на ЭТОТ один счётчик!"));
#endif   
  
  
  }
}
 
}}
//------------------------------------------Конец проверки - нет ли уже в исполнении тайкой же команды. 

if  (!AlreadyExecuting){         // если такой команды нет в буфере выполнения, то запишем в буфер

  bool empty_cells = 0; // наличие свободной ячейки в буфере 

//-----------------------------------------запись выполнения долгой команды в буфер
for (int i = 0; i<bufferEXElongCOM_size; i++) {
  if (bufferEXElongCOM_cell[i][cellState]==0) 
        {empty_cells = 1;
         bufferEXElongCOM_cell[i][Command_TX_COUNTER] = rxBuf_can[CAN_COUNTER]; // запись значения глобального счетчика в ячейку буфера
         bufferEXElongCOM_cell[i][CommandType] =  rxBuf_can[CAN_CommandType];    // запись типа команды в ячейку буфера
         bufferEXElongCOM_cell[i][CommandValue] = rxBuf_can[CAN_CommandValue];   // запись значения (байт1) команды в ячейку буфера
         bufferEXElongCOM_cell[i][target_Address]   = dev_addr;       // запись адреса отправителя команды в ячейку буфера
         bufferEXElongCOM_cell[i][cellState] = 1;                     // запись состояния ячейки  (1 - команда выполняется)
         bufferEXElongCOM_cell[i][DEVTYPE] = dev_type;                // запись типа устройства  в ячейку буфера
         bufferEXElongCOM_Timer[i] = curMillis;                       // нулим таймер
         bufferEXElongCOM_cell[i][flagTimerCommand] = 1;              // включаем таймер
         Cell = i;
         #ifdef debug
         Serial.println(); Serial.println (F("записал в буфер отчетов"));
         #endif 
         break;
         }
}
#ifdef debug //если при постановке в буфер нет места, пожалуемся в отладку
if (!empty_cells){ Serial.println(); Serial.println (F("Внимание!!! отсутствует свободная ячейка в буфере выполнения долгих команд. Отчёт о выполнении команды не будет доставлен!!!"));}
#endif 
    }


 rxBuf_can[CAN_REPORT] = EXECUTING;    // тип отчета будет - EXECUTING 
}

bool exe;
if (timercountRepeat_ON && countRepeat!=rxBuf_can[CAN_COUNTER] || !timercountRepeat_ON) exe = 1; //если в течение 1,2 сек глобальный счетчик одинаковый, то игнорируем выполнение команды
else  {rxBuf_can[CAN_REPORT] = COMPLETE; exe = 0;}

  
rxBuf_can[CAN_REPORT] = CommandExecuting (dev_type, rxBuf_can[CAN_CommandType], rxBuf_can[CAN_CommandValue], Cell, exe, AlreadyExecuting, rxBuf_can[CAN_CommandValue_2], rxBuf_can[CAN_CommandValue_3],rxBuf_can[CAN_CommandValue_4], rxBuf_can[CAN_SENSOR_NUM])  ; 
  
}
    TX  (LOW_PRIORITY, COMMAND_REPORT,        dev_addr,       dev_type,   EXTENDED,  DATA_CANFRAME     , len_can,        rxBuf_can) ; // отправляем отчёт в CAN    
// (priority, тип сообщения, адрес получателя, тип устройства,  тип ID,   тип FRAME,  длина поля данных кадра, DATa)
 
 timercountRepeat = curMillis ; timercountRepeat_ON = 1 ; countRepeat = rxBuf_can[CAN_COUNTER];        // взводим таймер (1,2 сек) определения команд с одинаковым глоб счетчиком)            
            
              
}
              break;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
case PARAMETER_REQUEST: //выполняется когда удаленный контроллер у меня запросил параметр датчика или еще чего то
  
  {            
 bool not_includ = 1;
 for (byte p ; p<SIZE_PARAM_ARRAY; p++){if (parameter[p][PARAMETER_TYPE]==dev_type) not_includ = 0;}  // выясняем присутствует ли запрашиваемый тип параметра на узле
 if (not_includ) { byte DataP[8]; Parameter_COUNTER++;  DataP[2] = NOT_INCLUDED; DataP[0] = Parameter_COUNTER; DataP[1] = 0;
              TX(1,PARAMETER, dev_addr, dev_type, EXTENDED, DATA_CANFRAME, 3, DataP);
 #ifdef debug 
 Serial.println();  Serial.println(); Serial.println(F("Принят запрос на тип параметра, отсутствующего на данном узле!"));
 #endif   
 break;
              }   
 }                
         
              byte sEnsoR;
              byte siZe; 
              byte iter;
              bool fullSens; 
              if (rxBuf_can[1]==0) { siZe = parameters_quantity; iter=0; fullSens = 1;}
              else {siZe = len_can; iter=1; fullSens = 0;}
    
   bool not_includ; not_includ=1;
           
              for (iter ; iter<siZe; iter++){
              byte DlC = 3; byte DataP[8]; bool ok = 0;


   if (fullSens){if (parameter[iter][PARAMETER_TYPE]==dev_type) {ok = true; sEnsoR = parameter[iter][PARAMETER_SENS];}}
   else {ok = true; sEnsoR = rxBuf_can[iter]; }            
        
       if (ok)     {  
              Parameter_COUNTER++;  
              
           if (param_addr(dev_type, sEnsoR)!=0xFF) {

            if ((parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)!= SERVICE_BYTE){
               if ((parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)<=BYTE_4)  DlC = (parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF) + 4;
               else if ((parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_4_SIGNED ||(parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_4_FLOAT) DlC = 8;
               else if ((parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_2_SIGNED) DlC = 6;              
               else if ((parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR] & 0xF)==BYTE_1_SIGNED) DlC = 5;              
  DataP[0]= Parameter_COUNTER;   DataP[1]= 0; DataP[2] = ON_REQUEST; DataP[3] = parameter [param_addr(dev_type,sEnsoR)][TYPE_VAR]; DataP[4] = parameter [param_addr(dev_type,sEnsoR)][PARAMETER_VALUE];
  DataP[5] = parameter [param_addr(dev_type,sEnsoR)+1][PARAMETER_VALUE]; DataP[6] = parameter [param_addr(dev_type,sEnsoR)+2][PARAMETER_VALUE]; DataP[7] =  parameter [param_addr(dev_type,sEnsoR)+3][PARAMETER_VALUE];    
           DataP[1] = sEnsoR;}
         else   { DataP[2] = NOT_INCLUDED; DataP[0] = Parameter_COUNTER; DataP[1] = sEnsoR;
 #ifdef debug 
 Serial.println();  Serial.println(); Serial.println(F("Принят запрос на сервисный байт!"));
 #endif             
              
              }
              }

              
              else { DataP[2] = NOT_INCLUDED; DataP[0] = Parameter_COUNTER; DataP[1] = sEnsoR;
 #ifdef debug 
 Serial.println();  Serial.println(); Serial.println(F("Принят запрос на № датчика, который отсутствует на данном типе параметра у данного узла!")); 
 #endif             
              
              }


               TX(LOW_PRIORITY, PARAMETER, dev_addr, dev_type, EXTENDED, DATA_CANFRAME, DlC, DataP);
               delay (5);
              }// end if(ok)
              
              
              }// end for
                          
              break;
              
              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~             
case STATUS_REQUEST: //выполняется когда удаленный контроллер у меня запрашивает состояние
              
               StatusTX (dev_addr);  
               
              break;       
              // выполняется когда мне отвечают на мой запрос     
              
              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
case COMMAND_REPORT: //выполняется когда удаленный контроллер отвечает на посланную команду

for (int i = 0; i<queueTX_size; i++) {
  if ( Command_TX_cell[i][cellState] != 0 ){

if (rxBuf_can[CAN_CommandType]== Command_TX_cell[i][CommandType] && 
    rxBuf_can[CAN_CommandValue]== Command_TX_cell[i][CommandValue] && 
    dev_addr    == Command_TX_cell[i][target_Address]   &&
    dev_type    == Command_TX_cell[i][DEVTYPE]          &&
    rxBuf_can[CAN_COUNTER]== Command_TX_cell[i][Command_TX_COUNTER]){       //если отчет совпадает с командой в ячейке очереди

      //----------и статус отчета "команда выполнена" , то вычёркиваем команду из очереди
      
            if (rxBuf_can[CAN_REPORT]==COMPLETE) {   
#ifdef debug

 Serial.println(); Serial.print (F("Команда \" "));  PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" на тип устройства ")); 
 if (Command_TX_cell[i][CommandType]!=PARAMETER_WRITE) PrinT(dev_type, TypeDEV);  
 else PrinT(dev_type, PAR);  
 Serial.print (F(" оппонентом ")); PrinT (dev_addr, ADDR); Serial.println (F(" выполнена успешно !"));
#endif 
 for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0;  
           } 

     //-----------и статус отчета "команда выполняется", то:    
         
 else if (rxBuf_can[CAN_REPORT]==EXECUTING){                                                                                                                             
                                      Command_TX_cell[i][Command_TX_count] = 0;     // сбросим счетчик количества отправления команды повторно, из-за отсутсвтия отчёта
                                      queueTX_Timer [i] = curMillis;                // обнулим таймер задержки отправки команды повторно
                                      Command_TX_cell[i][flagTimerCommand] = 1;     // включим таймер задержки отправки команды повторно
                                      Command_TX_cell[i][cellState] = EXECUTING;    // статус ячейки в очереди на отправку делаем  "команда выполняется"
#ifdef debug
 Serial.println(); Serial.print (F("Оппонент ")); PrinT(dev_addr, ADDR); Serial.print (F("команду \" ")); PrinT (Command_TX_cell[i][CommandType], COM);   
 Serial.print (F(" \""));  Serial.print (F(" на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" принял. Происходит выполнение команды!"));
#endif 
return;
 }
 
     //-----------и статус отчета "Ошибка выполнения команды оппонетом" , то вычёркиваем команду из очереди
 else if (rxBuf_can[CAN_REPORT]==FAIL_UNKNOWN)     { 
#ifdef debug

 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV);  Serial.println (F(" Получен отчёт от оппонента * Выполнение команды не завершено по неизвестным причинам! *"));
#endif 
 }  

 else if (rxBuf_can[CAN_REPORT]==UNKNOWN_COMMAND)     { 
#ifdef debug
 
 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * Неизвестная команда! *"));
#endif 
 }  

 else if (rxBuf_can[CAN_REPORT]==UNKNOWN_TYPEDEV)     { 
#ifdef debug
 
 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * Неизвестный тип устройства, посланный в команде! *"));
#endif 
 }  


 else if (rxBuf_can[CAN_REPORT]==NOT_INCLUDED)     { 
#ifdef debug
 
 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * Узел не укомплектован таким устройством! *"));
#endif 
 }  


 else if (rxBuf_can[CAN_REPORT]==NOT_SUITABLE)     { 
#ifdef debug
 
 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * Команда не поддерживается видом подключения устройства! *"));
#endif 
 }  

  else if (rxBuf_can[CAN_REPORT]==NEGATIVE_RESULT)     { 
#ifdef debug
 
 Serial.println(); Serial.print (F("Ошибка выполнения команды \" ")); PrinT (Command_TX_cell[i][CommandType], COM); Serial.print (F(" \""));
 Serial.print (F(" оппонентом!  на тип устройства ")); PrinT(dev_type, TypeDEV); Serial.println (F(" Получен отчёт от оппонента * По контролю устройства определено, что результат выполнения команды не достигнут! *"));
#endif 
 }  

  if (rxBuf_can[CAN_REPORT]==COMPLETE || rxBuf_can[CAN_REPORT] == FAIL_UNKNOWN || rxBuf_can[CAN_REPORT] == UNKNOWN_COMMAND || 
rxBuf_can[CAN_REPORT] == UNKNOWN_TYPEDEV || rxBuf_can[CAN_REPORT] == NOT_INCLUDED || rxBuf_can[CAN_REPORT] == NOT_SUITABLE || rxBuf_can[CAN_REPORT] == NEGATIVE_RESULT) {for (int j = 0; j<queue_size_columns; j++)  Command_TX_cell[i][j] = 0; } //вычеркиваем команду из очереди

break;
      
  
}}}

              break;
              
              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~             
case PARAMETER: //выполняется когда удаленный контроллер мне отвечают на мой запрос параметра датчика
              
             
// Функция обработки ответа на мой запрос параметра датчика   

              
              
              break;
             
      
      
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~              
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~           
case STATUS: { //выполняется когда удаленный контроллер мне отвечает на мой запрос состояния 
              
           #ifdef type_node_master
countNoTAnswerNode[dev_addr] = 0; //если получили ответ от узла, сбрасываем счетчик неответов в ноль  
StatusNode_OK[dev_addr] = true;   //и возвращаем статус "ЖИВ"
alarmAccident_send[dev_addr]=0;             //и флаг "авария отправлена" сбрасываем                                          
             
              #endif 
              #if defined (type_node_slave) or defined (type_node_mk)    

ss = rxBuf_can[CAN_SEC]; mm = rxBuf_can[CAN_MIN]; hh = rxBuf_can[CAN_HOURS]; dd = rxBuf_can[CAN_DAYS]; 

if (dev_addr == SendAddrStatus_master) {prevtimeStatus = curMillis; 
TimerStatus = 1;}
#endif
                                          
                                          }
                         
              break;  
              
      default: MCP2515_Init ();
               
      }
     
  }   
}







void TX(bool priority, uint8_t msg_type, uint8_t rem_addr, uint8_t dev_type, bool can_ID_type, bool can_frame_type, uint8_t DLC,  byte data[8])
{
uint32_t txId_can;

if (!can_ID_type) { txId_can = 5;}// тут как-то формируем 11 битный ID если нужно

else  txId_can = (priority & 0xFFFFFFFF)<<28 | (msg_type & 0xFFFFFFFF)<<24 | (node_address & 0xFFFFFFFF)<<16 | (rem_addr & 0xFFFFFFFF)<<8 | dev_type ;
       

if (can_ID_type){
if (can_frame_type) {txId_can = txId_can | 0xC0000000;  DLC = 0;} //  формируем 29 битный ID REMOTE_FRAME
else txId_can = txId_can | 0x80000000; }                          //  формируем 29 битный ID DATA_FRAME
else if (can_frame_type){ txId_can=txId_can|0x60000000; DLC = 0;} //  формируем 11 битный ID REMOTE_FRAME

byte sndStat =can.sendMsgBuf(txId_can, DLC, data);   //отсылаем кадр в CAN

#ifdef debug    

bool prin = 1;
if (msg_type == STATUS && !statusprint) prin = 0;
if (msg_type == PARAMETER && !TXparametrprint) prin = 0; 
  if (prin) {
  
Serial.println(); Serial.print(F("              Отправлено в CAN:  "));
if(sndStat == CAN_OK){
   if (msg_type<size_Message_ENUM){
 
 PrinT (msg_type, TypeMES); 
 
 if (msg_type==COMMAND || msg_type==COMMAND_REPORT) {Serial.print(F("  \" ")); PrinT (data[3], COM);
 Serial.print (F(" \" ")); Serial.print(F("На тип устройства:  "));
 
  
  if (data[3]!=PARAMETER_WRITE)   PrinT (dev_type, TypeDEV);
else PrinT (dev_type, PAR);
  
  Serial.print ("  ");
 
 
 
 Serial.print (F("   Глобальный счетчик:  "));  Serial.print (data[0], HEX);
 
 }
 
 
 }else Serial.print (F("CAN Сообщение "));
Serial.print (F("  Кому:  "));
PrinT (rem_addr, ADDR); 
Serial.print(F("  - ID ")); 
  Serial.print(txId_can, HEX);
Serial.print ("    "); if (DLC!=0) {for (int i=0; i<DLC; i++) {
if (data[i]<=0x0F) Serial.print ("0");
Serial.print (data[i], HEX); Serial.print (" "); }}
else Serial.print (F("  REMOTE FRAME   RTR"));
Serial.println();
}
else {Serial.print(F("Ошибка отправки CAN сообщения !!!  ")); Serial.print (sndStat); readErrorFlags_MCP2515 ();}}
#endif  
} 

// вызов TX (priority, тип сообщения, адрес получателя, тип устройства, тип ID, тип FRAME, длина поля данных кадра, data)
// тип ID     0 - 11 bit;   1 - 29 bit
// тип FRAME  0 - DATA;     1 - REMOTE FRAME
