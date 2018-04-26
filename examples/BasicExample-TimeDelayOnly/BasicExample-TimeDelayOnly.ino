// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager where the cloud is already connected but events
//  are published faster than the accepted publish rate

#include "PublishManager.h"

PublishManager<> publishManager;

void setup() {

  // Add publish events faster than the Particle cloud can accept
  for(int i = 0; i<6; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data.c_str());
  }
}

void loop() {

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up

  static int i;
  String data = String("Test in Loop: ") + String(i);
  Serial.printlnf("Data in loop. data: %s", data.c_str());

  publishManager.publish("Test", data);

  delay(2000);
  i++;
}
