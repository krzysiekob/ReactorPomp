#define STATUS_PAGE_OK 1
#define STATUS_PAGE_NOT 0


static const char okHeader[] PROGMEM = 
"HTTP/1.0 200 OK\r\n"
"Content-Type: text/html; charset=utf8\r\n"
"Pragma: no-cache\r\n"
;

static const char okHeaderJson[] PROGMEM = 
"HTTP/1.0 200 OK\r\n"
"Access-Control-Allow-Origin: *\r\n"
"Content-Type: application/json; charset=utf8\r\n"
"Pragma: no-cache\r\n"
;

static const char htmlHeader[] PROGMEM =
"<!DOCTYPE HTML>\n"
"<html>"
"<head>"
"<title>AquaReactor v0.1</title>"
"</head>"
"<body>"
;

static const char htmlFooter[] PROGMEM =
"</body></html>"
;


static const char redirect[] PROGMEM =
"HTTP/1.0 302 found\r\n"
"Location: /\r\n"
"\r\n"
;

void homePage(BufferFiller& buf) {

  buf.emit_p(PSTR("$F\r\n"
    "$F"
    ), okHeader, htmlHeader);

  buf.print(F("Reactor - ok"));  

  buf.emit_p(PSTR("$F"), htmlFooter);

}

void statusPage(BufferFiller& buf, uint8_t st) {
  buf.emit_p(PSTR("$F\r\n"), okHeaderJson);
  buf.emit_p(PSTR("{\"status\":\"$D\"}"),st);
}


/**
 * Send a 404 reply
 * @param BufferFiller buf the buffer used to write to the output
 */
void send404(BufferFiller& buf) {
  buf.emit_p(PSTR(
  "HTTP/1.0 404 Not found\r\n"
    "Content-Type: text/html\r\n"
    "\r\n"
    "<h1>404 Not found</h1>"));
}

void jsonPompPage(BufferFiller& buf) {

  int valPin = digitalRead(pin);

  buf.emit_p(PSTR("$F\r\n"), okHeaderJson);
  buf.emit_p(PSTR("[{\"flow\":\"$L\",\"maxFlow\":\"$L\",\"in\":\"$L\",\"out\":\"$L\",\"status\":\"$D\",\"minFlow\":\"$L\"}]"),
  Reactor.getFlow(), Reactor.getMaxFlow(), Reactor.getIn(), Reactor.getOut(), valPin, Reactor.getMinFlow());

}

/**
 * update Pomp
 *
 * @param BufferFiller buf the buffer used to write to the output
 * @param const char* data the incoming tcp data
 */
void updatePomp(BufferFiller& buf, const char *data) {
  // czyta dane wpisane po przecinku
  char temp[32];
  if (ether.findKeyVal(data + 5, temp, sizeof temp, "pomp") > 0) {
    long s[13];
    char delimiters[] = ",";
    char* valPosition;
    valPosition = strtok(temp, delimiters);

    int n = 0;
    while(valPosition != NULL){
      if (n <= sizeof(s)) {
        s[n] = atol(valPosition);
        valPosition = strtok(NULL, delimiters);
        Serial.println(s[n]);
      }
      n++;
    }


    // pierwsza pompka
    // 0 - pierwszy element w tablicy to przeplyw , czyli flow
    // 1 - drugi element w tablicy to maksymalny przeplyw maxFlow
    if (s[0] > 0 && s[1] > 0) {
      if (s[1] > 0) {
        Reactor.setMaxFlow(s[1]);      
      }
      if (s[0] >= Reactor.getMinFlow() && s[0] <= Reactor.getMaxFlow()) {
        Reactor.setFlow(s[0]);
        Reactor.calc();
      }      
      params[0].flow = s[0];
      params[0].maxFlow = s[1];
      EEPROM.writeBlock(address, params);
      statusPage(buf, STATUS_PAGE_OK);
    } 
    else {
      statusPage(buf, STATUS_PAGE_NOT);    
    }

  } 
  else {
    statusPage(buf, STATUS_PAGE_NOT);
  }
}


/**
 * Service any request on the webserver
 * @param BufferFiller buf the buffer filler to be used for output
 * @param word pos Position in the ethernet packet
 */
void serviceHttpRequest(BufferFiller& buf, const word pos) {
  buf = ether.tcpOffset();
  char * data = (char *) Ethernet::buffer + pos;

  // Check the input and prepare the returned content
  if (strncmp("GET / HTTP", data, 10) == 0) {
    homePage(buf);
  } 
  else if (strncmp("GET /pomp/", data, 9) == 0) { // 193.168.1.101/pomp [pobiera w formacie json ustawienia reactora]
    jsonPompPage(buf);
  } 
  else if (strncmp("GET /?pomp=", data, 10) == 0) { // np: 193.168.1.101/?pomp=1000,4000 [ustawia przeplyw pompy]m jezeli wiecej pomp niz jedna to wpisac po przecinkach, pierwsza wartosc co przeplyw, druga to max przeplyw
    updatePomp(buf, data);
  }
  else {
    send404(buf);
  }
  // send web page data
  ether.httpServerReply(buf.position());
}










