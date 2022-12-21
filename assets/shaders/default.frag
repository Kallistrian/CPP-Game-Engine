#version 330 compatibility
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}