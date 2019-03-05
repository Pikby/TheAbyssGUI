#version 330 core
in vec2 TexCoords;
in vec4 outColor;

out vec4 color;

uniform sampler2D text;
uniform float buf;
uniform float gamma;


void main()
{
  float dist = texture(text, TexCoords).r;
  //float alpha = smoothstep(200.0f/255.0f,1,dist);
  float alpha = smoothstep(buf-gamma,buf+gamma,dist);

  alpha = round(alpha);
  vec4 sampled = vec4(outColor.rgb, outColor.a*alpha);
  color = sampled;

}
