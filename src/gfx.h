#ifndef gfx_h
#define gfx_h


#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>

#include "vecs.h"
#include "common_inline.h"

#define FP_EPSILON 0.00001f

inline void drawRect(vec2f center, vec2f dir, const float& width, const float& height){

  // assuming dir points in +ve height direction
  vec2f perp = vec2f ( -dir.y(), dir.x() );

  // todo: unset TEST_MODE when needed
#ifdef TEST_MODE
  assert(fabs(dir.mag()-1.0f) <= FP_EPSILON);
  dir.normalize();
  perp.normalize();
#endif
 
  vec2f lr = center - dir * height*0.5f - perp * width*0.5f;
  vec2f lf = center + dir * height*0.5f - perp * width*0.5f;
  vec2f rf = center + dir * height*0.5f + perp * width*0.5f;
  vec2f rr = center - dir * height*0.5f + perp * width*0.5f;

  
  glVertex2f(lr.x(), lr.y());
  glVertex2f(lf.x(), lf.y());
  glVertex2f(rf.x(), rf.y());
  glVertex2f(rr.x(), rr.y());


}

#endif