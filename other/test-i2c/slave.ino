///////////////////////////////////// SLAVE RECEIVE
// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  /*masterUp = false;
  masterDown = false;*/
  while(Wire.available())    // slave may send less than requested
  {
    char data_in = Wire.read(); // receive byte per byte
    switch(data_in){
    case UP:
      masterUp = Wire.read();
      break;
    case DOWN:
      masterDown = Wire.read();
      break;
    case NAME:
      masterName[0] = Wire.read();
      masterName[1] = Wire.read();
      masterName[2] = Wire.read();
      break;
    }
  }
  slave_updated = true;
  disconnected = false;
}

///////////////////////////////////// SLAVE SEND
// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
char output[SLAVE_DATA_BUFFER_LENGTH];
void requestEvent()
{

  //the slave can only answer with one "write" so you have to put all the variables in an string
  output[0] = NAME;
  output[1] = slaveName[0];
  output[2] = slaveName[1];
  output[3] = slaveName[2];
  output[4] = UP; //identifier
  output[5] = slaveUp; //variable

  if(paused){
    output[4] = SLAVE_PAUSED;
    output[5] = SLAVE_PAUSED;
  }

  if(isMaster){
    output[4] = I_AM_MASTER;
    output[5] = I_AM_MASTER;
  }

  Wire.write(output);
}


///////////////////////////////////// SETUP SLAVE
void setupSlave(){
  Wire.begin(2);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

///////////////////////////////////// UPDATE SLAVE
void updateSlave(){
  byte timout = 0;
  paused = false;
  while(slave_updated == false){
    //wait for the master's interrupt
    delay(1);
    timout++;
    if(timout >= 20){
      gb.popup(F("No master"),2);
      disconnected = true;
      paused = true;
      slave_updated = false;
      break;
    }
  }
  slave_updated = false;
}


