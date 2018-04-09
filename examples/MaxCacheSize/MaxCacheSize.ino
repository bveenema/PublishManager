// Example usage for PublishManager library by Ben Veenema.

// Use case for Publish Manager where a cache size greater than the defualt (10 events)
//    is required.  Use caution in setting high cache sizes in larger programs as
//    this can quickly use up available memory.

#include "PublishManager.h"

PublishManager publishManager;

void setup() {

  // Add more publish events than the default cache size will allow
  //    publish events 11-20 (i=10 to i=19) will not be cached
  for(int i = 0; i<20; i++){
    String data = "Test: " + i;
    publishManager.publish("Test", data);
  }

  // delay for cache to clear
  delay(1000*11);

  // Update the Maximum Cache size
  publishManager.maxCacheSize(20);

  // Add same number of publish events, but this time all of them will be published
  for(int i = 0; i<20; i++){
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
