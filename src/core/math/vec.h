#pragma once

#include <math.h>
#include <assert.h>

namespace bpm{

    struct vec2{
        float x,y;

        vec2(float _x, float _y) : x(_x), y(_y) {;;}
        vec2(const vec2& other) : x(other.x), y(other.y) {;;}
    };

    struct vec3{
        float x,y,z;

        vec3(float _x, float _y) : x(_x), y(_y), z(0) {;;}
        vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {;;}
        vec3(const vec3& other) : x(other.x), y(other.y), z(other.z) {;;}

        inline float lenSQ() const { return (x*x + y*y + z*z); }
        inline float len() const { return sqrt(lenSQ()); } 

        inline vec3& negate() { 
            x = -x; y = -y; z = -z;
            return *this; 
        }
    
        inline vec3& normalize() { 
            float inv_len = 1.0f / len();
            x *= inv_len; y *= inv_len; z *= inv_len;
            return *this; 
        }

        inline vec3 normalized() const { return vec3(*this).normalize(); }
    };

    struct vec4{
        float x,y,z,w;

        vec4() : x(0), y(0), z(0), w(0) {;;}
        vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {;;}
        vec4(const vec4& other) : x(other.x), y(other.y), z(other.z), w(other.w) {;;}

        inline float operator[] (int i) const {
            switch(i){
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
            }
            assert(0 <= i && i <= 3);
            return 0.f;
        }
        inline float& operator[](int i) {
            switch(i){
                case 0: return x;
                case 1: return y;
                case 2: return z;
                case 3: return w;
            }
            assert(0 <= i && i <= 3);
            return x;
        }
    };

    struct vec3d{
        double x,y,z;

        vec3d(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {;;}
        vec3d(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {;;}

        vec3d(const vec3& other) : x(other.x), y(other.y), z(other.z) {;;}
        vec3d(const vec3d& other) : x(other.x), y(other.y), z(other.z) {;;}
    };

    

    /**
     * vec3 const operations
     */

     //vector equality
     static inline bool operator==(const vec3& a, const vec3& b) { return a.x==b.x && a.y==b.y && a.z==b.z; }
     static inline bool equals(const vec3& a, const vec3& b, float tolerance = 1e-5) { 
         return tolerance > ( fabs(a.x-b.x) + fabs(a.y-b.y) + fabs(a.z-b.z) ); 
    }

    //vector addition
    static inline vec3 operator+(const vec3& a, const vec3& b) { return vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
    static inline vec3 operator-(const vec3& a, const vec3& b) { return vec3(a.x-b.x, a.y-b.y, a.z-b.z); }

    //scalar multiplication (division is ommited)
    static inline vec3 operator*(const vec3& v, const float& f) { return vec3(v.x*f, v.y*f, v.z*f); }
    static inline vec3 operator*(const float& f, const vec3& v) { return v*f; }

    //dot product
    static inline float dot(const vec3& a, const vec3& b) { return a.x*b.x + a.y*b.y + a.z*b.z; }

    //cross product
    static inline vec3 operator*(const vec3& a, const vec3& b) { 
        return vec3( 
            a.y*b.z - a.z*b.y, 
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x 
        ); 
    }

    /**
     * vec3 mutable operations
     */

     //vector addition
     static inline vec3 operator+=(vec3& a, const vec3& b) { 
        a.x += b.x;  a.y += b.y;  a.z += b.z;
        return a; 
    }
    static inline vec3 operator-=(vec3& a, const vec3& b) { 
        a.x -= b.x;  a.y -= b.y;  a.z -= b.z;
        return a; 
    }
    //scalar multiplication (division is ommited)
    static inline vec3 operator*=(vec3& v, const float& f) { 
        v.x *= f;  v.y *= f;  v.z *= f;
        return v; 
    }

    

    /**
     * vec4 const operations
     */

    //vector addition
    static inline vec4 operator+(const vec4& a, const vec4& b) { return vec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
    static inline vec4 operator-(const vec4& a, const vec4& b) { return vec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }

    //scalar multiplication (division is ommited)
    static inline vec4 operator*(const vec4& v, const float& f) { return vec4(v.x*f, v.y*f, v.z*f, v.w*f); }
    static inline vec4 operator*(const float& f, const vec4& v) { return v*f; }

    //cross product
    static inline vec4 operator*(const vec4& a, const vec4& b) { 
        return vec4( 
            a.y*b.z - a.z*b.y, 
            a.z*b.x - a.x*b.z,
            a.x*b.y - a.y*b.x,
            1.0f
        ); 
    }

    /**
     * vec4 mutable operations
     */

     //vector addition
     static inline vec4 operator+=(vec4& a, const vec4& b) { 
        a.x += b.x;  a.y += b.y;  a.z += b.z;  a.w += b.w;
        return a; 
    }
    static inline vec4 operator-=(vec4& a, const vec4& b) { 
        a.x -= b.x;  a.y -= b.y;  a.z -= b.z;  a.w -= b.w;
        return a; 
    }
    //scalar multiplication (division is ommited)
    static inline vec4 operator*=(vec4& v, const float& f) { 
        v.x *= f;  v.y *= f;  v.z *= f;  v.w *= f;
        return v; 
    }

} 