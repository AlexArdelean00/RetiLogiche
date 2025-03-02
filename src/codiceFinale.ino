struct vec3{
  float x,y,z;
};

String inputString = ""; // for incoming serial data

// posizione dei sensori
vec3 S1 = {5, 5, 0}; // asse z
vec3 S2 = {5, 5, 10}; // asse z
vec3 S3 = {0, 5, 5}; // asse x
vec3 S4 = {10, 5, 5}; // asse x
vec3 S5 = {5, 0, 5}; // asse y
vec3 S6 = {5, 10, 5}; // asse y

float lato = 10; // cm lato cubesat
vec3 inputPoint;

float temp[6];
boolean dispSens[6] = {true, false, false, false, false, false};

void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  randomSeed(analogRead(0)); // seed per la funzione random
}

void loop() {
  // send data only when you receive data
  if (Serial.available() > 0) {    
    // read the incoming byte:
    inputString = Serial.readString();
    inputPoint = convertStringToVec(inputString); // legge il punto in cui stimare la temperatura

    for(int i=0; i<6; i++){
      if(dispSens[i])
        temp[i]=readTemperature(i);
      else
        temp[i]=random(-55, 150);
      Serial.print("t");
      Serial.print(i);
      Serial.print(": ");
      Serial.print(temp[i]);
      Serial.println();
    }
    
    float tx = interpolationX(inputPoint, temp[3], temp[4]);
    float ty = interpolationY(inputPoint, temp[5], temp[6]);
    float tz = interpolationZ(inputPoint, temp[1], temp[2]);
    float t = (tx+ty+tz)/3;
    Serial.print("temperatura stimata in ");
    vecPrint(inputPoint);
    Serial.print(": ");
    Serial.print(t);
    Serial.println();
  } 
}

// converte una stringa in un vettore tridimensionale
vec3 convertStringToVec(String s){
  char delimiter = ',';
  String str[3];
  int j=0;
  for(int i=0; i<s.length(); i++){
    if(s.charAt(i)!=delimiter){
      str[j]=s[i];
      j++;
    }     
  }
  vec3 in;
  in.x = str[0].toFloat();
  in.y = str[1].toFloat();
  in.z = str[2].toFloat();
  return in;
}

float interpolationX(vec3 p, float t3, float t4){
  float d1 = p.x-S3.x;
  float d2 = S4.x-p.x;
  float t = (d2/lato*t3+d1/lato*t4);
  return t;
}

float interpolationY(vec3 p, float t5, float t6){
  float d1 = p.y-S5.y;
  float d2 = S6.y-p.y;
  float t = (d2/lato*t5+d1/lato*t6);
  return t;
}

float interpolationZ(vec3 p, float t1, float t2){
  float d1 = p.z-S1.z;
  float d2 = S2.z-p.z;
  float t = (d2/lato*t1+d1/lato*t2);
  return t;
}

float readTemperature(int analog){
  int ADCValue = analogRead(analog);
  ADCValue-=512; //riferimento a GND (2.5*1024/5)
  float mv = (ADCValue/1024.0)*5*1000;
  float celsius = mv/10; // 10mV ogni grado
  return celsius;
}

void vecPrint(vec3 v){
  Serial.print("(");
  Serial.print(v.x);
  Serial.print(",");
  Serial.print(v.y);
  Serial.print(",");
  Serial.print(v.z);
  Serial.print(")");
}
