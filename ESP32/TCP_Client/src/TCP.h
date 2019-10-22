#include <WiFi.h>

class TCP
{
    private:
        const uint16_t port = 8080;
        const char * host = "192.168.1.249"; // ip or dns        
    public:
        long interval = 100;
        String messageRX;
        bool newMessage = false;
        
        void connectWifi();
        void sendData(const char* d);
};