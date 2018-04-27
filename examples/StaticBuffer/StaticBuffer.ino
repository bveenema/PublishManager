// Example/Test usage for StaticBuffer Class.

// Test sketch for StaticBuffer Class which is used by PublishManager

#include "StaticBuffer.h"
const size_t _maxEventName = 63;
const size_t _maxData = 255;
const size_t _maxCacheSize = 20;
struct pubEvent {
    char eventName[_maxEventName];
    char data[_maxData];
};
StaticBuffer<pubEvent> pubQueue(_maxCacheSize);

void setup() {
  Serial.begin(9600);
  while(!Serial.isConnected()){
    Particle.process();
  }

  delay(5000);
  uint32_t freemem = System.freeMemory();
  Serial.print("free memory: ");
  Serial.println(freemem);

  for(int i = 1; i < 6; i++){
    struct pubEvent x;
    char buffer[100];
    sprintf(buffer, "test data: %u",i);
    strcpy(x.eventName,"TEST");
    strcpy(x.data,buffer);
    pubQueue.put(x);
  }

  for(int i=0; i < 5; i++){
    struct pubEvent returnEvent = pubQueue.get();
    Serial.printlnf("Return Name: %s, Return Data: %s", returnEvent.eventName, returnEvent.data);
  }

}

void loop() {
}
