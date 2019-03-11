#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;
out vec4 outColor;

uniform mat4 projection;
struct Character
{
    vec2 size;    // Size of glyph
    vec2 bearing;  // Offset from baseline to left/top of glyph
    uint advance;    // Horizontal offset to advance to next glyph
    float xstart;
    float ystart;
};
uniform Character characters[128];

void main()
{
  int a = floatBitsToInt(vertex.z);
  vec2 tex = vec2( (a & 2)>>1,a & 1);
  int c = (a >> 8);
  int colorPacked = floatBitsToInt(vertex.w);
  outColor = vec4((colorPacked & 0xff)/255.0f,
                    ((colorPacked >> 8) & 0xff)/255.0f,
                    ((colorPacked >> 16) & 0xff)/255.0f,
                    ((colorPacked >> 24) & 0xff)/255.0f);

  Character character = characters[c];

  gl_Position =  projection*vec4(vertex.xy, 0, 1.0);


  float x = character.xstart + tex.x*(character.size.x);
  float y = character.ystart + tex.y*(character.size.y);

  TexCoords = vec2(x,y);
}
