#ifndef RoSump_H
#define RoSump_H

#include "Arduino.h"
#include <StopWatch.h> //http://playground.arduino.cc/Code/StopWatchClass


/*
Sterowanie pompa wlacz/wylacz w czasie
*/
class Reactor {
public:
  Reactor(int inPin, StopWatch in_sw_millis);
  void init();
  void loop();
  void setFlow(long inFlow); // ustawia przeplyw pompy jaki chcemy miec
  void setMaxFlow(long inMaxFlow); // ustawia max przeplyw pompy jaki chcemy miec
  long getFlow();
  long getIn();
  long getOut();
  long getMaxFlow();
  long getMinFlow();
  void calc(); // przelicza dane
private:
  StopWatch sw_millis;
  unsigned long maxFlow; // wydajnosc pompy ml/h
  unsigned long minFlow; // minimalny przeplyw, wartosc przeliczana na podstawie wartosci maksymalnej
  int pin;      // arduino pin ktorym jest sterowanie
  unsigned long flow; // jaki chcemy miec przeplyw w ml/h
  unsigned long in; // ile czasu pompa ma dzialac
  unsigned long out; // ile czasu pompa ma czekac
  long period; // czas calej petli, suma czasu dzialania i czasu czekania
  long prev; // poprzedni czas startu petli
  boolean debug;

};

#endif


