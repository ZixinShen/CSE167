#version 330 core
// This is a sample fragment shader.


struct Material {
    vec3 diffuse;
    vec3 specular;
    vec3 ambient;
    float shininess;
};

struct pointLight {
    vec3 position;
    vec3 color;
};


// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 posOutput;
in vec3 normalOutput;


uniform pointLight lightSource;
uniform Material material;
uniform vec3 viewPos;
uniform int colormode;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

//function prototypes
vec3 normalColoring(vec3 normal);
vec3 illumination(pointLight light, vec3 normal, vec3 position, vec3 viewDir);

void main()
{
    vec3 viewDir = normalize(viewPos - posOutput);
    vec3 normal = normalize(normalOutput);
    
    // normal coloring
    if(colormode == 1) {
        fragColor = vec4(normalColoring(normal),1.0f);
    } else {
        fragColor = vec4(illumination(lightSource,normal,posOutput,viewDir),1.0f);
    }
}

//normal coloring
vec3 normalColoring(vec3 normal)
{
    //Calculate RGB values corresponding to x,y,z components of the normal
    float red = (normal.x + 1.0f)/2.0f;
    float green = (normal.y + 1.0f)/2.0f;
    float blue = (normal.x + 1.0f)/2.0f;
    vec3 color = vec3(red, green, blue);
    
    return color;
}

// Calculate color using Phong Illumination Model
vec3 illumination(pointLight light, vec3 normal, vec3 position, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - position);

    
    // Diffuse shading
    vec3 diffuse = light.color* material.diffuse* max(dot(normal, lightDir),0.0);
    
    // Specular shading
    vec3 reflectDir = 2* dot(normal,lightDir)*normal - lightDir;
    vec3 specular = material.specular * light.color * pow(max(dot(reflectDir,viewDir),0.0), material.shininess);
    
    // Ambient shading
    vec3 ambient = light.color * material.ambient;
    
    // Linear Attenuation
    

    // Attenuation
    float distance = length(light.position - position);
    float attenuation = 1.0f / (1.0f + 0.15 * distance);
    
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return (ambient + diffuse + specular);
}
