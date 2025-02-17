#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColour;
layout (location = 3) in vec2 aTexture;

// Outputs that go to the fragment shader
out vec3 currentPos;
out vec3 normal;
out vec3 colour;
out vec2 texCoord;

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main() {
   currentPos = vec3(model*translation*rotation*scale*vec4(aPos, 1.0));
   normal = aNormal;
   colour = aColour;
   texCoord = mat2(1.0, 0.0, 0.0, -1.0)*aTexture;

   gl_Position = camMatrix*vec4(currentPos, 1.0);
}