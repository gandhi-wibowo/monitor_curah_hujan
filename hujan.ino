#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#define Select_PIN A5
#define Sensor 8
int Select_PINState;         // variable for reading the pushbutton status
int Sensor_State;         // variable for reading the pushbutton status
int sensorReading;
int j,Status,Menit,Jam,Kategori;
int Curah_Hujan,I,Intensitas,TotalCurahHujan; 
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = { 
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {

  pinMode(Select_PIN, INPUT);
  digitalWrite(Select_PIN, HIGH);
  pinMode(Sensor, INPUT);
  digitalWrite(Sensor, HIGH);
  

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Rain Gauge E-86");
  lcd.setCursor(0, 1);
  lcd.print("  by VERONICA");
  delay(3000);
  lcd.clear();
  
  Intensitas = 0;
  
  Select_PINState = digitalRead(Select_PIN);
  if (Select_PINState == LOW) {LCDMonitor();}

  Ethernet.begin(mac, ip);
  server.begin();
}

void Read_Sensor() {
    for (j=0;j<6000;j++){   //loop hingga 60 detik
      Sensor_State = digitalRead(Sensor);     //baca sensor jungkit
      if ((Sensor_State == LOW)&&(Status == 0)) {I++;Status = 1;}  //jika sensor berlogika 0 maka naikkan hitungan I
      if (Sensor_State == HIGH) {Status = 0;}  //reset status setelah jungkitan ke posisi awal
      
      if(j < 3000){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Intens:    mm/J ");
        lcd.setCursor(8, 0); 
        lcd.print(Intensitas);

        lcd.setCursor(0, 1);
        lcd.print("Total:      mm  ");
        lcd.setCursor(8, 1);
        lcd.print(TotalCurahHujan);
      }
     if(j > 3000){ 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("CrhHjn:     mm/J ");
        lcd.setCursor(8, 0);
        lcd.print(Curah_Hujan);
  
          if (Intensitas >= 1){lcd.setCursor(0, 1);lcd.print("  Hujan Ringan ");} // "Hujan Ringan"
          if (Intensitas >= 50)  {lcd.setCursor(0, 1);lcd.print("  Hujan Sedang ");} // "Hujan Sedang"
          if (Intensitas >= 100) {lcd.setCursor(0, 1);lcd.print("  Hujan Lebat  ");} //"Hujan Lebat"
          if (Intensitas >= 200) {lcd.setCursor(0, 1);lcd.print("  Sangat Lebat ");} // "Sangat Lebat"
      }
                  
        delay(10);  //tunda 10mS
    }
}

void Read_Sensor1() {
for (j=0;j<6000;j++){   //loop hingga 60 detik
  Sensor_State = digitalRead(Sensor);     //baca sensor jungkit
  if ((Sensor_State == LOW)&&(Status == 0)) {I++;Status = 1;}  //jika sensor berlogika 0 maka naikkan hitungan I
  if (Sensor_State == HIGH) {Status = 0;}  //reset status setelah jungkitan ke posisi awal

    delay(10);  //tunda 10mS
                    }
}

void LCDMonitor() {
while (Select_PINState == LOW){
  Select_PINState = digitalRead(Select_PIN);
                  
        Read_Sensor();                

  Intensitas = Intensitas + (I/6);   //Intensitas/menit 
            
  I = 0;
  
  if (Menit == 60){
    TotalCurahHujan = TotalCurahHujan + (Intensitas/10);
    Jam++;
    Curah_Hujan = (TotalCurahHujan/Jam);
    Menit=0;
    Intensitas = 0;}    //setelah 1 jam hitung curah hujan dan reset intensitas           
            
    Menit++;    
                              }
}


void Web() {
  
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connnection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                    // add a meta refresh tag, so the browser pulls again every 5 seconds:
            client.println("<meta http-equiv=\"refresh\" content=\"1\">");

            client.print("   ");
            client.println("<br />");       
            client.print(" Sistem Monitoring curah hujan berbasis WEB ");
            client.println("<br />");       
            client.print("   ");
            client.println("<br />");       
            client.println("<br />");   
          
            client.print("---  Intensitas    : ");
            client.print(Intensitas);
            client.print(" mm/jam");
            
            client.println("<br />");       
            client.println("<br />");   

            client.print("---  Total Curah Hujan    : ");
            client.print(TotalCurahHujan);
            client.print(" mm");
            
            client.println("<br />");       
            client.println("<br />");   
               
            client.print("---  Curah hujan : ");
            client.print(Curah_Hujan);
            client.print(" mm/jam");
            client.println("<br />"); 
            client.println("<br />");   
 
            client.print("---  Jam : ");
            client.print(Jam);
            client.print(" : ");
            client.print(Menit);
            client.println("<br />"); 
            client.println("<br />");   
            client.print("---  Kategori : ");
            
            if (Kategori == 1){client.print(" Hujan Ringan");}
            if (Kategori == 2){client.print(" Hujan Sedang");}
            if (Kategori == 3){client.print(" Hujan Lebat");}
            if (Kategori == 4){client.print(" Hujan Sangat Lebat");}
            
            client.println("<br />"); 
            client.println("<br />"); 
            client.println("</html>");
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
   // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}



void loop() {

  Web(); 

  Read_Sensor1();                
  Intensitas = Intensitas + (I/6);   //Intensitas/detik 
  
        if (Intensitas >= 1){Kategori = 1;} // "Hujan Ringan"
        if (Intensitas >= 50)  {Kategori = 2;} // "Hujan Sedang"
        if (Intensitas >= 100) {Kategori = 3;} //"Hujan Lebat"
        if (Intensitas >= 200) {Kategori = 4;} // "Sangat Lebat"

  I = 0;
  Menit++;
  
  if (Menit == 60){
    Jam++;
    TotalCurahHujan = TotalCurahHujan + (Intensitas/10);
    Curah_Hujan = (TotalCurahHujan/Jam);
    Menit=0;
    Intensitas = 0;}    //setelah 1 jam hitung curah hujan dan reset intensitas           
            
}


