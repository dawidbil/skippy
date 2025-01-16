#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	vec2 negateTexCoord = vec2(-TexCoord.x, TexCoord.y);
	FragColor = mix(texture(texture0, TexCoord), texture(texture1, negateTexCoord), 0.2);
}
