#pragma once

#include "external/klein.hpp"
#include "bse_common.h"

namespace bse
{
  //From https://www.jeremyong.com/klein/overview/

  //A plane is the manifestation of a reflection of E3E3 (consider the set of fixed points of a reflection).
  using plane = kln::plane;

  //A line is the manifestation of a rotation of E3E3 (consider the set of fixed points of a rotation).
  using line = kln::line;

  //A branch is a line through the origin
  using branch = kln::branch;

  //An ideal line is a line at infinity
  using ideal_line = kln::ideal_line;

  //A point is the manifestation of a roto-reflection of E3E3 (which has a single fixed point)
  using point = kln::point;

  //A direction is modeled as a point at infinity (homogeneous weight 00)
  using direction = kln::direction;

  //A rotor is the product of two intersecting planes (generating a rotation)
  using rotor = kln::rotor;

  //A translator is the product of two parallel planes (generating a translation)
  using translator = kln::translator;

  //A motor is a screw combining a rotation and translation along a screw axis
  using motor = kln::motor;


  float4 create_viewport_projector( float viewportWidth, float viewportHeight, float fovRadians, float zNear, float zFar )
  {
    float tanHalfFovx = 1.0f / tanf( fovRadians * 0.5f );
    float aspectRatio = viewportHeight / viewportWidth;
    float z = zFar / (zFar - zNear);
    return float4 { tanHalfFovx, aspectRatio * tanHalfFovx, z, zNear };
  }

  float4 project_positive_x_to_viewport( float3 p, float4 viewportProjector )
  {
    float4 result {};
    result.x = p.z * viewportProjector.x;
    result.y = p.y * viewportProjector.y;
    result.z = p.x * viewportProjector.z - viewportProjector.z * viewportProjector.w /* * p.w*/;
    result.w = p.x;
    return result;
  }
};
