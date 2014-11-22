#include <ST7565.h>
#include "ScreenController.h"

ScreenController :: ScreenController(int8_t SID, int8_t SCLK, int8_t AO, int8_t RST, int8_t CS) : disp(SID, SCLK, AO, RST, CS) {};

void ScreenController :: init(){
    disp.begin(0x20);
    disp.setpixel(10, 10, BLACK);
    delay(2000);
}

void ScreenController :: showimage(const uint8_t* image){
    disp.clear();
    disp.drawbitmap(0, 0, image, 128, 64, 1);
    disp.display();

}

void ScreenController :: looplines(char *(*messages)[12]){
    static long oldtime = 0;
    static int messagelinenumber = 12;
    static int startline = 0;
    long currenttime = millis();
    if(currenttime - oldtime > SCROLLINTERVAL){
        disp.clear();
        for(int i = 0; i < 5; i++){
            if(i + startline > messagelinenumber){
                startline = 0;
            }
            int offset = getcenteroffset(strlen(messages[0][startline+i]));
            disp.drawstring(offset, i*2, messages[0][startline+i]);
        }
        startline++;
        oldtime = millis();
        disp.display();
    }
    
}

void ScreenController :: loopimages(const uint8_t* images[], int size){
    static long oldtime = 0;
    static int curindex = 0;
    long currenttime = millis();
    int maxindex = size-1;
    if(currenttime - oldtime > SCROLLINTERVAL){
      if(++curindex > maxindex)
            curindex = 0;
      showimage(images[curindex]);
      oldtime = millis();
    }
}

void ScreenController :: showtext(char* text){
    disp.clear();
    disp.drawstring(1, 1, text);
    disp.display();
}
void ScreenController :: showword(char* word){
    int offset = getcenteroffset(strlen(word));
    disp.clear();
    disp.drawstring(offset, 4, word);
    disp.display();
}

int ScreenController :: getcenteroffset(int chars){
    int sparespace = (21-chars)*6;
    return sparespace/2;
}

