// Example usage for PublishManager library by Ben Veenema.

#include "PublishManager.h"

PublishManager publishManager;

void setup() {
  for(int i = 0; i<8; i++){
    String data = "Test: " + i;
    publishManager.publish("Test", data);
  }
}

void loop() {
  static int i;
  String data = "Test in Loop: " + i;
  publishManager.publish("Test", data);
  delay(2000);
  i++;
}
