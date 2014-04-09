#version 120

varying vec2 texCoord;
varying vec4 fragColor;
uniform sampler2D TextureSampler;

void main()
{
	vec4 color = texture2D(TextureSampler, texCoord) * fragColor * 1.4 - 0.2;
	gl_FragColor = color;
} 
