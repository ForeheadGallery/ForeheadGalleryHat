 #include "SendReceive.h"

bool SendReceive :: scan(const char* checked){
    while(Serial.available() > 0){
        if(Serial.read() == (checked[0] - '0'))
            return true;
    }
    return false;
}

char* SendReceive :: getmessage(){
    return messageholder;
}

void SendReceive :: recieve(int length, int endascii){
    static int pos = 0; ;
    char grabbed;
    char buffer[length];
    grabbed = Serial.read();
    if(pos < length && grabbed != endascii){
        buffer[pos] = grabbed;
        pos++;
    }else{
        buffer[pos] = '\0';
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
