#include <SoftwareSerial.h>

// These constants won't change.  They're used to give names
// to the pins used:
const int MOTORENABLE = 47;
const int BLADEENABLE = 41;
const int SABERREC = 53;
const int BUZZER = 45;
const int SABERSEND = 51;
const int SABERBAUD = 9600;
const int SABER_MOTOR1_FULL_STOP = 64;
const int SABER_MOTOR1_FULL_FORWARD = 94; //127; 
const int SABER_MOTOR1_TURN_FIVE = 88; //int(SABER_MOTOR1_FULL_FORWARD - (SABER_MOTOR1_FULL_FORWARD - SABER_MOTOR1_FULL_STOP) * .2);
const int SABER_MOTOR1_TURN_SIX = 82; //int(SABER_MOTOR1_FULL_FORWARD - (SABER_MOTOR1_FULL_FORWARD - SABER_MOTOR1_FULL_STOP) * .4);
const int SABER_MOTOR1_TURN_SEVEN = 76; //int(SABER_MOTOR1_FULL_FORWARD - (SABER_MOTOR1_FULL_FORWARD - SABER_MOTOR1_FULL_STOP) * .6);
const int SABER_MOTOR1_TURN_EIGHT = 70; //int(SABER_MOTOR1_FULL_FORWARD - (SABER_MOTOR1_FULL_FORWARD - SABER_MOTOR1_FULL_STOP) * .8);
const int SABER_MOTOR1_TURN_FIVE_REVERSE = 40; //int(SABER_MOTOR1_FULL_REVERSE + (SABER_MOTOR1_FULL_STOP - SABER_MOTOR1_FULL_REVERSE) * .8);
const int SABER_MOTOR1_TURN_SIX_REVERSE = 46; //int(SABER_MOTOR1_FULL_REVERSE + (SABER_MOTOR1_FULL_STOP - SABER_MOTOR1_FULL_REVERSE) * .6);
const int SABER_MOTOR1_TURN_SEVEN_REVERSE = 52; //int(SABER_MOTOR1_FULL_REVERSE + (SABER_MOTOR1_FULL_STOP - SABER_MOTOR1_FULL_REVERSE) * .4);
const int SABER_MOTOR1_TURN_EIGHT_REVERSE = 58; //int(SABER_MOTOR1_FULL_REVERSE + (SABER_MOTOR1_FULL_STOP - SABER_MOTOR1_FULL_REVERSE) * .2);
const int SABER_MOTOR1_FULL_REVERSE = 34; //1;
const int SABER_MOTOR2_FULL_STOP = 192;
const int SABER_MOTOR2_FULL_FORWARD = 222; //255;
const int SABER_MOTOR2_TURN_FOUR = 216; //int(SABER_MOTOR2_FULL_FORWARD - (SABER_MOTOR2_FULL_FORWARD - SABER_MOTOR2_FULL_STOP) * .2);
const int SABER_MOTOR2_TURN_THREE = 210; //int(SABER_MOTOR2_FULL_FORWARD - (SABER_MOTOR2_FULL_FORWARD - SABER_MOTOR2_FULL_STOP) * .4);
const int SABER_MOTOR2_TURN_TWO = 204; //int(SABER_MOTOR2_FULL_FORWARD - (SABER_MOTOR2_FULL_FORWARD - SABER_MOTOR2_FULL_STOP) * .6);
const int SABER_MOTOR2_TURN_ONE = 198; //int(SABER_MOTOR2_FULL_FORWARD - (SABER_MOTOR2_FULL_FORWARD - SABER_MOTOR2_FULL_STOP) * .8);
const int SABER_MOTOR2_TURN_FOUR_REVERSE = 168; //int(SABER_MOTOR2_FULL_REVERSE + (SABER_MOTOR2_FULL_STOP - SABER_MOTOR2_FULL_REVERSE) * .8);
const int SABER_MOTOR2_TURN_THREE_REVERSE = 174; //int(SABER_MOTOR2_FULL_REVERSE + (SABER_MOTOR2_FULL_STOP - SABER_MOTOR2_FULL_REVERSE) * .6);
const int SABER_MOTOR2_TURN_TWO_REVERSE = 180; //int(SABER_MOTOR2_FULL_REVERSE + (SABER_MOTOR2_FULL_STOP - SABER_MOTOR2_FULL_REVERSE) * .4);
const int SABER_MOTOR2_TURN_ONE_REVERSE = 186; //int(SABER_MOTOR2_FULL_REVERSE + (SABER_MOTOR2_FULL_STOP - SABER_MOTOR2_FULL_REVERSE) * .2);
const int SABER_MOTOR2_FULL_REVERSE = 162;  //128;
const int ALLSTOP = 0;

int i = 0; //control motor speed
int type;  //determines type of stop
int stopsig = 1; //determines if motors are in stopped state
int dir = 0; //0 = Forward 1 = Backward
boolean power = false; //turn motor controller on/off
signed int onespeed, twospeed; //speed of motor 1 and 2

SoftwareSerial SaberSerial = SoftwareSerial( SABERREC, SABERSEND );

////////////////////////////////////////////////////////////////////////////////////
//Used for forward and backward motion
////////////////////////////////////////////////////////////////////////////////////

void setEngineSpeed( signed char cNewMotorSpeed )
{ unsigned char cSpeedVal_Motor1 = 0; //motor val for motor 1

  unsigned char cSpeedVal_Motor2 = 0; //motor val for motor 2
  
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
  onespeed = cSpeedVal_Motor1;
  SaberSerial.write((byte) cSpeedVal_Motor2 );
  twospeed = cSpeedVal_Motor2;
}
/////////////////////////////////////////////////////////////////////////////////
//Used to determine proper stop for motors
/////////////////////////////////////////////////////////////////////////////////

void stopProcedure( int select)
{ signed char traverse;
  switch(select)
  {
    //for full forward orbackward motion
    case 0: 
      if(i < 0){
            for (i; i <= 0; i++){
              if(Serial.available()>0)
                break;
              traverse = i;
              setEngineSpeed( traverse ); 
              delay(30);
            }
          } 
      else{
          for (i; i >= 0; i--){
            if(Serial.available()>0)
              break;
            traverse = i;
            setEngineSpeed( traverse ); 
            delay(30);
            }
          } break;
    //for motor 1 speed > motor 2 speed      
    case 1:
      if(onespeed>SABER_MOTOR1_FULL_STOP){
        for (onespeed; onespeed > SABER_MOTOR1_FULL_STOP; onespeed--){
          if(Serial.available()>0)
            break;
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
          if(Serial.available()>0)
            break;
          if(twospeed < SABER_MOTOR2_FULL_STOP){
            twospeed = twospeed++;
            SaberSerial.write((byte)twospeed);
          }
          SaberSerial.write((byte)onespeed);
          delay(30);
        }
      } break;
    
    //for motor 2 speed > motor 1 speed
    case 2:
      if(twospeed>SABER_MOTOR2_FULL_STOP){
        for (twospeed; twospeed > SABER_MOTOR2_FULL_STOP; twospeed--){
          if(Serial.available()>0)
            break;
          if(onespeed > SABER_MOTOR1_FULL_STOP){
            onespeed = onespeed--;
            SaberSerial.write((byte)onespeed);
          }
          SaberSerial.write((byte)twospeed);
          delay(30);
        }
      }
      else{
        for (twospeed; twospeed < SABER_MOTOR2_FULL_STOP; twospeed++){
          if(Serial.available()>0)
            break;
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
  pinMode(MOTORENABLE, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BLADEENABLE, OUTPUT);
  // Open serial communications and wait for port to open
  Serial.begin(9600);
  Serial.println("Welcome");

  // set the data rate for the SoftwareSerial port
  SaberSerial.begin(SABERBAUD);
  //allow to init
  delay( 2000 );
  setEngineSpeed(ALLSTOP);
  
  
}

void loop() {
  signed char traverse; //speed value
  //check if character available
  if(Serial.available()>0) 
  {
    signed char data = Serial.read();
    switch(data)
    { 
      //turn motor controller on/off
      case 'O':
        power = !power;
        digitalWrite(MOTORENABLE, power);
        setEngineSpeed(ALLSTOP);
        break;
        
      //Full Forward  
      case'F':
        digitalWrite(BUZZER, LOW);
        if (dir == 1 && type == 1){
          i=0;
          signed int onespeeddif = abs(onespeed - 64);
          signed int twospeeddif = abs(twospeed - 192);
          if(onespeeddif > twospeeddif)
            stopProcedure(1);
          else if(onespeeddif < twospeeddif)
            stopProcedure(2); 
          stopsig = 1;
        }
        for (i; i <= 100; i++){
          if(Serial.available()>0)
            break;
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(30);
        } 
        dir = 0;
        type = 0;
        stopsig = 0;
        break; 
        
      //Full Reverse
      case'B':
        digitalWrite(BUZZER, HIGH);
        digitalWrite(BLADEENABLE, HIGH);
        dir = 1;
        stopsig = 0;
        if (type == 1){
          i=0;
          signed int onespeeddif = abs(onespeed - 64);
          signed int twospeeddif = abs(twospeed - 192);
          if(onespeeddif > twospeeddif)
            stopProcedure(1);
          else if(onespeeddif < twospeeddif)
            stopProcedure(2); 
        }
        for (i; i >= -100; i--){
          if(Serial.available()>0)
            break;
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(30);
        }
        type = 0;
        break; // Reverse
      
      //Stop
      case'S':
        digitalWrite(BUZZER, LOW);
        digitalWrite(BLADEENABLE, LOW);
        dir = 0;
        if (type == 1){
          i=0;
          signed int onespeeddif = abs(onespeed - 64);
          signed int twospeeddif = abs(twospeed - 192);
          if(onespeeddif > twospeeddif)
            stopProcedure(1);
          else if(onespeeddif < twospeeddif)
            stopProcedure(2); 
        }
        else
          stopProcedure(0);
        stopsig = 1; 
        break;
   /////////////////////////////////////////////     
      
      case'5':
        type = 1;
        if(stopsig == 1){
            int k = 192;
            for(int j = 64; j <= SABER_MOTOR1_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR2_TURN_FOUR){
                k = k++;
                SaberSerial.write((byte) k);
                twospeed=k;
              }
              SaberSerial.write((byte) j);
              onespeed=j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR1_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_FOUR_REVERSE);
            onespeed = SABER_MOTOR1_FULL_REVERSE;
            twospeed = SABER_MOTOR2_TURN_FOUR_REVERSE;
            i=-80;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_FOUR);
            onespeed = SABER_MOTOR1_FULL_FORWARD;
            twospeed = SABER_MOTOR2_TURN_FOUR;
            i=80;
          }
        }
        break;  //18 degree right turn
        
      case'6':
        type = 1;
        if(stopsig == 1){
            int k = 192;
            for(int j = 64; j <= SABER_MOTOR1_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR2_TURN_THREE){
                k = k++;
                SaberSerial.write((byte) k);
                twospeed=k;
              }
              SaberSerial.write((byte) j);
              onespeed=j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR1_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_THREE_REVERSE);
            onespeed = SABER_MOTOR1_FULL_REVERSE;
            twospeed = SABER_MOTOR2_TURN_THREE_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_THREE);
            onespeed = SABER_MOTOR1_FULL_FORWARD;
            twospeed = SABER_MOTOR2_TURN_THREE;
          }
        }break;  //36 degree RIGHT turn
        
      case'7':
        type = 1;
        if(stopsig == 1){
            int k = 192;
            for(int j = 64; j <= SABER_MOTOR1_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR2_TURN_TWO){
                k = k++;
                SaberSerial.write((byte) k);
                twospeed = k;
              }
              SaberSerial.write((byte) j);
              onespeed=j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR1_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_TWO_REVERSE);
            onespeed = SABER_MOTOR1_FULL_REVERSE;
            twospeed = SABER_MOTOR2_TURN_TWO_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_TWO);
            onespeed = SABER_MOTOR1_FULL_FORWARD;
            twospeed = SABER_MOTOR2_TURN_TWO;
          }
        } break;  //54 degree RIGHT turn
        
      case'8':
        type = 1;
        if(stopsig == 1){
            int k = 192;
            for(int j = 64; j <= SABER_MOTOR1_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR2_TURN_ONE){
                k = k++;
                SaberSerial.write((byte) k);
                twospeed = k;
              }
              SaberSerial.write((byte) j);
              onespeed = j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR1_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_ONE_REVERSE);
            onespeed = SABER_MOTOR1_FULL_REVERSE;
            twospeed = SABER_MOTOR2_TURN_ONE_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR2_TURN_ONE);
            onespeed = SABER_MOTOR1_FULL_FORWARD;
            twospeed = SABER_MOTOR2_TURN_ONE;
          }
        }break;  //72 degree RIGHT turn
 
      case'9':
        type = 1;
        if(stopsig == 1){
            for(int j = 64; j <= SABER_MOTOR1_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              SaberSerial.write((byte) j);
              onespeed = j;
              delay(30);
            }
            stopsig = 0;
            twospeed = SABER_MOTOR2_FULL_STOP;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR1_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR2_FULL_STOP);
            onespeed = SABER_MOTOR1_FULL_REVERSE;
            twospeed = SABER_MOTOR2_FULL_STOP;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR2_FULL_STOP);
            onespeed = SABER_MOTOR1_FULL_FORWARD;
            twospeed = SABER_MOTOR2_FULL_STOP;
          }
        }break;  //90 degree RIGHT turn
     ///////////////////////////////////////////
      case'4':
        type = 1;
        if(stopsig == 1){
            int k = 64;
            for(int j = 192; j <= SABER_MOTOR2_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR1_TURN_FIVE){
                k = k++;
                SaberSerial.write((byte) k);
                onespeed = k;
              }
              SaberSerial.write((byte) j);
              twospeed = j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR2_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_FIVE_REVERSE);
            twospeed = SABER_MOTOR2_FULL_REVERSE;
            onespeed = SABER_MOTOR1_TURN_FIVE_REVERSE;
            i=-80;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_FIVE);
            twospeed = SABER_MOTOR2_FULL_FORWARD;
            onespeed = SABER_MOTOR1_TURN_FIVE;
            i=80;
          }
        }break;  //18 degree LEFT turn
        
      case'3':
        type = 1;
        if(stopsig == 1){
            int k = 64;
            for(int j = 192; j <= SABER_MOTOR2_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR1_TURN_SIX){
                k = k++;
                SaberSerial.write((byte) k);
                onespeed = k;
              }
              SaberSerial.write((byte) j);
              twospeed = j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR2_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_SIX_REVERSE);
            twospeed = SABER_MOTOR2_FULL_REVERSE;
            onespeed = SABER_MOTOR1_TURN_SIX_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_SIX);
            twospeed = SABER_MOTOR2_FULL_FORWARD;
            onespeed = SABER_MOTOR1_TURN_SIX;
          }
        }break;  //36 degree LEFT turn
        
      case'2':
        type = 1;
        if(stopsig == 1){
          int k = 64;
            for(int j = 192; j <= SABER_MOTOR2_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR1_TURN_SEVEN){
                k = k++;
                SaberSerial.write((byte) k);
                onespeed = k;
              }
              SaberSerial.write((byte) j);
              twospeed = j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR2_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_SEVEN_REVERSE);
            twospeed = SABER_MOTOR2_FULL_REVERSE;
            onespeed = SABER_MOTOR1_TURN_SEVEN_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_SEVEN);
            twospeed = SABER_MOTOR2_FULL_FORWARD;
            onespeed = SABER_MOTOR1_TURN_SEVEN;
          }
        }break;  //54 degree LEFT turn
        
      case'1':
        type = 1;
        if(stopsig == 1){
            int k = 64;
            for(int j = 192; j <= SABER_MOTOR2_FULL_FORWARD; j++){
              if(Serial.available()>0)
                break;
              if(k<SABER_MOTOR1_TURN_EIGHT){
                k = k++;
                SaberSerial.write((byte) k);
                onespeed = k;
              }
              SaberSerial.write((byte) j);
              twospeed = j;
              delay(30);
            }
            stopsig = 0;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR2_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_EIGHT_REVERSE);
            twospeed = SABER_MOTOR2_FULL_REVERSE;
            onespeed = SABER_MOTOR1_TURN_EIGHT_REVERSE;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR1_TURN_EIGHT);
            twospeed = SABER_MOTOR2_FULL_FORWARD;
            onespeed = SABER_MOTOR1_TURN_EIGHT;
          }
        }break;  //72 degree LEFT turn
        
      case'0':
        type = 1;
        if(stopsig == 1){
            for(int j = 192; j <= SABER_MOTOR2_FULL_FORWARD; j++){
              SaberSerial.write((byte) j);
              twospeed = j;
              delay(30);
            }
            stopsig = 0;
            onespeed = SABER_MOTOR1_FULL_STOP;
        }
        else{
          if(dir == 1){
            SaberSerial.write((byte) SABER_MOTOR2_FULL_REVERSE);
            SaberSerial.write((byte) SABER_MOTOR1_FULL_STOP);
            twospeed = SABER_MOTOR2_FULL_REVERSE;
            onespeed = SABER_MOTOR1_FULL_STOP;
          }
          else{
            SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
            SaberSerial.write((byte) SABER_MOTOR1_FULL_STOP);
            twospeed = SABER_MOTOR2_FULL_FORWARD;
            onespeed = SABER_MOTOR1_FULL_STOP;
          }
        }break;  //90 degree LEFT turn
    }

    Serial.print("Data = ");
    Serial.println(data);
    Serial.print("Speed of Motor 1 = ");
    Serial.println(onespeed);
    Serial.print("Speed of Motor 2 = ");
    Serial.println(twospeed);
    delay(2);
  }
}
