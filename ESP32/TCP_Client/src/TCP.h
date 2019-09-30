#include <WiFi.h>

class TCP
{
    private:
        const uint16_t port = 8080;
        const char * host = "192.168.1.249"; // ip or dns
    public:
        void connectWifi();
        void sendData(const char* d);
};