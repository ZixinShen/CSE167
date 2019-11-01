
#ifndef Geometry_h
#define Geometry_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>

#include "Node.h"
#include "PointCloud.h"

class Geometry : public Node
{
protected:
    glm::mat4 model; // the modelview matrix
    glm::vec3 color;
    
    //vectors that store the 3D model
    std::vector<glm::vec3> inputPoints;
    std::vector<glm::vec3> inputNormals;
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> textureIndices;
    std::vector<unsigned int> normalIndices;
    std::vector<unsigned int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    
    GLuint vao;
    GLuint vbos[2];
    GLuint ebo;
    
    GLuint modelLoc;
public:
    Geometry(std::string filename, glm::vec3 color);
    ~Geometry();
    
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    
};



#endif

