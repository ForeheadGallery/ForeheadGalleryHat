 #include "SendReceive.h"
#include "stretch.h"

unsigned char* SendReceive :: getmessage(){
    return messageholder;
}

void SendReceive :: recieve(int length, bool endascii){
    static int pos = 0;
    int checknum;
    char grabbed = Serial.read();
    unsigned char buffer[length];
    if(endascii){
        checknum = 255;
    }else{
        checknum = 500;
    }
    if(pos < length && grabbed != checknum){
        Serial.print("buf ");
        Serial.println(grabbed,DEC);
        Serial.print("stretch  ");
        Serial.println(stretch[pos]);
        buffer[pos] = grabbed;
        pos++;
        delay(15);
    }else{
        if(endascii){
            buffer[pos] = '\0';
        }
        pos = 0;
        messageholder = buffer;
        messageready = true;
    }
}

bool SendReceive :: ismessageready(){
    if(messageready){
        messageready = false;
        return true;
    }
    return false;
};
