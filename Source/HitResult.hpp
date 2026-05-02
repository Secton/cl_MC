#pragma once

class HitResult {
    public:
        int x, y, z, o, f;
        bool nulled = false;
        HitResult() {
            this->x = 0; this->y = 0; this->z = 0;
            this->o = 0; this->f = 0;
            this->nulled = true;
        }
        HitResult(int x, int y, int z, int o, int f) {
            this->x = x; this->y = y; this->z = z;
            this->o = o; this->f = f;
        }
};