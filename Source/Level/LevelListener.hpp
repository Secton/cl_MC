#pragma once

struct LevelListener {
    virtual void tileChanged(int var1, int var2, int var3) = 0;
    virtual void lightColumnChanged(int var1, int var2, int var3, int var4) = 0;
    virtual void allChanged() = 0;

    virtual ~LevelListener() {};
};