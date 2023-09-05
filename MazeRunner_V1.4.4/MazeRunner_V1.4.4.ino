// Include necessary libraries
#include <SPI.h>               // For Serial Peripheral Interface communication
#include <Wire.h>              // For I2C communication
#include <Adafruit_GFX.h>      // Adafruit's graphics library for displays
#include <Adafruit_SSD1306.h>  // Adafruit's SSD1306 OLED display library
#include <NewPing.h>           // Library for using ultrasonic distance sensor
#include <L298NX2.h>           // Library for controlling motors using L298N module

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include "WebServer.h"
#include "ShortestPath.h"

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "MazeRunner";
const char* password = "robot1234";


const char* PARAM_INPUT_1 = "decrement";
const char* PARAM_INPUT_2 = "increment";



AsyncWebServer server(80);


void notFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "Not found");
}

#define SONAR_NUM 3       // Number of sensors.
#define MAX_DISTANCE 400  // Maximum distance (in cm) to ping.

int block = 10;
// int motorDelay = 5000;
// int front, left, right;  // Declare variables to store distances from sensors
//bool F = true, L = false, R = false;  // Define boolean variables to represent sensor directions

int IR[6] = { 33, 32, 35, 34, 39, 36 };
int A, B, C, D, E, F, sum;
int startPoint = 0, endPoint = 2, myLoc = 0;
String PATH = myPATH(endPoint);
bool start = false;


// Initialize OLED display (128x64 pixels) using I2C communication.
Adafruit_SSD1306 display(128, 32, &Wire, -1);



// Left Motor connections
int enL = 13, in1 = 14, in2 = 12;
// Right Motor connections
int enR = 25, in3 = 26, in4 = 27;

int LeftSpeed = 100;
int RightSpeed = 100;
int wait = 500;

L298N LeftMotor(enL, in1, in2);
L298N RightMotor(enR, in3, in4);


// Function to initialize the OLED display
void displaySetup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.display();                          // showing the Adafruit logo
  // do not forget display.display(); otherwise the picture will not be visible
  display.setTextColor(WHITE);
  //display.setTextSize(2);
}


void setup() {
  Serial.begin(115200);  // Initialize the serial communication for debugging

  pinMode(IR[0], INPUT);
  pinMode(IR[1], INPUT);
  pinMode(IR[2], INPUT);
  pinMode(IR[3], INPUT);
  pinMode(IR[4], INPUT);
  pinMode(IR[5], INPUT);


  displaySetup();  // Initialize the OLED display

  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  digitalWrite(IR[0], LOW);
  digitalWrite(IR[1], LOW);
  digitalWrite(IR[2], LOW);
  digitalWrite(IR[3], LOW);
  digitalWrite(IR[4], LOW);



  LeftMotor.setSpeed(LeftSpeed);
  RightMotor.setSpeed(RightSpeed);

  delay(200);
  setServer();

  //   if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  //   Serial.println("STA Failed to configure");
  // }
}



void readIR() {
  A = digitalRead(IR[0]);  // Measure front distance
  B = digitalRead(IR[1]);  // Measure left distance
  C = digitalRead(IR[2]);  // Measure right distance
  D = digitalRead(IR[3]);  // Measure left distance
  E = digitalRead(IR[4]);  // Measure right distance
  F = digitalRead(IR[5]);  // Measure right distance

  sum = A + B + C + D + E;
}

void loop() {
  // RightMotor.forward();
  // LeftMotor.forward();
  // LeftMotor.setSpeed(LeftSpeed);
  // RightMotor.setSpeed(RightSpeed);
  // readIR();
  // showEndPoint();

  if (start) {
    readIR();
    DisplayStatus();
    followPath();
  } else {
    readIR();
    showEndPoint();
    RightMotor.stop();
    LeftMotor.stop();
  }
}




void followPath() {
  if (0 == A && 0 == E) {
    //This is a single line path have to go straight
    goStraight();
  } else {
    //there has a stop point or multiple path
    choosePath();
  }
}




void goStraight() {
  if (1 == C) {
    RightMotor.forward();
    LeftMotor.forward();
    //the middle sensor is on the track.
    if (1 == B) {
      //the car has moved slightly on the right
      LeftMotor.setSpeed(LeftSpeed-10);
      RightMotor.setSpeed(RightSpeed);
    } else if (1 == D) {
      //the car has moved slightly on the left
      LeftMotor.setSpeed(LeftSpeed);
      RightMotor.setSpeed(RightSpeed-10);
    } else {
      //the car is straight on the track
      LeftMotor.setSpeed(LeftSpeed);
      RightMotor.setSpeed(RightSpeed);
    }
  } else {
    LeftMotor.setSpeed(LeftSpeed-10);
    RightMotor.setSpeed(RightSpeed-10);
    //the car is getting out from the track
    if (1 == B) {
      //the car has moved to the right
      LeftMotor.stop();
      RightMotor.forward();
    } else if (1 == D) {
      //the car has moved to the left
      RightMotor.forward();
      RightMotor.stop();
    }
  }
}

void choosePath() {
  if (PATH[myLoc] == 'F') {
    LeftMotor.setSpeed(LeftSpeed - 10);
    RightMotor.setSpeed(RightSpeed - 10);
    //go Straight and skip the line
    while (A == 1 || E == 1) {
      readIR();
      DisplayStatus();
    }
    myLoc++;
  } else if (PATH[myLoc] == 'L') {
    //go left
    goLeft();
    myLoc++;
  } else if (PATH[myLoc] == 'R') {
    //go right
    goRight();
    myLoc++;
  } else if (PATH[myLoc] == 'S') {
    //stop car
    RightMotor.stop();
    LeftMotor.stop();
    PATH = "END";
    myLoc = 0;
    start = false;
  }
}

void goRight() {
  LeftMotor.setSpeed(LeftSpeed-10);
  RightMotor.setSpeed(RightSpeed-10);
  while (1 == A || 1 == E) {
    readIR();
    DisplayStatus();
  }
  RightMotor.stop();
  LeftMotor.stop();
  delay(500);
  RightMotor.backward();
  LeftMotor.forward();
  while (0 == E) {
    readIR();
    DisplayStatus();
  }
  while (0 == C) {
    readIR();
    DisplayStatus();
  }
  RightMotor.stop();
  LeftMotor.stop();
  delay(500);
}


void goLeft() {  // Function to make the robot turn left
  LeftMotor.setSpeed(LeftSpeed-10);
  RightMotor.setSpeed(RightSpeed-10);
  while (1 == A || 1 == E) {
    readIR();
    DisplayStatus();
  }
  RightMotor.stop();
  LeftMotor.stop();
  delay(500);
  LeftMotor.backward();
  RightMotor.forward();
  while (0 == A) {
    readIR();
    DisplayStatus();
  }
  while (0 == C) {
    readIR();
    DisplayStatus();
  }
  RightMotor.stop();
  LeftMotor.stop();
  delay(500);
}



void setServer() {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    //request->send_P(200, "text/html", index_html);
    RightMotor.stop();
    LeftMotor.stop();
    myLoc = 0;
    endPoint = 2;
    PATH = myPATH(endPoint);
    start = false;
    request->send(200, "text/html", myHTML(myLoc, startPoint, endPoint, PATH));
  });


  server.on("/decrement", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (endPoint > 1) {
      endPoint = endPoint - 1;
    }
    PATH = myPATH(endPoint);
    Serial.println(endPoint);
    request->send(200, "text/html", myHTML(myLoc, startPoint, endPoint, PATH));
  });

  server.on("/increment", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (endPoint < 12) {
      endPoint = endPoint + 1;
    }
    PATH = myPATH(endPoint);
    Serial.println(endPoint);
    request->send(200, "text/html", myHTML(myLoc, startPoint, endPoint, PATH));
  });

  server.on("/start", HTTP_GET, [](AsyncWebServerRequest* request) {
    start = true;
    Serial.println(start);
    request->send(200, "text/html", myHTML(myLoc, startPoint, endPoint, PATH));
  });

  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest* request) {
    start = false;
    Serial.println(start);
    request->send(200, "text/html", myHTML(myLoc, startPoint, endPoint, PATH));
  });

  server.onNotFound(notFound);
  server.begin();
}



// Function to display the status information on the OLED display
void DisplayStatus() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.print("A:");
  display.print(A);

  display.setCursor(25, 0);
  display.print("B:");
  display.print(B);

  display.setCursor(50, 0);
  display.print("C:");
  display.print(C);

  display.setCursor(75, 0);
  display.print("D:");
  display.print(D);

  display.setCursor(100, 0);
  display.print("E:");
  display.print(E);

  display.setCursor(0, 10);
  display.print("CP:");
  display.print(myLoc);

  display.setCursor(30, 10);
  display.print("StartP:");
  display.print(startPoint);

  display.setCursor(80, 10);
  display.print("EndP:");
  display.print(endPoint);

  display.setCursor(0, 20);
  display.print("Route:");
  display.print(PATH);


  display.display();

  Serial.print("IR1:");
  Serial.print(A);

  Serial.print(" IR2:");
  Serial.print(B);

  Serial.print(" IR3:");
  Serial.print(C);

  Serial.print(" IR4:");
  Serial.print(D);

  Serial.print(" IR5:");
  Serial.print(E);

  Serial.print(" IR6:");
  Serial.print(F);

  Serial.println(" ");
}


// Function to display the status information on the OLED display
void showEndPoint() {
  display.clearDisplay();
  display.setTextSize(1);

  display.setCursor(0, 0);
  display.println(WiFi.localIP());


  display.setCursor(0, 10);
  display.print("CP:");
  display.print(myLoc);

  display.setCursor(30, 10);
  display.print("StartP:");
  display.print(startPoint);

  display.setCursor(80, 10);
  display.print("EndP:");
  display.print(endPoint);

  display.setCursor(0, 20);
  display.print("A:");
  display.print(A);

  display.setCursor(25, 20);
  display.print("B:");
  display.print(B);

  display.setCursor(50, 20);
  display.print("C:");
  display.print(C);

  display.setCursor(75, 20);
  display.print("D:");
  display.print(D);

  display.setCursor(100, 20);
  display.print("E:");
  display.print(E);

  display.setTextSize(0.5);

  display.setCursor(0, 30);
  display.print("F:");
  display.print(F);

  display.setCursor(50, 30);
  display.print("sum:");
  display.print(sum);

  display.display();
}
