#include <stdio.h>
#include <stdlib.h>

class PicoClient
{
private:
    int baudrate = 0;
    int address = 0;

public:
    PicoClient(int frequency, int address);
    void sendData(int command);
    void receiveRequest();
};