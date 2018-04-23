# PublishManager

A Particle library for managing your `Particle.publish()` events.  Makes sure you don't exceed the maximum publish rate of 1/second by caching `Particle.publish()` events and allows you to create publish events while you're offline utilizing the same cache!

PublishManager implements a First-In-First-Out (FIFO) queue so that `Particle.publish()'s` are published in the same order they are created and if you aren't generating publishes faster than 1/second, `publish events` will be sent at the same time they are created.

[This library uses dynamic memory allocation](#a-word-of-caution)

## Usage

Using PublishManager can be as simple as:

```
#include "PublishManager.h"
PublishManager publishManager;

void setup() {
  publishManager.publish("My_Event_Name","My_Data");
}

void loop() {
}
```
PublishManager creates it's own 1 second, Software Timer when it is initialized so you don't need to call a `.process()` or `.update()` method in your loop.

Storing publish events can use up a lot of memory so PublishManager limits the number of events to **10**. You can ovverride this (with caution) using the `.maxCacheSize(int newMax)` method. Absolute maximum cache size is 255.

See the [examples](examples) folder for more details.

## Documentation

```
PublishManager()
```
Constructor - Creates a 1 second Software Timer to automagically publish without calling a process() or update() method
___

```
bool publish(String eventName, String data)
```
Adds a publish event to the queue or publishes event immediately if timer has elapsed. Returns `true` if event is published or added to queue. Returns `false` is the queue is full and event is discarded
___

```
void maxCacheSize(uint8_t newMax)
```
Sets the max cache size for the Publish Queue. Default 10. Absolute maximum cache size is 255.
___
```
int16_t cacheSize()
```
Returns the number of Particle.publish() events in the queue. 0 if empty. -1 if empty and enough time has passed for instant publish.
___
```
void process()
```
** ONLY USED ON CORE - UNTESTED ** - Call `process()` every loop (*and at least 1x/sec*) when using a Core.  When using this library with a Particle Core, there is no Software Timer available.  In this case, the publish rate is controlled with the system tick (`millis()`) and is updated via `process()`.  `process()` is a valid call on non-core platforms but will execute no code.
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

PublishManager publishManager;

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

extern PublishManager publishManager

Class MyLibrary {
  public:
    MyLibrary();

    update(String message){
      publishManager.publish("MyLibrary", message);
    }

  private:
};
```

## A WORD OF CAUTION
This library makes use of [std::queue][f3af6535]. This standard library is robust in it's implementation, but utilizes [Dynamic Memory Allocation][225d2811].  Over a long period of time or many allocations and de-allocations, this library may lead to [Heap Fragmentation][b831396c] which could in turn cause erratic behavior and/or cause the micro-controller to reset.

It is unadvisable to use this library in a safety critical application without significant testing over long periods of time.

This library is intended primarily to store `Particle.publish()` events while the devices is offline and then empty the cache when the cloud is available. It is also able to store publish events for rare cases where more than 1/second publishes (and more than 4 at a time) are required.  If your application regularly needs publishing at a high rate, it is advisable to look at other protocols such as TCP and UDP

  [f3af6535]: http://en.cppreference.com/w/cpp/container/queue "cppreference.com - queue"
  [225d2811]: https://en.wikipedia.org/wiki/C_dynamic_memory_allocation "Wikipedia - Dynamic Memory Allocation"
  [b831396c]: https://stackoverflow.com/questions/3770457/what-is-memory-fragmentation "Stackoverflow - What is memory fragmentation"

## CHANGELOG

* v0.0.1 - Initial Release
* v0.0.2 - Add cacheSize()
* v0.0.3 - Additional method for using with Core's
* v0.0.4 - Add example publishWithTimeStamp

## LICENSE
Copyright 2018 Ben Veenema

Licensed under the MIT license
