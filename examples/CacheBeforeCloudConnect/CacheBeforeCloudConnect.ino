// Example usage for PublishManager library by Ben Veenema.

#include "PublishManager.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

PublishManager publishManager;

void setup() {

  // Cache some publish events before connecting to the cloud
  for(int i = 0; i<8; i++){
    String data = "Test: " + i;
    publishManager.publish("Test", data);
  }

  // Connect to the Particle cloud
  Particle.connect();
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
