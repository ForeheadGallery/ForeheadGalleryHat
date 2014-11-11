#include <ST7565.h>
#include <EEPROM.h>
#ifndef EEPROMANYTHING_H
#include "EEPROMAnything.h"
#endif
#include "ScreenController.h"
#include "SendReceive.h"

#define BUTTON_PIN 11

#define BACKLIGHT_PIN 3

#define DEBUG 1

const int PHOTO_SIZE = 1028;
const int TEXT_SIZE = 200;

const int PHOTO_AMOUNT = 20;
const int TEXT_AMOUNT = 20;

char* textbuffer;
char* photobuffer;

struct textsubmission{
    char* author;
    char text[TEXT_SIZE];
};

struct photosubmission{
    char* author;
    uint8_t __attribute__ ((progmem)) photo [PHOTO_SIZE];
};

photosubmission photosubmissions[PHOTO_AMOUNT];
textsubmission textsubmissions[TEXT_AMOUNT];

photosubmission photosubmissionbuffer;
textsubmission textsubmissionbuffer;


ScreenController screencontroller(10,9,8,7,6);
SendReceive sendrecieve;

enum States {SHOWSUBMISSIONS, RECIEVENEW};

enum RecieveStages {AUTHOR, TEXT, PHOTO};
RecieveStages recievestage = AUTHOR;
States currentstate = SHOWSUBMISSIONS;

long currenttime;

char* recievedmessage;

void setup(){
    Serial.begin(9600);
    //Store this shite as a stuct in EEProm
    for(int i = 0; i < PHOTO_AMOUNT; i+=2){
        EEPROM_readAnything(i+2, photosubmissions[i]);
    }

    for(int i = PHOTO_AMOUNT; i < TEXT_AMOUNT; i+=2){
        EEPROM_readAnything(i, textsubmissions[i]); 
    }
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

            break;
        case RECIEVENEW:
            screencontroller.showword("RECEIVING");
            recieve();
            break;
    }  
}

void recieve(){
    static bool finished = false;
    static bool ready = false;
    switch(recievestage){
        case AUTHOR:
            if(gotchar("#")){
                ready = true;
            }
            if (ready == true){
                sendrecieve.recieve(50);
                if(sendrecieve.ismessageready()){ 
                    textbuffer = sendrecieve.getmessage();
                    ready = false;
                    Serial.println("sent now checking for photo or text");
                    if(gotchar(">")){
                        recievestage = PHOTO;
                    }else if(gotchar("@")){
                        recievestage = TEXT;
                    }
                }
            }
        break;

        case PHOTO:
            sendrecieve.recieve(1028);
            if(sendrecieve.ismessageready()){
                photosubmissionbuffer.author = textbuffer;
                photobuffer = sendrecieve.getmessage();
                for(int i = 0; i < PHOTO_SIZE; i++){
                    photosubmissionbuffer.photo[i] = photobuffer[i];
                }
                showphotosubmission(photosubmissionbuffer);
            }
        break;
        case TEXT:
            sendrecieve.recieve(200);
            if(sendrecieve.ismessageready()){
                textsubmissionbuffer.author = textbuffer;
                for(int i = 0; i < TEXT_SIZE; i++){
                    textsubmissionbuffer.text[i] = sendrecieve.getmessage()[i];
                }
                showtextsubmission(textsubmissionbuffer);
            } 
                
        break;
    }
}

void showtextsubmission(struct textsubmission ts){
    screencontroller.showword(ts.author);
    delay(2000);
    screencontroller.showword(ts.text);
    delay(10000);
};

void showphotosubmission(struct photosubmission ps){
    screencontroller.showword(ps.author);
    delay(2000);
    screencontroller.showimage(ps.photo);
    delay(10000);
};

bool gotchar(const char* checked){
    while(Serial.available() > 0){
        if(Serial.read() == (checked[0] - '0'))
            return true;
    }
    return false;
}

void press(){
    static long oldtime = 0;
    if(currenttime - oldtime > 1000 && currentstate != RECIEVENEW){
        currentstate = States((int)currentstate + 1);
    }else if(currenttime - oldtime > 1000 && currentstate == RECIEVENEW){
        currentstate = SHOWSUBMISSIONS;
    }
}
