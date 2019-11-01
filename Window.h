#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>

#include "Transform.h"
#include "Geometry.h"
#include "shader.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc;
    static GLboolean mousePressed;
    static glm::vec3 currentPosition, lastPosition;
    static GLboolean displaySpheres;
    static GLboolean culling;
    static GLboolean demo;
    
    
    // Robot Army
    static Transform* robotArmy;
    static std::vector<Transform*> LB;
    static std::vector<Transform*> LT;
    static std::vector<Transform*> RB;
    static std::vector<Transform*> RT;
    static std::vector<glm::vec3> centers;
    static std::vector<Transform*> allRobots;
    
    // Body parts of the robot
    static Transform* robot;
    static Transform* body;
    static Transform* head;
    
    static Transform* leftArm;
    static Transform* rightArm;
    
    static Transform* leftLeg;
    static Transform* rightLeg;
    
    static Transform* leftEye;
    static Transform* rightEye;

    // Geometries
    static Geometry* bodyGeo;
    static Geometry* armGeo;
    static Geometry* headGeo;
    static Geometry* eyeballGeo;
    
    static double fov; // field of view
    static double originalFov; // used for demo mode
    
    // variables that store the six planes of view frustrum
    static glm::vec3 nearNormal;
    static glm::vec3 farNormal;
    static glm::vec3 leftNormal;
    static glm::vec3 rightNormal;
    static glm::vec3 topNormal;
    static glm::vec3 bottomNormal;
    
    static glm::vec3 nearPoint;
    static glm::vec3 farPoint;
    static glm::vec3 leftPoint;
    static glm::vec3 rightPoint;
    static glm::vec3 topPoint;
    static glm::vec3 bottomPoint;
    
    // radius of bounding sphere
    static float r;
    
    static glm::mat4 leftArmUpdate;
    static glm::mat4 rightArmUpdate;
    static glm::mat4 leftLegUpdate;
    static glm::mat4 rightLegUpdate;
    
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    
    static glm::vec3 trackBallMapping(glm::vec2 point);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    
    static void calculateFrustrum();
    static GLboolean Culled(glm::vec3 c, float radius);
};

#endif
