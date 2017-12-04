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

    static inline bool operator==(const mat4& a, const mat4& b) {
        bool equals = true;
        for(int i = 0; i < 4; ++i){
            for(int j = 0; j < 4; ++j){
                equals &= (a.e[i][j] == b.e[i][j]);
            }
        }
        return equals;
    }

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

    static inline void make_scale(mat4& M, const vec3& s){
        make_identity(M);
        M.e[0][0] *= s.x;
        M.e[1][1] *= s.y;
        M.e[2][2] *= s.z;
    }

    static inline void make_translation(mat4& M, const vec3& v){
        make_identity(M);
        M.e[3][0] = v.x;
        M.e[3][1] = v.y;
        M.e[3][2] = v.z;
    }

    static inline void make_rotation(mat4& M, const vec3& axis, float rad){
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

    //fails when forward axis aligns with up direction!
    static inline void make_view(mat4& M, const vec3& pos, const vec3& forward, const vec3& up){
        //define camera axes (forward, right, up)
        vec3 f = forward;
        vec3 r = (f*up).normalize();
        vec3 u = r*f;

        M.e[0][0] =  r.x;
        M.e[0][1] =  u.x;
        M.e[0][2] = -f.x;
        M.e[0][3] =   0.f;
    
        M.e[1][0] =  r.y;
        M.e[1][1] =  u.y;
        M.e[1][2] = -f.y;
        M.e[1][3] =   0.f;
    
        M.e[2][0] =  r.z;
        M.e[2][1] =  u.z;
        M.e[2][2] = -f.z;
        M.e[2][3] =   0.f;
    
        M.e[3][0] =  -pos.x;
        M.e[3][1] =  -pos.y;
        M.e[3][2] =  -pos.z;
        M.e[3][3] =  1.f;
    }

    static inline void make_view_lookat(mat4& M, const vec3& pos, const vec3& target, const vec3& up){
        make_view(M, pos, (target-pos).normalize(), up);
    }

    static inline void make_perspective(mat4& M, float y_fov, float aspect, float n, float f) {
        float const a = 1.f / tan(y_fov / 2.f);
        
            M.e[0][0] = a / aspect;
            M.e[0][1] = 0.f;
            M.e[0][2] = 0.f;
            M.e[0][3] = 0.f;
        
            M.e[1][0] = 0.f;
            M.e[1][1] = a;
            M.e[1][2] = 0.f;
            M.e[1][3] = 0.f;
        
            M.e[2][0] = 0.f;
            M.e[2][1] = 0.f;
            M.e[2][2] = -((f + n) / (f - n));
            M.e[2][3] = -1.f;
        
            M.e[3][0] = 0.f;
            M.e[3][1] = 0.f;
            M.e[3][2] = -((2.f * f * n) / (f - n));
            M.e[3][3] = 0.f;
    }

    static inline void make_orthographic(mat4& M, float l, float r, float b, float t, float n, float f)
    {
        M.e[0][0] = 2.f/(r-l);
        M.e[0][1] = M.e[0][2] = M.e[0][3] = 0.f;
    
        M.e[1][1] = 2.f/(t-b);
        M.e[1][0] = M.e[1][2] = M.e[1][3] = 0.f;
    
        M.e[2][2] = -2.f/(f-n);
        M.e[2][0] = M.e[2][1] = M.e[2][3] = 0.f;
        
        M.e[3][0] = -(r+l)/(r-l);
        M.e[3][1] = -(t+b)/(t-b);
        M.e[3][2] = -(f+n)/(f-n);
        M.e[3][3] = 1.f;
    }

}
