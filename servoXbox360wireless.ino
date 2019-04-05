
#define ENABLE_UHS_DEBUGGING 1
#include <controllerEnums.h>


#include <XBOXRECV.h>
#include <Servo.h>
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#define LOOPTIME 25 //Define the delay between timed loop cycles

//Define where each servo is attached//
//#define SERVO1 2 // Left/Right Trigger : Base Joint
//#define SERVO2 3 // LeftHat            : Up&Down
//#define SERVO3 4 // RightHat           : Forward&Backward
//#define SERVO4 5 // Left/Right D-pad   : Claw

//Define initial servo position(initial Condition); (initiates when "start" button pressed).
#define SERVO1_INIT 90
#define SERVO2_INIT 90
#define SERVO3_INIT 90
#define SERVO4_INIT 90

#define SERVO1_STEP 4 //Step Value for L/R Trigger [Base Joint]
#define SERVO4_STEP 2 //Step Value for L/R D-pad   [Claw]

//Define rate for servos controller by sticks (larger is faster)
#define SERVO2_RATE 1/32000  //default 32000
#define SERVO3_RATE 1/32000

#define SERVO1_DIRECTION 1 //Set to -1 to reverse servo direction
#define SERVO2_DIRECTION 1 //Set to -1 to reverse servo direction
#define SERVO3_DIRECTION 1 //Set to -1 to reverse servo direction
#define SERVO4_DIRECTION 1 //Set to -1 to reverse servo direction

//Define minimum servo angles
#define SERVO1_MIN 0
#define SERVO2_MIN 70
#define SERVO3_MIN 0
#define SERVO4_MIN 50

//Define maximum servo angles
#define SERVO1_MAX 180
#define SERVO2_MAX 130
#define SERVO3_MAX 100
#define SERVO4_MAX 85

#define STICK_CENTER 0 //Value from controller when sticks are centered
#define DEADZONE 7500 //Deadzone to prevent unwanted movement

//=======================Global Variables========================//

USB Usb;
XBOXRECV Xbox(&Usb);

//Instantiate servo variables
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

unsigned long previousTime = 0; //for loop timing

int s1 = SERVO1_INIT;
int s2 = SERVO2_INIT;
int s3 = SERVO3_INIT;
int s4 = SERVO4_INIT;


//=======================Setup========================//

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while(!Serial); // Wait for serial port to connect
#endif
  if (Usb.Init() == -1){
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Initiated"));
  
  servo1.attach(2);
  servo2.attach(3);
  servo3.attach(4);
  servo4.attach(5);
  
  //Initially All Servos Disabled
  servo1.detach();
  servo2.detach();
  servo3.detach();
  servo4.detach();
}

//=======================Setup========================//

void loop() {
  Usb.Task(); //Task USB

  if(Xbox.XboxReceiverConnected || Xbox.Xbox360Connected) {
    for(uint8_t i=0; i<4;i++){
    
      if(Xbox.Xbox360Connected[i]){ //This loop executes only when Xbox 360 controller is connected

        //Start of timed
        
        if(Xbox.getButtonClick(L2,i)){
          s1 = s1 + SERVO1_STEP*SERVO1_DIRECTION;
          Serial.print("L2: ");
        }
        else if(Xbox.getButtonClick(R2,i)){
          s1 = s1 - SERVO1_STEP*SERVO1_DIRECTION;
        }
          if(s1 > SERVO1_MAX) s1 = SERVO1_MAX;
          if(s1 < SERVO1_MIN) s1 = SERVO1_MIN;
        
        if(Xbox.getAnalogHat(LeftHatY,i) > STICK_CENTER + DEADZONE 
          || Xbox.getAnalogHat(LeftHatY,i) < STICK_CENTER - DEADZONE){
            s2 = s2 + (Xbox.getAnalogHat(LeftHatY,i) - STICK_CENTER)*SERVO2_RATE*SERVO2_DIRECTION;
          }
          if(s2 > SERVO2_MAX) s2 = SERVO2_MAX;
          if(s2 < SERVO2_MIN) s2 = SERVO2_MIN;
        
        if(Xbox.getAnalogHat(RightHatY,i) > STICK_CENTER + DEADZONE
          || Xbox.getAnalogHat(RightHatY,i) < STICK_CENTER - DEADZONE){
            s3 = s3 + (Xbox.getAnalogHat(RightHatY,i) - STICK_CENTER)*SERVO3_RATE*SERVO3_DIRECTION;
          }
          if(s3 > SERVO3_MAX) s3 = SERVO3_MAX;
          if(s3 < SERVO3_MIN) s3 = SERVO3_MIN;

        if(Xbox.getButtonClick(LEFT,i)){
          s4 = s4 + SERVO4_STEP*SERVO4_DIRECTION;
        }
        else if(Xbox.getButtonClick(RIGHT,i)){
          s4 = s4 - SERVO4_STEP*SERVO4_DIRECTION;
        }
          if(s4 > SERVO4_MAX) s4 = SERVO4_MAX;
          if(s4 < SERVO4_MIN) s4 = SERVO4_MIN;
        
        //Debug information about servo positions. Uncomment when you are determining servo limits.
//        Serial.print("  s1 = ");  
//        Serial.print(s1);
//        Serial.print("  s2 = ");
//        Serial.print(s2);
//        Serial.print("  s3 = ");
//        Serial.print(s3);
//        Serial.print("  s4 = ");
//        Serial.print(s4);

        // Safety Release
        // Detaches all servos - waiting for button to re-engage
        if(Xbox.getButtonClick(B,i)){
          Serial.print("B ");
          servo1.detach();
          servo2.detach();
          servo3.detach();
          servo4.detach();
        }
        
        //Safety Restore
        //Also : Initializing Button : creative way to start robot
//        if(Xbox.getButtonClick(A,i)){
//          Serial.print("A ");
//          servo1.attach(2);
//          servo2.attach(3);
//          servo3.attach(4);
//          servo4.attach(5);
//        }

        //Reset ALL Servos to initial positions when "start" button is pressed
        if(Xbox.getButtonClick(START,i)){
          Serial.print("Start ");
          servo1.attach(2);
          servo2.attach(3);
          servo3.attach(4);
          servo4.attach(5);
          s1 = SERVO1_INIT;
          s2 = SERVO2_INIT;
          s3 = SERVO3_INIT;
          s4 = SERVO4_INIT;
        }

        previousTime = millis(); //save time at end of loop
      } //Timed loop
      
        servo1.write(s1);
        servo2.write(s2);
        servo3.write(s3);
        servo4.write(s4);
    }
  }

}
