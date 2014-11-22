#include <Arduino.h>

class SendReceive{
    public:
        void recieve(int length, bool endascii = true);
        uint8_t* getmessage();
        void send();
        bool messageready = false;
        bool ismessageready();
        bool scan(const char* checked);
        bool isready = false;
        uint8_t* messageholder;
    private:
};
