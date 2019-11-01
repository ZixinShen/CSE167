
#ifndef Transform_h
#define Transform_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <list>

#include "Node.h"

class Transform : public Node
{
private:
    glm::mat4 M;
    std::list<Node*> children;
    
    glm::mat4 updateM;
    int forward = 0;
    GLboolean culled = false;
public:
    Transform(glm::mat4 M, glm::mat4 updateM);
    
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    void addChild(Node* n);
    void setCulling(GLboolean b);
    
    void setUpdateMatrix(glm::mat4 Ma);
    glm::mat4 getUpdateMatrix();
    
    
};



#endif
