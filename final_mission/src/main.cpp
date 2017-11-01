/*
 * Semestre 2018-1
 * Alumnos:
 * Barriga Martinez Diego Alberto
 * Oropeza Vilchis Luis Alberto
 *
 * Para cambiar de dibujo utilizar la flechas del teclado
 */

#include <GL/glut.h>

float mueve = 1.0f;
float rotacionX = 0.0f;
float rotacionY = 0.0f;
float rotacionZ = 0.0f;

/*
 * Función para inicializar parámetros
 */
void InitGL ( GLvoid )
{
	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);				// Fondo negro
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva
	glCullFace(GL_BACK);								// Configurado para eliminar caras traseras
	glEnable(GL_CULL_FACE);								// Activa eliminacion de caras ocultas

}


void dibujaEjes()
{
	
	glBegin(GL_LINES);
		//Eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-1000.0f, 0.0f, 0.0f);
		glVertex3f( 1000.0f, 0.0f, 0.0f);

		//Eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f, -1000.0f, 0.0f);
		glVertex3f(0.0f,  1000.0f, 0.0f);

		//Eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f, -1000.0f);
		glVertex3f(0.0f, 0.0f,  1000.0f);
	glEnd();

	glPointSize(10.0f);

	glBegin(GL_POINTS);
		//"Flecha" eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(1000.0f, 0.0f, 0.0f);

		//"Flecha" eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,  1000.0f, 0.0f);

		//"Flecha" eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();

	glPointSize(1.0f);

	glColor3f(1.0f,1.0f,1.0f);
}

void dibujaPiso(){
	float v[8][3] = {
						{-250.0f, 100.0f, 250.0f}, 
						{250.0f, 100.0f, 250.0f}, 
						{-250.0f, -100.0f, 250.0f},
						{250.0f, -100.0f, 250.0f},
						{-250.0f, -100.0f, -250.0f}, 
				  		{250.0f, -100.0f, -250.0f}, 
				  		{250.0f, 100.0f, -250.0f},
				  		{-250.0f, 100.0f, -250.0f}
			  		};

	//Plano superior
	glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.0f);
		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[6]);
		glVertex3fv(v[7]);
	glEnd();

	//Plano inferior
	glBegin(GL_POLYGON);
		glColor3f(0.4f, 0.7f, 0.7f);
		glVertex3fv(v[2]);
		glVertex3fv(v[4]);
		glVertex3fv(v[5]);
		glVertex3fv(v[3]);
	glEnd();	

	//Plano 1
	glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.0f);
		glVertex3fv(v[0]);
		glVertex3fv(v[2]);
		glVertex3fv(v[3]);
		glVertex3fv(v[1]);
	glEnd();

	//Plano 2
	glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.0f);
		glVertex3fv(v[1]);
		glVertex3fv(v[3]);
		glVertex3fv(v[5]);
		glVertex3fv(v[6]);
	glEnd();

	//Plano 3
	glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.0f);
		glVertex3fv(v[6]);
		glVertex3fv(v[5]);
		glVertex3fv(v[4]);
		glVertex3fv(v[7]);
	glEnd();

	//Plano 4
	glBegin(GL_POLYGON);
		glColor3f(0.7f, 0.7f, 0.0f);
		glVertex3fv(v[7]);
		glVertex3fv(v[4]);
		glVertex3fv(v[2]);
		glVertex3fv(v[0]);
	glEnd();
}


/*
 * Función que dibuja
 */
void display ( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpia pantalla y Depth Buffer
	glMatrixMode(GL_MODELVIEW); //TODO: Entender que hace
	glLoadIdentity();	// Reinicia matriz Modelview
	gluLookAt(100.0f + rotacionX, 520.0f + rotacionY, 100.f + rotacionZ, 0.0f, 0.0f, 0.0f, 0, 1, 0);
	dibujaEjes();
	dibujaPiso();
	glFlush(); //TODO: Entender que hace

}

/*
* Funcion para el reajuste de dibujo en ventana
*/

void reshape ( int width , int height )
{
	if (height==0) // Prevenir division entre cero
		height=1;

	glViewport(0, 0, width,height);

	glMatrixMode(GL_PROJECTION); // Seleccionamos Projection Matrix
	glLoadIdentity();

	//glFrustum(-500, 500, -500, 500, 0, 20);  //TODO: Cambio de valores
	gluPerspective(60, width/height, 1.0f, 10000.0f);

	glMatrixMode(GL_MODELVIEW); // Seleccionamos Modelview Matrix
	glLoadIdentity();
}


/*
* Función que maneja el teclado
*/
// Para cerrar la ventana con la tecla ESC
void keyboard(unsigned char key, int x, int y)
{
	 switch (key)
	 {
		case 27:
            exit(0);
			break;
		case 'a':
			rotacionX -= 3.0f;
			break;
		case 'd':
			rotacionX += 3.0f;
			break;
		case 's':
			rotacionY -= 3.0f;
			break;
		case 'w':
			rotacionY += 3.0f;
			break;
		case 'q':
			rotacionZ -= 3.0f;
			break;
		case 'e':
			rotacionZ += 3.0f;
			break;
	 }
	  glutPostRedisplay();
}

/*
* Función para el manejo de teclas especiales
*/
void specialKeys(int key, int x, int y)
{
	glutPostRedisplay();
}


//TODO: Función para las animaciones

/*
* Función principal
*/
int main (int argc, char** argv)
{
  glutInit            (&argc, argv); // Inicializamos OpenGL
  glutInitDisplayMode (GLUT_RGBA | GLUT_SINGLE); // Display Mode (Clores RGB y alpha | Buffer Sencillo )
  glutInitWindowSize  (1920, 1080); // Tamaño de la Ventana
  glutInitWindowPosition (-1, -1); //Posicion de la Ventana
  glutCreateWindow    ("Complejo Residencial"); // Nombre de la Ventana
  InitGL(); // Parametros iniciales de la aplicacion
  glutDisplayFunc(display); // Función de dibujo
  glutReshapeFunc(reshape); // Función en caso de cambio de tamano
  glutKeyboardFunc(keyboard); // Función de manejo de teclado
  glutSpecialFunc(specialKeys); // Función manejo teclas especiales
  glutMainLoop();

  return 0;
}