// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager where publish events are created prior to the
//  Particle cloud being available

#include "PublishManager.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

// Declare Instance of PublishManager using default sizes
//  (5 event cache, 63 character max event name, 255 byte max data)
//  Notes:  1.  You must use '<>' when creating the objet you may specify custom
//              cache size, max event name and max data see CustomCacheSize example
//          2.  Default chache uses 1590 bytes of (heap) memory
//          3.  Memory is allocated statically
PublishManager<> publishManager;

void setup() {

  // Cache some publish events before connecting to the cloud
  for(int i = 0; i<5; i++){
    String data = String("Test No Cloud: ") + String(i);
    publishManager.publish("Test before Cloud Connect", data);
  }

  // Connect to the Particle cloud
  Particle.connect();
}

void loop() {

  // Call .process() every loop and at least once per second
  publishManager.process();

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up
  static uint32_t prevPubEventAdd;
  if(millis() - prevPubEventAdd > 2000){
    prevPubEventAdd = millis();
    static int i;
    String data = String("Test in Loop: ") + String(i);

    publishManager.publish("Test", data);
    i++;
  }
}
