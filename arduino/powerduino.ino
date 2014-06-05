  #define encoder0PinA 2
  #define encoder0PinB 3
  
int sensorPin = A0;    // select the input pin for the potentiometer
int sensorValue = 0;
  volatile int encoder0Pos = 0;
  int tmp = -1;
  bool calibration = false;
  long int offset = 0;
  long int cpt = 0;
  void setup() {
    pinMode(encoder0PinA, INPUT); 
    pinMode(encoder0PinB, INPUT);
    
  // encoder pin on interrupt 0 (pin 2)
    attachInterrupt(0, doEncoderA, RISING);
    
    
  // set up the Serial Connection 
    Serial.begin (115200);
  }
  void loop(){
    
    // read the value from the sensor:
  sensorValue = analogRead(sensorPin); 
    
    if(Serial.available()>0) /* Signal from the Interface to calibrate */
    {
      Serial.read();
      offset = encoder0Pos;
      calibration = true;
      digitalWrite(13, HIGH);
    }
    
    
    //Check each changes in position
    if (tmp != encoder0Pos) {
      
      if(calibration){
        Serial.print(encoder0Pos-offset, DEC);
        Serial.print(" ");
        Serial.print(micros());
        Serial.print(" ");
        Serial.println(sensorValue);
        
      }
      tmp = encoder0Pos;
      
    
    }
   }
  // Interrupt on A changing state
  void doEncoderA(){
    if(digitalRead(encoder0PinA))  /* Meaning that pin 2 is HIGH */
    {
      if(digitalRead(encoder0PinB))/* If pin 3 is HIGH */
      {
         ++encoder0Pos;
      }else{                        /* Else if pin 3 is LOW */
        --encoder0Pos;
      }
    }
  }
