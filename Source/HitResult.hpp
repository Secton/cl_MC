#pragma once

class HitResult {
    public:
        int x, y, z, o, f;
        HitResult(int x, int y, int z, int o, int f) {
            this->x = x; this->y = y; this->z = z;
            this->o = o; this->f = f;
        }
};