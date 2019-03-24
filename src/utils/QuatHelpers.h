#ifndef SPACESHOOTER_QUATHELPERS_H
#define SPACESHOOTER_QUATHELPERS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class QuatHelpers {
public:
  // Get upwards pointing vector
  // Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
  static glm::vec3 getUp(glm::quat orientation);

  // Get forward pointing vector
  // Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
  static glm::vec3 getForward(glm::quat orientation);

  // Get leftward pointing vector
  // Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
  static glm::vec3 getLeft(glm::quat orientation);
};

#endif // SPACESHOOTER_QUATHELPERS_H
