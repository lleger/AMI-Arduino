
#include <SoftwareSerial.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int SABERREC = 19;
const int SABERSEND = 18;
const int SABERBAUD = 9600;
const int SABER_MOTOR1_FULL_FORWARD = 127; 
const int SABER_MOTOR1_TURN_FIVE = 115;
const int SABER_MOTOR1_TURN_SIX = 103;
const int SABER_MOTOR1_TURN_SEVEN = 91;
const int SABER_MOTOR1_TURN_EIGHT = 79;
const int SABER_MOTOR1_FULL_REVERSE = 1;
const int SABER_MOTOR1_FULL_STOP = 64;
const int SABER_MOTOR2_FULL_FORWARD = 255;
const int SABER_MOTOR2_TURN_FOUR = 243;
const int SABER_MOTOR2_TURN_THREE = 231;
const int SABER_MOTOR2_TURN_TWO = 219;
const int SABER_MOTOR2_TURN_ONE = 205;
const int SABER_MOTOR2_FULL_REVERSE = 128;
const int SABER_MOTOR2_FULL_STOP = 192;
const int ALLSTOP = 0;

int i = 0; //control motor speed
int type;  //determines type of stop
int stopsig = 1; //determines if motors are in stopped state
int dir = 0; //0 = Forward 1 = Backward
signed int onespeed, twospeed; //speed of motor 1 and 2

SoftwareSerial SaberSerial = SoftwareSerial( SABERREC, SABERSEND );

////////////////////////////////////////////////////////////////////////////////////

void setEngineSpeed( signed char cNewMotorSpeed )
{ unsigned char cSpeedVal_Motor1 = 0;

  unsigned char cSpeedVal_Motor2 = 0;
  
  // Check for full stop command
  if( cNewMotorSpeed == 0 )
  {
    // Send full stop command for both motors
    SaberSerial.write((byte)ALLSTOP);

    return;
  }  
  // Calculate the speed value for motor 1
  if( cNewMotorSpeed >= 100 )
  {
    cSpeedVal_Motor1 = SABER_MOTOR1_FULL_FORWARD;

    cSpeedVal_Motor2 = SABER_MOTOR2_FULL_FORWARD;
  }
  else if( cNewMotorSpeed <= -100 )
  {
    cSpeedVal_Motor1 = SABER_MOTOR1_FULL_REVERSE;

    cSpeedVal_Motor2 = SABER_MOTOR2_FULL_REVERSE;
  }
  else
  {
    // Calc motor 1 speed (Final value ranges from 1 to 127)
    cSpeedVal_Motor1 = map( cNewMotorSpeed, -100, 100, SABER_MOTOR1_FULL_REVERSE, SABER_MOTOR1_FULL_FORWARD );

    // Calc motor 2 speed (Final value ranges from 128 to 255)
    cSpeedVal_Motor2 = map( cNewMotorSpeed, -100, 100, SABER_MOTOR2_FULL_REVERSE, SABER_MOTOR2_FULL_FORWARD );
  }

  // Fire the values off to the Sabertooth motor controller
  SaberSerial.write((byte) cSpeedVal_Motor1 );

  SaberSerial.write((byte) cSpeedVal_Motor2 );
}
/////////////////////////////////////////////////////////////////////////////////
void stopProcedure( int select)
{ signed char traverse;
  switch(select)
  {
    case 0:
      if(i < 0){
            for (i; i <= 0; i++){
            traverse = i;
            setEngineSpeed( traverse ); 
            delay(30);
            }
          } 
          else{
            for (i; i >= 0; i--){
              traverse = i;
              setEngineSpeed( traverse ); 
              delay(40);
            }
          } break;
    case 1:
      if(onespeed>SABER_MOTOR1_FULL_STOP){
        for (onespeed; onespeed > SABER_MOTOR1_FULL_STOP; onespeed--){
          if(twospeed > SABER_MOTOR2_FULL_STOP){
            twospeed = twospeed--;
            SaberSerial.write((byte)twospeed);
          }
          SaberSerial.write((byte)onespeed);
          delay(30);
        }
      }
      else{
        for (onespeed; onespeed < SABER_MOTOR1_FULL_STOP; onespeed++){
          if(twospeed < SABER_MOTOR2_FULL_STOP){
            twospeed = twospeed++;
            SaberSerial.write((byte)twospeed);
          }
          SaberSerial.write((byte)onespeed);
          delay(30);
        }
      }
    break;
    case 2:
      if(twospeed>SABER_MOTOR2_FULL_STOP){
        for (twospeed; twospeed > SABER_MOTOR2_FULL_STOP; twospeed--){
          if(onespeed < SABER_MOTOR1_FULL_STOP){
            onespeed = onespeed--;
            SaberSerial.write((byte)onespeed);
          }
          SaberSerial.write((byte)twospeed);
          delay(30);
        }
      }
      else{
        for (twospeed; twospeed < SABER_MOTOR2_FULL_STOP; twospeed++){
          if(onespeed < SABER_MOTOR1_FULL_STOP){
            onespeed = onespeed++;
            SaberSerial.write((byte)onespeed);
          }
          SaberSerial.write((byte)twospeed);
          delay(30);
        } 
      } break;
  }
}
      
////////////////////////////////////////////////////////////////////////////////////



void setup() {
  pinMode( SABERSEND, OUTPUT );
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial.println("Welcome");

  // set the data rate for the SoftwareSerial port
  SaberSerial.begin(SABERBAUD);
  //allow to init
  delay( 2000 );
  setEngineSpeed(ALLSTOP);
  
}

void loop() {
  signed char traverse;
  if(Serial.available()>0) 
  {
    signed char data = Serial.read();
    switch(data)
    {  
      case'F':
        stopsig = 0;
        type = 0;
        dir = 0;
        for (i; i <= 100; i++){
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(40);
        } break; // Forward
        
      case'B':
        dir = 1;
        stopsig = 0;
        type = 0;
        for (i; i >= -100; i--){
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(40);
        } 
        break; // Reverse
      
      case'S':
        dir = 0;
        stopsig = 1;
        if(type == 0)
          stopProcedure(0);
        else if (type == 1){
          i=0;
          signed int onespeeddif = abs(onespeed - 64);
          signed int twospeeddif = abs(twospeed - 192);
          if(onespeeddif > twospeeddif)
            stopProcedure(1);
          else if(onespeeddif < twospeeddif)
            stopProcedure(2); 
        } break;
   /////////////////////////////////////////////     
      case'4':
        stopsig = 0;
        type = 1;
        i=80;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 192;
          for(int j = 64; j >= SABER_MOTOR1_FULL_FORWARD; j++){
            if(k<SABER_MOTOR2_TURN_FOUR){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_FOUR );
        }
        onespeed = SABER_MOTOR1_FULL_FORWARD;
        twospeed = SABER_MOTOR2_TURN_FOUR;
        break;  //18 degree left turn
        
      case'3':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 192;
          for(int j = 64; j >= SABER_MOTOR1_FULL_FORWARD; j++){
            if(k<SABER_MOTOR2_TURN_THREE){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_THREE );
        }
        onespeed = SABER_MOTOR1_FULL_FORWARD;
        twospeed = SABER_MOTOR2_TURN_THREE;
        break;  //36 degree left turn
        
      case'2':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 192;
          for(int j = 64; j >= SABER_MOTOR1_FULL_FORWARD; j++){
            if(k<SABER_MOTOR2_TURN_TWO){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_TWO );
        }
        onespeed = SABER_MOTOR1_FULL_FORWARD;
        twospeed = SABER_MOTOR2_TURN_TWO;
        break;  //54 degree left turn
        
      case'1':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 192;
          for(int j = 64; j >= SABER_MOTOR1_FULL_FORWARD; j++){
            if(k<SABER_MOTOR2_TURN_ONE){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_ONE );
        }
        onespeed = SABER_MOTOR1_FULL_FORWARD;
        twospeed = SABER_MOTOR2_TURN_ONE;
        break;  //72 degree left turn
 
      case'0':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          SaberSerial.write((byte) SABER_MOTOR2_FULL_STOP );
          for(int j = 64; j >= SABER_MOTOR1_FULL_FORWARD; j++){
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_FULL_STOP );
        }
        onespeed = SABER_MOTOR1_FULL_FORWARD;
        twospeed = SABER_MOTOR2_FULL_STOP;
        break;  //90 degree left turn
     ///////////////////////////////////////////
      case'5':
        stopsig = 0;
        type = 1;
        i=80;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 64;
          for(int j = 192; j >= SABER_MOTOR2_FULL_FORWARD; j++){
            if(k<SABER_MOTOR1_TURN_FIVE){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_FIVE );
        }
        onespeed = SABER_MOTOR1_TURN_FIVE;
        twospeed = SABER_MOTOR2_FULL_FORWARD;
        break;  //18 degree right turn
        
      case'6':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 64;
          for(int j = 192; j >= SABER_MOTOR2_FULL_FORWARD; j++){
            if(k<SABER_MOTOR1_TURN_SIX){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_SIX );
        }
        onespeed = SABER_MOTOR1_TURN_SIX;
        twospeed = SABER_MOTOR2_FULL_FORWARD;
        break;  //36 degree right turn
        
      case'7':
        stopsig = 0;
        type = 1; //alt stop signal
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 64;
          for(int j = 192; j >= SABER_MOTOR2_FULL_FORWARD; j++){
            if(k<SABER_MOTOR1_TURN_SEVEN){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_SEVEN );
        }
        onespeed = SABER_MOTOR1_TURN_SEVEN;
        twospeed = SABER_MOTOR2_FULL_FORWARD;
        break;  //54 degree right turn
        
      case'8':
        stopsig = 0;
        type = 1;
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          int k = 64;  //motor 1 stop bit
          for(int j = 192; j >= SABER_MOTOR2_FULL_FORWARD; j++){
            if(k<SABER_MOTOR1_TURN_EIGHT){
              k = k++;
              SaberSerial.write((byte) k);
            }
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_EIGHT );
        }
        onespeed = SABER_MOTOR1_TURN_EIGHT;
        twospeed = SABER_MOTOR2_FULL_FORWARD;
        break;  //72 degree right turn
        
      case'9':
        stopsig = 0;
        type = 1;
        //if motors are at zero
        if(dir == 1){
          stopProcedure(0);
          stopsig = 1;
          dir = 0;
        }
        if(stopsig == 1){
          SaberSerial.write((byte) SABER_MOTOR1_FULL_STOP);
          for(int j = 192; j >= SABER_MOTOR2_FULL_FORWARD; j++){
            SaberSerial.write((byte) j);
            delay(30);
          }
        }
        else{
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_FULL_STOP );
        }
        onespeed = SABER_MOTOR1_FULL_STOP;
        twospeed = SABER_MOTOR2_FULL_FORWARD;
        break;  //90 degree right turn
    }

    
    Serial.print("Traverse = ");
    Serial.println(traverse);
    Serial.print("Data = ");
    Serial.println(data);
    //Serial.flush();
    delay(2);
  }
}
