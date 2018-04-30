# PublishManager

A Particle library for managing your `Particle.publish()` events.  Store's your `Particle.publish()` events while you're offline and makes sure you don't exceed the maximum publish rate of 1/second by caching `Particle.publish()` events.

PublishManager implements a First-In-First-Out (FIFO), circular buffer so that `Particle.publish()'s` are published in the same order they are created and if you aren't generating publishes faster than 1/second, `publish events` will be sent at the same time they are created.

[This library uses static memory allocation](#memory-allocation)

## Usage

Using PublishManager can be as simple as:

```
#include "PublishManager.h"
PublishManager<> publishManager;

void setup() {
  publishManager.publish("My_Event_Name","My_Data");
}

void loop() {
  publishManager.process();
}
```
`.publish()` creates a Publish Event (pubEvent) that is stored in the cache of PublishManager and released to the cloud when it is safe to do so by calling `.process()`. The `.process()` method can (and should) be called more often than 1/second and is safe to call when the Particle device is offline. Process will make sure your device is connected to the Particle Cloud before publishing.

Note: In this simplified example, "My_Event_Name" would actually publish immediately and not be stored in the cache at all. A subsequent, immediate (under 1 second) call to `.publish()` would be stored in the cache.

Storing publish events can use up a lot of memory so PublishManager limits the number of events to **5**. You can ovverride this (with caution) using the by setting the variables between the `<` and `>` brackets. See the `CustomCacheSize` example for more info.

See the [examples](examples) folder for more details.

## Adding a Timestamp
When caching publish events for large durations of time, it can be useful to tag the event with a time stamp so the server can determine when the event occurred instead of when it was published.

In order for this library to remain agnostic to data formatting, no method has been included to automatically add a time stamp, but a simple `sprintf()` statement can easily add a time stamp to the cache:
```
// Publishs "data" as a JSON char string called buffer, which contains the
//  original data and a timestamp.
//  ex: {"data": "test: 0", "time": 1524500000}
void publishWithTimeStamp(String eventName, String data){
  char buffer[255];

  sprintf(buffer, "{\"data\": \"%s\", \"time\": %u}",data.c_str(), Time.now());

  publishManager.publish(eventName, buffer);
}
```

## Documentation

```
PublishManager<size_t _maxCacheSize, size_t _maxEventName, size_t _maxData> ()
```
Constructor - Creates a statically allocated circular buffer of **_maxCacheSize** elements, with **_maxEventName** maximum Publish Event Name and **_maxData** maximum data length. Defaults to 5, 63, 255 respectively.
Ex.
* `PublishManager<> publishManager() // default (5,63,255) ~1590 bytes`
* `PublishManager<15> publishManager() // Additional elements in cache (15,63,255) ~4770 bytes`
* `PublishManager<15,20,50> publishManager() // Custom Cache (15,20,50) ~1050 bytes`
___

```
bool publish(String eventName, String data)
```
Adds a publish event to the queue or publishes event immediately if timer has elapsed. Returns `true` if event is published or added to queue. Returns `false` is the queue is full, or the eventName or data is greater than the maximum* and event is discarded.

*If the cache is empty, you can publish an event greater than the size declared in the constructor as it bypasses the cache and is published immediately
___

```
int16_t cacheSize()
```
Returns the number of Particle.publish() events in the queue. 0 if empty. -1 if empty and enough time has passed for instant publish.
___

```
void process()
```
Call `.process()` every loop and at least 1x/sec. `.process()` can be safely called when the device is offline.
___

## Using PublishManager in multiple files
If your application requires PublishManager to be called from multiple files, like your own `.cpp` or `.c` files, create the `publishManager` object in your main `.ino` file and then use the `extern` keyword when "creating" the publishManager object in additional files.

This will prevent duplicate PublishManager objects from being created, and consequentally creating `Particle.publish()` events too often.

**Example**
___
_main.ino_
```
#include "PublishManager.h"
#include "MyLibrary.h"

PublishManager<> publishManager;

void setup() {
  // Do setup stuff
  publishManager.publish("My_Event_Name","My_Data");
}

void loop() {
  // Do loop stuff
}
```
_MyLibrary.h_
```
#pragma once
//MyLibrary.h - by Me

#include "Particle.h"
#include "PublishManager.h"

extern PublishManager<> publishManager

Class MyLibrary {
  public:
    MyLibrary();

    update(String message){
      publishManager.publish("MyLibrary", message);
    }

  private:
};
```

## Memory Allocation
This library uses a statically allocated circular buffer, based heavily on code from [Embedded Artistry](https://embeddedartistry.com/blog/2017/4/6/circular-buffers-in-cc) This means an entire block of heap memory is allocated at the beginning of the program execution and remains for the entire duration (unless PublishManager is `deleted` or goes out of scope).  This means that PublishManager can use a lot of memory; ~1590 bytes in the default case, with only a 5 event cache.  

PublishManager can be easily modified to use less memory or store more publish events by changing the default template. by instantiating PublishManager with `PublishManager<10,20,50>` PublishManager can store 10 events in less than half the space (~700 bytes)! But the programmer must take care to limit eventNames to less than 20 characters and data to less than 50.

## CHANGELOG

* v0.0.1 - Initial Release
* v0.0.2 - Add cacheSize()
* v0.0.3 - Additional method for using with Core's
* v0.0.4 - Add example publishWithTimeStamp
* v0.0.5 - Change to statically allocated memory. Remove Software Timer publish in favor of `.process()` method

## LICENSE
Copyright 2018 Ben Veenema

Licensed under the MIT license
