#version 330 core
out vec4 FragColor;

uniform vec4 color;

uniform vec2 resolution;

// comment
uniform CameraUBO
{
	mat4 view;
	mat4 projection;
	mat4 viewProjection;
	vec3 cameraPosition;
	float padding;
};


void main()
{
    vec2 uv = (gl_FragCoord.xy / vec2(800.0, 600.0)) * 2.0 - 1.0;

    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
} 