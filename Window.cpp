#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Starter Project";

// Objects to display.
PointCloud * Window::dragon;
PointCloud * Window::bunny;
PointCloud * Window::bear;

// The object currently displaying.
PointCloud * Window::currentObj; 

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

// Mouse positions
GLboolean Window::leftButtonDown;
glm::vec3 Window::currentPosition;
glm::vec3 Window::lastPosition;
int Window::colormode = 0;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shader.vert", "shader.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
    
    glUniform3fv(glGetUniformLocation(program, "viewPos"),1, glm::value_ptr(eye));
    
    // set the uniform variable lightSource
    glm::vec3 lightColor = glm::vec3(0.6,0.8,0.4);
    glm::vec3 lightPosition = glm::vec3(5,7,1);
    glUniform3fv(glGetUniformLocation(program, "lightSource.color"),1, glm::value_ptr(lightColor));
    glUniform3fv(glGetUniformLocation(program, "lightSource.position"),1, glm::value_ptr(lightPosition));
    glUniform1i(glGetUniformLocation(program, "colormode"), colormode);
    
	return true;
}

bool Window::initializeObjects()
{

    // Create the three models
    dragon = new PointCloud("dragon.obj",1);
    bunny = new PointCloud("bunny.obj",1);
    bear = new PointCloud("bear.obj",1);

    
	// Set cube to be the first to display
	currentObj = bunny;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
    delete bunny;
    delete dragon;
    delete bear;

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	currentObj->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.
	glm::mat4 model = currentObj->getModel();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Assign each of the model different material properties
    GLuint materialDiffuse = glGetUniformLocation(program, "material.diffuse");
    GLuint materialSpecular = glGetUniformLocation(program, "material.specular");
    GLuint materialAmbient = glGetUniformLocation(program, "material.ambient");
    GLuint materialShininess = glGetUniformLocation(program, "material.shininess");
    
    glm::vec3 diffuse, specular, ambient;
    float shininess;
    
    if( currentObj == bunny) {
        //very shiny, with no diffuse reflection
        diffuse = glm::vec3(0,0,0);
        specular = glm::vec3(0.7,0.7,0.16);
        ambient = glm::vec3(0.21,0.12,0.05);
        shininess = 1;
    }
    
    if(currentObj == dragon) {
        //only use diffuse reflection, with zero shininess
        diffuse = glm::vec3(0.5,0.7,0.6);
        specular = glm::vec3(0,0,0);
        ambient = glm::vec3(0,0,0);
        shininess = 0;
    }
    
    if(currentObj == bear) {
        //have significant diffuse and specular reflection components
        diffuse = glm::vec3(0.54,0.36,0.78);
        specular = glm::vec3(0.8,0.75,0.84);
        ambient = glm::vec3(0.15,0.22,0.13);
        shininess = 0.25;
    }
    
    glUniform3fv(materialDiffuse, 1, glm::value_ptr(diffuse));
    glUniform3fv(materialSpecular, 1, glm::value_ptr(specular));
    glUniform3fv(materialAmbient, 1, glm::value_ptr(ambient));
    glUniform1f(materialShininess, shininess);

    
    
	// Render the object.
	currentObj->draw();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
        case GLFW_KEY_F1:
            // Set currentObj to bunny
            currentObj = bunny;
            break;
        case GLFW_KEY_F2:
            // Set currentObj to dragon
            currentObj = dragon;
            break;
        case GLFW_KEY_F3:
            // Set currentObj to bear
            currentObj = bear;
            break;
        case GLFW_KEY_P:
            if(mods == GLFW_MOD_SHIFT) {
                ((PointCloud *)currentObj)->updatePointSize(2.0);
            } else {
                ((PointCloud *)currentObj)->updatePointSize(-2.0);
            }
            break;
        case GLFW_KEY_N:
            if(colormode == 0) {
                colormode = 1;
            } else {
                colormode = 0;
            }
            glUniform1i(glGetUniformLocation(program, "colormode"), colormode);
            break;
		default:
			break;
		}
        
	}
    
    
}

void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    //if the left button is pressed, update mouse position and modify
    // the model view matrix
    
    if( leftButtonDown) {
        currentPosition = trackBallMapping(glm::vec2(xpos, ypos));
        
        //calculte rotation angle and axis
        glm::vec3 axis = glm::cross(lastPosition,currentPosition);
        GLfloat angle = glm::acos(glm::dot(currentPosition, lastPosition));
        
        // multiply the current transformation matrix with rotation matrix
        currentObj->rotate(angle, axis);
        
        lastPosition = currentPosition;
    } else {
        lastPosition = trackBallMapping(glm::vec2(xpos, ypos));
    }
}


void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    // check if the left button is pressed
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
        leftButtonDown = true;
        
    }
    
    // check if the left button is released
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        leftButtonDown = false;
        
    }
    
    
}

glm::vec3 Window::trackBallMapping(glm::vec2 point) {
    glm::vec3 resultPosition; // the 3D position to be returned
    float depth; //depth of the mouse location
    
    resultPosition.x = (2.0f* point.x - width)/width;
    resultPosition.y = (height - 2.0f*point.y)/height;
    resultPosition.z = 0.0f;
    
    depth = glm::length(resultPosition);
    depth = (depth < 1.0f) ? depth : 1.0f;
    resultPosition.z = sqrtf(1.001f - depth * depth);
    
    resultPosition = glm::normalize(resultPosition);
    return resultPosition;
    
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Calculate the scale factor
    float factor = 1.0f + yoffset / height;
    
    currentObj->scale(factor);
    
}


