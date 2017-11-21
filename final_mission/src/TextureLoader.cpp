#include "TextureLoader.h"

TextureLoader::TextureLoader() {
	load();
}

void TextureLoader::load() {
	skyBox.LoadBMP("../res/Texturas/skybox.bmp");
	skyBox.BuildGLTexture();
	skyBox.ReleaseImage();

	avionVerde.LoadTGA("../res/Texturas/avion_verde.tga");
	avionVerde.BuildGLTexture();
	avionVerde.ReleaseImage();

	avionBlanco1.LoadTGA("../res/Texturas/avion_blanco1.tga");
	avionBlanco1.BuildGLTexture();
	avionBlanco1.ReleaseImage();

	avionBlanco2.LoadTGA("../res/Texturas/avion_blanco2.tga");
	avionBlanco2.BuildGLTexture();
	avionBlanco2.ReleaseImage();

	avionGris.LoadTGA("../res/Texturas/avion_gris.tga");
	avionGris.BuildGLTexture();
	avionGris.ReleaseImage();

	pasto1.LoadTGA("../res/Texturas/pasto1.tga");
	pasto1.BuildGLTexture();
	pasto1.ReleaseImage();

	pasto2.LoadTGA("../res/Texturas/pasto2.tga");
	pasto2.BuildGLTexture();
	pasto2.ReleaseImage();

	pasto3.LoadTGA("../res/Texturas/pasto3.tga");
	pasto3.BuildGLTexture();
	pasto3.ReleaseImage();

	tiles.LoadTGA("../res/Texturas/piso2.tga");
	tiles.BuildGLTexture();
	tiles.ReleaseImage();

	bathroom1.LoadTGA("../res/Texturas/tiles_bath1.tga");
	bathroom1.BuildGLTexture();
	bathroom1.ReleaseImage();

	bathroom2.LoadTGA("../res/Texturas/tiles_bath2.tga");
	bathroom2.BuildGLTexture();
	bathroom2.ReleaseImage();

	ladrillos1.LoadTGA("../res/Texturas/ladrillos1.tga");
	ladrillos1.BuildGLTexture();
	ladrillos1.ReleaseImage();

	ladrillos2.LoadTGA("../res/Texturas/ladrillos2.tga");
	ladrillos2.BuildGLTexture();
	ladrillos2.ReleaseImage();

	ladrillos3.LoadTGA("../res/Texturas/ladrillos3.tga");
	ladrillos3.BuildGLTexture();
	ladrillos3.ReleaseImage();

	ladrillos4.LoadTGA("../res/Texturas/ladrillos4.tga");
	ladrillos4.BuildGLTexture();
	ladrillos4.ReleaseImage();

	pinturaRoja.LoadTGA("../res/Texturas/pintura_roja.tga");
	pinturaRoja.BuildGLTexture();
	pinturaRoja.ReleaseImage();

	pinturaAmarilla.LoadTGA("../res/Texturas/pintura_amarilla.tga");
	pinturaAmarilla.BuildGLTexture();
	pinturaAmarilla.ReleaseImage();

	pinturaBlanca1.LoadTGA("../res/Texturas/pintura_blanca1.tga");
	pinturaBlanca1.BuildGLTexture();
	pinturaBlanca1.ReleaseImage();

	pinturaBlanca2.LoadTGA("../res/Texturas/pintura_blanca2.tga");
	pinturaBlanca2.BuildGLTexture();
	pinturaBlanca2.ReleaseImage();

	pinturaBlanca3.LoadTGA("../res/Texturas/pintura_blanca3.tga");
	pinturaBlanca3.BuildGLTexture();
	pinturaBlanca3.ReleaseImage();

	pinturaNegra.LoadTGA("../res/Texturas/pintura_negra.tga");
	pinturaNegra.BuildGLTexture();
	pinturaNegra.ReleaseImage();

	pinturaAzul.LoadTGA("../res/Texturas/pintura_azul.tga");
	pinturaAzul.BuildGLTexture();
	pinturaAzul.ReleaseImage();

	pinturaSucia.LoadTGA("../res/Texturas/pintura_sucia.tga");
	pinturaSucia.BuildGLTexture();
	pinturaSucia.ReleaseImage();

	pinturaSucia1.LoadTGA("../res/Texturas/sucio.tga");
	pinturaSucia1.BuildGLTexture();
	pinturaSucia1.ReleaseImage();

	sillon.LoadTGA("../res/Texturas/sillon.tga");
	sillon.BuildGLTexture();
	sillon.ReleaseImage();

	sillon1.LoadTGA("../res/Texturas/sillon1.tga");
	sillon1.BuildGLTexture();
	sillon1.ReleaseImage();

	almohada.LoadTGA("../res/Texturas/almohada.tga");
	almohada.BuildGLTexture();
	almohada.ReleaseImage();

	cobija.LoadTGA("../res/Texturas/cobija.tga");
	cobija.BuildGLTexture();
	cobija.ReleaseImage();

	noise.LoadTGA("../res/Texturas/noise.tga");
	noise.BuildGLTexture();
	noise.ReleaseImage();

	rotoplas.LoadTGA("../res/Texturas/rotoplas.tga");
	rotoplas.BuildGLTexture();
	rotoplas.ReleaseImage();

	madera.LoadTGA("../res/Texturas/madera.tga");
	madera.BuildGLTexture();
	madera.ReleaseImage();

	madera1.LoadTGA("../res/Texturas/madera1.tga");
	madera1.BuildGLTexture();
	madera1.ReleaseImage();

	/* Imagen para depurar las texturas*/
	cuadroDebug.LoadTGA("../res/Texturas/cuadro.tga");
	cuadroDebug.BuildGLTexture();
	cuadroDebug.ReleaseImage();
	createArrays();
}

void TextureLoader::createArrays() {
// Orden de los planos:
// 0 XZ Arriba
// 1 XZ Abajo
// 2 XY con Z positiva
// 3 XY con Z negativa
// 4 YZ con X positiva
// 5 YZ con X negativa

	techoA[0] = techoA[2] = techoA[3] = techoA[4] =  techoA[5] = pinturaBlanca3;
	techoA[1] = pinturaBlanca1;

	techoB[0] = techoB[2] = techoB[3] = techoB[4] =  techoB[5] = pinturaBlanca3;
	techoB[1] = pinturaBlanca1;

	techoC[0] = techoC[2] = techoC[3] = techoC[4] =  techoC[5] = pinturaBlanca3;
	techoC[1] = pinturaBlanca1;

	techoD[0] = techoD[2] = techoD[3] = techoD[4] =  techoD[5] = pinturaBlanca3;
	techoD[1] = pinturaBlanca1;

	techoE[0] = techoE[2] = techoE[3] = techoE[4] =  techoE[5] = pinturaBlanca3;
	techoE[1] = pinturaBlanca1;

	techoF[0] = techoF[2] = techoF[3] = techoF[4] =  techoF[5] = pinturaBlanca3;
	techoF[1] = pinturaBlanca1;

	techoG[0] = techoG[2] = techoG[3] = techoG[4] = techoG[5] =  pinturaBlanca3;
	techoG[1] = pinturaBlanca1;

	techoH[0] = techoH[2] = techoH[3] = techoH[4] = techoH[5] = pinturaBlanca3;
	techoH[1] = pinturaBlanca1;

	techoI[0] = techoI[2] = techoI[3] = techoI[4] = techoI[5] = pinturaBlanca3;
	techoI[1] = pinturaBlanca1;

	techoJ[0] = techoJ[2] = techoJ[3] = techoJ[4] =  techoJ[5] = pinturaBlanca3;
	techoJ[1] = pinturaBlanca1;

	bardaU[0] = bardaU[1] = ladrillos3;
	bardaU[2] = bardaU[3] = bardaU[4] = pinturaRoja;
	bardaU[5] = pinturaAmarilla;

	bardaT[0] = bardaT[1] = bardaT[4] = bardaT[3] = bardaT[5] = ladrillos3;
	bardaT[2] = pinturaAmarilla;

	bardaS[0] = bardaS[1] = bardaS[4] = bardaS[3] = bardaS[5] = ladrillos3;
	bardaS[2] = bathroom2;

	bardaR[0] = bardaR[1] = bardaR[4] = bardaR[3] = bardaR[5] = ladrillos3;
	bardaR[2] = pinturaSucia;

	bardaQ[0] = bardaQ[1] = bardaQ[3] = ladrillos3;
	bardaQ[2] = bardaQ[4] = pinturaAmarilla;
	bardaQ[5] = bathroom2;

	bardaP[0] = bardaP[1] = bardaP[3] = ladrillos3;
	bardaP[2] = pinturaRoja;
	bardaP[4] = bathroom2;
	bardaP[5] = pinturaSucia;

	bardaO[0] = bardaO[1] = bardaO[2] = bardaO[3] = bardaO[4] = ladrillos3;
	bardaO[5] = pinturaAmarilla;

	bardaNE[0] = bardaNE[1] = bardaNE[2] = bardaNE[3] = bardaNE[5] = ladrillos3;
	bardaNE[4] = pinturaSucia;

	bardaN[0] = bardaN[1] = bardaN[3] = bardaN[4] = bardaN[5] = ladrillos3;
	bardaN[2] = pinturaRoja;

	bardaM[0] = bardaM[1] = bardaM[4] = bardaM[3] = bardaM[5] = ladrillos3;
	bardaM[2] = pinturaRoja;

	bardaL[0] = bardaL[1] = bardaL[2] = bardaL[4] = bardaL[5] = pinturaRoja;
	bardaL[3] = bathroom2;

	bardaK[0] = bardaK[1] = ladrillos3;
	bardaK[2] = bardaK[4] = bardaK[5] = pinturaRoja;
	bardaK[3] = pinturaAmarilla;

	bardaJ[0] = bardaJ[1] = bardaJ[4] = bardaJ[5] =  pinturaRoja;
	bardaJ[3] = pinturaSucia;
	bardaJ[2] = pinturaAzul;

	bardaI[0] = bardaI[1] = bardaI[2] = bardaI[3] = bardaI[4] = bardaI[5] = pinturaRoja;

	bardaH[0] = bardaH[1] = bardaH[2] = bardaH[3] = bardaH[4] = ladrillos3;
	bardaH[5] = pinturaRoja;

	bardaG[0] = bardaG[1] = bardaG[2] = bardaG[3] = bardaG[5] = ladrillos3;
	bardaG[4] = pinturaAzul;

	bardaF[0] = bardaF[1] = bardaF[2] = ladrillos3;
	bardaF[4] = bardaF[3] = pinturaRoja;
	bardaF[5] = pinturaAzul;

	bardaE[0] = bardaE[1] = bardaE[2] = bardaE[4] = bardaE[5] = ladrillos3;
	bardaE[3] = pinturaRoja;

	bardaA[0] = bardaA[1] = bardaA[2] = bardaA[4] = bardaA[5] = ladrillos3;
	bardaA[3] = bathroom1;

	bardaB[0] = bardaB[1] = bardaB[2] = bardaB[3] = bardaB[5] = ladrillos3;
	bardaB[4] = bathroom1;

	bardaC[0] = bardaC[1] = bardaC[2] = bardaC[3] = bardaC[4] = ladrillos3;
	bardaC[5] = bathroom1;

	bardaD[0] = bardaD[1] = bardaD[5] = ladrillos3;
	bardaD[2] = bathroom1;
	bardaD[4] = pinturaRoja;
	bardaD[3] = pinturaAzul;
}
