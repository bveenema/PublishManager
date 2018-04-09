// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager where publish events are created prior to the
//  Particle cloud being available

#include "PublishManager.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

PublishManager publishManager;

void setup() {

  // Cache some publish events before connecting to the cloud
  for(int i = 0; i<6; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data);
  }

  // Connect to the Particle cloud
  Particle.connect();
}

void loop() {

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up
  static int i;
  String data = String("Test in Loop: ") + String(i);

  publishManager.publish("Test", data);

  delay(2000);
  i++;
}
