#include "Window.h"

int Window::width;
int Window::height;

const char* Window::windowTitle = "GLFW Project 3";

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 10, 20); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.

GLboolean Window::mousePressed;
glm::vec3 Window::currentPosition;
glm::vec3 Window::lastPosition;
GLboolean Window::displaySpheres;
GLboolean Window::culling = false;
GLboolean Window::demo;
double Window::originalFov = 60.0;

Transform* Window::robotArmy;
std::vector<Transform*> Window::LB;
std::vector<Transform*> Window::LT;
std::vector<Transform*> Window::RB;
std::vector<Transform*> Window::RT;
std::vector<glm::vec3> Window::centers;
std::vector<Transform*> Window::allRobots;

Transform* Window::robot;
Transform* Window::body;
Transform* Window::leftArm;
Transform* Window::rightArm;

Transform* Window::head;

Transform* Window::leftLeg;
Transform* Window::rightLeg;

Transform* Window::leftEye;
Transform* Window::rightEye;

Geometry* Window::bodyGeo;
Geometry* Window::armGeo;

Geometry* Window::headGeo;
Geometry* Window::eyeballGeo;

double Window::fov;

glm::vec3 Window::nearNormal;
glm::vec3 Window::farNormal;
glm::vec3 Window::leftNormal;
glm::vec3 Window::rightNormal;
glm::vec3 Window::topNormal;
glm::vec3 Window::bottomNormal;

glm::vec3 Window::nearPoint;
glm::vec3 Window::farPoint;
glm::vec3 Window::leftPoint;
glm::vec3 Window::rightPoint;
glm::vec3 Window::topPoint;
glm::vec3 Window::bottomPoint;

float Window::r;

glm::mat4 Window::leftArmUpdate = inverse(glm::translate(glm::vec3(0,-0.5,0))) * glm::rotate(0.2f, glm::vec3(1,0,0)) * glm::translate(glm::vec3(0,-0.5,0));
glm::mat4 Window::rightArmUpdate = inverse(glm::translate(glm::vec3(0,-0.5,0))) * glm::rotate(-0.2f, glm::vec3(1,0,0)) * glm::translate(glm::vec3(0,-0.5,0));
glm::mat4 Window::leftLegUpdate = inverse(glm::translate(glm::vec3(0,-0.25,0))) * glm::rotate(0.2f, glm::vec3(-1,0,0)) * glm::translate(glm::vec3(0,-0.25,0));
glm::mat4 Window::rightLegUpdate = inverse(glm::translate(glm::vec3(0,-0.25,0))) * glm::rotate(-0.2f, glm::vec3(-1,0,0)) * glm::translate(glm::vec3(0,-0.25,0));


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
    
	return true;
}

bool Window::initializeObjects()
{
    
    armGeo = new Geometry("limb_s.obj",glm::vec3(0,1,0));
    bodyGeo = new Geometry("body_s.obj", glm::vec3(0,1,0));
    headGeo = new Geometry("head_s.obj", glm::vec3(0,1,0));
    eyeballGeo = new Geometry("eyeball_s.obj", glm::vec3(0,0,0));
    
    
    glm::mat4 leftArmT = glm::mat4(1);
    glm::mat4 rightArmT = glm::mat4(1);
    glm::mat4 headT = glm::mat4(1);
    glm::mat4 leftLegT = glm::mat4(1);
    glm::mat4 rightLegT = glm::mat4(1);
    glm::mat4 leftEyeT = glm::mat4(1);
    glm::mat4 rightEyeT = glm::mat4(1);
    
    
    // Create transform matrices for arms
    leftArmT = glm::translate( glm::vec3(-1.3,0,0)) * leftArmT;
    rightArmT = glm::translate(glm::vec3(1.3,0,0)) * rightArmT;
    
    leftArmT = glm::scale(glm::vec3(1,2,1)) * leftArmT;
    rightArmT = glm::scale(glm::vec3(1,2,1)) * rightArmT;
    
    // set transform matrices for legs
    leftLegT = glm::translate( glm::vec3(-0.5,-1.5,0)) * leftLegT;
    rightLegT = glm::translate(glm::vec3(0.5,-1.5,0)) * rightLegT;
    
    // set transform matrice for head
    headT = glm::translate( glm::vec3(0,1.2,0)) * headT;
    
    // set transform matrices for eyes
    leftEyeT = glm::translate( glm::vec3(-0.5,0.5,0.7)) * leftEyeT;
    rightEyeT = glm::translate(glm::vec3(0.5,0.5,0.7)) * rightEyeT;
    
    leftEyeT =  leftEyeT* glm::scale( glm::vec3(1.3,1.3,1.3)) ;
    rightEyeT = rightEyeT* glm::scale( glm::vec3(1.3,1.3,1.3)) ;
    
    // create all the transform nodes
    robot = new Transform(glm::mat4(1), glm::mat4(1));
    body = new Transform(glm::mat4(1), glm::mat4(1));
    leftArm = new Transform(leftArmT,leftArmUpdate);
    rightArm = new Transform(rightArmT,rightArmUpdate);
    head = new Transform(headT, glm::mat4(1));
    leftLeg = new Transform(leftLegT,leftLegUpdate);
    rightLeg = new Transform(rightLegT,rightLegUpdate);
    leftEye = new Transform(leftEyeT, glm::mat4(1));
    rightEye = new Transform(rightEyeT,glm::mat4(1));
    
    // Add children to each transform node
    robot->addChild(body);
    robot->addChild(leftArm);
    robot->addChild(rightArm);
    robot->addChild(head);
    head->addChild(leftEye);
    head->addChild(rightEye);
    robot->addChild(leftLeg);
    robot->addChild(rightLeg);
    
    body->addChild(bodyGeo);
    leftArm->addChild(armGeo);
    rightArm->addChild(armGeo);
    head->addChild(headGeo);
    leftEye->addChild(eyeballGeo);
    rightEye->addChild(eyeballGeo);
    leftLeg->addChild(armGeo);
    rightLeg->addChild(armGeo);
    
    /* Initialize robot army */
    robotArmy = new Transform(glm::mat4(1), glm::mat4(1));
    glm::vec3 center;
    for(int i = 1; i<= 5; i++) {
        for(int j= 1; j <= 5; j++) {
            glm::mat4 t1 = glm::translate(glm::vec3(-i*5+2.5,0,j*5-2.5));
            Transform* lb = new Transform(t1,glm::mat4(1));
            LB.push_back(lb);
            allRobots.push_back(lb);
            lb->addChild(robot);
            robotArmy->addChild(lb);
            center = t1 * glm::vec4(0,0,0,1);
            centers.push_back(center);
        
            glm::mat4 t2 = glm::translate(glm::vec3(-i*5+2.5,0,-j*5+2.5));
            Transform* lt = new Transform(t2, glm::mat4(1));
            LT.push_back(lt);
            allRobots.push_back(lt);
            lt->addChild(robot);
            robotArmy->addChild(lt);
            center = t2 * glm::vec4(0,0,0,1);
            centers.push_back(center);
        
            glm::mat4 t3 = glm::translate(glm::vec3(i*5-2.5,0,j*5-2.5));
            Transform* rb = new Transform(t3,glm::mat4(1));
            RB.push_back(rb);
            allRobots.push_back(rb);
            rb->addChild(robot);
            robotArmy->addChild(rb);
            center = t3 * glm::vec4(0,0,0,1);
            centers.push_back(center);
        
            glm::mat4 t4 = glm::translate(glm::vec3(i*5-2.5,0,-j*5+2.5));
            Transform* rt = new Transform(t4, glm::mat4(1));
            RT.push_back(rt);
            allRobots.push_back(rt);
            rt->addChild(robot);
            robotArmy->addChild(rt);
            center = t4 * glm::vec4(0,0,0,1);
            centers.push_back(center);
        }
        
    }
    
    // calculate radius of bounding sphere
    PointCloud* sphere = new PointCloud("sphere.obj",1);
    r = sphere->getSphereRadius()*0.2f;
    
	return true;
}

void Window::cleanUp()
{

	// Delete the shader program.
    delete robot;
    delete body;
    delete leftArm;
    delete rightArm;
    delete bodyGeo;
    delete armGeo;
    
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
    fov = 60.0f;
	Window::projection = glm::perspective(glm::radians(fov),
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
    
	// Perform any updates to make the robot walk
    robotArmy->update(glm::mat4(1));
    //sleep(1);
    leftArm->setUpdateMatrix(inverse(leftArm->getUpdateMatrix()));
    rightArm->setUpdateMatrix(inverse(rightArm->getUpdateMatrix()));
    leftLeg->setUpdateMatrix(inverse(leftLeg->getUpdateMatrix()));
    rightLeg->setUpdateMatrix(inverse(rightLeg->getUpdateMatrix()));
    

}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Specify the values of the uniform variables we are going to use.
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  
    
	// Render the object.
    if(culling) {
        int counter = 0;
        calculateFrustrum();
        for(int i=0; i< centers.size(); i++ ) {
            GLboolean culled = Culled(centers[i], r);
            allRobots[i]->setCulling(culled);
            if(!culled) {
                counter++;
            }
        }
        robotArmy->draw(program, glm::mat4(1));
        std::cout << counter << "\n";
    } else {
        for(int i=0; i< centers.size(); i++ ) {
            allRobots[i]->setCulling(false);
        }
        robotArmy->draw(program, glm::mat4(1));
    }
    
    // display bounding spheres if key is on
    if(displaySpheres) {
        for(int i=0; i< centers.size(); i++) {
            PointCloud* sphere = new PointCloud("sphere.obj",1);
            glm::mat4 model = glm::translate(centers[i]) * sphere->getModel();
            model = model * glm::scale(glm::vec3(0.2,0.2,0.2));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            sphere->draw();
        }
        
    }

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
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

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    
    fov -= yoffset;
    if(fov < 0) {
        fov = 1;
    } else if (fov > 180) {
        fov = 179;
    }
    projection = glm::perspective(glm::radians(fov),double(width) / (double)height, 1.0, 1000.0);
    
    // update fov value if not in demo mode
    if(!demo) {
        originalFov = fov;
    }
    
    calculateFrustrum();
}

void Window::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    //if the left button is pressed, update mouse position
    
    if( mousePressed ) {
        currentPosition = trackBallMapping(glm::vec2(xpos, ypos));
        
        //calculte rotation angle and axis
        glm::vec3 axis = glm::cross(lastPosition,currentPosition);
        float dotProduct = glm::dot(currentPosition, lastPosition);
        dotProduct = (dotProduct < -1.0f) ? -1.0f : dotProduct;
        dotProduct = (dotProduct > 1.0f) ? 1.0f : dotProduct;
        GLfloat angle = glm::acos(dotProduct);
        axis = glm::normalize(axis);
        
        // apply rotation to the camera's direction vector
        glm::vec4 new_up = glm::vec4(up,1) * glm::rotate(angle,axis);
        up = glm::vec3(new_up.x, new_up.y, new_up.z);
        up = glm::normalize(up);
        
        glm::vec3 direction = eye - center;
        glm::vec4 newDirection = glm::vec4(direction,1) * glm::rotate(angle,axis);
        direction = glm::vec3(newDirection.x, newDirection.y, newDirection.z);
        
        // change the point of view
        center = eye - direction;
        
        // update the view matrix
        view = glm::lookAt(Window::eye, Window::center, Window::up);
        
        // send the matrix to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
       
        

        lastPosition = currentPosition;
    } else {
        lastPosition = trackBallMapping(glm::vec2(xpos, ypos));
    }
    
    calculateFrustrum();
    
}

void Window::mouseCallback(GLFWwindow* window, int button, int action, int mods) {
    // check if the left button is pressed
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        
        mousePressed = true;
        
    }
    
    // check if the left button is released
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mousePressed = false;
        
    }
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
            case GLFW_KEY_B:
                displaySpheres = !displaySpheres;
                break;
            case GLFW_KEY_C:
                culling = !culling;
                break;
            case GLFW_KEY_D:
                demo = !demo;
                break;
        }
    }
    
}

void Window::calculateFrustrum()
{
    // calculate the information needed to obtain the planes
    glm::vec3 direction = glm::normalize(eye-center);
    float ratio = (float) width / (float) height;
    float nearDist = 1.0f;
    float farDist = 1000.0f;
    float Hnear = 2 * glm::tan(glm::radians(originalFov)/2.0) * nearDist;
    float Wnear = Hnear * ratio;
    float Hfar = 2 * glm::tan(glm::radians(originalFov)/2.0) * farDist;
    float Wfar = Hfar * ratio;
    
    glm::vec3 Z = direction;
    glm::vec3 X = glm::cross(up,Z);
    X = glm::normalize(X);
    glm::vec3 Y = glm::cross(Z,X);
    
    
    // calculate points on each plane
    nearPoint = eye - Z * nearDist;
    farPoint = eye - Z * farDist;
    leftPoint = nearPoint - X * Wnear /2.0f;
    rightPoint = nearPoint +X * Wnear /2.0f;
    topPoint = nearPoint + Y * Hnear / 2.0f;
    bottomPoint = nearPoint - Y * Hnear / 2.0f;
    
    // calculate normals of each plane
    glm::vec3 a = (nearPoint +X * Wnear /2.0f) - eye;
    a = glm::normalize(a);
    rightNormal = -glm::cross(a, Y);
    
    a = (nearPoint - X * Wnear /2.0f) - eye;
    a = glm::normalize(a);
    leftNormal = -glm::cross(Y, a);
    
    a = (nearPoint + Y * Hnear / 2.0f) - eye;
    a = glm::normalize(a);
    topNormal = -glm::cross(X, a);
    
    a = (nearPoint - Y * Hnear / 2.0f) - eye;
    a = glm::normalize(a);
    bottomNormal = -glm::cross(a, X);
    
    nearNormal = -Z;
    
    farNormal = Z;
    
}


GLboolean Window::Culled(glm::vec3 c, float radius)
{
    GLboolean result = false;
    
    // calculate distances from center of sphere to six planes of frustrum
    float distanceNear = glm::dot((c - nearPoint), nearNormal);
    float distanceFar = glm::dot((c - farPoint), farNormal);
    float distanceTop = glm::dot((c - topPoint), topNormal);
    float distanceBottom = glm::dot((c - bottomPoint), bottomNormal);
    float distanceLeft = glm::dot((c - leftPoint), leftNormal);
    float distanceRight = glm::dot((c - rightPoint), rightNormal);
    
    if(distanceNear < -radius || distanceFar < -radius || distanceTop < -radius || distanceBottom < -radius || distanceLeft < -radius || distanceRight < -radius) {
        result = true;
    } else {
        result = false;
    }
    
    return result;
    
}


