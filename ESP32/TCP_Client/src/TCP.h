#include <ETH.h>

class TCP
{
    private:
        const uint16_t port = 8080;
        const char * host = "192.168.1.208"; // ip or dns        
    public:
        long interval = 100;
        String messageRX;
        bool newMessage = false;
        static bool eth_connected;
        static void Event(WiFiEvent_t event);
        void TestClient(const char * host, uint16_t port);
        
        void sendData(const char* d);
};