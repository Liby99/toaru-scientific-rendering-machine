#version 410

in vec3 fragColor;

out vec4 finalColor;

void main() {
  finalColor = vec4(fragColor, 1);
}
