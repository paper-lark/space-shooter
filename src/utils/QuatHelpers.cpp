#include "QuatHelpers.h"

// Get upwards pointing vector
// Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
glm::vec3 QuatHelpers::getUp(glm::quat orientation) {
  return glm::normalize(glm::vec3(2 * (orientation.x * orientation.y - orientation.w * orientation.z),
                                  1 - 2 * (orientation.x * orientation.x + orientation.z * orientation.z),
                                  2 * (orientation.y * orientation.z + orientation.w * orientation.x)));
}

// Get forward pointing vector
// Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
glm::vec3 QuatHelpers::getForward(glm::quat orientation) {
  return glm::normalize(glm::vec3(2 * (orientation.x * orientation.z + orientation.w * orientation.y),
                                  2 * (orientation.y * orientation.z - orientation.w * orientation.x),
                                  1 - 2 * (orientation.x * orientation.x + orientation.y * orientation.y)));
}

// Get leftward pointing vector
// Read more: https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
glm::vec3 QuatHelpers::getLeft(glm::quat orientation) {
  return glm::normalize(glm::vec3(1 - 2 * (orientation.y * orientation.y + orientation.z * orientation.z),
                                  2 * (orientation.y * orientation.x + orientation.w * orientation.z),
                                  2 * (orientation.x * orientation.z - orientation.y * orientation.w)));
}