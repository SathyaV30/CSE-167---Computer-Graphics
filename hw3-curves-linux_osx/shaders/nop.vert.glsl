# version 330 core
// Please do not change the version directive to 150 or older.
// The version must be 330 or newer in order for modern OpenGL to work.

layout (location = 0) in vec2 position;

// Projection and modelview matrices
uniform mat4 projection;
uniform mat4 modelview;

void main() {
    gl_Position = projection * modelview * vec4(position, 0.0f, 1.0f);
}

