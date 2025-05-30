#version 440
layout(location = 0) in vec2 vTexCoord;
layout(location = 0) out vec4 fragColor;
layout(binding = 1) uniform sampler2D srcTex;
void main() {
    vec4 color = texture(srcTex, vTexCoord);
    color.rgb *= color.a;
    color.rgb = color.bgr;
}