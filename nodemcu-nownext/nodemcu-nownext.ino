 /* PINS for nodemcu 0.9 */
#define D0 16 // LED esp?
#define D1 5 // I2C Bus SCL (clock)
#define D2 4 // I2C Bus SDA (data)
#define D3 0
#define D4 2 // Same as "LED_BUILTIN", but inverted logic
#define D5 14 // SPI Bus SCK (clock)
#define D6 12 // SPI Bus MISO 
#define D7 13 // SPI Bus MOSI
#define D8 15 // SPI Bus SS (CS)
#define D9 3 // RX0 (Serial console)
#define D10 1 // TX0 (Serial console)

#include <ESP8266WiFi.h>

#include <SPI.h>
#include <Adafruit_GFX.h>


#define useSSD1306 true
//boolean usePCD8544 = true;

// For OLED
#if useSSD1306
  #include <Wire.h>
  #include <Adafruit_SSD1306.h>
  
  #define OLED_RESET D0  // RST-PIN for OLED (not used)
  #define OLED_SDA    D1  // SDA-PIN for I2C OLED
  #define OLED_SCL    D2  // SCL-PIN for I2C OLED
#endif

Adafruit_SSD1306 display(OLED_RESET);

// for nokia 5110
  // Nokia 5110 pins definition
  // Serial clock out (SCLK)
  // Serial data out (DIN) (DN<MOSI>)
  // Data/Command select (D/C) (DC)
  // LCD chip select (CS) (SCE)
  // LCD reset (RST)

#if usePCD8544 
  #include <Adafruit_PCD8544.h>
  Adafruit_PCD8544 display = Adafruit_PCD8544(D8, D3, D2, D1, D7);
#endif

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

WiFiManager wifiManager;

// Setting up data values
boolean startRead = false;
int stringPos;
int stage;
String tempValue;
String charValue;
String dataValue;
String temperature1;
String weather1;
String temperature2;
String weather2;
String timefrom;
String outsidetemp;
String infotext;

int rainArray[] = {2, 4, 8, 3, 6, 2, 4, 8, 3, 6, 2, 4, 8, 3, 6};

int loopCounter=0;
int displayView=1;

#define clear_day_width 50
#define clear_day_height 50
const char clear_day_bits[] PROGMEM= {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x18, 
  0x00, 0x03, 0x60, 0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0xF0, 0x00, 0x00, 
  0x00, 0x7C, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x78, 
  0x00, 0x00, 0x00, 0xF0, 0xC0, 0x0F, 0x38, 0x00, 0x00, 0x00, 0x60, 0xF0, 
  0x7F, 0x18, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x7E, 0xF8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xE0, 0x03, 0x00, 
  0x00, 0x00, 0x00, 0x0F, 0xC0, 0x07, 0x00, 0x00, 0x00, 0x80, 0x07, 0x80, 
  0x07, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x07, 0x00, 0x00, 0x00, 0xC0, 
  0x03, 0x00, 0x0F, 0x00, 0x00, 0xC0, 0xC3, 0x01, 0x00, 0x0E, 0x0F, 0x00, 
  0xE0, 0xC7, 0x01, 0x00, 0x8E, 0x1F, 0x00, 0xE0, 0xC7, 0x01, 0x00, 0x8E, 
  0x1F, 0x00, 0xC0, 0xC3, 0x01, 0x00, 0x0E, 0x0F, 0x00, 0x00, 0xC0, 0x03, 
  0x00, 0x0F, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0x0F, 0x00, 0x00, 0x00, 
  0x80, 0x07, 0x80, 0x07, 0x00, 0x00, 0x00, 0x80, 0x0F, 0x80, 0x07, 0x00, 
  0x00, 0x00, 0x00, 0x1F, 0xE0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xF8, 
  0x01, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xF8, 0x7F, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xC0, 0x1F, 0x38, 0x00, 0x00, 
  0x00, 0xF8, 0x00, 0x00, 0x78, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0xF8, 
  0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x18, 0x00, 
  0x03, 0x60, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, };

void setup() {
    Serial.begin(9600);
  
  /*if(usePCD8544){  
     // Gjør klar skjermen
     // for nokia 5110
     display.begin();
     display.setContrast(50);
  }*/
  
  if(useSSD1306){  
    //for oled
      // initial I2C bus and OLED display
    Wire.begin(OLED_SDA, OLED_SCL);
    Wire.setClock(400000);
  
    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  }
  
    
    // Endre kontrast 
    display.clearDisplay();
    display.display();
  
    delay(100);
   
    // Koble til med wifi-manager
   
    displayInfo("Koble til wifi espYR og 192.168.244.1     i nettleser",1,0); //192.168.4.1 depending on library version
    wifiManager.autoConnect("espYR");
}

void loop() {


// Om loopCounter er 0, hent data fra yr. Hvis ikke, vis data som er hentet ned - skjermbilde for skjermbilde.
if(loopCounter==0){

  displayView=1; // Startskjermbilde

  
  stringPos = 0; // teller for inkomne tegn i XML
  stage = 0; // Brukes for å holde orden på hvor langt vi er kommet i XMLen
  tempValue = ""; // Midlertidig verdi
  charValue = ""; // Verdi på tegnet som nå ligger i buffer fra XML
  dataValue = ""; // Verdien hentet ut på det vi ønsker oss.
  temperature1 = ""; 
  weather1 = "";
  temperature2 = "";
  weather2 = "";
  timefrom = "";

/* --------- Weather data ------------ */

  const char* host = "www.yr.no";
  String url = "/place/Norway/Oslo/Oslo/Oppsal/varsel.xml"; // Bytt ut med din lokasjon


  displayInfo("connecting to ",1,0);
  displayInfo(host,1,0);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    displayInfo("connection failed",1,0);
    return;
  }

displayInfo("Laster..",1,0);
 
  
  Serial.print("Requesting URL: ");
  displayInfo(url,1,0);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
 
  //memset( &inString, 0, 32 ); //clear inString memory

  while(client.available()){
    //String line = client.readStringUntil('\r');
    //Serial.print(line);
 
    char c = client.read();
    
     if (c == '<' ) { // Sjekk om vi har starten på en xml node
        startRead = true; // i såfall, la oss lese inn
      }

      //Kort forklart - leser XML fra yr tegn for tegn. Om den finner tegnet < er det starten på en XML-node. Leser så inn XML 
      //noden inn i en streng. Sjekker så, for hvert tegn, om strengen innholder kombinasjonen av tegn vi ønsker oss. Om den 
      // finner kombinasjonen av tegn, begynner den å lese ut / lagre funnet data. Bruker så stage til å sjekke hvor vi er i 
      // xml og tilegner den utleste verdien til en variabel (vær, temperatur, eller tidspunkt.)

      // leser inn innhold til en streng
      if(startRead == true){
          charValue = String(c); // konverter til string
          tempValue = tempValue + charValue; // legg til string
          
          stringPos++;

      // les ut vær-kode: sjekk om XML-strengen inneholder symbol number "numberEx=" og sjekk om vi er forbi tegn 38
           if(tempValue.indexOf("numberEx=") > 0)
            {
               if (c == '"' && dataValue != "=" ) {
                displayInfo(tempValue,1,1);
                //displayInfo("Fant ver",1,1);
                startRead = false; // slutt å lese
                displayInfo(dataValue,1,1); // skriv ut linjen til skjerm
                tempValue = ""; // nullstill streng
                stringPos = 0; // Nullstill teller+
                
                stage++;

               }else{

                 if (c == '"' && dataValue == "=" ){
                  dataValue="";
                }else{
               
                dataValue = dataValue + charValue;
               }
               }
            }
       

      // les ut temperatur. sjekk om det inneholder temperatur, og sjekk om vi er forbi tegn 35
          if(tempValue.indexOf("temperature") > 0 && stringPos > 35)
            {
               if (c == '"' ) {
                //displayInfo("Fant grader",1,1);
                startRead = false; // slutt å lese
                displayInfo("dataValue: "+dataValue,1,1); // skriv ut linjen til skjerm

                tempValue = ""; // nullstill streng
                stringPos = 0; // Nullstill teller+

                stage++;

              
                 }else{
                dataValue = dataValue + charValue;
               }
            }

         if(tempValue.indexOf("time from=") > 0 && stringPos > 23 && weather1!="" && timefrom=="")
            {
               if (stringPos == 26 ) {
                //displayInfo("Fant tidsinterval",1,1);
                startRead = false; // slutt å lese
                displayInfo("dataValue: "+dataValue,1,1); // skriv ut linjen til skjerm

                tempValue = ""; // nullstill streng
                stringPos = 0; // Nullstill teller+

                stage++;

              
                 }else{
                dataValue = dataValue + charValue;
               }
            }   
        
        if (c == '>' ) { // ferdig med en xml-node
          startRead = false; // slutt å lese
          displayInfo("debug:"+tempValue,1,1); // DEBUG: skriv ut linjen til konsoll
          tempValue = ""; // nullstill streng
          stringPos = 0; // Nullstill teller
        }

        //Stages


      if(stage==1 && weather1==""){
         weather1=getWeatherText(dataValue);
         dataValue="";
        }else if(stage==2 && temperature1==""){
         temperature1=dataValue;
         dataValue="";
        }else if(stage==3 && timefrom==""){
         timefrom=dataValue;
         dataValue="";
        }else if(stage==4 && weather2==""){
         weather2=getWeatherText(dataValue);
         dataValue="";
        }else if(stage==5 && temperature2==""){
         temperature2=dataValue;
         dataValue="";
         displayInfo("All done!: "+stage,1,0);
         break;
        }
       
        
      }
      
    // end
      

  }

  displayInfo("closing connection to yr weather",1,0);



/* --------- Thingspeak ------------ */

const char* host2 = "api.thingspeak.com";
String url2 = "/channels/160764/fields/1/last"; // Thingspeak channel

if (!client.connect(host2, httpPort)) {
    displayInfo("connection failed",1,0);
    return;
  }
  displayInfo("Open connection to thingspeak",1,0);

    Serial.print("Requesting URL: ");
  displayInfo(url2,1,0);
  
  // This will send the request to the server
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host2 + "\r\n" + 
               "User-Agent: KjartanMichalsenrESP8266\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);


while(client.available()){
    int nexttemp = 0;
    String line = client.readStringUntil('\r\n');
 
    displayInfo("<"+line+">",1,1);
    outsidetemp = line;
}


/* --------- Now next ------------ */

displayInfo("NowNext",1,0);

const char* host3 = "api.met.no";
//host3 = "d7.no"; 
host3 = host;
String url3 = "/weatherapi/nowcast/0.9/?lat=60.10;lon=9.58";
url3 = "/sted/Norge/Finnmark/Vardø/Vardø/varsel_nu.xml";
url3 = "/place/Norway/Oslo/Oslo/Oppsal/varsel_nu.xml";
//url3 = "/place/Norway/Oslo/Oslo/Norsk%20Rikskringkasting/varsel_nu.xml";
//url3 = "/ver/sample.xml";

if (!client.connect(host3, httpPort)) {
    displayInfo("connection failed",1,0);
    return;
}
  displayInfo("Open connection to now next api",1,0);

    Serial.print("Requesting URL: ");
  displayInfo(url3,1,0);
  
  // This will send the request to the server
  client.print(String("GET ") + url3 + " HTTP/1.1\r\n" +
               "Host: " + host3 + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);

startRead = false;
int rainCount = 0;

while(client.available()){
    //String line = client.readStringUntil('\r\n');

    
    float f;
    int rm;
    char c = client.read();
    
      if (c == '<' ) { // Sjekk om vi har starten på en xml node
        startRead = true; // i såfall, la oss lese inn
      }
  

      // leser inn innhold til en streng
      if(startRead == true){
          charValue = String(c); // konverter til string
          tempValue = tempValue + charValue; // legg til string
          
          stringPos++;

           if(tempValue.indexOf("value=") > 0)
            {
               if (c == '"' && dataValue != "=" ) {
             
                //displayInfo("Fant nedbor:",1,0);
                startRead = false; // slutt å lese
                displayInfo(dataValue,1,1); // skriv ut linjen til skjerm
                tempValue = ""; // nullstill streng
                stringPos = 0; // Nullstill teller+

                /* ------- Start Handle Values  ------------ */

                f = dataValue.toFloat();
                f = f * 10;
                rm = (int) f; 

                rainArray[rainCount] = rm;
                
                rainCount++;
                dataValue = "";

                /* ------- End  Handle Values  ------------ */
                
               }else{

                if (c == '"' && dataValue == "=" ){
                  dataValue="";
                  }else{
               
                  dataValue = dataValue + charValue;
                }
               }
            }

        if (c == '>' ) { // ferdig med en xml-node
          startRead = false; // slutt å lese
          displayInfo(""+tempValue,1,1); // skriv ut linjen til konsoll
          tempValue = ""; // nullstill streng
          stringPos = 0; // Nullstill teller
        }

      }
}

/* --------- InfoScreen ------------ */

const char* host4 = "d7.no";
String url4 = "/analytics/smart.php"; // PHP proxy

if (!client.connect(host4, httpPort)) {
    displayInfo("connection failed",1,1);
    return;
  }
  displayInfo("Open connection",1,1);

    Serial.print("Requesting URL: ");
  displayInfo(url4,1,1);
  
  // This will send the request to the server
  client.print(String("GET ") + url4 + " HTTP/1.1\r\n" +
               "Host: " + host4 + "\r\n" + 
               "User-Agent: KjartanMichalsenrESP8266\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);


while(client.available()){
    String infoline = client.readStringUntil('\r\n');
 
    displayInfo("<"+infoline+">",10,1);
    infotext = infoline;
}


/* --------- END get values ------------ */





 } //end loopcounter

  
  


// 3 sekunder x 200 = 10 minutter. Oppdater med data fra yr hvert 10. minutt. 
  
  display.clearDisplay();

  if(displayView==1){
    display.setTextSize(2);
    if(weather1.length()>16){
        display.setTextSize(1);
      }
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(""+weather1+":"+temperature1);
    
      }

if(displayView==2){
    display.setTextColor(WHITE);
    display.setTextSize(2);
    if(weather1.length()>16){
        display.setTextSize(1);
      }
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println(""+weather2+":"+temperature2);
    display.setCursor(90,24);
    display.setTextSize(1);
    display.println("Kl "+timefrom);
      }

if(displayView==3){
 
    display.setTextColor(WHITE);
    display.setCursor(0,0);
           display.setTextSize(1);
    //display.println("Temp ute:");       
    display.println("Google Assistant:");
    display.setTextSize(3);
    display.setCursor(0,10);
    //display.println(""+outsidetemp+"");
    display.println(""+infotext+"");
    
      }

if(displayView==4){
  int MAX = 12;
  int willItRain = 0;
  for (int i = 0; i < MAX; i++ ){
    display.fillRect((i*5),(48-(rainArray[ i ]*3)), 5,  (rainArray[ i ]*3), WHITE);
    willItRain = willItRain + rainArray[ i ];
  }
   display.setCursor(0,0);
   display.setTextSize(1);
   display.println("Regn neste 90");

    if(willItRain == 0){
       display.clearDisplay();
        display.println("Opplett neste 90");
      }
  
  displayView=0;
}
      

  display.display();
    displayView++;

   loopCounter++;
   if(loopCounter==200){loopCounter=0;}
  
  //Oppdater skjerm hvert 3 sekund. 
  delay(3000);
}


