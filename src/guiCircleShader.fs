#version 330 core
in vec2 TexCoords;
out vec4 color;


uniform vec2 center;
uniform float radius;
uniform float fall_off_distance;
void main()
{
  vec2 fragPos = gl_Position.xy;
  if(sqrt(pow(center.x-fragPos.x,2)+pow(center.y-fragPos.x,2))
  {
    color = vec4(1);
  }
  else
  {
    color = vec4(vec3(0),1);
  }
}
