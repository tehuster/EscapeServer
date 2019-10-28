#include <WiFi.h>

class TCP
{
    private:
        const uint16_t port = 8080;
        const char * host = "192.168.1.208"; // ip or dns        
    public:
        long interval = 1000;
        String messageRX;
        bool newMessage = false;
        
        void connectWifi();
        void sendData(const char* d);
};