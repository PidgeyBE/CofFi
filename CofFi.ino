/*************************************************************
CofFi by pidgey.be
 *************************************************************/




#include <ESP8266WiFi.h>
#include <Servo.h> 
 
Servo myservo;  // create servo object to control a servo 
                // twelve servo objects can be created on most boards
 

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "";
char pass[] = "";

// Remote site information
const char http_site[] = "www.xxx.xx";
const int http_port = 80;


WiFiClient client;

int coffeeStandby = 90;
int coffeeOn = 20;
int coffeeGo = 161;
int makeTime = 35; //seconds
int heatupTime = 60;

int servoPin = 4;
int servoTransPin = 5;

int debug = 1;
int manual = 0;
int incomingByte = 0;
int total = 0;
int pollingTime = 60; //seconds

void setup()
{
  pinMode(2,INPUT_PULLUP); //trying to make sure these pins are high or low on deepsleep restart to avoid errors
  pinMode(0,INPUT_PULLUP);
  pinMode(servoTransPin, OUTPUT);
  digitalWrite(servoTransPin, LOW);
  pinMode(15,INPUT);
  digitalWrite(15, LOW);
  if(debug) {Serial.begin(9600); Serial.println("hello");}
  //initServo();  
  connectWifi();
  if(manual && debug) manualControl();
  else checkForCoffeeRequest();
 
  goSleep();
 
}

void loop()
{

}

void goSleep()
{
  if(getPage(String("CofFi_get.php?deepsleep=w")))
  {
    String response = readResponse();
    pollingTime = response.toInt();
    if(pollingTime==0) pollingTime = 60;
  }
  if(debug) Serial.println(String("going to sleep for ")+pollingTime+"s...");
  ESP.deepSleep(1000L*1000*pollingTime, WAKE_RF_DEFAULT); //us
}

void manualControl()
{
   while(true)
   {
      if (Serial.available() > 0) {
                // read the incoming byte:
                total = 0;
                int i=1;
                while(Serial.available()>0)
                {
                  incomingByte = Serial.parseInt();
                }
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
                myservo.attach(servoPin);
                digitalWrite(servoTransPin, HIGH);
                setServo(incomingByte);
                setServo(coffeeStandby);
                digitalWrite(servoTransPin, LOW);
                myservo.detach();
      }
   }
}

void initServo()
{
  myservo.attach(servoPin);  // attaches the servo on GIO2 to the servo object 
  setServo(coffeeStandby);
  myservo.detach();
}

void connectWifi()
{
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
   delay(500);
   if(debug) Serial.print(".");
  }
}



void setServo(int degr)
{
  if(debug) {Serial.print("going to "); Serial.print(degr); }
  
  int current = myservo.read();
  int stepp = 1;
  if((degr-current) <0) stepp = -1; //turn clockwise or anticlockwise
    for(int pos = current; pos != degr; pos = pos+stepp) 
    {                                  // in steps of 1 degree 
         
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(15);                       // waits 15ms for the servo to reach the position          
    } 
}

void checkForCoffeeRequest()
{
  if(getPage("CofFi_get.php"))
  {
    String response = readResponse();
    if(debug) Serial.println("response is "+response);
    if(response.indexOf("CofFi")!=-1)
    {
      int id = response.substring(response.indexOf(" ")+1).toInt(); //get coffee id out of response
      makeCoffee(id);      
    }    
  }   
}

void makeCoffee(int id)
{
  getPage(String("CofFi_set.php?id=")+id+"&start=1");
  myservo.attach(servoPin);
  digitalWrite(servoTransPin, HIGH);
  //turn device on
  setServo(coffeeOn);
  setServo(coffeeStandby);
  getPage(String("CofFi_set.php?id=")+id+"&heatup=1");
  delay(1000*heatupTime); //warm up time

  //start coffee
  setServo(coffeeGo);
  setServo(coffeeStandby);
  getPage(String("CofFi_set.php?id=")+id+"&making=1");
  delay(1000*makeTime); //coffeemaking time

  //stop coffee
  setServo(coffeeGo);
  setServo(coffeeStandby);

  //turn off
  setServo(coffeeOn);
  setServo(coffeeStandby);

  getPage(String("CofFi_set.php?id=")+id+"&done=1");

  digitalWrite(servoTransPin, LOW);
  myservo.detach();
}

String readResponse()
{
  String lees; 
  delay(500);
    while ( client.available() ) 
    {
      char c = client.read();
      if (c=='\n') {lees="";} //skip header
      else lees += c;
    }
    if(debug) Serial.println("+"+lees+"+");
    return lees;
}


// Perform an HTTP GET request to a remote page
bool getPage(String page) {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  String getter = "GET /domotix/"+page+" HTTP/1.1";
  if(debug) Serial.println("going to "+getter);
  client.println(getter);
  client.println("Host: www.xxx.be");
  client.println("User-Agent: PidgeyCofFi");
  client.println("Connection: close");
  client.println();
  
  return true;
}

