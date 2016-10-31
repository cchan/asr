#include <DRV8835MotorShield.h>

DRV8835MotorShield motors;

const int LED = 12;

const int tol = 30;//change later
const int pins[4] = {A0, A1, A2, A3}; //LL, LR, RL, RR
int blacks[4] = {587, 590, 445, 363};
int whites[4] = {501, 494, 265, 196};

const int FWD = 200;
const int TURN = 100;

int mode = 1;

void getAverage(int vals[]){
  
  int LL = 0, LR = 0, RL = 0, RR = 0;
  
  for(int i = 0; i < 10; i++){
    //Calibrate black.
    LL += analogRead(pins[0]);
    LR += analogRead(pins[1]);
    RL += analogRead(pins[2]);
    RR += analogRead(pins[3]);

    delay(200);
  }
  vals[0] = LL / 10;
  vals[1] = LR / 10;
  vals[2] = RL / 10;
  vals[3] = RR / 10;
}

void setup() {
  for(int i = 0; i < (sizeof pins)/(sizeof pins[0]); i++)
    pinMode(pins[i], INPUT);
  
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  int vals[4];
  
  delay(1000);

  for(int i = 0; i<5; i++){
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
  delay(100);
  };
  
  Serial.print("BLACK... ");
  digitalWrite(LED, HIGH);
  getAverage(vals);
  for(int i = 0; i < 4; i++)
  {
    blacks[i] = vals[i];
    Serial.println(blacks[i]);
  }
  digitalWrite(LED, LOW);
  
  Serial.println("COMPLETE");
  
  delay(3000);
  Serial.print("WHITE... ");
  
  digitalWrite(LED, HIGH);
  getAverage(vals);
  for(int i = 0; i < 4; i++)
  {
    whites[i] = vals[i];
    Serial.println(whites[i]);
  }
  Serial.println("COMPLETE");
  digitalWrite(LED, LOW);

  delay(8000);
}

int compare(int n1, int n2){
  int a = analogRead(pins[n1]);
  int b = analogRead(pins[n2]);
  if(abs(a - blacks[n1]) < tol && abs(b - blacks[n2]) < tol)
    return 1;
  else if(abs(a - whites[n1]) < tol && abs(b - whites[n2]) < tol)
    return -1;
  else
    return 0;
}

void loop() {
  //read in sensor values for photoresistor LL, LR, RL, RR and value for mode
  int LL = analogRead(pins[0]);
  int LR = analogRead(pins[1]);
  int RL = analogRead(pins[2]);
  int RR = analogRead(pins[3]);
  
  for(int i=0; i<4; i++)
  {
    Serial.print(blacks[i]);
    Serial.print(", ");
  }
    Serial.println();
  for(int i = 0; i< 4; i++)
  {
    Serial.print(whites[i]);
    Serial.print(", ");
  }
    Serial.println();
    
  Serial.print(LL);
  Serial.print(", ");
  Serial.print(LR);
  Serial.print(", ");
  Serial.print(RL);
  Serial.print(", ");
  Serial.println(RR);
  
  Serial.print(compare(0, 1));
  Serial.print(", ");
  Serial.println(compare(2, 3));
  
  int speedLeft = FWD;
  int speedRight = FWD;
  
  speedLeft += compare(0, 1) * TURN;
  speedRight += compare(2, 3) * TURN;
  
  //read in the value of "mode" here
  
  speedLeft /= mode;//Recalculate motor speeds
  speedRight /= mode; 
  
  motors.setSpeeds(speedLeft, speedRight);
}


