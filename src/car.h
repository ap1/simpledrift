#ifndef car_h
#define car_h

#include "vecs.h"
#include "gfx.h"
#include <GL/glew.h>
#include <GL/glut.h>


class twheel{
private:
public:
  float ypos;
  float dia;
  float width;
};


// all units are meters

//  
//   ___________
//  |           |
// |0|_________|1|
// | |         | |
//  |           |
//  |           |
//  |           |
//  |           |
//  |   (0,0)   |
// |2|____.____|3|
// | |         | |
//  |           |
//  `-----------'
//  

class car{
protected:
  twheel  wheels_[4];
  float   wheelbase_; // wheelbase
  float   length_;    // body length
  float   width_;     // body width
  float   height_;    // body height
  float   weight_;    // curb weight (kgs)
  float   cgPos_;     // y-coord of center-of-gravity
  float   bodyPos_;   // y-coord of center of chassis

public:
  car(){}

  void draw(const vec2f& pos, const vec2f& bodydir, const vec2f& wheeldir){

    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_QUADS);
    // body
    //{
    //  vec2f lr = vec2f(pos) + vec2f( -0.5f*width_, bodyPos_-0.5f*length_ );
    //  glVertex2f( lr.x() + 0.0f,   lr.y() + 0.0f);
    //  glVertex2f( lr.x() + 0.0f,   lr.y() + length_);
    //  glVertex2f( lr.x() + width_, lr.y() + length_);
    //  glVertex2f( lr.x() + width_, lr.y() + 0.0f);
    //}

    drawRect(pos + wheelbase_ * 0.5f * bodydir, bodydir, width_, length_);

    // wheels
    for(int i=0; i<4; i++)
    {
      vec2f ctr = vec2f(pos) + 
        vec2f( 
        (i%2==1?-1:1) * 0.5f*width_    - (i%2==0?0.8f:0.2f)*wheels_[i].width,
              wheels_[i].ypos - 0.5f*wheels_[i].dia);

      //glVertex2f( lr.x() + 0.0f,             lr.y() + 0.0f);
      //glVertex2f( lr.x() + 0.0f,             lr.y() + wheels_[i].dia);
      //glVertex2f( lr.x() + wheels_[i].width, lr.y() + wheels_[i].dia);
      //glVertex2f( lr.x() + wheels_[i].width, lr.y() + 0.0f);

      drawRect(pos + wheelbase_ * 0.5f * wheeldir, wheeldir, wheels_[i].width, wheels_[i].dia);
    }
    glEnd();
  }

};

class nissan_370z : public car{
private:
public:
  nissan_370z():car(){

    wheelbase_        = 2.550f;
    length_           = 4.240f;
    width_            = 1.850f;
    height_           = 1.320f;
    weight_           = 1466.0f;
    bodyPos_          = (0.5f * wheelbase_);

    wheels_[0].dia    = 0.675f;
    wheels_[1].dia    = 0.675f;
    wheels_[2].dia    = 0.675f;
    wheels_[3].dia    = 0.675f;

    wheels_[0].width  = 0.285f;
    wheels_[1].width  = 0.285f;
    wheels_[2].width  = 0.285f;
    wheels_[3].width  = 0.285f;

    wheels_[0].ypos   = wheelbase_;
    wheels_[1].ypos   = wheelbase_;
    wheels_[2].ypos   = 0.0f;
    wheels_[3].ypos   = 0.0f;

  }
};

#endif