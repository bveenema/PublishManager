#pragma once

/* PublishManager library by Ben Veenema
 */

#include "Particle.h"
#include <queue>

#if PLATFORM_ID == 0
  #define IS_CORE TRUE
#elif PLATFORM_ID == 2
  #define IS_CORE TRUE
#else
  #define IS_CORE FALSE
#endif

template <size_t _maxEventName = 63, size_t _maxData = 255, uint8_t _maxCacheSize = 5>

class PublishManager
{
public:
  /**
   * Constructor - Creates 1 second Software Timer to automagically publish
   *               without calling a "process" method
   */
  #if IS_CORE == FALSE
    PublishManager(const uint8_t maxCache = 5) : publishTimer(1000, &PublishManager::publishTimerCallback, *this, false) {
      publishTimer.start();
    };
  #else
    PublishManager(){}
  #endif

  /**
   * publish -  Publishes event immediately if timer has elapsed or adds a
   *            publish event (pubEvent) to the queue
   *            Returns true if event is published or added to queue. Returns
   *            false is the queue is full and event is discarded
   */
   bool publish(const char* eventName, const char* data) {
     if(pubQueue.size() >= _maxCacheSize) return false;

     if(FLAG_canPublish && pubQueue.empty() && Particle.connected()){
       Particle.publish(eventName, data, 60, PRIVATE);
       FLAG_canPublish = false;
     } else {
       pubEvent newEvent;
       strcpy(newEvent.eventName,eventName);
       strcpy(newEvent.data,data);
       pubQueue.push(newEvent);
     }

     // start the timer if it isn't already running
     #if IS_CORE == FALSE
       if(!publishTimer.isActive()){
         publishTimer.start();
       }
     #endif

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
  void process(){
    #if IS_CORE == TRUE
      if(millis() - previousPubCallback > 1000){
        previousPubCallback = millis();
        this->publishTimerCallback();
      }
    #endif
  }

private:

  #if IS_CORE == FALSE
    Timer publishTimer;
  #endif
  bool FLAG_canPublish = true;

  #if IS_CORE == TRUE
    uint32_t previousPubCallback = 0;
  #endif

  uint8_t _maxCacheSize = 10;

  struct pubEvent {
      char eventName[_maxEventName];
      char data[_maxData];
  };

  pubEvent pubQueue[_maxCacheSize];
  int8_t headIndex = 0;
  int8_t tailIndex = 0;

  //https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc




  std::queue<pubEvent> pubQueue;

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
         #if IS_CORE == FALSE
           publishTimer.stop();
         #endif
       }
   };
};
