#include <ST7565.h>
#include "austineyes.h"
#include "opensub.h"
#include "austinspiral.h"
#include "ScreenController.h"

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
    char* author;
    char text[TEXT_SIZE];
};

struct photosubmission{
    char* author;
    uint8_t __attribute__ ((progmem)) photo [PHOTO_SIZE];
};


photosubmission photosubmissionbuffer;
textsubmission textsubmissionbuffer;


ScreenController screencontroller(10,9,8,7,6);

enum States {SHOWSUBMISSIONS};

States currentstate = SHOWSUBMISSIONS;

long currenttime;

char* recievedmessage;

void setup(){
    Serial.begin(9600);
    photosubmission austineye;
    photosubmission austinspirals;
    photosubmission openforsub;
    austineye.author = "By Austin Retzlaff";
    memcpy(austineye.photo, austineyes, 1024);

    austinspirals.author = "By Austin Retzlaff";
    memcpy(austinspirals.photo, austinspiral, 1024);

    openforsub.author = "tiny.cc/submittoforehead";
    memcpy(openforsub.photo, opensub, 1024);
   
    photosubmission photosubmissions[] = {openforsub, austineye, austinspirals, openforsub};
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BACKLIGHT_PIN, OUTPUT);
    screencontroller.init();
    analogWrite(BACKLIGHT_PIN, 100);

    while(true){
        for(int i=0; i < sizeof(photosubmissions)/sizeof(*photosubmissions); i++){
            showphotosubmission(photosubmissions[i]);
        }
}
}

void loop(){
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
        screencontroller.showimage(ps.photo);
        delay(20000);
        screencontroller.showword(ps.author);
        delay(3000);
};

bool gotchar(int checked){
    while(Serial.available() > 0){
        if(Serial.read() == checked)
            return true;
    }
    return false;
}

