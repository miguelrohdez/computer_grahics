#ifndef FINAL_PROJECT_ELEMENTS_H
#define FINAL_PROJECT_ELEMENTS_H

#include "Texture.h"

/*
 * Declaración de prototipos de funciones que dibujan los objetos
 */

void dibujaEjes();
void dibujarTerreno();
void dibujarPiso();
void dibujarMesa();
/* Función que dibuja un sillon */
void dibujaSillon(float scale, CTexture sillonT);
/* Función que dibuja una cama*/
void dibujaCama(float scale, CTexture cobijaT);
void dibujaMuebleTv(float scale=1.0f);
void dibujaTv(float scale, CTexture screeT);
void dibujaTapete(float scale=1.0f);
void dibujaCarretera();
void dibujaGarage();
void dibujarCasa(float heightWall, float scale, float textureRep=0.8f);
void dibujarSkyBox(float animax);
/* Función para depurar los planos */
void dibujarPlano();
void dibujaLavabo(float scale);
void testCylinder();
void testCone();
void dibujarRotoplas(float scale);
void dibujaTocador(float scale=1.0f);
void dibujaAlberca(float scale=1.0f);
void dibujarRetrete();
void dibujaSilla(float scale);
void dibujarMesaCristal(float scale);
void dibujarEstante(float scale);
void dibujarEstufa(float scale);
void dibujarRefrigerador(float scale);

#endif
