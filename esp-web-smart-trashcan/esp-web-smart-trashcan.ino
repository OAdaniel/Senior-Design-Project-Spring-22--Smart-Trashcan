#include <WiFi.h>
#include <FirebaseESP32.h>
 
#define TIME_TO_SLEEP  0.5   // in minutes
#define FIREBASE_HOST "Your info"
#define FIREBASE_AUTH "Your info"
#define WIFI_SSID "Your info"
#define WIFI_PASSWORD "your info"
#define MQ135_THRESHOLD_1 1000
#define DISTANCE_THRESHOLD 10

const int trigPin = 25;
const int echoPin = 26;
const int ledPin = 5;
float duration, distance;

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

 
void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  
  wifiDbSetup();
  getReadings();
  goToDeepSleep();

}
 
void loop()
{

}

void wifiDbSetup(){
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
  Serial.print(".");
  delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
   
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
   
  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
   
  Serial.println("------------------------------------");
  Serial.println("Connected...");
  
}
void goToDeepSleep(){
  Serial.println("Going to sleep for half a minute.... ");
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * 60 * 1000000);
  esp_deep_sleep_start();
}

void getReadings(){
 //MQ135
 
  int MQ135_data = analogRead(A0);
  if(MQ135_data < MQ135_THRESHOLD_1){
    Serial.print("Fresh Air ");
  }
  else {
    Serial.print("Poor Air. ");
  }
  
  Serial.println(MQ135_data);
  json.set("/airQuality", MQ135_data);

  //HC-SR04
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  
  Serial.print("Distance: ");
  Serial.println(distance);
  json.set("/depth", distance);
  
  if(distance<DISTANCE_THRESHOLD){
    digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level) 
  }
  else{
    digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW      
  }
   
  Firebase.push(firebaseData,"/Device1",json);

  delay(100);
  
 }
