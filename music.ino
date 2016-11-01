const int SINE[]={128,176,218,245,255,245,218,176,128,79,37,10,0,10,37,79};
const int musicOut[8] = {11,6,5,4,3,2,1,0};
const int musicIn = A5;

bool playing = false;

const int greyscaleTol = 9;
int prevValue = 0;

const int PRGreyScale = A5;

const int greyScales[8] = {737, 727, 712, 693, 658, 638, 623, 615};
const int Notes[8] = {262, 294, 330, 349, 392, 440, 494, 523};

int readFreq()
{
  // H Y S T E R E S I S kek
  int valueRN = 0;
  for(int i = 0; i < 10; i++){
    valueRN += analogRead(PRGreyScale);
    for(int j = 0; j < 400; j++);
  }
  valueRN /= 10;

    
    Serial.println(valueRN);
    if(abs(prevValue - valueRN) < greyscaleTol)
      valueRN = prevValue;
    else
      prevValue = valueRN;
    
    //some kind of play notes thing that's like playNotes(Notes[n])

    int minDiff = abs(greyScales[0] - valueRN);
    int minIndex = 0;

    for(int i = 0; i < 8; i++)
      if(minDiff > abs(greyScales[i] - valueRN)){
        minDiff = abs(greyScales[i] - valueRN);
        minIndex = i;
      }

    //Serial.print(valueRN);
    //Serial.print(" vs ");
    //Serial.println(greyScales[minIndex]);

    return Notes[minIndex] * 2;
}



void setupMusic(){
  for(int i = 0; i < 8; i++)
    pinMode(musicOut[i], OUTPUT);
  pinMode(PRGreyScale, INPUT);
  
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
ISR(TIMER2_COMPA_vect) {
  if(playing){
    toBinary8 ( SINE[tick]);
    tick = (tick + 1) % (sizeof(SINE)/ sizeof(SINE[0]));
  }
}

void toBinary8 (int num) {
  int i = 2;
  num >>= i;
  for(; i < 8; i++){
    digitalWrite(musicOut[7-i], num & 1);
    num >>= 1;
  }
}

void stopPlaying(){
  cli();
  playing = false;
  sei();
}

void setNote(int freq){
  cli();
  playing = true;
  OCR2A = 16000000/freq/515;// = (16*10^6) / (250hz*256) - 1 (must be <256)
  sei();
}

