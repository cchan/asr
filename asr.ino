void setup() {
  Serial.begin(9600);
  setupMusic();
  calibrateMotors();
}

int notes[] = {330,330,349,392,392,349,330,294,
262,262,294,330,330,294,294,294,
330,330,349,392,392,349,330,294,
262,262,294,330,294,262,262,262,
294,294,330,262,294,349,330,262,
294,349,330,294,262,294,392,392,
330,330,349,392,392,349,330,294,
262,262,294,330,294,262,262,262};

void delayWhile(int ms, void (*cb)(void)){
  int mindelay = 100;
  while(ms > mindelay){
    delay(mindelay);
    ms -= mindelay;
    cb();
  }
  delay(ms);
  cb();
}

int i = 0;
void loop() {
  //int freq = 0;
  //for(int i = 0; i < 10; i++){
  //  freq += readFreq();
  //  delay(10);
  //}
  //freq /= 10;
  setNote(notes[i++] * 2);
  setMotors();
  delay(370);
  i %= sizeof(notes)/sizeof(notes[0]);
}


