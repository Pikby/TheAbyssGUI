#version 330 core
in vec2 TexCoords;
in vec4 outColor;

out vec4 color;

uniform sampler2D text;
uniform float buf;
uniform float gamma;


void main()
{


  //float buf = 0.7;
  //float gamma = 0.5;
  float dist = texture(text, TexCoords).r;
  //float alpha = smoothstep(200.0f/255.0f,1,dist);
  float alpha = smoothstep(buf-gamma,buf+gamma,dist);
  //0.700003:0.526997


  vec4 sampled;
  if(alpha > 0.7)
  {
    sampled = vec4(outColor.rgb, outColor.a);
  }
  else if(alpha > 0.6)
  {
    //sampled = vec4(vec3(0),outColor.a);
  }
  else sampled = vec4(0);
  color = sampled;

}
