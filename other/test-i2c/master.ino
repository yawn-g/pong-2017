///////////////////////////////////// SETUP MASTER
void setupMaster(){
  Wire.begin();
}

///////////////////////////////////// UPDATE MASTER
void updateMaster(){
  //read from slave
  masterRead();
  //updateGame();
  masterWrite();
}


///////////////////////////////////// MASTER READ
void masterRead(){
  if(Wire.requestFrom(2, SLAVE_DATA_BUFFER_LENGTH)){    // request 6 bytes from slave device #2
    disconnected = false;
    paused = false;
    while(Wire.available()){    // slave may send less than requested
      char data_in = Wire.read(); // receive byte per byte
      switch(data_in){
      case UP:
        slaveUp = Wire.read();
        break;
      case DOWN:
        slaveDown = Wire.read();
        break;
      case NAME:
        for (byte i = 0; i < 3; i++) {
          Serial.println(char(data_in));
          slaveName[i] = Wire.read();
        }
        break;
      case SLAVE_PAUSED:
        gb.popup(F("Slave paused"),2);
        paused = true;
        break;
      case I_AM_MASTER:
        gb.popup(F("Only 1 master"),2);
        paused = true;
        break;
      default:
        gb.popup(F("Wrong slave data"),2);
        paused = true;
        break;
      }
    }
  }
  else{
    gb.popup(F("Slave disconnected"),2);
    paused = true;
    disconnected = true;
  }
}

///////////////////////////////////// MASTER WRITE
void masterWrite(){
  Wire.beginTransmission(2); // transmit to device #2
  Wire.write(UP); //identifier
  Wire.write(masterUp); //variable
  Wire.write(DOWN);
  Wire.write(masterDown);
  Wire.write(NAME);
  Wire.write(masterName[0]);
  Wire.write(masterName[1]);
  Wire.write(masterName[2]);
  Wire.endTransmission();    // stop transmitting
}

