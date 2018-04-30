#pragma once

/* PublishManager library by Ben Veenema
 */

#include "Particle.h"
#include "StaticBuffer.h"

template <size_t _maxCacheSize = 5, size_t _maxEventName = 63, size_t _maxData = 255>

class PublishManager
{
public:
  /**
   * Constructor - Creates 1 second Software Timer to automagically publish
   *               without calling a "process" method
   */
    PublishManager() : pubQueue(_maxCacheSize) {};

  /**
   * publish -  Publishes event immediately if timer has elapsed or adds a
   *            publish event (pubEvent) to the queue
   *            Returns true if event is published or added to queue. Returns
   *            false is the queue is full and event is discarded
   *            Also returns false if data or eventName are too long unless the
   *            buffer is empty and the event can be published immediately.
   */
   bool publish(const char* eventName, const char* data) {
     if(pubQueue.full()) return false;

     if(FLAG_canPublish && pubQueue.empty() && Particle.connected()){
       Particle.publish(eventName, data, 60, PRIVATE);
       FLAG_canPublish = false;
     } else {
       if(strlen(data) > _maxData) return false;
       if(strlen(eventName) > _maxEventName) return false;
       pubEvent newEvent;
       strcpy(newEvent.eventName,eventName);
       strcpy(newEvent.data,data);
       pubQueue.put(newEvent);
     }

     return true;
   };

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
  void process(){
      if(millis() - previousPubCallback > 1000){
        previousPubCallback = millis();
        this->publishTimerCallback();
      }
  }

private:

  struct pubEvent {
      char eventName[_maxEventName];
      char data[_maxData];
  };

  StaticBuffer<pubEvent> pubQueue;

  bool FLAG_canPublish = true;

  uint32_t previousPubCallback = 0;

  /**
   * publishTimerCallback - Removes the front element from the queue and publishes
   *                        If there is no element in the queue, sets FLAG_canPublish
   */
   void publishTimerCallback() {
       if (!pubQueue.empty() && Particle.connected()) {
         pubEvent frontEvent = pubQueue.get();
         Particle.publish(frontEvent.eventName, frontEvent.data, 60, PRIVATE);
         FLAG_canPublish = false;
       }else if(pubQueue.empty()){
         FLAG_canPublish = true;
       }
   };
};
