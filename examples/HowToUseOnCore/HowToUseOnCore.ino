// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager on Particle Core Platform

#include "PublishManager.h"

PublishManager publishManager;

void setup() {

  // Add publish events faster than the Particle cloud can accept
  for(int i = 0; i<6; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data);
  }
}

void loop() {

  // Call process() method to check if an event should be published - process()
  //  can be called on platforms other than Core but will not have any effect.
  publishManager.process();

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up
  static unsigned int previousCreateEvent;
  if(millis() - previousCreateEvent > 2000){
    static int i;
    String data = String("Test in Loop: ") + String(i);
    Serial.printlnf("Data in loop. data: %s", data.c_str());

    publishManager.publish("Test", data);
    i++;
  }
}
