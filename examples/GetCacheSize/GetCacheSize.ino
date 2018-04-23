// Example usage for PublishManager library by Ben Veenema.

// How to use .cacheSize() to return the current size of the Particle.publsish()
//      cache.

#include "PublishManager.h"

PublishManager publishManager;

void setup() {
  Serial.begin(9600);
  while(!Serial.isConnected()){
    Particle.process();
  }

  // publishManager initializes the cache empty and instant publish as ready
  int cacheSize = publishManager.cacheSize();
  Serial.printlnf("Cache Size: %i", cacheSize); // Prints "-1"

  // Add some publish events to the cache
  for(int i = 0; i<5; i++){
    String data = String("Test: ") + String(i);
    publishManager.publish("Test", data);
  }

  // publishManager slowly publishes the cache
  Serial.printf("Cache Size: %i..", publishManager.cacheSize()); // Prints "4.."
  delay(1000); // delay is only necessary in example to wait for the even to publish to the cloud
  while(publishManager.cacheSize() > -1){
    int cacheSize = publishManager.cacheSize();
    Serial.printf("%i..", cacheSize); // Prints "3.. 2.. 1.. 0.."
    delay(1000);
  }
  Serial.printf("%i", publishManager.cacheSize()); // Prints "-1"

}

void loop() {

}
