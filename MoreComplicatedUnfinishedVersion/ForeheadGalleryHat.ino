#include <ST7565.h>
#include "stretch.h"
#include <EEPROM.h>
#ifndef EEPROMANYTHING_H
#include "EEPROMAnything.h"
#endif
#include "ScreenController.h"
#include "SendReceive.h"

#define BUTTON_PIN 11

#define BACKLIGHT_PIN 3

#define DEBUG 1

#define AUTHOR_CHAR 35
#define PHOTO_CHAR 60
#define TEXT_CHAR 62

const int PHOTO_SIZE = 1024;
const int TEXT_SIZE = 200;
const int AUTHOR_SIZE = 50;

const int PHOTO_AMOUNT = 20;
const int TEXT_AMOUNT = 20;

char textbuffer[TEXT_SIZE];
char authorbuffer[AUTHOR_SIZE];
char photobuffer[PHOTO_SIZE];

struct textsubmission{
    char author[AUTHOR_SIZE];
    char text[TEXT_SIZE];
};

struct photosubmission{
    char author[AUTHOR_SIZE];
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
    screencontroller.showword("done");
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
            if (ready == true){
                sendrecieve.recieve(50);
                if(sendrecieve.ismessageready()){ 
                    for(int i = 0; i < AUTHOR_SIZE; i++){
                        authorbuffer[i] = sendrecieve.getmessage()[i];
                    }
                    Serial.print("got ");
                    Serial.println(authorbuffer);
                    ready = false;
                    while(recievestage == AUTHOR){
                        char sr_read = Serial.read();
                        if(sr_read == TEXT_CHAR){
                            recievestage = TEXT;
                        }else if(sr_read == PHOTO_CHAR){
                            recievestage = PHOTO;
                        }
                    }
                }
            }else if(gotchar(AUTHOR_CHAR)){
                ready = true;
            }
            break;

        case PHOTO:
            sendrecieve.recieve(PHOTO_SIZE, false);
            if(sendrecieve.ismessageready()){
                memcpy(photosubmissionbuffer.author, authorbuffer, AUTHOR_SIZE);
                memcpy(photosubmissionbuffer.photo, sendrecieve.getmessage(), 1024);
                /* for(int i = 0; i < PHOTO_SIZE; i++){ */
                /*     photosubmissionbuffer.photo[i] = sendrecieve.getmessage()[i]; */
                /*     Serial.print("stretch: "); */
                /*     Serial.println(stretch[i]); */
                /*     Serial.print("got: "); */
                /*     Serial.println(sendrecieve.getmessage()[i]); */
                /*     delay(10); */
                /* } */
                showphotosubmission(photosubmissionbuffer);
            }
            break;

        case TEXT:
            sendrecieve.recieve(TEXT_SIZE);
            if(sendrecieve.ismessageready()){
                memcpy(textsubmissionbuffer.author, authorbuffer, AUTHOR_SIZE);
                memcpy(textsubmissionbuffer.text, sendrecieve.getmessage(), TEXT_SIZE);
                showtextsubmission(textsubmissionbuffer);
            }

            break;
    }
}

void showtextsubmission(struct textsubmission ts){
    while(true){
        screencontroller.showword(ts.author);
        delay(2000);
        screencontroller.showtext(ts.text);
        delay(2000);
    }
};

void showphotosubmission(struct photosubmission ps){
    while(true){
        screencontroller.showword(ps.author);
        delay(2000);
        screencontroller.showimage(ps.photo);
        delay(10000);
        Serial.println( "next");
        screencontroller.showimage(stretch);
        delay(10000);
    }
};

bool gotchar(int checked){
    while(Serial.available() > 0){
        if(Serial.read() == checked)
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
