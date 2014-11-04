#include <ST7565.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include "ScreenController.h"
#include "SendReceive.h"

#define BUTTON_PIN 11

#define BACKLIGHT_PIN 3

#define DEBUG 1

struct TextSubmission{
    char author[50];
    char text[200];
} submission;

struct PhotoSubmission{
    char author[50];
    char __attribute__ ((progmem)) photo [1024];
} photosubmission;

const uint8_t* images[] = {
    stretch,
    wouda,
    johntab,
    glasses,
    thankyou,
    opensub
};

const uint8_t* opensubimages[]={
    wouda,
    opensub
};

ScreenController screencontroller(10,9,8,7,6);
SendReceive sendrecieve;

enum States {SHOWSUBMISSIONS, RECIEVENEW};

States currentstate = SHOWSUBMISSIONS;

long currenttime;

char* recievedmessage;
char messagebuffer[200];

void setup(){
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    attachInterrupt(BUTTON_PIN, press, RISING);
    EEPROM_readAnything(0, messagebuffer);
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
        for(int i = 0; i<200; i++){
            messagebuffer[i] = sendrecieve.messageholder[i];
        }
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
