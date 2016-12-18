#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define PI 3.14159265359

glm::mat4 computeMatricesFromInputs();
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif