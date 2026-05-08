#pragma once

class AABB {
    private:
        float epsilon = 0.0f;
    public:
        float x0, y0, z0;
        float x1, y1, z1;

        AABB() {
            this->x0 = 0;
            this->y0 = 0;
            this->z0 = 0;
            this->x1 = 1;
            this->y1 = 1;
            this->z1 = 1;
        }

        AABB(float x0, float y0, float z0, float x1, float y1, float z1) {
            this->x0 = x0;
            this->y0 = y0;
            this->z0 = z0;
            this->x1 = x1;
            this->y1 = y1;
            this->z1 = z1;
        }

        AABB expand(float xa, float ya, float za) {
            float _x0 = this->x0;
            float _y0 = this->y0;
            float _z0 = this->z0;
            float _x1 = this->x1;
            float _y1 = this->y1;
            float _z1 = this->z1;
            if (xa < 0.0f) _x0 += xa;
            if (xa > 0.0f) _x1 += xa;

            if (ya < 0.0f) _y0 += ya;
            if (ya > 0.0f) _y1 += ya;

            if (za < 0.0f) _z0 += za;
            if (za > 0.0f) _z1 += za;
            
            return AABB(_x0, _y0, _z0, _x1, _y1, _z1);
        }

        AABB grow(float xa, float ya, float za) {
            float _x0 = this->x0 - xa;
            float _y0 = this->y0 - ya;
            float _z0 = this->z0 - za;
            float _x1 = this->x1 + xa;
            float _y1 = this->y1 + ya;
            float _z1 = this->z1 + za;
            return AABB(_x0, _y0, _z0, _x1, _y1, _z1);
        }

        float clipXCollide(const AABB& c, float xa) {
            float max;
            if (c.y1 <= this->y0 || c.y0 >= this->y1) return xa;
            if (c.z1 <= this->z0 || c.z0 >= this->z1) return xa;
            if ((xa > 0.0f && c.x1 <= this->x0 && (max = this->x0 - c.x1 - this->epsilon) < xa)
             || (xa < 0.0f && c.x0 >= this->x1 && (max = this->x1 - c.x0 + this->epsilon) > xa))
                xa = max;
            return xa;
        }

        float clipYCollide(const AABB& c, float ya) {
            float max;
            if (c.x1 <= this->x0 || c.x0 >= this->x1) return ya;
            if (c.z1 <= this->z0 || c.z0 >= this->z1) return ya;
            if (ya > 0.0f && c.y1 <= this->y0 && (max = this->y0 - c.y1 - this->epsilon) < ya)
                ya = max;
            if (ya < 0.0f && c.y0 >= this->y1 && (max = this->y1 - c.y0 + this->epsilon) > ya)
                ya = max;
            return ya;
        }

        float clipZCollide(const AABB& c, float za) {
            float max;
            if (c.x1 <= this->x0 || c.x0 >= this->x1) return za;
            if (c.y1 <= this->y0 || c.y0 >= this->y1) return za;
            if ((za > 0.0f && c.z1 <= this->z0 && (max = this->z0 - c.z1 - this->epsilon) < za)
             || (za < 0.0f && c.z0 >= this->z1 && (max = this->z1 - c.z0 + this->epsilon) > za))
                za = max;
            return za;
        }

        bool intersects(const AABB& c) {
            if (c.x1 <= this->x0 || c.x0 >= this->x1) return false;
            if (c.y1 <= this->y0 || c.y0 >= this->y1) return false;
            if (c.z1 <= this->z0 || c.z0 >= this->z1) return false;
            return true;
        }

        void move(float xa, float ya, float za) {
            this->x0 += xa;
            this->y0 += ya;
            this->z0 += za;
            this->x1 += xa;
            this->y1 += ya;
            this->z1 += za;
        }
};