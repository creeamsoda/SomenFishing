#pragma once
#define DRAWER_H
#include <Siv3D.hpp>
#include "Somen.h"

class Drawer
{
private:
    /* data */
    Texture m_somenTex;
public:
    Drawer(Texture somenTex);
    void DrawSomen(Array<Somen> somenArray);
};


