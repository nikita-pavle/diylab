#include <SoftwareSerial.h>
SoftwareSerial MySerial(13, 12); // RX | TX
#define IN1 8 // Direction control for motor A
#define IN2 9 // Direction control for motor A
#define IN3 10 // Direction control for motor B
#define IN4 11
#define ENAB 3  //enable pins for both motors (speed control)
#define S1 2 //for both color sensors
#define S2 4   //for both sensors to read data
#define S3 7    //for both sensors to read data
#define OUT_1 6 //Output Pin for first sensor
#define OUT_2 5 //Output Pin for second sensor
String sensorLeft, sensorRight, command="";
char character;
int i=0;

void setup() {
    // Setup for sensor 1
  pinMode(ENAB, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(OUT_1, INPUT);
  analogWrite(ENAB, 80); 
  digitalWrite(S1, HIGH);

  // Setup for sensor 2
  pinMode(OUT_2, INPUT);
   pinMode(IN1, OUTPUT);    //control pins for motor
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600);
  // HC-05 default baNd rate is usually 9600
  MySerial.begin(9600);
}
void loop() {
   while(!MySerial.available()){
    if(MySerial.available()){
      break;
    }
   }

   while(MySerial.available()){
    character = MySerial.read();
    Serial.println(character);
    command.concat(character);
    Serial.println(command);
   if((command.equals("red"))||(command.equals("blue"))||(command.equals("green"))){ // if color name matches
     Serial.println("now");
     break;
   }                                                                                                                                                      
  
   }

   sensorLeft=readColor(1);
   sensorRight=readColor(2);
   Serial.println(sensorLeft);
   Serial.println(sensorRight);
    delay(2000);
   rotateCar360();

   while(1)
  {
   sensorLeft=readColor(1);
   sensorRight=readColor(2);
    if(sensorLeft.equals(command)&&sensorRight.equals(command))
      moveForward();
    else if(sensorLeft.equals(command)&&(!sensorRight.equals(command)))
        rotateLeft();
    else if(sensorRight.equals(command)&&(!sensorLeft.equals(command)))
        rotateRight();
  }
  i++;
  
  
}


void rotateCar360() {
  while(1)
  {
  analogWrite(ENAB, 75);
   digitalWrite(IN1, HIGH);
   digitalWrite(IN3, HIGH);
   delay(500);
   StopCar();
   delay(800);
   Serial.println("Left" + readColor(1));
   Serial.println("Right" + readColor(2));
   if(readColor(1).equals(command)&&readColor(2).equals(command))
   { 
    Serial.println("Lets go");
    StopCar();
    Detect();
    exit(0);
   }
   digitalWrite(IN2, HIGH);
   digitalWrite(IN4, HIGH);
   delay(400);
   StopCar();
   delay(800);
   analogWrite(ENAB, 80);
   digitalWrite(IN1, HIGH);
   digitalWrite(IN2, LOW);
   digitalWrite(IN3, LOW);
   digitalWrite(IN4, HIGH);
   delay(300);
   StopCar();
   delay(800);
  }
  

}
void Detect()
{
   while(1)
  {
   sensorLeft=readColor(1);
   sensorRight=readColor(2);
   //Serial.println(sensorRight);
    Serial.println("Leftrot" + readColor(1));
   Serial.println("Rightrot" + readColor(2));
    if(sensorRight.equals("black")||sensorLeft.equals("black"))
     {
      Serial.println("Black");
      StopCar();
      exit(0);
     }
    if(sensorLeft.equals(command)&&sensorRight.equals(command))
      moveForward();
    else if(sensorLeft.equals(command)&&(!sensorRight.equals(command)))
        rotateLeft();
    else if(sensorRight.equals(command)&&(!sensorLeft.equals(command)))
        rotateRight();
    delay(500);
  } 
}
void StopCar(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

}
void moveForward() {
  Serial.println("Moving");
  analogWrite(ENAB, 75);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(200);
 if(!(sensorLeft.equals(command)&&sensorRight.equals(command)))
      exit(0);
}
void rotateLeft()
{
  Serial.print("MOVING LEFT");
  analogWrite(ENAB, 80);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(1000);
  if((sensorLeft.equals(command)&&sensorRight.equals(command)))
      exit(0);
}
void rotateRight()
{
  Serial.println("Moving Rught");
  analogWrite(ENAB, 80);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(1000);
  if((sensorLeft.equals(command)&&sensorRight.equals(command)))
      exit(0);
}
String readColor(int sensor) {
  int red1 = 0,red2=0, green1 = 0,green2=0, blue1 = 0,blue2=0 , BLACK_THRESHOLD=100;
    digitalWrite(S2, LOW);
    digitalWrite(S3, LOW);
    red1= pulseIn(OUT_1, LOW);
    red2 = pulseIn(OUT_2, LOW);
    //Serial.println(red1 , red2);
    // Reading green color
    digitalWrite(S2, HIGH);
    digitalWrite(S3, HIGH);
    green1 = pulseIn(OUT_1, LOW);
    green2 = pulseIn(OUT_2, LOW);
    //Serial.println(green1 , green2);
    // Reading blue color
    digitalWrite(S2, LOW);
    digitalWrite(S3, HIGH);
    blue1 = pulseIn(OUT_1, LOW);
    blue2 = pulseIn(OUT_2, LOW);  
    //Serial.println(blue1 , blue2);
 // Simple color classification based on the highest value
 if ((red1 >25 && green1 > 35 && blue1 > 20 && sensor==1)||(red2 > 20 && green2 > 30 && blue2 > 25 && sensor==2)){
    return "black";
  }
  else if ((red1 < 7  && green1 < 7 && blue1 < 7 && sensor==1)||(red2 <7 && green2 <7  && blue2 < 7  && sensor==2)){
    return "white";
  }
  else if((red1 < green1 && red1 < blue1 && sensor==1)||(red2<green2 && red2<blue2 && sensor==2)) {
    return "red";
  } else if((green1 < red1&& green1 < blue1 && sensor==1)||(green2 < red2 && green2 < blue2 && sensor==2)) {
    return "green";
  } else if((blue1 < red1 && blue1 < green1 && sensor==1)||(blue2 < red2 && blue2 < green2 && sensor==2)) {
    return "blue";
  }
    else
    return "unknown";
}
