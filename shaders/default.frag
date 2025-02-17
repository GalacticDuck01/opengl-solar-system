#version 330 core

in vec3 currentPos;
in vec3 normal;
in vec3 colour;
in vec2 texCoord;

out vec4 FragColour;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform vec3 camPos;

vec4 PointLight() {
    vec3 lightVector = lightPos - currentPos;
    float dist = length(lightVector);
    float a = 0.05;
    float b = 0.01;
    float intensity = 1.0/(a*dist*dist + b*dist + 1.0);

    float ambient = 0.2;

    vec3 unitNormal = normalize(normal);
    vec3 lightDirection = normalize(lightVector);
    float diffuse = max(dot(unitNormal, lightDirection), 0.0);
    float specularLight = 0.5;
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectDirection = reflect(-lightDirection, unitNormal);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    float specular = specularAmount*specularLight;

    return lightColour*((intensity*diffuse + ambient)*texture(diffuse0, texCoord) + intensity*specular*texture(specular0, texCoord).r);
}

vec4 DirectionalLight() {
    float ambient = 0.2;

    vec3 unitNormal = normalize(normal);
    vec3 lightDirection = normalize(vec3(1.0, 1.0, 0.0));
    float diffuse = max(dot(unitNormal, lightDirection), 0.0);
    float specularLight = 0.5;
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectDirection = reflect(-lightDirection, unitNormal);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    float specular = specularAmount*specularLight;

    return lightColour*((diffuse + ambient)*texture(diffuse0, texCoord) + specular*texture(specular0, texCoord).r);
}

vec4 SpotLight() {
    vec3 lightVector = lightPos - currentPos;

    // cos(angle)s for two cones that create a soft spotlight
    float outerCone = 0.90;
    float innerCone = 0.95;

    float ambient = 0.2;

    vec3 unitNormal = normalize(normal);
    vec3 lightDirection = normalize(lightVector);
    float diffuse = max(dot(unitNormal, lightDirection), 0.0);
    float specularLight = 0.5;
    vec3 viewDirection = normalize(camPos - currentPos);
    vec3 reflectDirection = reflect(-lightDirection, unitNormal);
    float specularAmount = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    float specular = specularAmount*specularLight;

    float angle = dot(vec3(0.0, -1.0, 0.0), -lightDirection);
    float intensity = clamp((angle - outerCone)/(innerCone - outerCone), 0.0, 1.0);

    return lightColour*((intensity*diffuse + ambient)*texture(diffuse0, texCoord) + intensity*specular*texture(specular0, texCoord).r);
}

void main() {
    FragColour = PointLight();
}