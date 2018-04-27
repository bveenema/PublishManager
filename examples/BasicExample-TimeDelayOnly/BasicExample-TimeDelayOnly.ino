// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager where the cloud is already connected but events
//  are published faster than the accepted publish rate. Demonstrates how to send
//  most data types

#include "PublishManager.h"

// Declare Instance of PublishManager using default sizes
//  (5 event cache, 63 character max event name, 255 byte max data)
//  Notes:  1.  You must use '<>' when creating the objet you may specify custom
//              cache size, max event name and max data see CustomCacheSize example
//          2.  Default chache uses 1590 bytes of (heap) memory
//          3.  Memory is allocated statically
PublishManager<> publishManager;

void setup() {

  // Add publish events faster than the Particle cloud can accept
  for(int i = 0; i<2; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data);
  }

  // PublishManager only accepts const char* or String eventNames and data
  String myString = "My String";
  publishManager.publish("String Test", myString);
  //    To send an int or float, convert to string
  //    Note: Advanced users can use itoa(), sprintf() and similar to convert to const char*
  int myInt = 42;
  float myFloat = 867.5309;
  publishManager.publish("Int Test", String(myInt));
  publishManager.publish("Float Test", String(myFloat,4));

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
