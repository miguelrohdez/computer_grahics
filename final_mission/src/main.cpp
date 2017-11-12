/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 *
 */

#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include "Prisma.h"
#include "Camera.h"

CCamera camara;
GLfloat g_lookupdown = 0.0f;    // Look Position In The Z-Axis (NEW)

float mueve = 1.0f;
float rotacionX = 0.0f;
float rotacionY = 0.0f;
float rotacionZ = 0.0f;
float traslacionX = 0.0f;
float traslacionY = 0.0f;
float traslacionZ = 0.0f;


/*
 * Texturas
 */
CTexture terreno;
CTexture tiles;
CTexture pared;
CTexture textureSkyBox;


void inicializarTexturas() {
    textureSkyBox.LoadBMP("Texturas/skybox.bmp");
	textureSkyBox.BuildGLTexture();
	textureSkyBox.ReleaseImage();

    terreno.LoadTGA("Texturas/tierra.tga");
    terreno.BuildGLTexture();
    terreno.ReleaseImage();

    tiles.LoadTGA("Texturas/piso2.tga");
    tiles.BuildGLTexture();
    tiles.ReleaseImage();

    pared.LoadTGA("Texturas/ladrillos2.tga");
    pared.BuildGLTexture();
    pared.ReleaseImage();
}

/*
 * Función para inicializar parámetros
 */
void InitGL() {
    glShadeModel(GL_SMOOTH);                            // Activa Smooth Shading
    glClearColor(0.3f, 0.3f, 0.3f, 0.5f);                // Fondo negro
    glClearDepth(1.0f);                                    // Valor para el Depth Buffer
    glEnable(GL_DEPTH_TEST);                            // Activa Depth Testing
    glDepthFunc(GL_LEQUAL);                                // Tipo de Depth Testing a usar
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Correccion de cálculos de perspectiva
    glCullFace(GL_BACK);                                // Configurado para eliminar caras traseras
    glEnable(GL_CULL_FACE);                                // Activa eliminacion de caras ocultas
    camara.Position_Camera(0,2.5f,3, 0,2.5f,0, 0, 1, 0);
    inicializarTexturas();
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
    Prisma p = Prisma(500, 10, 500, c);
    p.setTexture(terreno);
    p.setRepetitionTexture(20);
    p.draw();
}

void dibujarPiso() {
    Color c = Color(1.0f, 1.0f, 1.0f);
    Prisma p = Prisma(140, 5, 140, c);
    p.setTexture(tiles);
    p.setRepetitionTexture(20);
    p.draw();
}

void dibujarCasa() {
    Color n = Color(1.0f, 1.0f, 1.0f);
    Prisma c = Prisma(100, 25, 0.3, n);
    c.setTexture(pared);
    //c.setRepetitionTexture(2);
    glTranslatef(0, 0, -65.5);
    c.draw(); // A
    glTranslatef(25, 0, 30);
    c.draw(50, 25, 0.3); // E
    glTranslatef(-60, 0, 0);
    c.draw(30, 25, 0.3); // F
    glTranslatef(0, 0, 20);
    c.draw(30, 25, 0.3); // H
    glTranslatef(5, 0, 40);
    c.draw(40, 25, 0.3); // M
    glTranslatef(35, 0, 40);
    c.draw(50, 25, 0.3); // O
    glTranslatef(-25, 0, -7.5);
    c.draw(0.3, 25, 15); // Ñ
    glTranslatef(0, 0, -25);
    c.draw(0.3, 25, 15); // N
    glTranslatef(10, 0, -27.5);
    c.draw(0.3, 25, 40); // L
    glTranslatef(10, 0, -25);
    c.draw(0.3, 25, 30); // J
    glTranslatef(-20, 0, 0);
    c.draw(0.3, 25, 10); // I
    glTranslatef(-30, 0, 0);
    c.draw(0.3, 25, 90); // G
    glTranslatef(40, 0, -30);
    c.draw(0.3, 25, 30); // D
    glTranslatef(40, 0, -5);
    c.draw(0.3, 25, 20); // C
    glTranslatef(20, 0, 5);
    c.draw(0.3, 25, 30);// B
    glTranslatef(-20, 0, 70);
    c.draw(0.3, 25, 90);
}

void dibujarSkyBox() {
    Prisma p = Prisma(500, 200, 500, Color(1.0f, 1.0f, 1.0f));
    p.setTexture(textureSkyBox);
    p.drawSky();
}

/*
 * Función que dibuja
 */
void display(void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpia pantalla y Depth Buffer
    glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
    glLoadIdentity();    // Reinicia matriz Modelview
    glTranslatef(rotacionX, rotacionY, rotacionZ);
    //gluLookAt(100.0f + traslacionX, 520.0f + traslacionY, 10.f + traslacionZ, 0.0f, 0.0f, 0.0f, 0, 1, 0);
    glRotatef(g_lookupdown,1.0f,0,0);
    gluLookAt(camara.mPos.x, camara.mPos.y, camara.mPos.z, camara.mView.x, camara.mView.y, camara.mView.z, camara.mUp.x,   camara.mUp.y,   camara.mUp.z);

    dibujaEjes();
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
        glTranslatef(0,95,0);
        dibujarSkyBox();
    glPopMatrix();
    glPushMatrix();
            dibujarTerreno();
    glPopMatrix();

    glTranslatef(0, 7.5, 0);
    glPushMatrix();
        dibujarPiso();
    glPopMatrix();
    glTranslatef(0, 2.5, 0);
    glPushMatrix();
        glTranslatef(0, 12.5, 0);
        dibujarCasa();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);



    glFlush(); //TODO: Entender que hace
    glutSwapBuffers();
}

/*
* Funcion para el reajuste de dibujo en ventana
*/

void reshape(int width, int height) {
    if (height == 0) // Prevenir division entre cero
        height = 1;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION); // Seleccionamos Projection Matrix
    glLoadIdentity();

    //glFrustum(-500, 500, -500, 500, 0, 20);  //TODO: Cambio de valores
    gluPerspective(60, width / height, 1.0f, 10000.0f);

    glMatrixMode(GL_MODELVIEW); // Seleccionamos Modelview Matrix
    glLoadIdentity();
}


/*
* Función que maneja el teclado
*/
void keyboard ( unsigned char key, int x, int y )  // Create Keyboard Function
{
	switch ( key ) {

		case 'w':   //Movimientos de camara
		case 'W':
			camara.Move_Camera( CAMERASPEED+0.2 );
			break;

		case 's':
		case 'S':
			camara.Move_Camera(-(CAMERASPEED+0.2));
			break;

		case 'a':
		case 'A':
			camara.Strafe_Camera(-(CAMERASPEED+0.4));
			break;

		case 'd':
		case 'D':
			camara.Strafe_Camera( CAMERASPEED+0.4 );
			break;

    case 'e':
    case 'E':
      camara.UpDown_Camera(CAMERASPEED);
      break;

    case 'q':
    case 'Q':
      camara.UpDown_Camera(-CAMERASPEED);
        break;

		case 27:        // Cuando Esc es presionado...
			exit ( 0 );   // Salimos del programa
			break;
		default:        // Cualquier otra
			break;
  }

  glutPostRedisplay();
}

/*
* Función para el manejo de teclas especiales
*/
void arrow_keys ( int a_keys, int x, int y )  // Funcion para manejo de teclas especiales (arrow keys)
{
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


//TODO: Función para las animaciones

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
    glutMainLoop();

    return 0;
}
