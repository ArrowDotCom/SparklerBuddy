
//Sparkler Bot Code from Arrow Electronics
//******Thanks for making with us!********
//This is written for an arduino mega, but should run fine on an uno.
//The code is very basic.  It reads an analog input (a 2 potentiometer joystick)
//and decides which of the 8 directions to move the puck.  Up/upright/right/downright/down etc.
//Based on the direction, the needed X,Y position is updated and the hypotenuse of each resultant triangle is calculated.
//The hypotenuse is the length the string should be, so the program moves each motor accordingly.  Make sure you get
//the motor spool circumfrences and starting point right, or the code will move the motors inaccurately.  
//Have fun, and show us what you make!  

#include <Stepper.h>

//Set up all the variables I'll need later
//IMPORTANT - the X/Y position variables MUST be floating point numbers.  Even a long int is too small to contain all
//the data for position, then it wraps to a negative number, you try to take the square root of a negative number,
//your motors twitch angrily, it's ugly.  Use float.  

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution if using a different motor
int RPM = 40;  //60 RPM seems like a good place to start, but moved to 40 because it was too jerky
int TotalX = 1000; //My motors are 1000mm apart, so that's the largest X we can have
float LeftString = 707;  //string lengths when positioned "home" in center of board (in mm)
float RightString = 707;
float NewLeftString = 707;
float NewRightString = 707;
float AsqBsqL = 0;
int SpoolCir = 66; //Our spool has a circumference of 6.6cm, or 66mm
float CurrentX = 500;
float CurrentY = 500;
int LeftDelta = 0;
int RightDelta = 0;
int NewX = 0;  //Set these up as 0 so you don't get junk when you try to change them.
int NewY = 0;
int MoveScaler = 4; //Scaler in case moving 1 isn't fast enough for drawing
int StepperScaler = 0;  //Will use this to pair steps per revolution to mm
int joyPinX = 0; //Analog pin 0 associated with x movement of joystick
int joyPinY = 1; //Analog pin 1 associated with y movement of joystick
int joyValueX = 0;   //Reading of x axis
int joyValueY = 0;  // Reading of y axis
int joyDir = 0;  //case assignment

//set up LEDs for prettiness in the joystick box
int ledUP = 14;
int ledRIGHT = 15;
int ledDOWN = 16;
int ledLEFT = 17;


// initialize the stepper library for left motor on pins 3 through 6:
Stepper LeftStepper(stepsPerRevolution, 3, 4, 5, 6);

// initialize the stepper library for right motor on pins 8 through 11:
Stepper RightStepper(stepsPerRevolution, 8, 9, 10, 11);


void setup() {
 // initialize the serial port:
  Serial.begin(9600);
  
 //Set motor speeds
  LeftStepper.setSpeed(RPM);
  RightStepper.setSpeed(RPM);
  
  Serial.print("Motor Speed Set to");
  Serial.print(RPM);
  Serial.println( "rpm.");


}

void loop() {
  //First, we need to read the potentiometer to get direction:
  joyValueX = analogRead(joyPinX);  //get value between 0 and 1024 from A0
  delay(100);  //apparently a delay is needed between A pins
  joyValueY = analogRead(joyPinY);  //get value between 0 and 1024 from A1

  //Change XY positions based on which direction the joystick moved:

 /* Serial.print("X: ");
  Serial.print(joyValueX);
  Serial.print("    Y: ");
  Serial.println(joyValueY);
  */
  
  if ((joyValueX>400 && joyValueX<600) && joyValueY>600){
    //This means straight down
    joyDir = 1;
    digitalWrite(ledDOWN, HIGH);
    CurrentY = CurrentY - 2*MoveScaler;
    Serial.println("Joystick DOWN");
  }
  
  if (joyValueX>600 && joyValueY>600){
    //Down Right
    joyDir = 2;
    digitalWrite(ledDOWN, HIGH);
    digitalWrite(ledRIGHT, HIGH);
    CurrentX = CurrentX - 1*MoveScaler;
    CurrentY = CurrentY - 1*MoveScaler;
    Serial.println("Joystick DOWN RIGHT");
   }
     
  if (joyValueX>600 && (joyValueY>400 && joyValueY<600)){
    //Straight Right
    joyDir = 3;
    digitalWrite(ledRIGHT, HIGH);
    CurrentX = CurrentX - 2*MoveScaler;
    Serial.println("Joystick RIGHT");
   }

    if (joyValueX>600 && joyValueY<400){
    //up Right
    joyDir = 4;
    digitalWrite(ledUP, HIGH);
    digitalWrite(ledRIGHT, HIGH);
    CurrentX = CurrentX + 1*MoveScaler;
    CurrentY = CurrentY + 1*MoveScaler;
    Serial.println("Joystick UP RIGHT");
   }
   
  if ((joyValueX>400 && joyValueX<600) && joyValueY<400){
    //This means straight up
    joyDir = 5;
    digitalWrite(ledUP, HIGH);
    CurrentY = CurrentY + 2*MoveScaler;
    Serial.println("Joystick UP");
  }
  
   if (joyValueX<400 && joyValueY<400){
    //Down Right
    joyDir = 6;
    digitalWrite(ledUP, HIGH);
    digitalWrite(ledLEFT, HIGH);
    CurrentX = CurrentX + 1*MoveScaler;
    CurrentY = CurrentY + 1*MoveScaler;
    Serial.println("Joystick UP LEFT");
   }
  if (joyValueX<400 && (joyValueY>400 && joyValueY<600)){
    //Straight Left
    joyDir = 7;
    digitalWrite(ledLEFT, HIGH);
    CurrentX = CurrentX + 2*MoveScaler;
    Serial.println("Joystick LEFT");
   }
   
  if (joyValueX<400 && joyValueY>600){
    //down Left
    joyDir = 8;
    digitalWrite(ledLEFT, HIGH);
    digitalWrite(ledDOWN, HIGH);
    CurrentX = CurrentX + 1*MoveScaler;
    CurrentY = CurrentY - 1*MoveScaler;
    Serial.println("Joystick DOWN LEFT");
   }
  if (joyDir = 0){
    Serial.println("Joystick 0,0");
  }


//Do math to decide which motors to spin, and how much
StepperScaler = stepsPerRevolution/SpoolCir; //this is how many steps the motors must do to release 1mm of string.  For me, it's 2. 
AsqBsqL = (CurrentX*CurrentX) + (CurrentY*CurrentY);
//Serial.println(CurrentX*CurrentX);
//Serial.println(CurrentY*CurrentY);
//Serial.println(AsqBsqL);
NewLeftString = sqrt(AsqBsqL);
LeftDelta = LeftString - NewLeftString;
NewRightString = sqrt(((TotalX-CurrentX)*(TotalX-CurrentX)) + (CurrentY*CurrentY));
RightDelta = RightString - NewRightString;

//Spin the motors accordingly
LeftStepper.step(StepperScaler*LeftDelta);
RightStepper.step(-StepperScaler*RightDelta);

//Reset the things
joyDir = 0;
LeftString = NewLeftString;
RightString = NewRightString;
//Turning off all the LEDs each time causes a blinking thing to happen if the direction
//does not change.  Given more time, a few simple "if" loops would clean this up a lot. 
digitalWrite(ledUP, LOW);
digitalWrite(ledRIGHT, LOW);
digitalWrite(ledDOWN,  LOW);
digitalWrite(ledLEFT, LOW);

Serial.print("X:  ");
Serial.print(CurrentX);
Serial.print("     Y:  ");
Serial.println(CurrentY);
//Serial.print("Last Left:  ");
//Serial.print(LeftString);
//Serial.print("     Right:  ");
//Serial.println(RightString);
delay(50);


}
