#include <Arduino.h>

class SendReceive{
    public:
        void recieve(int length, int endascii = 255);
        char* getmessage();
        void send();
        bool messageready = false;
        bool ismessageready();
        bool scan(const char* checked);
        bool isready = false;
        char* messageholder;
    private:
};
