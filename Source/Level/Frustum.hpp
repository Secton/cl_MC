#pragma once

#include "../Physics/AABB.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <array>
#include <cmath>



class Frustum {
    public:
        std::array<std::array<float, 4>, 6> m_Frustum;
        inline static const int  RIGHT = 0;
        inline static const int   LEFT = 1;
        inline static const int BOTTOM = 2;
        inline static const int    TOP = 3;
        inline static const int   BACK = 4;
        inline static const int  FRONT = 5;
        inline static const int      A = 0;
        inline static const int      B = 1;
        inline static const int      C = 2;
        inline static const int      D = 3;
    private:
        // std::array<float, 16> proj;
        // std::array<float, 16> modl;
        // std::array<float, 16> clip;
        static Frustum frustum;
    public:
        std::array<float, 16> proj;
        std::array<float, 16> modl;
        std::array<float, 16> clip;

        Frustum() {}

        static Frustum getFrustum() {
            frustum.calculateFrustum();
            return frustum;
        }
    

    private:
        void normalizePlane(std::array<std::array<float, 4>, 6>* frustum, int side) {
            auto& plane = (*frustum)[side];
            float magnitude = (float)std::sqrt(
                plane[0] * plane[0] +
                plane[1] * plane[1] +
                plane[2] * plane[2]
            );
            // std::array<float, 4> fArray = frustum[side];
            // fArray[0] = fArray[0] / magnitude;
            // std::array<float, 4> fArray2 = frustum[side];
            // fArray2[1] = fArray[1] / magnitude;
            // std::array<float, 4> fArray3 = frustum[side];
            // fArray3[2] = fArray[2] / magnitude;
            // std::array<float, 4> fArray4 = frustum[side];
            // fArray4[3] = fArray[3] / magnitude;
            plane[0] /= magnitude;
            plane[1] /= magnitude;
            plane[2] /= magnitude;
            plane[3] /= magnitude;
        }

    public:
        void calculateFrustum() {
            // this->proj.fill(0);
            // this->modl.fill(0);
            // this->clip.fill(0);
            // glMatrixMode(GL_PROJECTION);
            // glLoadIdentity();
            // gluPerspective(45.0f, float(1024)/float(768), 0.1f, 100.0f);
            glGetFloatv(GL_PROJECTION_MATRIX, this->proj.data());
            glGetFloatv(GL_MODELVIEW_MATRIX, this->modl.data());
            // Yikes!
            #define tc this->clip
            #define tm this->modl
            #define tp this->proj
            tc[0]  = tm[0]  * tp[0] + tm[1]  * tp[4] + tm[2]  * tp[8]  + tm[3]  * tp[12];
            tc[1]  = tm[0]  * tp[1] + tm[1]  * tp[5] + tm[2]  * tp[9]  + tm[3]  * tp[13];
            tc[2]  = tm[0]  * tp[2] + tm[1]  * tp[6] + tm[2]  * tp[10] + tm[3]  * tp[14];
            tc[3]  = tm[0]  * tp[3] + tm[1]  * tp[7] + tm[2]  * tp[11] + tm[3]  * tp[15];
            tc[4]  = tm[4]  * tp[0] + tm[5]  * tp[4] + tm[6]  * tp[8]  + tm[7]  * tp[12];
            tc[5]  = tm[4]  * tp[1] + tm[5]  * tp[5] + tm[6]  * tp[9]  + tm[7]  * tp[13];
            tc[6]  = tm[4]  * tp[2] + tm[5]  * tp[6] + tm[6]  * tp[10] + tm[7]  * tp[14];
            tc[7]  = tm[4]  * tp[3] + tm[5]  * tp[7] + tm[6]  * tp[11] + tm[7]  * tp[15];
            tc[8]  = tm[8]  * tp[0] + tm[9]  * tp[4] + tm[10] * tp[8]  + tm[11] * tp[12];
            tc[9]  = tm[8]  * tp[1] + tm[9]  * tp[5] + tm[10] * tp[9]  + tm[11] * tp[13];
            tc[10] = tm[8]  * tp[2] + tm[9]  * tp[6] + tm[10] * tp[10] + tm[11] * tp[14];
            tc[11] = tm[8]  * tp[3] + tm[9]  * tp[7] + tm[10] * tp[11] + tm[11] * tp[15];
            tc[12] = tm[12] * tp[0] + tm[13] * tp[4] + tm[14] * tp[8]  + tm[15] * tp[12];
            tc[13] = tm[12] * tp[1] + tm[13] * tp[5] + tm[14] * tp[9]  + tm[15] * tp[13];
            tc[14] = tm[12] * tp[2] + tm[13] * tp[6] + tm[14] * tp[10] + tm[15] * tp[14];
            tc[15] = tm[12] * tp[3] + tm[13] * tp[7] + tm[14] * tp[11] + tm[15] * tp[15];
            #undef tm
            // But it doesn't end there...
            this->m_Frustum[0][0] = this->clip[3]  - this->clip[0];
            this->m_Frustum[0][1] = this->clip[7]  - this->clip[4];
            this->m_Frustum[0][2] = this->clip[11] - this->clip[8];
            this->m_Frustum[0][3] = this->clip[15] - this->clip[12];
            this->normalizePlane(&this->m_Frustum, 0);
            this->m_Frustum[1][0] = this->clip[3]  + this->clip[0];
            this->m_Frustum[1][1] = this->clip[7]  + this->clip[4];
            this->m_Frustum[1][2] = this->clip[11] + this->clip[8];
            this->m_Frustum[1][3] = this->clip[15] + this->clip[12];
            this->normalizePlane(&this->m_Frustum, 1);
            this->m_Frustum[2][0] = this->clip[3]  + this->clip[1];
            this->m_Frustum[2][1] = this->clip[7]  + this->clip[5];
            this->m_Frustum[2][2] = this->clip[11] + this->clip[9];
            this->m_Frustum[2][3] = this->clip[15] + this->clip[13];
            this->normalizePlane(&this->m_Frustum, 2);
            this->m_Frustum[3][0] = this->clip[3]  - this->clip[1];
            this->m_Frustum[3][1] = this->clip[7]  - this->clip[5];
            this->m_Frustum[3][2] = this->clip[11] - this->clip[9];
            this->m_Frustum[3][3] = this->clip[15] - this->clip[13];
            this->normalizePlane(&this->m_Frustum, 3);
            this->m_Frustum[4][0] = this->clip[3]  - this->clip[2];
            this->m_Frustum[4][1] = this->clip[7]  - this->clip[6];
            this->m_Frustum[4][2] = this->clip[11] - this->clip[10];
            this->m_Frustum[4][3] = this->clip[15] - this->clip[14];
            this->normalizePlane(&this->m_Frustum, 4);
            this->m_Frustum[5][0] = this->clip[3]  + this->clip[2];
            this->m_Frustum[5][1] = this->clip[7]  + this->clip[6];
            this->m_Frustum[5][2] = this->clip[11] + this->clip[10];
            this->m_Frustum[5][3] = this->clip[15] + this->clip[14];
            this->normalizePlane(&this->m_Frustum, 5);
        }

        bool pointInFrustum(float x, float y, float z) {
            int i = 0;
            while (i < 6) {
                if (this->m_Frustum[i][0] * x + this->m_Frustum[i][1] * y +
                    this->m_Frustum[i][2] * z + this->m_Frustum[i][3] <= 0.f)
                    return false;
                i++;
            }
            return true;
        }

        bool sphereInFrustum(float x, float y, float z, float radius) {
            int i = 0;
            while (i < 6) {
                if (this->m_Frustum[i][0] * x + this->m_Frustum[i][1] * y +
                    this->m_Frustum[i][2] * z + this->m_Frustum[i][3] <= -radius)
                    return false;
                i++;
            }
            return true;
        }

        bool cubeFullyInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
            int i = 0;
            while (i < 6) {
                if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                if (!(this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 + this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                ++i;
            }
            return true;
        }

        bool cubeInFrustum(float x1, float y1, float z1, float x2, float y2, float z2) {
            int i = 0;
            while (i < 6) {
                // Oh jeez...
                if (!(this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 +
                    this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 +
                    this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 +
                    this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 +
                    this->m_Frustum[i][2] * z1 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y1 +
                    this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y1 +
                    this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x1 + this->m_Frustum[i][1] * y2 +
                    this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f ||
                    this->m_Frustum[i][0] * x2 + this->m_Frustum[i][1] * y2 +
                    this->m_Frustum[i][2] * z2 + this->m_Frustum[i][3] > 0.0f))
                    return false;
                ++i;
            }
            return true;
        }

        bool cubeInFrustum(AABB aabb) {
            return this->cubeInFrustum(aabb.x0, aabb.y0, aabb.z0, aabb.x1, aabb.y1, aabb.z1);
        }
};
inline Frustum Frustum::frustum;