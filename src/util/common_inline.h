// ---------------------------------------------------
//  Written by Anjul Patney in 2010
// ---------------------------------------------------

#ifndef common_inline_h
#define common_inline_h

#include <string>
#include <vector>
#include <list>
#include <math.h>
#include <cstdlib>
#include <sstream>
#include <assert.h>
#include <ctime>
#include "vecs.h"

// useful macros
#define __DEB                    {printf("Reached %s (%d)\n",__FILE__,__LINE__);}
#define __ERR                    {printf("ERROR AT %s (%d)\n",__FILE__,__LINE__); exit(1);}
#define RAND_01                  ((float)rand()/(float)RAND_MAX)
#define TIME_SO_FAR(reason,...)  {printf("----> Time : %0.2f [", ((float)(clock()-frameStartTime))/(float)CLOCKS_PER_SEC); printf(reason,__VA_ARGS__); printf("]\n");}
#define assertPrint(expr,msg,...){if(!(expr)){printf("[ASSERT] "); printf(msg,__VA_ARGS__); assert(0);}}
#define DeleteIfNotNull(x)       {if(x!=NULL){delete    x ; x=NULL;}}
#define DeleteArrayIfNotNull(x)  {if(x!=NULL){delete [] x ; x=NULL;}}
#define CLR_RED                  (0x000000ff)
#define CLR_GREEN                (0x0000ff00)
#define CLR_BLUE                 (0x00ff0000)
#define CLR_BLACK                (0x00000000)
#define CLR_WHITE                (0x00ffffff)
#define CLR_GREY                 (0x00aaaaaa)
#define CLR_DARKGREY             (0x00555555)

#define TEST_MODE

typedef unsigned int uint;

// -- disables the fopen_s warning. I don't like doing this, 
// -- but I can't use fopen_s because I'd like my code to be
// -- portable. Plus, no one's putting buffer overflow exploits
// -- in research code :-)
#pragma warning (disable : 4996)

template<typename to, typename from>
inline to lexical_cast(from const &x) {
    std::stringstream os;
    to ret;
    os << x;
    os >> ret;
    return ret;  
}

inline void trimString(std::string& str){
    std::string::size_type pos = str.find_last_not_of(' ');
    if(pos != std::string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(' ');
        if(pos != std::string::npos) str.erase(0, pos);
    }
    else str.erase(str.begin(), str.end());
}

// tokenize a string based on a set of single-char delimiters
inline void ltokenize(const std::string& str,const std::string& delimiters, std::list<std::string> &tokens)
{
    tokens.clear();
    // if empty, return empty
    if(str=="") return;
    // skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


inline void vtokenize_degen(const std::string& str,const std::string& delimiters, std::vector<std::string> &tokens)
{
    using namespace std;
    tokens.clear();
    string::size_type delimPos = 0, tokenPos = 0, pos = 0;

    if(str.length()<1)  return;
    while(1){
        delimPos = str.find_first_of(delimiters, pos);
        tokenPos = str.find_first_not_of(delimiters, pos);

        if(string::npos != delimPos){
            if(string::npos != tokenPos){
                if(tokenPos<delimPos){
                    tokens.push_back(str.substr(pos,delimPos-pos));
                }else{
                    tokens.push_back("");
                }
            }else{
                tokens.push_back("");
            }
            pos = delimPos+1;
        } else {
            if(string::npos != tokenPos){
                tokens.push_back(str.substr(pos));
            } else {
                tokens.push_back("");
            }
            break;
        }
    }
}


// tokenize a string based on a set of single-char delimiters
inline void vtokenize(const std::string& str,const std::string& delimiters, std::vector<std::string> &tokens)
{
    tokens.clear();
    // if empty, return empty
    if(str=="") return;
    // skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        // found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


inline void swap(float &x, float &y){
    float z=x; x=y; y=z;
}

inline void swap(int &x, int &y){
    int z=x; x=y; y=z;
}

inline float saturatef(float v, float vmax=1.0f, float vmin=0.0f){
    return ((v < vmin) ? vmin : (v > vmax) ? vmax : v);
}

inline uint rgbaFloatToInt(float r, float g, float b, float a=0.0f){
    r = saturatef(r); g = saturatef(g); b = saturatef(b); a = saturatef(a);
    return ((uint)(a*255)<<24) | ((uint)(b*255)<<16) | ((uint)(g*255)<<8) | (uint)(r*255);
}

inline float        minf(const float& a, const float& b)                  { return (a < b) ? a : b; }
inline float        maxf(const float& a, const float& b)                  { return (a > b) ? a : b; }

inline double rand01(){
    return ((double)rand()/(double)RAND_MAX);
}

inline void shuffle(float *samp, int count){
    for(int i=0;i<(count-1);i++){
        int other = i + (rand()%(count-i));
        swap(samp[i],samp[other]);
    }
}

inline void dispMat(float mat[], int size, int cols){
    printf("showing matrix:\n");
    for(int i=0; i<size; i++){
        printf("%0.3f ", mat[i]);
        if(i%cols == (cols-1)) printf("\n");
    }
}


inline float Determinant4f(const float m[16])
{
    return
        m[12]*m[9]*m[6]*m[3]-
        m[8]*m[13]*m[6]*m[3]-
        m[12]*m[5]*m[10]*m[3]+
        m[4]*m[13]*m[10]*m[3]+
        m[8]*m[5]*m[14]*m[3]-
        m[4]*m[9]*m[14]*m[3]-
        m[12]*m[9]*m[2]*m[7]+
        m[8]*m[13]*m[2]*m[7]+
        m[12]*m[1]*m[10]*m[7]-
        m[0]*m[13]*m[10]*m[7]-
        m[8]*m[1]*m[14]*m[7]+
        m[0]*m[9]*m[14]*m[7]+
        m[12]*m[5]*m[2]*m[11]-
        m[4]*m[13]*m[2]*m[11]-
        m[12]*m[1]*m[6]*m[11]+
        m[0]*m[13]*m[6]*m[11]+
        m[4]*m[1]*m[14]*m[11]-
        m[0]*m[5]*m[14]*m[11]-
        m[8]*m[5]*m[2]*m[15]+
        m[4]*m[9]*m[2]*m[15]+
        m[8]*m[1]*m[6]*m[15]-
        m[0]*m[9]*m[6]*m[15]-
        m[4]*m[1]*m[10]*m[15]+
        m[0]*m[5]*m[10]*m[15];
}

inline int GenerateInverseMatrix4f(float i[16], const float m[16])
{
    float x=Determinant4f(m);
    if (x==0) return 0;

    i[0]= (-m[13]*m[10]*m[7] +m[9]*m[14]*m[7] +m[13]*m[6]*m[11]
    -m[5]*m[14]*m[11] -m[9]*m[6]*m[15] +m[5]*m[10]*m[15])/x;
    i[4]= ( m[12]*m[10]*m[7] -m[8]*m[14]*m[7] -m[12]*m[6]*m[11]
    +m[4]*m[14]*m[11] +m[8]*m[6]*m[15] -m[4]*m[10]*m[15])/x;
    i[8]= (-m[12]*m[9]* m[7] +m[8]*m[13]*m[7] +m[12]*m[5]*m[11]
    -m[4]*m[13]*m[11] -m[8]*m[5]*m[15] +m[4]*m[9]* m[15])/x;
    i[12]=( m[12]*m[9]* m[6] -m[8]*m[13]*m[6] -m[12]*m[5]*m[10]
    +m[4]*m[13]*m[10] +m[8]*m[5]*m[14] -m[4]*m[9]* m[14])/x;
    i[1]= ( m[13]*m[10]*m[3] -m[9]*m[14]*m[3] -m[13]*m[2]*m[11]
    +m[1]*m[14]*m[11] +m[9]*m[2]*m[15] -m[1]*m[10]*m[15])/x;
    i[5]= (-m[12]*m[10]*m[3] +m[8]*m[14]*m[3] +m[12]*m[2]*m[11]
    -m[0]*m[14]*m[11] -m[8]*m[2]*m[15] +m[0]*m[10]*m[15])/x;
    i[9]= ( m[12]*m[9]* m[3] -m[8]*m[13]*m[3] -m[12]*m[1]*m[11]
    +m[0]*m[13]*m[11] +m[8]*m[1]*m[15] -m[0]*m[9]* m[15])/x;
    i[13]=(-m[12]*m[9]* m[2] +m[8]*m[13]*m[2] +m[12]*m[1]*m[10]
    -m[0]*m[13]*m[10] -m[8]*m[1]*m[14] +m[0]*m[9]* m[14])/x;
    i[2]= (-m[13]*m[6]* m[3] +m[5]*m[14]*m[3] +m[13]*m[2]*m[7]
    -m[1]*m[14]*m[7] -m[5]*m[2]*m[15] +m[1]*m[6]* m[15])/x;
    i[6]= ( m[12]*m[6]* m[3] -m[4]*m[14]*m[3] -m[12]*m[2]*m[7]
    +m[0]*m[14]*m[7] +m[4]*m[2]*m[15] -m[0]*m[6]* m[15])/x;
    i[10]=(-m[12]*m[5]* m[3] +m[4]*m[13]*m[3] +m[12]*m[1]*m[7]
    -m[0]*m[13]*m[7] -m[4]*m[1]*m[15] +m[0]*m[5]* m[15])/x;
    i[14]=( m[12]*m[5]* m[2] -m[4]*m[13]*m[2] -m[12]*m[1]*m[6]
    +m[0]*m[13]*m[6] +m[4]*m[1]*m[14] -m[0]*m[5]* m[14])/x;
    i[3]= ( m[9]* m[6]* m[3] -m[5]*m[10]*m[3] -m[9]* m[2]*m[7]
    +m[1]*m[10]*m[7] +m[5]*m[2]*m[11] -m[1]*m[6]* m[11])/x;
    i[7]= (-m[8]* m[6]* m[3] +m[4]*m[10]*m[3] +m[8]* m[2]*m[7]
    -m[0]*m[10]*m[7] -m[4]*m[2]*m[11] +m[0]*m[6]* m[11])/x;
    i[11]=( m[8]* m[5]* m[3] -m[4]*m[9]* m[3] -m[8]* m[1]*m[7]
    +m[0]*m[9]* m[7] +m[4]*m[1]*m[11] -m[0]*m[5]* m[11])/x;
    i[15]=(-m[8]* m[5]* m[2] +m[4]*m[9]* m[2] +m[8]* m[1]*m[6]
    -m[0]*m[9]* m[6] -m[4]*m[1]*m[10] +m[0]*m[5]* m[10])/x;

    return 1;
} 


inline void vtransform(const float m[16], const vec4f& vin, vec4f& vout){
    vout.x() = vin.peekx() * m[ 0] + vin.peeky() * m[ 4] + vin.peekz() * m[ 8] + vin.peekw() * m[12];
    vout.y() = vin.peekx() * m[ 1] + vin.peeky() * m[ 5] + vin.peekz() * m[ 9] + vin.peekw() * m[13];
    vout.z() = vin.peekx() * m[ 2] + vin.peeky() * m[ 6] + vin.peekz() * m[10] + vin.peekw() * m[14];
    vout.w() = vin.peekx() * m[ 3] + vin.peeky() * m[ 7] + vin.peekz() * m[11] + vin.peekw() * m[15];
}

inline void vtransform(const float m[16], const vec3f& vin, vec4f& vout){
    vec4f vh = vec4f(vin.peekx(), vin.peeky(), vin.peekz(), 1.0f);
    vtransform(m, vh, vout);
}

inline void ntransform(const float m[16], const vec3f& vin, vec3f& vout){
    vout.x() = vin.peekx() * m[ 0] + vin.peeky() * m[ 1] + vin.peekz() * m[ 2];
    vout.y() = vin.peekx() * m[ 4] + vin.peeky() * m[ 5] + vin.peekz() * m[ 6];
    vout.z() = vin.peekx() * m[ 8] + vin.peeky() * m[ 9] + vin.peekz() * m[10];

}


inline void pswap(float* &x, float* &y){
    float* temp;

    temp = x;
    x = y;
    y = temp;
}

// ------------------------------------------------------------------
// functions to swap two pointers
// ------------------------------------------------------------------
inline void pswap(int* &x, int* &y){
    int* temp;

    temp = x;
    x = y;
    y = temp;
}

inline void pswap(uint* &x, uint* &y){
    uint* temp;

    temp = x;
    x = y;
    y = temp;
}


// ------------------------------------------------------------------
// function to divide two numbers in floating point,
// take the ceiling, and return another integer
// very useful for CUDA 
// ------------------------------------------------------------------
inline int ceil_int_div(int numer, int denom){
    return (int)ceilf((float)numer / (float) denom);
}

#endif
