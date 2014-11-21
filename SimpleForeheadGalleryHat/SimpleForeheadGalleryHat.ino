#include <ST7565.h>
#include "austineyes.h"
#include "opensub.h"
#include "austinspiral.h"
#include "ElydahWomanDoing.h"
#include "KevinHooked.h"
#include "lukethersufly.h"
#include "ScreenController.h"
#define TEXT_SIZE 200
#define PHOTO_SIZE 1024

#define BUTTON_PIN 11

#define BACKLIGHT_PIN 3
#define BACKLIGHT_HIGH 100

#define DEBUG 1

struct textsubmission{
    char* author;
    char text[TEXT_SIZE];
};

struct photosubmission{
    char* author;
    uint8_t __attribute__ ((progmem)) photo [PHOTO_SIZE];
};

ScreenController screencontroller(10,9,8,7,6);

void setup(){
    Serial.begin(9600);
#include "photosubmissions.h"
#include "textsubmissions.h"
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    attachInterrupt(BUTTON_PIN, press, RISING);
    screencontroller.init();
    while(true){
        for(int i=0; i < sizeof(photosubmissions)/sizeof(*photosubmissions); i++){
            showphotosubmission(photosubmissions[i]);
        }
    }
}

void loop(){
}

void showtextsubmission(struct textsubmission ts){
    screencontroller.showtext(ts.text);
    fadein();
    delay(2000);
    screencontroller.showword(ts.author);
    delay(2000);
    fadeout();
};

void showphotosubmission(struct photosubmission ps){
    screencontroller.showimage(ps.photo);
    fadein();
    delay(20000);
    fadeout();
    screencontroller.showword(ps.author);
    fadein();
    delay(10000);
    fadeout();
};

void fadeout(){
    for(int i = BACKLIGHT_HIGH; i > 0; i--){
        analogWrite(BACKLIGHT_PIN, i);
        delay(10);
    }
};

void fadein(){
    for(int i = 0; i < BACKLIGHT_HIGH; i++){
        analogWrite(BACKLIGHT_PIN, i);
        delay(10);
    }
}
void press(){
    Serial.println("goit reosss");
    fadeout();
    screencontroller.showword("tiny.cc/fgsubmit");
    fadein();
    delay(4000);
}
