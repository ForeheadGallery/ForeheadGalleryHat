#include <ST7565.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "ScreenController.h"
#include "SendReceive.h"

#define BUTTON_PIN 11

#define BACKLIGHT_PIN 3

#define DEBUG 1

const int PHOTO_AMOUNT;
const int TEXT_AMOUNT;

struct textsubmission{
    char author[50];
    char text[200];
};

struct photosubmission{
    char author[50];
    char __attribute__ ((progmem)) photo [1024];
};

EEPROM_readAnything(0, PHOTO_AMOUNT);
EEPROM_readAnything(1, TEXT_AMOUNT);

stuct photosubmission photosubmissions[PHOTO_AMOUNT];
stuct textsubmission textsubmissions[TEXT_AMOUNT];

//Store this shite as a stuct in EEProm
for(int i = 0; i < PHOTO_AMOUNT; i+=2){
    EEPROM_readAnything(i+2, photosubmissions[i]);
}

for(int i = PHOTO_AMOUNT; i < TEXT_AMOUNT; i+=2){
    EEPROM_readAnything(i, textsubmissions[i]); 
}

ScreenController screencontroller(10,9,8,7,6);
SendReceive sendrecieve;

enum States {SHOWSUBMISSIONS, RECIEVENEW};

States currentstate = SHOWSUBMISSIONS;

long currenttime;

char* recievedmessage;

void setup(){
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    attachInterrupt(BUTTON_PIN, press, RISING);
    screencontroller.init();
    analogWrite(BACKLIGHT_PIN, 100);
}

void loop(){
    currenttime = millis();
    if(Serial.available()){
      currentstate = RECIEVENEW;
    }   
    switch(currentstate){
        case SHOWSUBMISSIONS:
            displayrandomwords();
            break;

        case RECIEVENEW:
            screencontroller.showword("RECEIVING");
            recieve();
            break;
}  

void recieve(){
    sendrecieve.recieve();
    if(sendrecieve.ismessageready()){
        EEPROM_writeAnything(0, messagebuffer);
    }
}

void press(){
    static long oldtime = 0;
    if(currenttime - oldtime > 1000 && currentstate != RECIEVENEW){
        currentstate = States((int)currentstate + 1);
    }else if(currenttime - oldtime > 1000 && currentstate == RECIEVENEW){
        currentstate = SHOWSUBMISSIONS;
    }
}
