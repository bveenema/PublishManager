/* PublishManager library by Ben Veenema
 */

#include "PublishManager.h"

/**
 * Constructor.
 */
PublishManager::PublishManager()
{
  // be sure not to call anything that requires hardware be initialized here, put those in begin()
}

/**
 * Example method.
 */
void PublishManager::begin()
{
    // initialize hardware
    Serial.println("called begin");
}

/**
 * Example method.
 */
void PublishManager::process()
{
    // do something useful
    Serial.println("called process");
    doit();
}

/**
* Example private method
*/
void PublishManager::doit()
{
    Serial.println("called doit");
}
