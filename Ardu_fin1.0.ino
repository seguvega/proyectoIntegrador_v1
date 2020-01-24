#include <Servo.h>
#define motor A0         ///Conf ----> Puerto del motor 
#define piston A2        //Conf -----> puerto del piston
#define triggerEmisor 2  //trigger
#define echoReceptor 3  //Echo
#define infraPin 9      //Infrarojo
#define servo 12      //servo
String DatosE,DatosR,d1,d2;
int distancia, cont=0, value = 0, vm , vs, b1, b2,b3,i,var2; 
//------------------------
const int enPin = 5; 
const int in1Pin = 7;
const int in2Pin = 4;
char k='2',var1='9';
int ks = 3;
Servo servoMotor;
void setup() {
  Serial.begin(115200); // Inicia el puerto de comunicaciones en serie
  Serial2.begin(115200);
  pinMode(triggerEmisor,OUTPUT); // El emisor emite por lo que es configurado como salida
  pinMode(echoReceptor,INPUT);   // El receptor recibe por lo que es configurado como entrada
  pinMode(infraPin , INPUT);  //definir pin como entrada
  pinMode(in1Pin, OUTPUT); //Configura in1Pin como salida
  pinMode(in2Pin, OUTPUT); //Configura in2Pin como salida
  servoMotor.attach(servo);
}
void loop() {  
  if(var1!=k){
  //-----------------------------Motor banda--------------------------------/
  int speed = map(var1, '0', '9', 0, 255); //Re-mapea un numero desde un rango a otro
  analogWrite(enPin, speed); //’0′ a 0 ‘9’ a 255
  digitalWrite(in1Pin,HIGH);
  digitalWrite(in2Pin,LOW);
  if(var1=='9'){
    vm=1;
  }else{
    vm=0;
  }
  k=var1;
  }
  //Para estabilizar el valor del pin Trig se establece a LOW
  digitalWrite (triggerEmisor, LOW);
  delayMicroseconds(10);
  //Se lanzan los 8 pulsos del emisor
  digitalWrite (triggerEmisor, HIGH);
  delayMicroseconds(10);
  digitalWrite (triggerEmisor, LOW);
  distancia= pulseIn (echoReceptor, HIGH); 
  distancia=distancia/52; 
  //-----------------------------Servo---------------------------------------/
  servoMotor.write(180);
  if(distancia <= ks || distancia <= var2 ){//valor para que se active el servo
    // Desplazamos a la posición 0º
    vs=1;
    delay(150);
    servoMotor.write(0);
    delay(160); 
    servoMotor.write(180);
    delay(100); 
  }else{
    vs=0;
  }
  //------------------------Sensor Infra--------------------------/
  value = digitalRead(infraPin);  //lectura digital de pin
  delay(100);
  if (value == 0) {
      cont++;
      Serial2.print(DatosE); 
  }else if(value == 1){
  }
   DatosE = String('d')+String(distancia)+String('i')+String(value)+String('c')+String(cont)+String('m')+String(vm)+String('s')+String(vs)+String('\n');
  //Serial.print(DatosE);
  delay(100);
  //------------------------  Resivir datos del Nodo WiFi  --------------------------//
 while(Serial2.available())
 // if(false)
  {
    DatosR = Serial2.readStringUntil('\n');
     //Serial.println("lo que me llega: "+DatosR);  
     for(i=0;i<DatosR.length()-1;i++){
       DatosR.charAt(i);
       if(DatosR.charAt(i)=='m'){
         b1=i;
       }else if(DatosR.charAt(i)=='s'){
         b2=i;
       }
     }
     d1=DatosR.substring(b1+1,b2);
    //Serial.println("valor de d1: "+d1);
     d2=DatosR.substring(b2+1,DatosR.length());
     //Serial.println("valor de d2: "+d2+);    
  } 
    if(d1=="1"){
      var1='9';  
     }else if (d1=="0"){
      var1='0';        
     }
     if(d2=="1"){
      var2=6;  
     }else if (d2=="0"){
      var2=3;
     }  
}
