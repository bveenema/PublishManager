# PublishManager

A Particle library for managing your `Particle.publish()` events.  Makes sure you don't exceed the maximum publish rate of 1/second by caching `Particle.publish()` events and allows you to create publish events while you're offline utilizing the same cache!

PublishManager implements a First-In-First-Out (FIFO) queue so that `Particle.publish()'s` are published in the same order they are created and if you aren't generating publishes faster than 1/second, `publish events` will be sent at the same time they are created.

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

Storing publish events can use up a lot of memory so PublishManager limits the number of events to **10**. You can ovverride this (with caution) using the `.maxCacheSize(int newMax)` method

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
Sets the max cache size for the Publish Queue. Default 10
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

## Welcome to your library!

To get started, modify the sources in [src](src). Rename the example folder inside [examples](examples) to a more meaningful name and add additional examples in separate folders.

To compile your example you can use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

Libraries can also depend on other libraries. To add a dependency use [`particle library add`](https://docs.particle.io/guide/tools-and-features/cli#adding-a-library) or [library management](https://docs.particle.io/guide/tools-and-features/dev/#managing-libraries) in Desktop IDE.

After the library is done you can upload it with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. If you wish to make your library public, use `particle library publish` or `Publish` command.

_TODO: update this README_

## Contributing

Here's how you can make changes to this library and eventually contribute those changes back.

To get started, [clone the library from GitHub to your local machine](https://help.github.com/articles/cloning-a-repository/).

Change the name of the library in `library.properties` to something different. You can add your name at then end.

Modify the sources in <src> and <examples> with the new behavior.

To compile an example, use `particle compile examples/usage` command in [Particle CLI](https://docs.particle.io/guide/tools-and-features/cli#update-your-device-remotely) or use our [Desktop IDE](https://docs.particle.io/guide/tools-and-features/dev/#compiling-code).

After your changes are done you can upload them with `particle library upload` or `Upload` command in the IDE. This will create a private (only visible by you) library that you can use in other projects. Do `particle library add PublishManager_myname` to add the library to a project on your machine or add the PublishManager_myname library to a project on the Web IDE or Desktop IDE.

At this point, you can create a [GitHub pull request](https://help.github.com/articles/about-pull-requests/) with your changes to the original library.

If you wish to make your library public, use `particle library publish` or `Publish` command.

## LICENSE
Copyright 2018 Ben Veenema

Licensed under the MIT license
