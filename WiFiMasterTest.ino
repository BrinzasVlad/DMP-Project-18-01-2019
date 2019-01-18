#include <Wire.h>
#include <LiquidCrystal.h>
#define DEBUG true
#define SLAVE 9

//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

/*
 * Commands table:
 */
#define STOP 0
#define FWD 1
#define BACK 2
#define LT 3
#define RT 4

void setup() {
    Wire.begin(); //Start as Master, slave at address 9 as per above
    //lcd.begin(16, 2);
    
    Serial.begin(115200);
    Serial1.begin(115200);
    sendData("AT+RST\r\n", 2000, false); // reset module
    sendData("AT+CWMODE=2\r\n", 1000, false); // configure as access point
    sendData("AT+CIFSR\r\n", 1000, DEBUG); // get ip address
    sendData("AT+CWSAP?\r\n", 2000, DEBUG); // get SSID info (network name)
    sendData("AT+CIPMUX=1\r\n", 1000, false); // configure for multiple connections
    sendData("AT+CIPSERVER=1,80\r\n", 1000, false); // turn on server on port 80
}

void loop() {
    if (Serial1.available()) {
        if (Serial1.find("+IPD,")) {
            delay(500);
            int connectionId = Serial1.read() - 48; // read() function returns
            // ASCII decimal value and 0 (the first decimal number) starts at 48
            
            String webpage = "<h1>Command Screen!</h1>";
            webpage += "<p><a href=\"/l0\"><button style=\"height:100px;width:150px\">FWD</button></a></p>";
            webpage += "<p><a href=\"/l1\"><button style=\"height:100px;width:150px\">STOP</button></a></p>";
            webpage += "<p><a href=\"/l2\"><button style=\"height:100px;width:150px\">BACK</button></a></p>";
            webpage += "<p><a href=\"/l3\"><button style=\"height:100px;width:150px\">LT</button></a></p>";
            webpage += "<p><a href=\"/l4\"><button style=\"height:100px;width:150px\">RT</button></a></p>";
            
            String cipSend = "AT+CIPSEND=";
            cipSend += connectionId;
            cipSend += ",";
            
            cipSend += webpage.length();
            cipSend += "\r\n";
            sendData(cipSend, 100, DEBUG);
            sendData(webpage, 150, DEBUG);

            String closeCommand = "AT+CIPCLOSE=";
            closeCommand += connectionId; // append connection id
            closeCommand += "\r\n";
            sendData(closeCommand, 300, DEBUG);
        }
    }
}

String sendData(String command, const int timeout, boolean debug) {
    String response = "";
    Serial1.print(command); // send command to the esp8266
    long int time = millis();
    while ((time + timeout) > millis()) {
        while (Serial1.available()) {
            char c = Serial1.read(); // read next char
            response += c;
        }
    }

    if (response.indexOf("/l0") != -1) {
        //lcd.clear();
        //lcd.print("FWD");
        sendCommand(FWD);
    }
    if (response.indexOf("/l1") != -1) {
        //lcd.clear();
        //lcd.print("STOP");
        sendCommand(STOP);
    }
    if (response.indexOf("/l2") != -1) {
        //lcd.clear();
        //lcd.print("BACK");
        sendCommand(BACK);
    }
    if (response.indexOf("/l3") != -1) {
        //lcd.clear();
        //lcd.print("LT");
        sendCommand(LT);
    }
    if (response.indexOf("/l4") != -1) {
        //lcd.clear();
        //lcd.print("RT");
        sendCommand(RT);
    }
    if (debug) {
        Serial.print(response);
    }
    return response;
}

void sendCommand(int command) {
    Wire.beginTransmission(SLAVE);
    Serial.print("\n\n\nI am sending command ");
    Serial.print(command);
    Serial.print("\n\n\n");
    Wire.write(command);
    Wire.endTransmission();
}

