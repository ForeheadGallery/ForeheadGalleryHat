 #include "SendReceive.h"

bool SendReceive :: scan(){
    while(Serial.available() > 0){
        if(Serial.read() == 35)
            return true;
    }
    return false;
}

char* SendReceive :: getmessage(){
    return messageholder;
}

void SendReceive :: recieve(){
    static bool isready = false;
    static int pos = 0; 
    char grabbed;
    if(isready){
        grabbed = Serial.read();
        if(pos < 200 && grabbed != 255){
            messageholder[pos] = grabbed;
            pos++;
        }else{
            messageholder[pos] = '\0';
            pos = 0;
            Serial.println("Message Recieved Successfully");
            messageready = true;
            isready = false;
        }
    }else if(scan()){
        isready = true;
    }
}

bool SendReceive :: ismessageready(){
    if(messageready){
        messageready = false;
        return true;
    }
    return false;
};
