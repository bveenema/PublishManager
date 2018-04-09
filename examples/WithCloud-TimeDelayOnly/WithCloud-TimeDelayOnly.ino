// Example usage for PublishManager library by Ben Veenema.

#include "PublishManager.h"

PublishManager publishManager;

void setup() {

  // Add publish events faster than the Particle
  for(int i = 0; i<4; i++){
    String data = "Test: " + i;
    publishManager.publish("Test", data);
  }
}

void loop() {

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up

  static int i;
  String data = "Test in Loop: " + i;

  publishManager.publish("Test", data);
  
  delay(2000);
  i++;
}
