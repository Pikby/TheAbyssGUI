#version 330 core
in vec2 TexCoords;
in vec4 outColor;

out vec4 color;

uniform sampler2D text;



void main()
{

  vec4 sampled = vec4(outColor.rgb, outColor.a*texture(text, TexCoords).r);
  color = sampled;

}
