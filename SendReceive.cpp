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

void SendReceive :: recieve(int length){
    static int pos = 0; 
    char buffer[length];
    char grabbed;
    grabbed = Serial.read();
    if(pos < length && grabbed != 255){
        buffer[pos] = grabbed;
        pos++;
    }else{
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
