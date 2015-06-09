#include "Reactor.h"

Reactor::Reactor(int inPin, StopWatch in_sw_millis) {
  pin = inPin;

  maxFlow = 0;
  flow = 0;
  in = 0;
  out = 0;
  period = 0;
  prev = 0;
  debug = false;
  sw_millis = in_sw_millis;

}

void Reactor::init() {
  pinMode(pin, OUTPUT);
  sw_millis.start();
}

void Reactor::setFlow(long inFlow) {
  flow = inFlow;
  prev = 0; // restart
}
void Reactor::setMaxFlow(long inMaxFlow) {
  maxFlow = inMaxFlow;
  prev = 0; // restart
}
long Reactor::getFlow() {
  return flow;
}
long Reactor::getIn() {
  return in;
}
long Reactor::getOut() {
  return out;
}
long Reactor::getMaxFlow() {
  return maxFlow;
}
long Reactor::getMinFlow() {
  return minFlow;
}


void Reactor::calc() {

  minFlow = maxFlow/60;
  if (minFlow > 0) {
    minFlow++;
  }

  float proc = 0;
  if (maxFlow > 0) {
    proc = (float) 100*flow/maxFlow;
  }
  if (debug) {
    Serial.print("maxFlow=");
    Serial.println(maxFlow);

    Serial.print("flow=");
    Serial.println(flow);


    Serial.print("proc=");
    Serial.println(proc);
  }

  int n = 1;
  if (proc > 0 && proc <= 5) {
    n = 1;
  }
  else if (proc > 5 && proc <= 10) {
    n = 4;
  }
  else if (proc > 10 && proc <= 25) {
    n = 15;
  } 
  else if (proc > 25 && proc <= 50) {
    n = 30;
  } 
  else if (proc > 50 && proc <= 75) {
    n = 30;
  } 
  else if (proc > 75) {
    n = 1;
  }

  // ile czasu ma dzialac, w milisekundach
  long pompIn = 0;
  if (maxFlow > 0) {
    float pompInTmp = (float)flow/maxFlow;
    pompIn = 3600000*pompInTmp;
  }
  // ile czasu ma czekac w milisekundach
  long pompOut = 3600000 - pompIn;

  // jeden cykl
  in = pompIn/n;
  out = pompOut/n;
  period = in + out;
  

  if (debug) {
    Serial.print("n=");
    Serial.println(n);

    Serial.print("pompIn=");
    Serial.println(pompIn);
    Serial.print("pompOut=");
    Serial.println(pompOut);

    Serial.print("in=");
    Serial.println(in);
    Serial.print("out=");
    Serial.println(out);

    Serial.print("period=");
    Serial.println(period);

  }
  prev = 0;

}

void Reactor::loop() {

  unsigned long cur = sw_millis.elapsed();

  if (prev == 0) {
    prev = cur;
  }
  long diff = cur - prev;
  if (diff >= 0 && diff <= in) {
    digitalWrite(pin, LOW);
    if (debug) {
      Serial.print("wlaczone=");
      Serial.println(diff);
    }
  } 
  else if (diff > in && diff <= period) {
    digitalWrite(pin, HIGH); 
    if (debug) {
      Serial.print("wylaczone=");
      Serial.println(diff);
    }
  } 
  else {
    prev = 0;
    sw_millis.reset();    
    sw_millis.start();    
    if (debug) {
      Serial.print("RESET");      
      Serial.println(diff);
    }
  }

}






