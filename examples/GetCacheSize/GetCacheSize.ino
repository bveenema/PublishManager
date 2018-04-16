// Example usage for PublishManager library by Ben Veenema.

// How to use .cacheSize() to return the current size of the Particle.publsish()
//      cache.

#include "PublishManager.h"

PublishManager publishManager;

void setup() {
  Serial.begin(9600);
  delay(5000);

  // publishManager initializes the cache empty and instant publish as ready
  int cacheSize = publishManager.cacheSize();
  Serial.printlnf("Cache Size: %i", cacheSize); // Prints "-1"

  // Add some publish events to the cache
  for(int i = 0; i<5; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data);
  }

  // publishManager slowly publishes the cache
  Serial.printf("Cache Size: %i..", cacheSize); // Prints "4.."
  while(publishManager.cacheSize() > 0){
    int cacheSize = publishManager.cacheSize();
    Serial.printf("%i..", cacheSize); // Prints "3.. 2.. 1.. 0.. -1"
    delay(1000);
  }

}

void loop() {

}
