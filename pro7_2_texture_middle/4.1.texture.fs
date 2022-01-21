#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord0;
in vec2 TexCoord1;

// texture samplers
uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture0, TexCoord0), texture(texture1, TexCoord1), 0.2);
}