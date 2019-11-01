

#include <stdio.h>
#include <unistd.h>
#include "Transform.h"

Transform::Transform(glm::mat4 M, glm::mat4 updateM)
{
    this->M = M;
    this->updateM = updateM;
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C)
{
    //traverse the list of children and call each child node's draw function
    glm::mat4 M_new = C*M;
    if(!culled) {
        for(Node* node : children) {
            node->draw(shaderProgram, M_new);
        }
    }
}

void Transform::update(glm::mat4 C)
{
    
    M = M * updateM;
    // traverse the list of children and call each child node's update function
    for(Node* node : children) {
        node->update(C*M);
    }
    
}

void Transform::addChild(Node* n)
{
    children.push_back(n);
}

void Transform::setCulling(GLboolean b)
{
    this->culled = b;
}

void Transform::setUpdateMatrix(glm::mat4 Ma)
{
    updateM = Ma;
}

glm::mat4 Transform::getUpdateMatrix() {
    return updateM;
}


