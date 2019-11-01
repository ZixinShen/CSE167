
#include "Geometry.h"

Geometry::Geometry(std::string filename, glm::vec3 color)
{
    /*
     * load a 3D model
     */
    std::ifstream objFile(filename,std::ios::in); // The obj file we are reading.
    
    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.
        
        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;
            
            // Load vertices
            if (label == "v")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
                // Process the point. For example, you can save it to a.
                inputPoints.push_back(point);
            }
            
            //load normals
            if (label == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                
                inputNormals.push_back(normal);
            }
            
            //load face indices
            if (label == "f") {
                
                // store indices into the vertex list
                // adjust 1-based indices to 0-based indices
                
                for(int i=0; i < 3; i++) {
                  unsigned int vertexIndex;
                  ss >> vertexIndex;
                  ss.ignore(256, '/');
                  vertexIndices.push_back(vertexIndex-1);
                
                  unsigned int textureIndex;
                  ss >> textureIndex;
                  ss.ignore(256, '/');
                  textureIndices.push_back(textureIndex-1);
                
                  unsigned int normalIndex;
                  ss >> normalIndex;
                  ss.ignore(256, ' ');
                  normalIndices.push_back(normalIndex-1);
                
               }
            }
        }
        
        // Re-order v//vn data so that their indices align
        for(unsigned i = 0; i < vertexIndices.size(); i++) {
            vertices.push_back(inputPoints[vertexIndices[i]]);
            normals.push_back(inputNormals[normalIndices[i]]);
            indices.push_back(i);
        }
        
    }
    else
    {
        std::cerr << "Can't open the file " << filename << std::endl;
    }
    
    objFile.close();
    
    // Set the color of the geometry
    this->color = color;
    
    
    // Generate a vertex array (VAO) and 2 vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glGenBuffers(1, &ebo);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
                 vertices.data(), GL_DYNAMIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    // Bind to the second VBO. We will use it to store the normals.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
                 normals.data(), GL_DYNAMIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    //Bind to the ebo
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(),
                 indices.data(), GL_DYNAMIC_DRAW);
    
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

Geometry::~Geometry()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
    
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C)
{
    
    // Set modelview matrix to the current C matrix
    model = C;
    
    /* render the object */
    
    // Send the model matrix to the shader
    modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    
    // Set the color of the geometry
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw points
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
    
}

void Geometry::update(glm::mat4 C)
{
    model = C;
}



