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
   *            Returns true if event is published or added to queue. Returns
   *            false is the queue is full and event is discarded
   */
   bool publish(String eventName, String data) {
     if(pubQueue.size() >= _maxCacheSize) return false;

     if(FLAG_canPublish && pubQueue.empty() && Particle.connected()){
       Particle.publish(eventName, data, 60, PRIVATE);
       FLAG_canPublish = false;
     } else {
       pubEvent newEvent = {.eventName=eventName, .data=data};
       pubQueue.push(newEvent);
     }

     // start the timer if it isn't already running
     if(!publishTimer.isActive()){
       publishTimer.start();
     }

     return true;
   };

  /**
   * maxCacheSize - Sets the max cache size for pubQueue. Default 10
   */
  void maxCacheSize(uint8_t newMax) {
    _maxCacheSize = newMax;
  }

  /**
   * cacheSize -  Returns the number of stored pubEvents in the queue.
   *              Returns -1 if empty and FLAG_canPublish is true
   */
  int16_t cacheSize() {
    if(pubQueue.size() == 0 && FLAG_canPublish) return -1;
    return pubQueue.size();
  }

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

  uint8_t _maxCacheSize = 10;

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
       }else if(pubQueue.empty()){
         FLAG_canPublish = true;
         publishTimer.stop();
       }
   };
};
