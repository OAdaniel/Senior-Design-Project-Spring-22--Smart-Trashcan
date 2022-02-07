,#include <WiFi.h>
#include <FirebaseESP32.h>
 
 
#define FIREBASE_HOST "https://esp32-web-smart-trashcan-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "sytOmeudeDgHUZbmhvadWkSrkkLKMY0lYUsy8MrV"
#define WIFI_SSID "Livelifefully2.4"
#define WIFI_PASSWORD "Ethiopian123"
#define MQ135_THRESHOLD_1 1000

const int trigPin = 25;
const int echoPin = 26;
float duration, distance;

//Define FirebaseESP32 data object
FirebaseData firebaseData;
FirebaseJson json;

 
void setup()
{
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(9600);

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
 
void loop()
{

//MQ135

int MQ135_data = analogRead(A0);
if(MQ135_data < MQ135_THRESHOLD_1){
Serial.print("Fresh Air ");
}else {
Serial.print("Poor Air. ");
}
Serial.println(MQ135_data);
json.set("/airQuality", MQ135_data);
Firebase.RTDB.pushInt(&firebaseData, "Device1/airQuality", MQ135_data);
//Firebase.RTDB.setInt(&firebaseData, "Device1/airQuality", MQ135_data);
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
Firebase.RTDB.pushInt(&firebaseData, "Device1/depth", distance);
//Firebase.RTDB.setInt(&firebaseData, "Device1/depth", distance);
//Firebase.push(firebaseData,"/Device1",json);
//Firebase.set(firebaseData,"/Device1",json);
delay(5000);
 
}
