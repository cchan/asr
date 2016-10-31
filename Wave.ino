
int CC[]={128,176,218,245,255,245,218,176,
128,79,37,10,0,10,37,79};

int pins[8] = {6,7,8,9,10,11,12,13};

void setup() {
  for(int i = 0; i < 8; i++)
    pinMode(pins[i], OUTPUT);

  Serial.begin(9600);
  
  cli();//stop interrupts

  //set timer2 interrupt every 128us
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 7.8khz increments
  OCR2A = 32;// = (16*10^6) / (250hz*256) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 64 prescaler
  TCCR2B |= (1 << CS11) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
  
  sei();//allow interrupts
}

int tick = 0;

int i = 0;
ISR(TIMER2_COMPA_vect) {
  //if(i ^= 1)
  //  digitalWrite(pins[0], HIGH);
  //else
  //  digitalWrite(pins[0], LOW);
  toBinary8 ( CC[tick]);
  tick = (tick + 1) % (sizeof(CC)/ sizeof(CC[0]));
}


void toBinary8 (int num) {
  int i = 2;
  num >>= i;
  for(; i < 8; i++){
    digitalWrite(pins[7-i], num & 1);
    num >>= 1;
  }
}

int notes[] = {440};

void playNote(int freq, int del){
  cli();
  Serial.println(freq);
  OCR2A = 16000000/freq/515;// = (16*10^6) / (250hz*256) - 1 (must be <256)
  sei();
  delay(del);
}

void loop(){
  for(int i = 0; i < (sizeof notes)/(sizeof notes[0]); i++)
    playNote(notes[i], 500);
}

