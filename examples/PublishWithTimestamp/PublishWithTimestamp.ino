// Example usage for PublishManager library by Ben Veenema.

// Use case for PublishManager where a timestamp of when the publish event was
//  created is stored in the cache

#include "PublishManager.h"

PublishManager<10> publishManager;

void setup() {

  // Add 5 events at nearly the same time
  for(int i = 0; i<5; i++){
    String data = String("Test: ") + String(i);
    publishWithTimeStamp("Test", data);
  }

  // Add 5 more events spaced out by 1 second
  for(int i = 5; i<10; i++){
    String data = String("Test: ") + String(i);
    publishWithTimeStamp("Test", data);
    delay(1000);
  }
}

void loop() {

  // Continue adding events to the queue at a slow enough pace for the publish
  // rate to catch up

  static int i;
  String data = String("Test in Loop: ") + String(i);
  Serial.printlnf("Data in loop. data: %s", data.c_str());

  publishWithTimeStamp("Test", data);

  delay(2000);
  i++;
}


// Publishes "data" as a JSON char string called buffer, which contains the
//  original data and a timestamp.
//  ex: {"data": "test: 0", "time": 1524500000}
void publishWithTimeStamp(String eventName, String data){
  char buffer[255];

  sprintf(buffer, "{\"data\": \"%s\", \"time\": %u}",data.c_str(), Time.now());

  publishManager.publish(eventName, buffer);
}
