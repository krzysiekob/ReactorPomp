#include <EEPROMex.h>
#include <StopWatch.h> //http://playground.arduino.cc/Code/StopWatchClass
#include <EtherCard.h>
#include "Reactor.h";
StopWatch sw_millis;
int pin = 7; // pin arduino pod ktory podpiety jest przekaznik
Reactor Reactor(pin, sw_millis);

// zapis struct do EEPROM
const int countPomp = 1;
struct Pomp {
  long flow;
  long maxFlow;
}
params[countPomp];
const int address = 0;

#include "MyWebserver.h"
#include "MyEthernet.h"


void setup() {
  Serial.begin(9600);
  Serial.println("START");
  
  EEPROM.readBlock(address, params); 
  long flow = params[0].flow;
  long maxFlow = params[0].maxFlow;
 
  Reactor.init();
  Reactor.setFlow(flow);
  Reactor.setMaxFlow(maxFlow);
  Reactor.calc();
  Ethernetsetup();
}


void loop() {
  Ethernetloop();
  Reactor.loop();
}


