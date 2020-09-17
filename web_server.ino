#include <Adafruit_NeoPixel.h>
#include  <Chrono.h>
#include <FS.h> 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Wire.h>
/*para programar el nodeMCU hay que cruzar los cables tx rx*/
/*const char* ssid = "LAB";
const char* password = "4E17@27!";*/
#define PIN1      D8//pantalla 1 cable azul en el pro mini era 2
#define PIN2      D7//pantalla 2 cable blanco azul en el pro mini era 3
#define PIN3      D6//pantalla 3 cable blanco verde en el pro mini era 5
#define PIN4      D5//pantalla 4 cable naranja en el pro mini era 6
#define PIN5      D4//puntos cable verde en el pro mini era 4
#define N_LEDS 49
#define N_POINTS 4
#define n_values 4
#define RELAY     D3

//cronometros
Chrono myChrono;


//variables y configuracion de los segmentos
int minutosContador=0;
int banderaStop=0;
int minutosLimite=0;
int bandera = 0;
int lr = 40;
int lg = 0;
int lb = 0;
int segundos = 0;
int segundosCont = 0;
int decenaSegundos = 0;
int decenaSegundosCont = 0;
int minutos = 0;
int minutosCont = 0;
int decenaMinutos = 0;
int decenaMinutosCont = 0;
int n1SegmentsON[2]= {2,7};
int n1elementCount = sizeof(n1SegmentsON) / sizeof(int);
int n1Limits[2]= {7,42};
//numero dos
int n2SegmentsON[5]= {6,7,1,4,3};
int n2elementCount = sizeof(n2SegmentsON) / sizeof(int);
int n2Limits[5]= {35,42,0,21,14};
//numero tres
int n3SegmentsON[5]= {6,7,1,2,3};
int n3elementCount = sizeof(n3SegmentsON) / sizeof(int);
int n3Limits[5]= {35,42,0,7,14};
//numero cuatro
int n4SegmentsON[4]= {5,1,7,2};
int n4elementCount = sizeof(n4SegmentsON) / sizeof(int);
int n4Limits[4]= {28,0,42,7};
//numero cinco
int n5SegmentsON[5]= {6,5,1,2,3};
int n5elementCount = sizeof(n5SegmentsON) / sizeof(int);
int n5Limits[5]= {35,28,0,7,14};
//numero seis
int n6SegmentsON[6]= {6,5,1,2,3,4};
int n6elementCount = sizeof(n6SegmentsON) / sizeof(int);
int n6Limits[6]= {35,28,0,7,14,21};
//numero siete
int n7SegmentsON[3]= {6,7,2};
int n7elementCount = sizeof(n7SegmentsON) / sizeof(int);
int n7Limits[3]= {35,42,7};
//numero ocho
int n8SegmentsON[7]= {1,2,3,4,5,6,7};
int n8elementCount = sizeof(n8SegmentsON) / sizeof(int);
int n8Limits[7]= {0,7,14,21,28,35,42};
//numero nueve
int n9SegmentsON[5]= {6,7,2,1,5};
int n9elementCount = sizeof(n9SegmentsON) / sizeof(int);
int n9Limits[5]= {35,42,7,0,28};
//numero cero
int n0SegmentsON[6]= {2,3,4,5,6,7};
int n0elementCount = sizeof(n0SegmentsON) / sizeof(int);
int n0Limits[6]= {7,14,21,28,35,42};

Adafruit_NeoPixel display1 = Adafruit_NeoPixel(N_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel display2 = Adafruit_NeoPixel(N_LEDS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel display3 = Adafruit_NeoPixel(N_LEDS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel display4 = Adafruit_NeoPixel(N_LEDS, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel display5 = Adafruit_NeoPixel(N_POINTS, PIN5, NEO_GRB + NEO_KHZ800);

const char* ssid = "tablero";
const char* password = "bt12345678";

byte x = 0;
ESP8266WebServer server(80);

void setup() {
   //Serial.begin(115200);
  
    

   pinMode(RELAY,OUTPUT);
   digitalWrite(RELAY,LOW);
   //Wire.begin();
   WiFi.softAP(ssid,password);
   display1.begin();
  display2.begin();
  display3.begin();
  display4.begin();
  display5.begin();

  for (size_t i = 1; i <= 5; i++)
  {
    drawNum(2,i,lr,lg,lb);
  }
  for (size_t i = 1; i <= 5; i++)
  {
    drawNum(0,i,lr,lg,lb);
  }
  for (int i = 0; i < N_POINTS; i++)//contar los segementos que deben de encender spliteados por comma
      {
        display5.setPixelColor(i, display5.Color(lr, lg, lb));
      }
  display5.show();
  /* WiFi.mode(WIFI_STA);
   WiFi.begin(ssid,password); 
   // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());*/

server.begin(); 
server.on("/", fileindex);
server.on("/contacto", contact);
server.on("/iniciar", HTTP_POST, handleIniciar);
server.on("/color", HTTP_POST, handleColor);//cambiar el handle
server.on("/otros", HTTP_POST, handleOtros);//cambiar el handle
server.on("/index.html", fileindex);
server.on("/bootstrap.min.css", bootstrap);
server.on("bootstrap.min.css", bootstrap);
server.on("/iro.js", colorPicker);
server.on("/popper.min.js", popper);
server.on("/bootstrap.min.js", bootstrapmin);
server.on("bootstrap.min.js", bootstrapmin);
server.on("/logo.png", image);
server.on("logo.png", image);
server.on("/logo2.png", image2);
server.on("logo2.png", image2);

//Serial.print("iniciado");
//NEW
SPIFFS.begin(); 


}

void loop() {
  
   
server.handleClient();
if(bandera ==1){
       
       
       if (myChrono.hasPassed(1000)) {
            myChrono.restart(); // Restart the chronometer.
          segundos++;
          segundosCont++;
          
          if (segundos>9)
          {
            segundos = 0;
            drawNum(segundos,1,lr,lg,lb);
            //digitalWrite(RELAY,HIGH);
          }
          drawNum(segundos,1,lr,lg,lb);
        } 
        
        if (segundosCont==10) {
          segundosCont = 0;
          decenaSegundos++;
          decenaSegundosCont++;
          
          if (decenaSegundos==6)
          {
            decenaSegundos = 0;
            drawNum(decenaSegundos,2,lr,lg,lb);
          }
          drawNum(decenaSegundos,2,lr,lg,lb);
        }
        
        if (decenaSegundosCont == 6) {
          decenaSegundosCont=0;
          minutos++;
          minutosCont++;
          
          if (minutos>9)
          {
            minutos = 0;
            drawNum(minutos,3,lr,lg,lb);
          }
          drawNum(minutos,3,lr,lg,lb);
        }
        if (minutosCont == 10) {
          minutosCont=0;
          decenaMinutos++;
          
          
          if(decenaMinutos== minutosLimite/10){
                segundos = 0;
                segundosCont = 0;
                decenaSegundos = 0;
                decenaSegundosCont = 0;
                minutos = 0;
                minutosCont = 0;
                decenaMinutos = 0;
                decenaMinutosCont = 0;
                bandera = 0;
                digitalWrite(RELAY,HIGH);
                 for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(2,i,lr,lg,lb);
                    }
                 for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(0,i,lr,lg,lb);
                    }
                 for (int i = 0; i < N_POINTS; i++)//contar los segementos que deben de encender spliteados por comma
                    {
                      display5.setPixelColor(i, display5.Color(lr, lg, lb));
                    }
                 display5.show();
                 delay(5000);
                 digitalWrite(RELAY,LOW);
            }
           drawNum(decenaMinutos,4,lr,lg,lb);
          
        }
  }
  else
  {
    bandera = 0;
    myChrono.restart();
    
  }
     

}

void handleIniciar() {                         // If a POST request is made to URI /login
 
  if (server.arg("time") != NULL){
      //Serial.println("Se envio un valor para el tiempo");
      Serial.println(server.arg("time"));
      minutosLimite = server.arg("time").toInt();
      bandera = 1;
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      

    }

  if(server.arg("time") == NULL){
    
    erroR();
   
  }
}

void handleColor() {                         // If a POST request is made to URI /login
  //Serial.println("Entro al trip de los colores");
  File file = SPIFFS.open("/index.html.gz", "r"); 
  size_t sent = server.streamFile(file, "text/html");
  
   

    if(bandera == 0 ){
       if (server.arg("r") != NULL){
      //Serial.println("Se envio un valores para r");
      //Serial.println(server.arg("r"));
      lr = server.arg("r").toInt(); 
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");

      
    }
    if (server.arg("g") != NULL){
      //Serial.println("Se envio un valor para el verde");
      //Serial.println(server.arg("g"));
      lg = server.arg("g").toInt();
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");


    }
    if (server.arg("b") != NULL){
      //Serial.println("Se envio un valor para el azul");
      //Serial.println(server.arg("b"));
      lb = server.arg("b").toInt();
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      

    }
    
        for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(2,i,lr,lg,lb);
                    }
                 for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(0,i,lr,lg,lb);
                    }
                 for (int i = 0; i < N_POINTS; i++)//contar los segementos que deben de encender spliteados por comma
                    {
                      display5.setPixelColor(i, display5.Color(lr, lg, lb));
                    }
                 display5.show();
      }
    
  /*if (server.arg("time") != NULL){
      Serial.println("Se envio un valor para el tiempo");
      Serial.println(server.arg("time"));
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      Wire.beginTransmission(4); // transmit to device #4
      //Wire.write("p1 is ");        // sends five bytes
      Wire.write(server.arg("time").toInt());              // sends one byte  
      Wire.endTransmission();    // stop transmitting

    }

  if(server.arg("p1") == NULL){
    //server.send(200, "text/html", "<meta charset=\"utf-8\"><link href=\"bootstrap.min.css\" rel=\"stylesheet\"> <h2>Olvidó seleccionar el valor de alguna pantalla</h2> <a class=\"btn btn-lg btn-primary\" href=\"/\" role=\"button\">Regresar</a>");
    erroR();
    
  }
  /*File file = SPIFFS.open("/index.html.gz", "r"); 
    size_t sent = server.streamFile(file, "text/html");*/
}

void handleOtros() {                         // If a POST request is made to URI /login
      
  
  if (server.arg("pausa") != NULL){
      //Serial.println("Se envio un valor para Pausa");
      //Serial.println(server.arg("pausa"));
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      if(banderaStop == 1){
        myChrono.resume();
        banderaStop = 0;
        }
      else if(banderaStop == 0){
        myChrono.stop();
        banderaStop++;
        }
       
      /*File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      Wire.beginTransmission(4); // transmit to device #4
      //Wire.write("p1 is ");        // sends five bytes
      Wire.write(server.arg("time").toInt());              // sends one byte  
      Wire.endTransmission();    // stop transmitting*/

    }
  if (server.arg("reset") != NULL){
      //Serial.println("Se envio un valor para reset");
      //Serial.println(server.arg("reset"));
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
                segundos = 0;
                segundosCont = 0;
                decenaSegundos = 0;
                decenaSegundosCont = 0;
                minutos = 0;
                minutosCont = 0;
                decenaMinutos = 0;
                decenaMinutosCont = 0;
                bandera = 0;
                banderaStop=0;
                
                 for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(2,i,lr,lg,lb);
                    }
                 for (size_t i = 1; i <= 5; i++)
                    {
                      drawNum(0,i,lr,lg,lb);
                    }
                 for (int i = 0; i < N_POINTS; i++)//contar los segementos que deben de encender spliteados por comma
                    {
                      display5.setPixelColor(i, display5.Color(lr, lg, lb));
                    }
                 display5.show();
      
      /*File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      Wire.beginTransmission(4); // transmit to device #4
      //Wire.write("p1 is ");        // sends five bytes
      Wire.write(server.arg("time").toInt());              // sends one byte  
      Wire.endTransmission();    // stop transmitting*/

    }
    if (server.arg("leds") != NULL){
      //Serial.println("Se envio un valor para Pausa");
      //Serial.println(server.arg("pausa"));
      File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      rainbowCycle(20);
      colorWipe(display1.Color(255, 0, 0), 35);
      colorWipe(display1.Color(0, 255, 0), 35);
      colorWipe(display1.Color(0, 0, 255), 35);
       
      /*File file = SPIFFS.open("/index.html.gz", "r"); 
      size_t sent = server.streamFile(file, "text/html");
      Wire.beginTransmission(4); // transmit to device #4
      //Wire.write("p1 is ");        // sends five bytes
      Wire.write(server.arg("time").toInt());              // sends one byte  
      Wire.endTransmission();    // stop transmitting*/

    }  
}

void erroR()
{
  File file = SPIFFS.open("/error.html.gz", "r"); 
  size_t sent = server.streamFile(file, "text/html");
}
void image(){
  File file = SPIFFS.open("/logo.png", "r"); 
  size_t sent = server.streamFile(file, "image/png");
  }
void image2(){
  File file = SPIFFS.open("/logo2.png", "r"); 
  size_t sent = server.streamFile(file, "image/png");
  }


void fileindex()
{
  File file = SPIFFS.open("/index.html.gz", "r"); 
  size_t sent = server.streamFile(file, "text/html");
}
void contact()
{
  File file = SPIFFS.open("/contacto.html.gz", "r"); 
  size_t sent = server.streamFile(file, "text/html");
}

void bootstrap()
{
  File file = SPIFFS.open("/bootstrap.min.css.gz", "r"); 
  size_t sent = server.streamFile(file, "text/css");
}
void colorPicker()
{
  File file = SPIFFS.open("/iro.js.gz", "r"); 
  size_t sent = server.streamFile(file, "application/javascript");
}
void popper()
{
  File file = SPIFFS.open("/popper.min.js.gz", "r"); 
  size_t sent = server.streamFile(file, "application/javascript");
}
void bootstrapmin()
{
  File file = SPIFFS.open("/bootstrap.min.js.gz", "r"); 
  size_t sent = server.streamFile(file, "application/javascript");
}

//fincion para dibujar numeros
void drawNum(int numero , int pantalla, int r, int g , int b){

for (int i = 0; i < N_POINTS; i++)//contar los segementos que deben de encender spliteados por comma
    {
      display5.setPixelColor(i, display5.Color(r, g, b));
    }
  display5.show();

if (numero == 1)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n1elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n1Limits[i]; j < n1Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n1elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n1Limits[i]; j < n1Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n1elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n1Limits[i]; j < n1Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n1elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n1Limits[i]; j < n1Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 1


if (numero == 2)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n2elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n2Limits[i]; j < n2Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n2elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n2Limits[i]; j < n2Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n2elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n2Limits[i]; j < n2Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n2elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n2Limits[i]; j < n2Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 2   
if (numero == 3)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n3elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n3Limits[i]; j < n3Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n3elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n3Limits[i]; j < n3Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n3elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n3Limits[i]; j < n3Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n3elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n3Limits[i]; j < n3Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 3 
if (numero == 4)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n4elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n4Limits[i]; j < n4Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n4elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n4Limits[i]; j < n4Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n4elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n4Limits[i]; j < n4Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n4elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n4Limits[i]; j < n4Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 4
if (numero == 5)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n5elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n5Limits[i]; j < n5Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n5elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n5Limits[i]; j < n5Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n5elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n5Limits[i]; j < n5Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n5elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n5Limits[i]; j < n5Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 5
if (numero == 6)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n6elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n6Limits[i]; j < n6Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n6elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n6Limits[i]; j < n6Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n6elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n6Limits[i]; j < n6Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n6elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n6Limits[i]; j < n6Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 6
if (numero == 7)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n7elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n7Limits[i]; j < n7Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n7elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n7Limits[i]; j < n7Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n7elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n7Limits[i]; j < n7Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n7elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n7Limits[i]; j < n7Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 7
if (numero == 8)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n8elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n8Limits[i]; j < n8Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n8elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n8Limits[i]; j < n8Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n8elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n8Limits[i]; j < n8Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n8elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n8Limits[i]; j < n8Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 8
if (numero == 9)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n9elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n9Limits[i]; j < n9Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n9elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n9Limits[i]; j < n9Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n9elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n9Limits[i]; j < n9Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n9elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n9Limits[i]; j < n9Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 9
if (numero == 0)
{
  if (pantalla == 1)
    {
      display1.clear();

      for (int i = 0; i < n0elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n0Limits[i]; j < n0Limits[i]+7; j++)
        {
          display1.setPixelColor(j, display1.Color(r, g, b));
        }      
      }
    
      display1.show(); 
    }
  if (pantalla == 2)
    {
      display2.clear();

      for (int i = 0; i < n0elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n0Limits[i]; j < n0Limits[i]+7; j++)
        {
          display2.setPixelColor(j, display2.Color(r, g, b));
        }      
      }
    
      display2.show(); 
    }
    if (pantalla == 3)
    {
      display3.clear();

      for (int i = 0; i < n0elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n0Limits[i]; j < n0Limits[i]+7; j++)
        {
          display3.setPixelColor(j, display3.Color(r, g, b));
        }      
      }
    
      display3.show(); 
    }
    if (pantalla == 4)
    {
      display4.clear();

      for (int i = 0; i < n0elementCount; i++)//contar los segementos que deben de encender spliteados por comma
      {
        for (int j = n0Limits[i]; j < n0Limits[i]+7; j++)
        {
          display4.setPixelColor(j, display4.Color(r, g, b));
        }      
      }
    
      display4.show(); 
    }       
}//fin del numero 0
}


void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<display1.numPixels(); i++) {
      display1.setPixelColor(i, Wheel((i+j) & 255));
      display2.setPixelColor(i, Wheel2((i+j) & 255));
      display3.setPixelColor(i, Wheel3((i+j) & 255));
      display4.setPixelColor(i, Wheel4((i+j) & 255));
    }
    display1.show();
    display2.show();
    display3.show();
    display4.show();
    delay(wait);
  }
}
  void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<display1.numPixels(); i++) {
      display1.setPixelColor(i, c);
      display2.setPixelColor(i, c);
      display3.setPixelColor(i, c);
      display4.setPixelColor(i, c);
      display1.show();
      display2.show();
      display3.show();
      display4.show();
      delay(wait);
  }
}
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
 for(j=0; j<256*3; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< display1.numPixels(); i++) {
      display1.setPixelColor(i, Wheel(((i * 150 / display1.numPixels()) + j) & 255));
      display2.setPixelColor(i, Wheel2(((i * 150 / display2.numPixels()) + j) & 255));
      display3.setPixelColor(i, Wheel3(((i * 150 / display3.numPixels()) + j) & 255));
      display4.setPixelColor(i, Wheel4(((i * 150 / display4.numPixels()) + j) & 255));
    }
    display1.show();
    display2.show();
    display3.show();
    display4.show();
    delay(wait);
  }

  /*for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< display2.numPixels(); i++) {
      
    }
    display2.show();
    delay(wait);
  }
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< display3.numPixels(); i++) {
      
    }
    display3.show();
    delay(wait);
  }
  for(j=0; j<256; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< display4.numPixels(); i++) {
      
    }
    display4.show();
    delay(wait);
  }*/
}
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return display1.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return display1.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return display1.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
uint32_t Wheel2(byte WheelPos) {
  if(WheelPos < 85) {
   return display2.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return display2.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return display2.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
uint32_t Wheel3(byte WheelPos) {
  if(WheelPos < 85) {
   return display3.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return display3.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return display3.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
uint32_t Wheel4(byte WheelPos) {
  if(WheelPos < 85) {
   return display4.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return display4.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return display4.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
