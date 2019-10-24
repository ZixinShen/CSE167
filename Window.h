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

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"

class Window
{
public:
	static int width;
	static int height;
	static const char* windowTitle;
    static PointCloud * dragon;
    static PointCloud * bunny;
    static PointCloud * bear;
	static PointCloud * currentObj;
    static PointCloud * light;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc;
    static GLboolean leftButtonDown;
    static glm::vec3 lastPosition;
    static glm::vec3 currentPosition;
    static int colormode;
    static glm::vec3 lightColor;
    static glm::vec3 lightPosition;
    static int mouseMode;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static glm::vec3 trackBallMapping(glm::vec2 point);
};

#endif
