# version 330 core
// Please do not change the version directive to 150 or older.
// The version must be 330 or newer in order for modern OpenGL to work.

// Pass the color in via a uniform variable. Fragment position has already
// been computed in the vertex shader.
uniform vec3 color;

// There is no longer a gl_FragColor variable. The first output will determine the color
out vec4 fragColor;

void main (void) 
{        
	fragColor = vec4(color, 1.0f);
}
