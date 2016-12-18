// Include GLFW
#include <GLFW/glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, 10 ); 
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;


// Initial cube rotation : 0
glm::quat cube_rotation;
glm::mat4 cube_translation;
glm::mat4 cube_scaling;

glm::mat4 computeMatricesFromInputs(){
	

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos = 1024 / 2, ypos = 768 / 2;
	//glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, 1024/2, 768/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(1024/2 - xpos );
	verticalAngle   += mouseSpeed * float( 768/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// -===- TRANSLATION -===-
	// Strafe up
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		cube_translation *= glm::translate(glm::vec3(0, .05, 0));

		glm::scale(glm::vec3(2, 2, 2));
		glm::translate(glm::vec3(1, 0, 0));
	}
	// Strafe down
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		cube_translation *= glm::translate(glm::vec3(0, -.05, 0));
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cube_translation *= glm::translate(glm::vec3(.05, 0, 0));
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		cube_translation *= glm::translate(glm::vec3(-.05, 0, 0));
	}

	// -===- ROTATION -===-
	// Roatate down
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		
		glm::vec3 EulerAngles(.5 * PI, 0, 0);
		cube_rotation = glm::quat(EulerAngles);
	}
	// Roatate up
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		glm::vec3 EulerAngles(-.5 * PI, 0, 0);
		cube_rotation = glm::quat(EulerAngles);
	}
	// Roatate left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		//glm::rotate()
		glm::vec3 EulerAngles(0, -.5 * PI, 0);
		cube_rotation = glm::quat(EulerAngles);
	}
	// Roatate right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 EulerAngles(0, .5 * PI, 0);
		cube_rotation = glm::quat(EulerAngles);
	}
	// Roatate falling left
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glm::vec3 EulerAngles(0, 0, .5 * PI);
		cube_rotation = glm::quat(EulerAngles);
	}
	// Roatate falling right
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		glm::vec3 EulerAngles(0, 0, -.5 * PI);
		cube_rotation = glm::quat(EulerAngles);
	}

	// -===- SCALING -===-
	// Scaling bigger
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(2, 2, 2));
	}
	// Scaling smaller
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(.5, .5, .5));
	}

	// -===- SHEARING -===-
	// Shearing bigger left-right
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(2, 1, 1));
	}
	// Shearing smaller left-right
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(.5, 1, 1));
	}
	// Shearing bigger bottom-top
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(1, 2, 1));
	}
	// Shearing smaller bottom-top
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	{
		cube_scaling += glm::scale(glm::vec3(1, .5, 1));
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;

	return cube_translation * toMat4(cube_rotation) * cube_scaling;
}