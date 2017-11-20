/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "Prisma.h"
#include "Camera.h"
#include "Plane.h"
#include "TextureLoader.h"
#include "Cylinder.h"
#include "Cone.h"
#include "AnimationPlane.h"

CCamera camara;
GLfloat g_lookupdown = 0.0f; // Posición en el eje Z

float rotacionX = 0.0f;
float rotacionY = 0.0f;
float rotacionZ = 0.0f;

/*
 * KeyFrames
 */
AnimationPlane plane;

/*
 * Para usar texturas
 */
TextureLoader textures;

/*
 * Colores
 */
Color saddleBrown = Color(0.545f, 0.271f, 0.075f);
Color sienna = Color(0.627f, 0.322f, 0.176f);
Color cafeMesa = Color(0.09f, 0.06f, 0.05f);

/*
 * Función para inicializar parámetros
 */
void InitGL() {
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);                // Fondo negro
	glClearDepth(1.0f);                                    // Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);                            // Activa Depth Testing
	glDepthFunc(GL_LEQUAL);                                // Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Correccion de cálculos de perspectiva
	camara.Position_Camera(100, 200, 100, 0, 200, 0, 0, 1, 0);
	textures.load();
	plane.loadData();
	plane.setTextures(textures.avionGris, textures.avionVerde);
}


void dibujaEjes() {
	glBegin(GL_LINES);
		//Eje X
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-1000.0f, 0.0f, 0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		//Eje Y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, -1000.0f, 0.0f);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		//Eje Z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, -1000.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	glPointSize(10.0f);
	glBegin(GL_POINTS);
		//"Flecha" eje X
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);
		//"Flecha" eje Y
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1000.0f, 0.0f);
		//"Flecha" eje Z
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	glPointSize(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
}

void dibujarTerreno() {
	Color c = Color(1.0f, 1.0f, 1.0f);
	Prisma p = Prisma(2000, 5, 2000, c);
	p.setRepetitionTexture(0.5);
	p.setTexture(textures.pasto2);
	p.draw();
}

void dibujarPiso() {
	Color c = Color(1.0f, 1.0f, 1.0f);
	Prisma p = Prisma(280, 1, 280, c);
	p.setTexture(textures.tiles);
	p.draw();
}

void dibujarMesa() {
	Prisma m = Prisma(8, 0.5, 3, cafeMesa);

	m.setScale(2.0f);
	m.draw();
	glTranslatef(0.0f, 0.75, 0.0f);
	m.draw(8.5, 0.25, 3.5, sienna);
	glTranslatef(0.0f, 7.25f, 0.0f);
	m.draw(7.0f, 7.0f, 3.0f, cafeMesa);
	glTranslatef(0.0f, 7.2f, 0.0f);
	m.draw(8.5, 0.2, 3.5, sienna);
	glTranslatef(0.0f, 0.4f, 0.6f);
	m.draw(15, 0.2, 10, cafeMesa);
}

/* Función que dibuja un sillon */
void dibujaSillon(float scale, CTexture sillonT=textures.sillon){

	// Base
	Prisma s = Prisma(20*scale, 3.5*scale, 10*scale, cafeMesa);
	s.draw(sillonT);

	// Recarga brazos
	glPushMatrix();
		glTranslatef(8.5*scale, 2.8*scale, 0);
		s.draw(2*scale, 2*scale, 10*scale);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-8.5*scale, 2.8*scale, 0);
		s.draw(2*scale, 2*scale, 10*scale);
	glPopMatrix();

	// Respaldo
	glPushMatrix();
		glTranslatef(0*scale, 3*scale, 4.3*scale);
		s.draw(15*scale, 3*scale, 1.5*scale);
	glPopMatrix();
}

/* Función que dibuja una cama*/
void dibujaCama(float scale){

	Prisma cam = Prisma(20*scale, 2*scale, 23*scale, cafeMesa);

	cam.draw(textures.cobija);

	//Almohadas
	glPushMatrix();
		glTranslatef(5*scale, 1*scale, 9*scale);
		cam.draw(8*scale, 0.7f*scale, 4*scale, textures.almohada);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5*scale, 1*scale, 9*scale);
		cam.draw(8*scale, 0.7f*scale, 4*scale, textures.almohada);
	glPopMatrix();

	//Cabecera
	glPushMatrix();
		glTranslatef(0*scale, 0.5*scale, 11.9*scale);
		cam.draw(20*scale, 7*scale, 0.7f*scale, textures.madera1);
	glPopMatrix();

	//Base
	glPushMatrix();
		glTranslatef(0*scale, -2*scale, -0.1*scale);
		cam.draw(21.2*scale, 2*scale, 23.2*scale, textures.madera);
	glPopMatrix();
}


void dibujaMuebleTv(float scale=1.0f){
	Prisma mtv = Prisma(20*scale, 15*scale, 7*scale, cafeMesa);
	mtv.draw(textures.madera1);
}

void dibujaTv(float scale=1.0f){
	// Base
	Prisma tv = Prisma(5.5*scale, 0.2*scale, 0.3*scale, cafeMesa);
	tv.draw(textures.pinturaNegra);

	// Base (cilindro)
	glPushMatrix();
		glTranslatef(0*scale, 0.05*scale, 0*scale);
		Cylinder b(0.3*scale, 1.5*scale, textures.pinturaNegra);
		b.draw();
	glPopMatrix();

	// Marco
	glPushMatrix();
		glTranslatef(0*scale, 5*scale, 0*scale);
		tv.draw(15*scale, 7*scale, 0.5*scale);

		// Pantalla
		glPushMatrix();
			glTranslatef(0*scale, 0*scale, -0.2*scale);
			tv.setRepetitionTexture(0.7f);
			tv.draw(13*scale, 6*scale, 0.2*scale, textures.noise);
		glPopMatrix();
	glPopMatrix();


}


void dibujarCasa(float heightWall, float scale, float textureRep=0.8f) {
	float anchoBarda = 1.1f;
	Color n = Color(1.0f, 1.0f, 1.0f);
	Prisma c = Prisma(30, heightWall, anchoBarda, n);

	c.setRepetitionTexture(textureRep);
	c.setTexture(textures.bardaA);
	c.setScale(scale);
	glPushMatrix(); // Bardas
		glTranslatef(-50 * scale, 0, 50 * scale); // A
		c.draw();
		c.setTexture(textures.bardaA);

		glTranslatef(-15 * scale, 0, -10 * scale); // B
		c.draw(anchoBarda, heightWall, 20, textures.bardaB);

		glTranslatef(30 * scale, 0, 0); // C
		c.draw(anchoBarda, heightWall, 20, textures.bardaC);

		glTranslatef(-20 * scale, 0, -10 * scale); // D
		c.setRepetitionTexture(0.4f);
		c.draw(20, heightWall, anchoBarda, textures.bardaD);
		c.setRepetitionTexture(textureRep);

		glTranslatef(75 * scale, 0, 0); // E
		c.draw(90, heightWall, anchoBarda, textures.bardaE);

		glTranslatef(-55 * scale, 0, -15 * scale); // F
		c.setRepetitionTexture(0.3f);
		c.draw(anchoBarda, heightWall, 30, textures.bardaF);

		glTranslatef(-30 * scale, 0, -5 * scale); // G
		c.setRepetitionTexture(0.25f);
		c.draw(anchoBarda, heightWall, 40, textures.bardaG);
		c.setRepetitionTexture(textureRep);

		glTranslatef(130 * scale, 0, -5 * scale); // H
		c.draw(anchoBarda, heightWall, 50, textures.bardaH);

		glTranslatef(-85 * scale, 0, -5 * scale); // I
		c.draw(30, heightWall, anchoBarda, textures.bardaI);

		glTranslatef(-30 * scale, 0, -10 * scale); // J
		c.setRepetitionTexture(0.3f);
		c.draw(30, heightWall, anchoBarda, textures.bardaJ);
		c.setRepetitionTexture(textureRep);

		glTranslatef(55 * scale, 0, 0); // K
		c.draw(40, heightWall, anchoBarda, textures.bardaK);

		glTranslatef(-25 * scale, 0, -10 * scale); // L
		c.draw(10, heightWall, anchoBarda, textures.bardaL);

		glTranslatef(52.5 * scale, 0, 0); // M
		c.draw(15, heightWall, anchoBarda, textures.bardaM);

		glTranslatef(25 * scale, 0, 0); // N
		c.draw(15, heightWall, anchoBarda, textures.bardaN);

		glTranslatef(-122.5 * scale, 0, -10 * scale); // Ñ
		c.setRepetitionTexture(0.3f);
		c.draw(anchoBarda, heightWall, 40, textures.bardaNE);
		c.setRepetitionTexture(textureRep);

		glTranslatef(90 * scale, 0, -5 * scale); // O
		c.draw(anchoBarda, heightWall, 30, textures.bardaO);

		glTranslatef(-60 * scale, 0, 0); // P
		c.setRepetitionTexture(0.3f);
		c.draw(anchoBarda, heightWall, 30, textures.bardaP);
		c.setRepetitionTexture(textureRep);

		glTranslatef(20 * scale, 0, 0); // Q
		c.draw(anchoBarda, heightWall, 30, textures.bardaQ);

		glTranslatef(-35 * scale, 0, -15 * scale); // R
		c.setRepetitionTexture(0.3f);
		c.draw(30, heightWall, anchoBarda, textures.bardaR);
		c.setRepetitionTexture(textureRep);

		glTranslatef(25 * scale, 0, 0); // S
		c.draw(20, heightWall, anchoBarda, textures.bardaS);

		glTranslatef(30 * scale, 0, 0); // T
		c.draw(40, heightWall, anchoBarda, textures.bardaT);

		glTranslatef(20 * scale, 0, 35 * scale); // U
		c.draw(anchoBarda, heightWall, 10, textures.bardaU);
	glPopMatrix();

	//Secciones de Techo
	heightWall += 0.55; // Se suma ancho del techo
	glPushMatrix();
		glTranslatef(-50 * scale, heightWall, 40 * scale); // AT
		c.draw(30, anchoBarda, 20, textures.techoA);
		glTranslatef(30 * scale, 0, -25 * scale); // BT
		c.draw(30, anchoBarda, 30, textures.techoB);
		glTranslatef(-30 * scale, 0, -5 * scale); // CT
		c.draw(30, anchoBarda, 40, textures.techoC);
		glTranslatef(60 * scale, 0, 0); // DT
		c.draw(30, anchoBarda, 40, textures.techoD);
		glTranslatef(35 * scale, 0, -5 * scale); // ET
		c.draw(40, anchoBarda, 50, textures.techoE);
		glTranslatef(-55 * scale, 0, -10 * scale); // FT
		c.draw(10, anchoBarda, 10, textures.techoF);
		glTranslatef(-15 * scale, 0, -5 * scale); // GT
		c.draw(20, anchoBarda, 20, textures.techoG);
		glTranslatef(-25 * scale, 0, -20 * scale); // HT
		c.draw(30, anchoBarda, 40, textures.techoH);
		glTranslatef(55 * scale, 0, 0); // IT
		c.draw(40, anchoBarda, 40, textures.techoI);
		glTranslatef(-30 * scale, 0, -5 * scale); // JT
		c.draw(20, anchoBarda, 30, textures.techoJ);
	glPopMatrix();
}

void dibujarSkyBox() {
	Prisma p = Prisma(2000, 1500, 2000, Color(1.0f, 1.0f, 1.0f));

	p.setTexture(textures.skyBox);
	p.drawSky();
}

/* Función para depurar los planos */
void dibujarPlano() {
	Vertex v1 = Vertex(5,0,0);
	Vertex v2 = Vertex(0,0,0);
	Vertex v3 = Vertex(0,10,0);
	Vertex v4 = Vertex(5,10 ,0);
	Plane p = Plane(v1, v2, v3, v4, textures.cuadroDebug);

	p.draw();
}


void dibujaAvion(){

}

void testCylinder() {
	Cylinder c(10, 4, 10, textures.cuadroDebug);
	c.draw();
}

void testCone() {
	Cone c(10, 10, textures.cuadroDebug);
	c.draw();
}

void dibujarRotoplas(float scale) {
	Cylinder c(4.5*scale, 7*scale, textures.rotoplas);
	c.draw();
	glTranslatef(0*scale, 7*scale, 0*scale);
	c.draw(4.5*scale, 2.5*scale, 2*scale, textures.pinturaNegra);
}

void dibujaTocador(float scale=1.0f){
	Prisma t = Prisma(45*scale, 15*scale, 8*scale, cafeMesa);
	t.draw(textures.madera1);
}

/*
 * Función que dibuja
 */
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
	glLoadIdentity();
	glTranslatef(rotacionX, rotacionY, rotacionZ);
	glRotatef(g_lookupdown,1.0f,0,0);
	gluLookAt(camara.mPos.x, camara.mPos.y, camara.mPos.z,
			  camara.mView.x, camara.mView.y, camara.mView.z,
			  camara.mUp.x, camara.mUp.y, camara.mUp.z);

	dibujaEjes();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Skybox
		glTranslatef(0,95,0);
		dibujarSkyBox();
	glPopMatrix();

	glPushMatrix(); // Pasto
		dibujarTerreno();
	glPopMatrix();

	glTranslatef(0, 2.5, 0);
	glPushMatrix(); // Piso casa
		dibujarPiso();
	glPopMatrix();

	glTranslatef(0, 0.5, 0);
	glPushMatrix(); // Paredes
		glTranslatef(0, 25, 0);
		dibujarCasa(25.0, 2.0);
	glPopMatrix();

	glTranslatef(10, 0, 10);
	glPushMatrix();
		dibujarMesa();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(95, 2, -10);
		glRotatef(180, 0, 1, 0);
		dibujaSillon(2.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(63, 2, 20);
		glRotatef(-90, 0, 1, 0);
		dibujaSillon(2.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(107, 2, 40);
		glRotatef(40, 0, 1, 0);
		dibujaMuebleTv(0.7f);
		glPushMatrix();
			glTranslatef(0, 5.5, 0);
			dibujaTv(1.5f);
		glPopMatrix();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(-50, 50, -70);
		dibujarRotoplas(3.0f);
	glPopMatrix();


	glPushMatrix();
		glTranslatef(10, 5, -84);
		glRotatef(180, 0, 1, 0);
		dibujaCama(2.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-35, 6, -80);
		glRotatef(90, 0, 1, 0);
		dibujaTocador();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10, 6, -33.5);
		dibujaMuebleTv();
		glPushMatrix();
			glTranslatef(0, 7.5, 0);
			dibujaTv(2.0f);
		glPopMatrix();
	glPopMatrix();
	glTranslatef(20, 20, -20);
	plane.draw();
	glDisable(GL_TEXTURE_2D);
	glFlush(); //TODO: Entender que hace
	glutSwapBuffers();
}

void animation() {
	plane.update();
	glutPostRedisplay();
}
/*
* Funcion para el reajuste de dibujo en ventana
*/
void reshape(int width, int height) {
	float relation;
	height = (height == 0) ? 1 : height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION); // Seleccionamos Projection Matrix
	glLoadIdentity();
	relation = (float)width / (float)height;
	gluPerspective(20*relation, relation, 1.0f, 10000.0f);
	glMatrixMode(GL_MODELVIEW); // Seleccionamos Modelview Matrix
	glLoadIdentity();
}


/*
* Función que maneja el teclado
*/
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'w':   //Movimientos de camara
		case 'W':
			camara.Move_Camera( CAMERASPEED );
			break;
		case 's':
		case 'S':
			camara.Move_Camera(-(CAMERASPEED));
			break;
		case 'a':
		case 'A':
			camara.Strafe_Camera(-(CAMERASPEED));
			break;
		case 'd':
		case 'D':
			camara.Strafe_Camera( CAMERASPEED);
			break;
		case 'e':
		case 'E':
			camara.UpDown_Camera(CAMERASPEED);
			break;
		case 'q':
		case 'Q':
			camara.UpDown_Camera(-CAMERASPEED);
			break;
		/* Temporal para keyframes */
		case 'z':
			plane.left();
			break;
		case 'Z':
			plane.right();
			break;
		case 'x':
			plane.fordward();
			break;
		case 'X':
			plane.backward();
			break;
		case 'c':
			plane.up();
			break;
		case 'C':
			plane.down();
			break;
		case 'v':
			plane.rotateXPositive();
			break;
		case 'V':
			plane.rotateXNegative();
			break;
		case 'M':
		case 'm':
			plane.saveToFile();
			break;
		case 'n':
		case 'N':
			plane.saveKeyframe();
			break;
		case 'b':
			plane.setActivate(true);
			break;
		case 'B':
			plane.setActivate(false);
			break;
		case 27:        // Cuando Esc es presionado...
			exit(0);   // Salimos del programa
			break;
		default:        // Cualquier otra
			break;
	}
	glutPostRedisplay();
}

/*
* Función para el manejo de teclas especiales
*/
void arrow_keys(int a_keys, int x, int y) {
	switch ( a_keys ) {
		case GLUT_KEY_UP:     // Presionamos tecla ARRIBA...
			g_lookupdown -= 1.0f;
			break;
		case GLUT_KEY_DOWN:               // Presionamos tecla ABAJO...
			g_lookupdown += 1.0f;
			break;
		case GLUT_KEY_LEFT:
			camara.Rotate_View(-CAMERASPEED);
			break;
		case GLUT_KEY_RIGHT:
			camara.Rotate_View( CAMERASPEED);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

/*
* Función principal
*/
int main(int argc, char **argv) {
	glutInit(&argc, argv); // Inicializamos OpenGL
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); // Display Mode (Clores RGB y alpha | Buffer Sencillo )
	glutInitWindowSize(1920, 1080); // Tamaño de la Ventana
	glutInitWindowPosition(-1, -1); //Posicion de la Ventana
	glutCreateWindow("Complejo Residencial"); // Nombre de la Ventana
	InitGL(); // Parametros iniciales de la aplicacion
	glutDisplayFunc(display); // Función de dibujo
	glutReshapeFunc(reshape); // Función en caso de cambio de tamano
	glutKeyboardFunc(keyboard); // Función de manejo de teclado
	glutSpecialFunc(arrow_keys); // Función manejo teclas especiales
	glutIdleFunc(animation);
	glutMainLoop();

	return 0;
}
