// Example usage for PublishManager library by Ben Veenema.

// How to customize/optimize the cache to allow conserve memory or increase the
//    cache size

#include "PublishManager.h"

// Declare Instance of PublishManager using custom sizes. Useful for saving
//  memory or increaseing cache size if you can limit eventName length and/or
//  data size
//
//  FORMAT: <size_t maxCacheSize, size_t maxEventName, size_t maxData>
//  Notes:  1.  Memory used is ~equal to (maxEventName + maxData) * maxCacheSize
//          2.  Default cache uses 1590 bytes of (heap) memory -> 5,63,255
//          3.  Memory is allocated statically
//
PublishManager<10,20,50> publishManager; // 10 events in cache, max eventName = 20, max data length = 50 (~700 bytes)
//PublishManager<30> publishManager; // 30 events in cache, default max eventName (63), default max data length (255) (~9540 bytes)

void setup() {
  Serial.begin(9600);
  while(!Serial.isConnected()){
    Particle.process();
  }

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
  Serial.printf("Cache Size: %i..", publishManager.cacheSize()); // Prints "4.."
  delay(1100); // delay is only necessary in example to wait for the even to publish to the cloud
  publishManager.process();
  while(publishManager.cacheSize() > -1){
    int cacheSize = publishManager.cacheSize();
    Serial.printf("%i..", cacheSize); // Prints "3.. 2.. 1.. 0.."
    delay(1100);
    publishManager.process();
  }
  Serial.printlnf("%i", publishManager.cacheSize()); // Prints "-1"

  // Add more events to the cache than maxCacheSize only events 0-9 will be accepted
  for(int i=0; i<20; i++){
    String data = String("OverLoad Test: ") + String(i);
    if(publishManager.publish("Overload Test", data) == false){
      Serial.printlnf("REJECTED %s", data.c_str());
    }
  }

  while(publishManager.cacheSize() > -1){
    publishManager.process();
  }

  // Publish Event with eventName > maxEventName and data > maxData
  // Compilation will fail if the following line is uncommented
  // publishManager.process("This is way too long of an event name", "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Ae");

  // Publish Event with data > maxData generated after compile
  pinMode(A0, INPUT);
  int myRawData = analogRead(A0);
  float myInterpretedData = 867.5309 + myRawData;
  String myDataOutput = String("Bacon ipsum dolor amet frankfurter tenderloin,") + String(myInterpretedData);
  if(publishManager.publish("Test data too long", myDataOutput) == false){
    Serial.printlnf("REJECTED %s: ",myDataOutput.c_str());
  }

}

void loop() {
  publishManager.process();
}
