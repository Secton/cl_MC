#pragma once

#include <GL/gl.h>
#include <SDL3/SDL_log.h>

class Tesselator {
    private:
        inline static const int MAX_VERTICES = 100000;
        float c_vertexBuffer[300000];
        float c_texCoordBuffer[200000];
        float c_colorBuffer[300000];
        int vertices = 0;
        float u,v; float r,g,b;
        bool hasColor = false;
        bool hasTexture = false;

    public:
        void flush() {
            glVertexPointer(3, GL_FLOAT, 0, c_vertexBuffer);

            if (this->hasTexture)
                glTexCoordPointer(2, GL_FLOAT, 0, c_texCoordBuffer);
            if (this->hasColor)
                glColorPointer(3, GL_FLOAT, 0, c_colorBuffer);
            
            glEnableClientState(GL_VERTEX_ARRAY);
            
            if (this->hasTexture) glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            if (this->hasColor)   glEnableClientState(GL_COLOR_ARRAY);

            glDrawArrays(GL_QUADS, 0, this->vertices);
            
            glDisableClientState(GL_VERTEX_ARRAY);
            
            if (this->hasTexture) glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            if (this->hasColor)   glDisableClientState(GL_COLOR_ARRAY);
            this->clear();
        }
    
    private:
        void clear() {
            this->vertices = 0;
            // this->c_vertexBuffer = {};
            // this->c_texCoordBuffer = {};
            // this->c_colorBuffer = {};
        }
        
    public:
        void init() {
            this->clear();
            this->hasColor = false;
            this->hasTexture = false;
        }
        
        void tex(float u, float v) {
            this->hasTexture = true;
            this->u = u; this->v = v;
        }

        void color(float r, float g, float b) {
            this->hasColor = true;
            this->r = r; this->g = g; this->b = b;
        }

        void vertex(float x, float y, float z) {
            c_vertexBuffer[this->vertices * 3 + 0] = x;
            c_vertexBuffer[this->vertices * 3 + 1] = y;
            c_vertexBuffer[this->vertices * 3 + 2] = z;

            if (this->hasTexture) {
                c_texCoordBuffer[this->vertices * 2 + 0] = this->u;
                c_texCoordBuffer[this->vertices * 2 + 1] = this->v;
            }
            if (this->hasColor) {
                c_colorBuffer[this->vertices * 3 + 0] = this->r;
                c_colorBuffer[this->vertices * 3 + 1] = this->g;
                c_colorBuffer[this->vertices * 3 + 2] = this->b;
            }
            this->vertices++;
            if (this->vertices == 100000) this->flush();
        }
};