#ifndef FINAL_PROJECT_TEXTURE_LOADER_H
#define FINAL_PROJECT_TEXTURE_LOADER_H

#include "Texture.h"

class TextureLoader {
public:
	CTexture bathroom1;
	CTexture bathroom2;
	CTexture pasto1;
	CTexture pasto2;
	CTexture pasto3;
	CTexture tiles;
	CTexture ladrillos1;
	CTexture ladrillos2;
	CTexture ladrillos3;
	CTexture ladrillos4;
	CTexture pinturaRoja;
	CTexture pinturaBlanca1;
	CTexture pinturaBlanca2;
	CTexture pinturaBlanca3;
	CTexture pinturaAmarilla;
	CTexture skyBox;
	CTexture cuadroDebug;
	CTexture bardaA[6];
	CTexture bardaB[6];
	CTexture bardaC[6];
	CTexture bardaD[6];
	CTexture bardaE[6];
	CTexture bardaF[6];
	CTexture bardaG[6];
	CTexture bardaH[6];
	CTexture bardaI[6];
	CTexture bardaJ[6];
	CTexture bardaK[6];
	CTexture bardaL[6];
	CTexture bardaM[6];
	CTexture bardaN[6];
	CTexture bardaNE[6];
	CTexture bardaO[6];
	CTexture bardaP[6];
	CTexture bardaQ[6];
	CTexture bardaR[6];
	CTexture bardaS[6];
	CTexture bardaT[6];
	CTexture bardaU[6];
	TextureLoader();
	void load();
	void createArrays();
	CTexture* createTexture(char *name);
};

#endif
