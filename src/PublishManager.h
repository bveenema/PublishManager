#pragma once

/* PublishManager library by Ben Veenema
 */

#include "Particle.h"
#include <queue>

class PublishManager
{
public:
  /**
   * Constructor - Creates 1 second Software Timer to automagically publish
   *               without calling a "process" method
   */
  PublishManager() : publishTimer(1000, &PublishManager::publishTimerCallback, *this, false) {
      publishTimer.start();
  };

  /**
   * publish -  Publishes event immediately if timer has elapsed or adds a
   *            publish event (pubEvent) to the queue
   */
   void publish(String eventName, String data) {
     if(FLAG_canPublish && Particle.connected()){
       Particle.publish(eventName, data, 60, PRIVATE);
       FLAG_canPublish = false;
     } else {
       pubEvent newEvent = {.eventName=eventName, .data=data};
       pubQueue.push(newEvent);
     }
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
  bool FLAG_canPublish = true;

  /**
   * publishTimerCallback - Removes the front element from the queue and publishes
   *                        If there is no element in the queue, sets FLAG_canPublish
   */
   void publishTimerCallback() {
       if (!pubQueue.empty() && Particle.connected()) {
         pubEvent frontEvent = pubQueue.front();
         pubQueue.pop();
         Particle.publish(frontEvent.eventName, frontEvent.data, 60, PRIVATE);
         FLAG_canPublish = false;
       }else{
         FLAG_canPublish = true;
       }
   };
};
