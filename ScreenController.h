#include <Arduino.h>

#define SCROLLINTERVAL 5000

class ScreenController{
	
public:

	ScreenController(int8_t SID, int8_t SCLK, int8_t A0, int8_t RST, int8_t CS);
    void init();
    void startScroll();
    void showimage(const uint8_t* image);
    void loopimages(const uint8_t* images[], int size);
    void looplines(char *(*messages)[12]);
    void showword(char* word);
    ST7565 disp;

private:

    int getcenteroffset(int chars);
    
};
    
