# include "Drawer.h"

Drawer::Drawer(Texture somenTex){
    m_somenTex = somenTex;
}

void Drawer::DrawSomen(Array<Somen> somenArray){
    for(Somen& somen : somenArray){
        somen.Draw(m_somenTex);
    }
}