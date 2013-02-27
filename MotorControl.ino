
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
////////////////////////////////////////////////////////////////////////////////////

int i = 0;
int type;
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
        type = 0;
        for (i; i <= 100; i++){
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(40);
        } break; // Forward
        
      case'B':
        type = 0;
        for (i; i >= -100; i--){
          traverse = i;
          setEngineSpeed( traverse ); 
          delay(40);
          } break; // Reverse
      
      case'S':
        if(type == 0){ 
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
          }
        }
        else if (type == 1){
          i=0;
          SaberSerial.write((byte)ALLSTOP);
        } break;
   /////////////////////////////////////////////     
      case'4':
        type = 0;
        i=80;
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_FOUR );
        break;  //18 degree left turn
        
      case'3':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_THREE );
        break;  //36 degree left turn
        
      case'2':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_TWO );
        break;  //54 degree left turn
        
      case'1':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_TURN_ONE );
        break;  //72 degree left turn
 
      case'0':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR1_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR2_FULL_STOP );
        break;  //90 degree left turn
     ///////////////////////////////////////////
      case'5':
        type = 0;
        i=80;
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_FIVE );
        break;  //18 degree right turn
        
      case'6':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_SIX );
        break;  //36 degree right turn
        
      case'7':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_SEVEN );
        break;  //54 degree right turn
        
      case'8':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_TURN_EIGHT );
        break;  //72 degree right turn
        
      case'9':
        type = 1;
        SaberSerial.write((byte) SABER_MOTOR2_FULL_FORWARD);
        SaberSerial.write((byte) SABER_MOTOR1_FULL_STOP );
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
