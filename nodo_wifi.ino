#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
//------------------- Inicializacion de las Variables --------------------------
int contconexion = 0;
const char* ssid = "PROYECTO_VI";
const char* password = "1234567pro";
const char* host = "192.168.100.10";
String strurl = "/proyecto/eje.php";
const uint16_t port = 8080;
//const int pul = D1;
String chipid,datos,d1,d2,d3,d4,d5,d6,d7,enviar;
unsigned long timeout;
int est,i,o,b1,b2,b3,b4,b5,b6,b7;
SoftwareSerial nodo(D7,D8); //rx tx

  //-----------------------   Enviar Datos a la Base de Datos SQL  ------------------------//

String enviarDatos(String dat) {
  String linea = "error";
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Fallo de conexión…");
  }

  if (client.connected()) {
    Serial.println("Sending to Server: ");                   
    client.println("POST "+strurl+" HTTP/1.1");           
    Serial.print("POST "+strurl+" HTTP/1.1");           
    client.println("Host: 192.168.100.10");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(dat.length());
    client.println();
    client.print(dat);
    client.println(); 
  }                 
  timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 50000) {
      Serial.println("Cliente fuera de tiempo !");
      client.stop();
    }
  }
  //---------------------- Imprime en la consola las lo que el servidor manda  ---------//
  while (client.available()) {
    linea = client.readStringUntil('\n');
  }
  Serial.println(linea);
  for (o=0;o<=linea.length()-1;o++){
    linea.charAt(o);
    if(linea.charAt(o)=='m'){
       b6=o;
    }else if(linea.charAt(o)=='s'){
      b7=o;
    }
  }
  d6=linea.substring(b6+1,b7);
  d7=linea.substring(b7+1,linea.length());
  //Serial.println("Motor: "+d6);
  //Serial.println("Piston: "+d7);
  
   // ---------------------  Manda los datos a arduino

  enviar = String("m")+String(d6)+String("s")+String(d7)+String('\n');
  nodo.print(enviar); //Mando el dato del servidor 

}

//-------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  nodo.begin(115200);
  Serial.begin(115200);
  pinMode(D7,INPUT);
  pinMode(D8,OUTPUT);
  ///////////////////////////////
  Serial.print("chipId: ");
  chipid = String(ESP.getChipId());
  Serial.println(chipid);

  // Conexión WIFI
  WiFi.begin(ssid, password);
  //----------------------------------------------------------------///
  while (WiFi.status() != WL_CONNECTED and contconexion < 50) {
    delay(500);
    Serial.print("...");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

// ----------------------- Resive datos del Arduino --------------------------//
while(nodo.available()){
   datos = nodo.readStringUntil('\n');
   Serial.println(datos);
   for(i=0;i<=datos.length()-1;i++){
    datos.charAt(i);
    if(datos.charAt(i)=='d'){
       b1=i;
    }else if(datos.charAt(i)=='i'){
      b2=i;
    }else if(datos.charAt(i)=='c'){
      b3=i;
    }else if (datos.charAt(i)=='m'){
      b4=i;
    }else if (datos.charAt(i)=='s'){
      b5=i;
    }
   }
   d1=datos.substring(b1+1,b2);
   //Serial.println("Sensor Ultrasonido: "+d1);
   d2=datos.substring(b2+1,b3);
   //Serial.println("Sensor Infrarojo: "+d2);
   d3=datos.substring(b3+1,b4);
   //Serial.println("Contador Elementos : "+d3);
   d4=datos.substring(b4+1,b5);
   //Serial.println("Estado del Motor : "+d4);
   d5=datos.substring(b5+1,datos.length());
   //Serial.println("Estado del Servo : "+d5);
// ---------------------------- Manda Datos a la Base de Datos --------------------------------//
  enviarDatos("chipid="+chipid+"&Ultra="+String(d1)+"&Infra="+String(d2)+"&Cont="+String(d3)+"&Motor="+String(d4)+"&Servo="+String(d5));
}
}
