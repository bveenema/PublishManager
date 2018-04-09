// Example usage for PublishManager library by Ben Veenema.

#include "PublishManager.h"

// Initialize objects from the lib
PublishManager publishManager;

void setup() {
    // Call functions on initialized library objects that require hardware
    publishManager.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    publishManager.process();
}
