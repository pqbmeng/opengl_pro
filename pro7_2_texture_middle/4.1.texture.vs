#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord0;
layout (location = 3) in vec2 aTexCoord1;

out vec3 ourColor;
out vec2 TexCoord0;
out vec2 TexCoord1;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord0 = vec2(aTexCoord0.x, aTexCoord0.y);
	TexCoord1 = vec2(aTexCoord1.x, aTexCoord1.y);
}