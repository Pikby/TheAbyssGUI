#version 330 core
in vec2 TexCoords;
in vec2 FragPos;
out vec4 color;
#define PI 3.14159265

uniform vec2 origin;
uniform vec2 radius_squared;
uniform float thresh_hold;

float calcAlpha(float x,float y,vec2 radius_squared,float threshHold)
{
  float a = (1-pow(x,4)/radius_squared.x-pow(y,4)/radius_squared.y);
  return a;
}
void main()
{
  float alpha = calcAlpha(abs(origin.x-FragPos.x),abs(origin.y-FragPos.y),radius_squared,1);
  color = vec4(alpha);
}
