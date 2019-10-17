#include "PointCloud.h"


PointCloud::PointCloud(std::string objFilename, GLfloat pointSize)
: pointSize(pointSize)
{
    /*
     * Section 2: Currently, all the points are hard coded below.
     * Modify this to read points from an obj file.
     */
    std::ifstream objFile(objFilename,std::ios::in); // The obj file we are reading.
    
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
            
            // If the line is about vertex (starting with a "v").
            if (label == "v")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;
                
                // Process the point. For example, you can save it to a.
                points.push_back(point);
            }
            
            //load normals
            if (label == "vn") {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                
                normals.push_back(normal);
            }
            
            //load face indices
            if (label == "f") {
                std::vector<unsigned int> vect;
              
                // store indices into the vertex list
                // adjust 1-based indices to 0-based indices
                unsigned int first;
                ss >> first;
                ss.ignore(256, ' ');
                vect.push_back(first-1);
                
                unsigned int second;
                ss >> second;
                ss.ignore(256, ' ');
                vect.push_back(second-1);
                
                unsigned int third;
                ss >> third;
                ss.ignore(256, ' ');
                vect.push_back(third-1);
                
                faceIndices.push_back(glm::ivec3(vect[0], vect[1], vect[2]));
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }
    
    objFile.close();
    
    
    
    /*
     * Section 4, you will need to normalize the object to fit in the
     * screen.
     */
    
    
    //Centering the model
    
    //traverse the vertices of the model
    for(int i= 0;i< points.size(); i++) {
        
        // Find minimum and maximum coordinates along each dimension
        if(points[i].x > xMax) {
            xMax = points[i].x;
        }
        if(points[i].y > yMax) {
            yMax = points[i].y;
        }
        if(points[i].z > zMax) {
            zMax = points[i].z;
        }
        if(points[i].x < xMin) {
            xMin = points[i].x;
        }
        if(points[i].y < yMin) {
            yMin = points[i].y;
        }
        if(points[i].z < zMin) {
            zMin = points[i].z;
        }
        
    }
    
    //determine the center point of the model
    GLfloat centerX = (GLfloat)((xMin+xMax)/2);
    GLfloat centerY = (GLfloat)((yMin+yMax)/2);
    GLfloat centerZ = (GLfloat)((zMin+zMax)/2);
    
    modelCenter = glm::vec3(centerX, centerY, centerZ);
    
    //shift all model coordinates by subtracting the coordinates of the center point
    for(int i=0; i< points.size(); i++) {
        GLfloat xCoor = points[i].x - modelCenter.x;
        GLfloat yCoor = points[i].y - modelCenter.y;
        GLfloat zCoor = points[i].z - modelCenter.z;
        points[i] = glm::vec3(xCoor,yCoor,zCoor);
    }
    
    
    // Scaling the model
    
    //calculate the maximum distance any point is from the center
    for(int i=0; i< points.size(); i++) {
        GLfloat d = sqrt( pow(points[i].x,2) +
                         pow(points[i].y,2) +
                         pow(points[i].z,2));
        if(d > maxDistance) {
            maxDistance = d;
        }
    }
    
    GLfloat scale = 10.5/maxDistance;
    
    
    // Set the model matrix to an identity matrix.
    model = glm::mat4(1);
    // Set the color.
    color = glm::vec3(1, 1, 1);
    

    model = glm::scale(glm::vec3(scale,scale,scale)) * model;

    
    // Generate a vertex array (VAO) and 2 vertex buffer objects (VBO).
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);
    glGenBuffers(1, &ebo);
    
    // Bind to the VAO.
    glBindVertexArray(vao);
    
    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
                 points.data(), GL_DYNAMIC_DRAW);
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faceIndices.size(),
                 faceIndices.data(), GL_DYNAMIC_DRAW);
    
    
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

PointCloud::~PointCloud()
{
    // Delete the VBO and the VAO.
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
    
}

void PointCloud::draw()
{
    // Bind to the VAO.
    glBindVertexArray(vao);
    // Set point size.
    glPointSize(pointSize);
    // Draw points
    glDrawElements(GL_TRIANGLES, 3*faceIndices.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void PointCloud::update()
{
   
}

void PointCloud::updatePointSize(GLfloat size)
{
    /*
     * Section 3: Implement this function to adjust the point size.
     */
    pointSize += size;
    glPointSize(pointSize);
    
}

void PointCloud::rotate(float angle, glm::vec3 axis)
{
    // Update the transformation matrix by multiplying a rotation matrix
    model = glm::rotate(angle, axis)*model;
}

void PointCloud::scale(float factor)
{
    model = glm::scale(glm::vec3(factor, factor, factor)) * model;
}
