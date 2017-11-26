/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
//#include <AL/al.h>
//#include <AL/alc.h>
#include "Camera.h"
#include "TextureLoader.h"
#include "AnimationPlane.h"
#include "AnimationReloj.h"
#include "Elements.h"

CCamera camara;
GLfloat g_lookupdown = 0.0f; // Posición en el eje Z

float animax = 0.0f;
bool sentido = true;

/*
 * KeyFrames
 */
AnimationReloj reloj;
AnimationPlane avion;

/*
 * Para usar texturas
 */
TextureLoader textures;

/*
 * Función que carga y da valores a las animaciones
 */
void loadKeyFrames() {
	reloj.loadData();
	avion.loadData();

	reloj.setTextures(textures.reloj, textures.pinturaNegra);
	avion.setTextures(textures.avionGris, textures.avionVerde);
	reloj.setActivate(true);
}

/*
 * Función para inicializar parámetros
 */
void InitGL() {
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);                // Fondo negro
	glClearDepth(1.0f);                                    // Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);                            // Activa Depth Testing
	glDepthFunc(GL_LEQUAL);                                // Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Correccion de cálculos de perspectiva
	camara.Position_Camera(50, 50, 10, 0, 50, 0, 0, 1, 0);
	textures.load();
	loadKeyFrames();
}

/*
 * Función que dibuja
 */
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
	glLoadIdentity();
	glRotatef(g_lookupdown,1.0f,0,0);
	gluLookAt(camara.mPos.x, camara.mPos.y, camara.mPos.z,
			  camara.mView.x, camara.mView.y, camara.mView.z,
			  camara.mUp.x, camara.mUp.y, camara.mUp.z);

	dibujaEjes();
	glEnable(GL_TEXTURE_2D);
	glPushMatrix(); // Skybox
		glTranslatef(0,450,0);
		dibujarSkyBox(animax);
	glPopMatrix();

	glPushMatrix(); // Pasto
		dibujarTerreno();
	glPopMatrix();

	glTranslatef(0, 2.5, 0); // Eleva a la altura del pasto
	glPushMatrix(); // Piso casa
		dibujarPiso();
	glPopMatrix();

	glTranslatef(0, 0.5, 0); //Eleva a la altura del piso
	glPushMatrix(); // Paredes
		glTranslatef(0, 50, 0);
		dibujarCasa(25.0, 4.0);
	glPopMatrix();

	glPushMatrix(); // Mesa madera
		glTranslatef(0, 0, 60);
		dibujarMesa();
	glPopMatrix();

	// Sección de la sala
	glPushMatrix();
		glTranslatef(200, 5.25, 15);
		glRotatef(180, 0, 1, 0);
		dibujaSillon(3.0f, textures.sillon1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(150, 5.25, 60);
		glRotatef(-90, 0, 1, 0);
		dibujaSillon(3.0f, textures.sillon1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(240, 10, 100);
		glRotatef(40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();

	glPushMatrix(); // Tapete
		glTranslatef(200, 2, 70);
		dibujaTapete();
	glPopMatrix();


	// Sección del techo
	glPushMatrix();
		glTranslatef(-50, 100, -70);
		dibujarRotoplas(4.0f);
	glPopMatrix();


	// Sección de recamara amarilla
	glPushMatrix();
		glTranslatef(35, 12, -150);
		glRotatef(180, 0, 1, 0);
		dibujaCama(4.0f);
	glPopMatrix();

	glPushMatrix(); // Tocador
		glTranslatef(-48, 15, -140);
		glRotatef(90, 0, 1, 0);
		dibujaTocador(2.0f);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(35, 15, -50);
		dibujaMuebleTv(2.0f);
		glPushMatrix();
			glTranslatef(0, 15, 0);
			dibujaTv(4.0f);
		glPopMatrix();
	glPopMatrix();

	// Alberca
	glPushMatrix();
		glTranslatef(-100, 0, 550);
		glRotatef(90, 0, 1, 0);
		dibujaAlberca(1.5f);
	glPopMatrix();

	// Carretera
	glPushMatrix();
		glTranslatef(0, 0, -700);
		dibujaCarretera();
	glPopMatrix();

	// Garage
	glPushMatrix();
		glTranslatef(100, 0, 236);
		dibujaGarage();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(90, 70, -37.75);
		reloj.draw();
	glPopMatrix();
	glPushMatrix(); // Retretes
		glTranslatef(-100, 0, -170);
		glRotatef(180, 0, 1, 0);
		dibujarRetrete();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-230, 0, 160);
		glRotatef(90, 0, -1, 0);
		dibujarRetrete();
	glPopMatrix();
	glPushMatrix(); // Lavabos
		glTranslatef(-130, 45, -140);
		dibujaLavabo(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-200, 40, 190);
		glRotatef(90, 0, 1, 0);
		dibujaLavabo(4);
	glPopMatrix();
	glPushMatrix(); // Sillas
		glTranslatef(-25, 10, 80);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(5, 10, 80);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(25, 10, 40);
		glRotatef(180, 0, 1, 0);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-5, 10, 40);
		glRotatef(180, 0, 1, 0);
		dibujaSilla(4.0f);
	glPopMatrix();
	glPushMatrix(); // Cuarto Azul
		glTranslatef(-220, 10, 75);
		dibujaCama(3.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-240, 10, -60);
		glRotatef(-40, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix(); // Cuarto
		glTranslatef(-180, 10, -160);
		glRotatef(180, 0, 1, 0);
		dibujaCama(3.0f);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-205, 10, -30);
		glRotatef(180, 0, 1, 0);
		dibujaMuebleTv(1.4f);
		glPushMatrix();
			glTranslatef(0, 10.5, 0);
			dibujaTv(3.0f);
		glPopMatrix();
	glPopMatrix();
	glPushMatrix();
		avion.draw();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(-250, 0.1, -188);
		glRotatef(90, 0, 1, 0);
		dibujarEstante(4.0f);
	glPopMatrix();
	glEnable(GL_BLEND); // Figuras con opacidad
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glTranslatef(200, 2, 70);
		dibujarMesaCristal(4.0f);
	glPopMatrix();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
}

void animation() {

	if(sentido){
		animax += 0.0001;
		if(animax >= 1.0){
			sentido = false;
		}
	}
	else{
		animax -= 0.0001;
		if(animax <= 1.0){
			sentido = true;
		}
	}

	reloj.update();
	avion.update();
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
			//reloj.left();
			break;
		case 'Z':
			//reloj.right();
			break;
		case 'x':
			//reloj.fordward();
			break;
		case 'X':
			//reloj.backward();
			break;
		case 'c':
			//reloj.up();
			break;
		case 'C':
			//reloj.down();
			break;
		case 'v':
			reloj.rotateManecilla();
			break;
		case 'V':
			//reloj.rotateXNegative();
			break;
		case 'M':
		case 'm':
			//reloj.saveToFile();
			break;
		case 'n':
		case 'N':
			//reloj.saveKeyframe();
			break;
		case 'b':
			avion.setActivate(true);
			break;
		case 'B':
			//reloj.setActivate(false);
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
void arrowKeys(int key, int x, int y) {
	switch (key) {
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
	glutSpecialFunc(arrowKeys); // Función manejo teclas especiales
	glutIdleFunc(animation);
	glutMainLoop();

	return 0;
}
