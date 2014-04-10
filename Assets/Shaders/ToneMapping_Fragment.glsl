#version 120

varying vec2 texCoord;
varying vec4 fragColor;
uniform sampler2D TextureSampler;

void main()
{
	vec4 color = texture2D(TextureSampler, texCoord) * fragColor * 1.4 - 0.2;
	
	// Naive brightness correction for values above 1.0
	// Try to correct it by adding green
	vec3 overshot = clamp(color.rgb - 1.0, 0.0, 1.5);
	float overshotL = overshot.r * 0.2126 + overshot.g * 0.7152 + overshot.b * 0.0722;
	color = color + vec4(overshotL*0.25, overshotL, overshotL*0.10, color.a);
	// ... then by adding blue ...
	overshot = clamp(color.rgb - 2.0, 0.0, 1.5);
	overshotL = overshot.r * 0.2126 + overshot.g * 0.7152 + overshot.b * 0.0722;
	color = color + vec4(overshotL*0.25, overshotL*0.10, overshotL*2, color.a);
	// ... and by adding red ...
	overshot = clamp(color.rgb - 2.5, 0.0, 1.5);
	overshotL = overshot.r * 0.2126 + overshot.g * 0.7152 + overshot.b * 0.0722;
	color = color + vec4(overshotL, overshotL*0.10, 0, color.a);
	
	// Apply
	gl_FragColor = color;
} 
