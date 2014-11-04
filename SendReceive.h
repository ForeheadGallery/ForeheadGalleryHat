#include <Arduino.h>

class SendReceive{
    public:
        void recieve();
        char* getmessage();
        void send();
        bool messageready = false;
        bool ismessageready();
        bool scan();
        bool isready = false;
        char messageholder[200];
    private:
};
