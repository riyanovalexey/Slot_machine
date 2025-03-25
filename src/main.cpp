#include "SlotMachine.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <locale>

int main() {
    SlotMachine slotMachine;
    slotMachine.init();
    
    slotMachine.run();
    
    return 0;
}
