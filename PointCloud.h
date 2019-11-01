#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <limits>

#include "Object.h"

class PointCloud : public Object
{
private:
    GLuint vao;
    GLuint vbos[2];
    GLuint ebo;
	GLfloat pointSize;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faceIndices;
    
    GLfloat xMin= std::numeric_limits<GLfloat>::max();
    GLfloat yMin= std::numeric_limits<GLfloat>::max();
    GLfloat zMin = std::numeric_limits<GLfloat>::max();
    GLfloat xMax = std::numeric_limits<GLfloat>::min();
    GLfloat yMax = std::numeric_limits<GLfloat>::min();
    GLfloat zMax = std::numeric_limits<GLfloat>::min();
    glm::vec3 modelCenter;
    GLfloat maxDistance = 0.0f;
    
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size);
    void rotate(float angle, glm::vec3 axis);
    void scale(float factor);
    void translate(glm::vec3 direction);
    float getSphereRadius();
};

#endif

