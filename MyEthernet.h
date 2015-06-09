// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = {
  0x74, 0x69, 0x69, 0x2D, 0x30, 0x32
};

static byte myip[] = {
  193,168,1,101 };

byte Ethernet::buffer[700];
BufferFiller bfill;


void Ethernetsetup() {

    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
      Serial.println(F("Failed to access Ethernet controller"));
    }
    ether.staticSetup(myip);
/*
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println(F("Failed to access Ethernet controller"));

  Serial.println(F("Setting up DHCP"));
  if (!ether.dhcpSetup())
    Serial.println(F("DHCP failed"));

  ether.printIp("My IP: ", ether.myip);
  ether.printIp("Netmask: ", ether.netmask);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);
*/
}

void Ethernetloop() {
  word plen = ether.packetReceive();
  word pos;

  if (plen) {
    if ((pos = ether.packetLoop(plen))) {
      serviceHttpRequest(bfill, pos);
    }
  }

}


