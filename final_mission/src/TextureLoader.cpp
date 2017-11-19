#include "TextureLoader.h"

TextureLoader::TextureLoader() {
	load();
}

void TextureLoader::load() {
	skyBox.LoadBMP("Texturas/skybox.bmp");
	skyBox.BuildGLTexture();
	skyBox.ReleaseImage();

	pasto1.LoadTGA("Texturas/pasto1.tga");
	pasto.BuildGLTexture();
	pasto.ReleaseImage();

	pasto2.LoadTGA("Texturas/pasto2.tga");
	pasto.BuildGLTexture();
	pasto.ReleaseImage();

	pasto3.LoadTGA("Texturas/pasto3.tga");
	pasto.BuildGLTexture();
	pasto.ReleaseImage();

	tiles.LoadTGA("Texturas/piso2.tga");
	tiles.BuildGLTexture();
	tiles.ReleaseImage();

	fachadaExterior.LoadTGA("Texturas/ladrillos2.tga");
	fachadaExterior.BuildGLTexture();
	fachadaExterior.ReleaseImage();

	fachadaInterior.LoadTGA("Texturas/pintura_roja.tga");
	fachadaInterior.BuildGLTexture();
	fachadaInterior.ReleaseImage();

	/* Imagen para depurar las texturas*/
	cuadroDebug.LoadTGA("Texturas/cuadro.tga");
	cuadroDebug.BuildGLTexture();
	cuadroDebug.ReleaseImage();
	createArrays();
}

void TextureLoader::createArrays() {
	bardaA[0] = bardaA[1] = fachadaInterior;
	bardaA[2] = fachadaInterior;
	bardaA[3] = fachadaExterior;
	bardaA[4] = bardaA[5] = fachadaExterior;
}
