#pragma once

/* PublishManager library by Ben Veenema
 */

// This will load the definition for common Particle variable types
#include "Particle.h"

// typedef struct pubEvent {
//     String eventName;
//     String data;
//     struct pubEvent *next;
// };

// This is your main class that users will import into their application
class PublishManager
{
public:
  /**
   * Constructor - Creates 1 second Software Timer to automagically publish
   *               without calling a "process" method
   */
  PublishManager() : publishTimer(1000, &PublishQueue::PublishTimerCallback, *this, false) {
      publishTimer.start();
  };

  /**
   * publish -  Adds a publish event (pubEvent) to the queue
   */
   void publish(String eventName, String data) {
       pubEvent newEvent = {.eventName=eventName, .data=data};
       pubQueue.push(newEvent);
   };

  /**
   * process -  RESERVED - may be used in future if library is to be used
   *            without software timer
   */
  void process();

private:
  struct pubEvent {
      String eventName;
      String data;
  };
  std::queue<pubEvent> pubQueue;
  Timer publishTimer;
  bool FLAG_canPublish = false;
  /**
   * publishTimerCallback - Removes the front element from the queue and publishes
   *                       
   */
   void publishTimerCallback() {
       if (!pubQueue.empty()) {
           pubEvent frontEvent = pubQueue.front();
           pubQueue.pop();
           Particle.publish(frontEvent.eventName, frontEvent.data, 60, PRIVATE);
       }
   };
};
