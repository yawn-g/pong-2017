void updateDisplay(){
  gb.display.println(gb.getCpuLoad());

  gb.display.print(F("Master: "));
  gb.display.println(masterUp);
  gb.display.print(F("Slave: "));
  gb.display.println(slaveUp);
  gb.display.print(F("Slave is "));
  for (byte i = 0; i < 3; i++) {
    gb.display.print(slaveName[i]);
  }
  gb.display.println();
  gb.display.print(F("Master is "));
  for (byte i = 0; i < 3; i++) {
    gb.display.print(masterName[i]);
  }
  gb.display.println();
}


