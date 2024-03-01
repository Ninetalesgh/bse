#h

#version 450 core


#vs

layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 1) in vec3 vertexColor;

//uniform mat4 MVP;
uniform vec4 projector;

uniform vec3 scale;
uniform vec3 position;
uniform vec4 rotation;

vec4 rotate( vec4 r, vec4 p)
{
  vec4 scale = vec4(0, 2, 2, 2);
  vec4 t1 = r * r.xwyz; t1 -= r.x * r.xzwy; t1 *= scale;
  vec4 t2 = r.x * r.xwyz; t2 += r.xzwy * r; t2 *= scale;
  vec4 t3 = r * r; t3 += r.yxxx * r.yxxx;
  vec4 t4 = r.zwyz * r.zwyz; t4 += r.wzwy * r.wzwy; t3 -= t4 * vec4(-1.0, 1.0, 1.0, 1.0);
  vec4 q; q = t1 * p.xwyz; q += t2 * p.xzwy; q += t3 * p;
  return q;
}

vec4 project_perspective( vec3 point, vec4 viewportProjector )
{
  vec4 result;
  float invDistance = 1.0 / point.x;
  result.x = viewportProjector.x * invDistance * point.z;
  result.y = viewportProjector.y * invDistance * point.y;
  result.z = viewportProjector.z - invDistance * viewportProjector.w;  
  result.w = 1;
  return result;
}

vec4 project_orthographic( vec3 point, vec4 viewportProjector )
{
  vec4 result;
  result.x = viewportProjector.x * point.z;
  result.y = viewportProjector.y * point.y;
  result.z = point.x * viewportProjector.z - viewportProjector.w;  
  result.w = 1;
  return result;
}

void main()
{      
  vec3 p = scale * vertexPosition_modelspace + position;

  gl_Position = project_orthographic(p, projector);

  //fragmentColor = vertexColor;
  //fragmentColor = vec3(1.0, 1.0, 1.0);
  //uv = vertexUV;
}


#fs

//in vec2 uv;
//in vec3 fragmentColor;

uniform vec4 objectColor;

out vec4 color;  

//uniform sampler2D textureSampler;

void main()
{                           
  //color = fragmentColor;  
  color = objectColor;
  //color = texture(textureSampler, uv).rgb;
}
