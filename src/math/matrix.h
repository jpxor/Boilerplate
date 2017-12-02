#pragma once

#include <math.h>
#include "math/vec.h"

namespace bpm{

    
    struct mat4{
        float e[4][4];

        mat4() {;;}
        mat4(float f) {
            for(int i = 0; i < 4; ++i){
                for(int j = 0; j < 4; ++j){
                    e[i][j] = i==j ? f : 0.0f;
                }
            }
        }
        mat4(const mat4& other) {
            for(int i = 0; i < 4; ++i){
                for(int j = 0; j < 4; ++j){
                    e[i][j] = other.e[i][j];
                }
            }
        }
    };

    /**
     * Matrix const operations
     */

    static inline mat4 operator*(const mat4& a, const mat4& b) { 
        mat4 M;
        for(int c = 0; c < 4; ++c){
            for(int r = 0; r < 4; ++r) {
                M.e[c][r] = 0.f;
                for(int k = 0; k < 4; ++k){
                    M.e[c][r] += a.e[k][r] * b.e[c][k];
                }
            }
        }
        return M;
    }

    static inline vec4 operator*(const mat4& M, const vec4& v) { 
        float r[4];
        for(int j = 0; j < 4; ++j) {
            r[j] = 
                + M.e[0][j] * v.x
                + M.e[1][j] * v.y
                + M.e[2][j] * v.z
                + M.e[3][j] * v.w;
        }
        return vec4(r[0], r[1], r[2], r[3]);
    }

    static inline vec3 operator*(const mat4& M, const vec3& v) { 
        float r[3];
        for(int j = 0; j < 3; ++j) {
            r[j] = 
                + M.e[0][j] * v.x
                + M.e[1][j] * v.y
                + M.e[2][j] * v.z
                + M.e[3][j];
        }
        return vec3(r[0], r[1], r[2]);
    }

    /**
     * Matrix utils
     */

    static inline void make_identity(mat4& M){
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                M.e[i][j] = i==j ? 1.0f : 0.0f;
            }
        }
    }

    static inline void make_scale(const vec3& s, mat4& M){
        make_identity(M);
        M.e[0][0] *= s.x;
        M.e[1][1] *= s.y;
        M.e[2][2] *= s.z;
    }

    static inline void make_translation(const vec3& v, mat4& M){
        make_identity(M);
        M.e[3][0] = v.x;
        M.e[3][1] = v.y;
        M.e[3][2] = v.z;
    }

    static inline void make_rotation(const vec3& axis, float rad, mat4& M){
        float cos = cosf(rad);        
        float sin = sinf(rad);
        float lmcos = 1-cos;

        float ux = axis.x;
        float uy = axis.y;
        float uz = axis.z;

        float xy = ux*uy*lmcos;
        float xz = ux*uz*lmcos;
        float yz = uy*uz*lmcos;

        float xsin = ux*sin;
        float ysin = uy*sin;
        float zsin = uz*sin;

        M.e[0][0] = cos + ux*ux*lmcos; 
        M.e[0][1] = xy + zsin;
        M.e[0][2] = xz - ysin;
        M.e[0][3] = 0;

        M.e[1][0] = xy - zsin;
        M.e[1][1] = cos + uy*uy*lmcos;
        M.e[1][2] = yz + xsin;
        M.e[1][3] = 0;

        M.e[2][0] = xz + ysin;
        M.e[2][1] = yz - xsin;
        M.e[2][2] = cos + uz*uz*lmcos;
        M.e[2][3] = 0;

        M.e[3][0] = 0;
        M.e[3][1] = 0;
        M.e[3][2] = 0;
        M.e[3][3] = 1;
    }

    static inline void make_lookat_rotation(const vec3& pos, const vec3& target, const vec3& up, mat4& M){
        //i dont understand this one yet
        //See the OpenGL GLUT documentation for gluLookAt for a description 
        vec3 f = (target - pos).normalize();
        vec3 s = (f*up).normalize();
        vec3 t = s*f;

        M.e[0][0] =  s.x;
        M.e[0][1] =  t.x;
        M.e[0][2] = -f.x;
        M.e[0][3] =   0.f;
    
        M.e[1][0] =  s.y;
        M.e[1][1] =  t.y;
        M.e[1][2] = -f.y;
        M.e[1][3] =   0.f;
    
        M.e[2][0] =  s.z;
        M.e[2][1] =  t.z;
        M.e[2][2] = -f.z;
        M.e[2][3] =   0.f;
    
        M.e[3][0] =  0.f;
        M.e[3][1] =  0.f;
        M.e[3][2] =  0.f;
        M.e[3][3] =  1.f;

        mat4 T; 
        make_translation( vec3(-pos.x, -pos.y, -pos.z), T);
        M = M*T;//correct order? //needs to be in place!
    }

    static inline void make_perspective(float y_fov, float aspect, float n, float f, mat4& M) {

    }

}
