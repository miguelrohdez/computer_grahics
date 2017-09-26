#include "Main.h"
#include "3ds.h"

HDC			hDC=NULL;		// Dispositivo de contexto GDI
HGLRC		hRC=NULL;		// Contexto de renderizado
HWND		hWnd=NULL;		// Manejador de ventana
HINSTANCE	hInstance;		// Instancia de la aplicacion

bool	keys[256];			// Arreglo para el manejo de teclado
bool	active=TRUE;		// Bandera de ventana activa

int glWidth;
int glHeight;

//CAMBIOS ADICIONO
COLORREF ShowColorDialog(HWND);
COLORREF gColor = RGB(0, 0, 0);
bool teclaColor=false;

//Apuntador para primitivas de cuadricas
GLUquadricObj	*e;

//Nombre y ubicación de los modelos
#define FILE_NAME1  "Modelos/modelo1.3ds"
#define FILE_NAME2  "Modelos/modelo2.3ds"
#define FILE_NAME3  "Modelos/modelo3.3ds"

//Contenedores de texturas de cada modelo
CTga textureModel1[4];
CTga textureModel2[4];
CTga textureModel3[4];

//Contenedor de texturas para el escenario
CTga textureName[20];

//Objeto que da acceso a las funciones del cargador 3ds
CLoad3DS g_Load3ds;

//Instancias de la estructura que almacenaran los datos de cada modelo
t3DModel g_3DModel1;
t3DModel g_3DModel2;
t3DModel g_3DModel3;

//Objeto para acceder a las variables de control del personaje
paramObjCam player1;

//Variable para cambiar entre render sólido y alambrado
int renderModo;

//variables para la cámara
CVector PosCam;
CVector ObjCam;

//Variables para el movimiento de las trampas del piso
float posGrupoTramp1[2];
float posGrupoTramp2[2];

//Variables para el movimiento del fuego
float escFuego[4];
int   dirFuego[4];

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

GLvoid ReDimensionaEscenaGL(GLsizei width, GLsizei height)	// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0, 0, width, height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1.0f, 2000.0f);
	
	glMatrixMode(GL_MODELVIEW);							// Selecciona la Matriz de Vista de Modelo
	glLoadIdentity();									// Resetea la Matriz de Vista de Modelo

	glWidth=width;
	glHeight=height;


}

int CargaModelos()
{
	if(!g_Load3ds.Load3DSFile(FILE_NAME1, &g_3DModel1, textureModel1))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME2, &g_3DModel2, textureModel2))
		return 0;
	if(!g_Load3ds.Load3DSFile(FILE_NAME3, &g_3DModel3, textureModel3))
		return 0;
	
	return TRUE;
}

void DescargaModelos()
{
	g_Load3ds.UnLoad3DSFile(&g_3DModel1, textureModel1);
	g_Load3ds.UnLoad3DSFile(&g_3DModel2, textureModel2);
	g_Load3ds.UnLoad3DSFile(&g_3DModel3, textureModel3);
}

void inicializaCamara()
{
	PosCam=CVector(0.0f, 50.0f, 140.0f);
	ObjCam=CVector(0.0f, 50.0f, 0.0f);
}

void inicializaTrampas()
{
	//Trampas de piso
	posGrupoTramp1[0]=-60.0f;
	posGrupoTramp1[1]=-80.0f;

	posGrupoTramp2[0]=-45.0f;
	posGrupoTramp2[1]=-65.0f;

	//Fuego de los lanzallamas
	escFuego[0]=0.0f;
	escFuego[1]=0.0f;
	escFuego[2]=0.0f;
	escFuego[3]=0.0f;

	dirFuego[0]=1; //fuego saliendo del lanzallamas
	dirFuego[1]=1; //fuego saliendo del lanzallamas
	dirFuego[2]=1; //fuego saliendo del lanzallamas
	dirFuego[3]=1; //fuego saliendo del lanzallamas
}

void actualizaTrampas()
{
	//Se actualiza la posición de las trampas de piso

	//Grupo 1
	if(PosCam.x > 320.0f)
	{
		if(posGrupoTramp1[0] < 0.0f)
			posGrupoTramp1[0]+=0.5f;

		if(posGrupoTramp1[1] < 0.0f)
			posGrupoTramp1[1]+=0.5f;
	}

	//Grupo 2
	if(PosCam.x > 1640.0f)
	{
		if(posGrupoTramp2[0] < 15.0f)
			posGrupoTramp2[0]+=0.5f;

		if(posGrupoTramp2[1] < 15.0f)
			posGrupoTramp2[1]+=0.5f;
	}

	//Se actualiza la escala de las llamas

	//Se actualiza la posición de las trampas de techo
	
}

int IniGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	inicializaCamara();
	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Fondo negro
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Configurado para eliminar caras traseras
	glEnable(GL_CULL_FACE);								// Activa eliminacion de caras ocultas

	e=gluNewQuadric();

	//Se inicializan las posiciones de las trampas
	inicializaTrampas();

	return TRUE;										
}

void DibujaEjes()
{
	glBegin(GL_LINES);
		//Eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f(-100.0f, 0.0f, 0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//Eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f, -100.0f, 0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//Eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f, -100.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glPointSize(10.0f);

	glBegin(GL_POINTS);
		//"Flecha" eje X
		glColor3f(1.0f,0.0f,0.0f);
		glVertex3f( 100.0f, 0.0f, 0.0f);

		//"Flecha" eje Y
		glColor3f(0.0f,1.0f,0.0f);
		glVertex3f(0.0f,  100.0f, 0.0f);

		//"Flecha" eje Z
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(0.0f, 0.0f,  100.0f);
	glEnd();

	glPointSize(1.0f);

	glColor3f(1.0f,1.0f,1.0f);
}

void dibujaCilindro(float radio, int lados, float altura, int modoRender)
{
	float ang;
	float a[3], b[3], c[3], d[3];
	float delta;

	delta=360.0f/lados;
	
	for(int i=0; i < lados; i++)  //Por la forma de renderizar de openGL se usa for
	{
		ang=i*delta;
		
		a[0]=radio*(float)cos(ang*PI/180.0f);  //Conversión de ángulo a RAD
		a[1]=0.0f;
		a[2]=radio*(float)sin(ang*PI/180.0f);

		b[0]=a[0];  // Se calcula igual y se reutiliza
		b[1]=altura;  // El vertice b está arriba por ello se usa altura 
		b[2]=a[2];

		ang=(i+1)*delta;  // Se utiliza theta sig. Delta theta = i+1
		
		c[0]=radio*(float)cos(ang*PI/180.0f);
		c[1]=altura;
		c[2]=radio*(float)sin(ang*PI/180.0f);

		d[0]=c[0];
		d[1]=0.0f;
		d[2]=c[2];

		glColor3f(1.0f,0.0f,0.0f);

		if(modoRender == 1) glBegin(GL_QUADS);// sólido
		else if(modoRender == 2) glBegin(GL_LINE_LOOP);// alambrado
			glVertex3f(a[0], a[1], a[2]);
			glVertex3f(b[0], b[1], b[2]);
			glVertex3f(c[0], c[1], c[2]);
			glVertex3f(d[0], d[1], d[2]);
		glEnd();

		//Tapa superior
		glColor3f(1.0f,1.0f,0.0f);

		if(modoRender == 1) glBegin(GL_TRIANGLES);// sólido
		else if(modoRender == 2) glBegin(GL_LINE_LOOP);// alambrado
			glVertex3f(c[0], c[1], c[2]);
			glVertex3f(b[0], b[1], b[2]);
			glVertex3f(0.0f, altura, 0.0f);
		glEnd();

		//Tapa inferior
		glColor3f(0.0f,0.0f,1.0f);

		if(modoRender == 1) glBegin(GL_TRIANGLES);// sólido
		else if(modoRender == 2) glBegin(GL_LINE_LOOP);// alambrado
			glVertex3f(a[0], a[1], a[2]);
			glVertex3f(d[0], d[1], d[2]);
			glVertex3f(0.0f, 0.0f, 0.0f);
		glEnd();

		glColor3f(1.0f,1.0f,1.0f);
	}
}

void dibujaCono(float radio, int lados, float altura, int modoRender)
{
	float ang;
	float a[3], b[3], c[3];
	float delta;

	delta=360.0f/lados;
	
	for(int i=0; i < lados; i++)  //Por la forma de renderizar de openGL se usa for
	{
		ang=i*delta;
		
		a[0]=radio*(float)cos(ang*PI/180.0f);  //Conversión de ángulo a RAD
		a[1]=0.0f;
		a[2]=radio*(float)sin(ang*PI/180.0f);

		ang=(i+1)*delta;  // Se utiliza theta sig. Delta theta = i+1

		b[0]=radio*(float)cos(ang*PI/180.0f);
		b[1]=0.0f;
		b[2]=radio*(float)sin(ang*PI/180.0f);
		
		c[0]=0.0f;
		c[1]=altura;
		c[2]=0.0f;

		if(lados==4 && i==2 || lados==4 && i==0)
			glColor3f(0.6f, 0.6f, 0.6f);
		else
			glColor3f(0.8f, 0.8f, 0.8f);

		if(modoRender == 1) glBegin(GL_TRIANGLES);// sólido
		else if(modoRender == 2) glBegin(GL_LINE_LOOP);// alambrado
			glVertex3f(a[0], a[1], a[2]);
			glVertex3f(c[0], c[1], c[2]);
			glVertex3f(b[0], b[1], b[2]);
		glEnd();

		//Tapa inferior
		glColor3f(0.0f,0.0f,1.0f);

		if(modoRender == 1) glBegin(GL_TRIANGLES);// sólido
		else if(modoRender == 2) glBegin(GL_LINE_LOOP);// alambrado
			glVertex3f(a[0], a[1], a[2]);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(b[0], b[1], b[2]);
		glEnd();

		glColor3f(1.0f,1.0f,1.0f);
	}
}

void dibujaFuego(float escala)
{
	glColor3f(1.0f, 1.0f, 0.0f);

	

	glColor3f(1.0f, 1.0f, 1.0f);
}

void dibujaLanzallamas()
{

}

void dibujaTrampa1()
{

	glPushMatrix();		
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCilindro(1.75f, 12, 30.0f, 2);
	glPopMatrix();

	glPushMatrix();		
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCilindro(2.5f, 12, 6.0f, 2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -2.0f, 0.0f);
		glRotatef(0.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCilindro(3.5f, 12, 2.0f, 2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0f, -2.0f, -2.0f);
		glRotatef(180.0f, 0.0f, 0.0f, 0.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 5, 3.0f, 2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.75f, -2.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 0.0f, 0.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 5, 3.0f, 2);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-2.0f, -2.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 0.0f, 0.0f);  // ángulo, x, y, z (del eje a girar) 
		//glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 5, 3.0f, 2);
	glPopMatrix();


}

void dibujaTrampa2()
{
	//1
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.0f, 0.0f);
		glVertex3f(-1.0f,  0.0f, 10.0f);
		glVertex3f( 1.0f,  0.0f, 10.0f);
		glVertex3f( 1.0f, 60.0f, 10.0f);
		glVertex3f(-1.0f, 60.0f, 10.0f);
	glEnd();

	//2
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.0f, 0.0f);
		glVertex3f( 1.0f, 0.0f,  10.0f);
		glVertex3f(-1.0f, 0.0f,  10.0f);
		glVertex3f(-1.0f, 0.0f, -10.0f);
		glVertex3f( 1.0f, 0.0f, -10.0f);
	glEnd();

	//3
	glBegin(GL_QUADS);
		glColor3f(0.4f, 0.0f, 0.0f);
		glVertex3f(-1.0f, 60.0f,  10.0f);
		glVertex3f( 1.0f, 60.0f,  10.0f);
		glVertex3f( 1.0f, 60.0f, -10.0f);
		glVertex3f(-1.0f, 60.0f, -10.0f);
	glEnd();

	//4
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.0f);
		glVertex3f(1.0f,  0.0f,  10.0f);
		glVertex3f(1.0f,  0.0f, -10.0f);
		glVertex3f(1.0f, 60.0f, -10.0f);
		glVertex3f(1.0f, 60.0f,  10.0f);
	glEnd();

	//5
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.0f, 0.0f);
		glVertex3f(-1.0f,  0.0f, -10.0f);
		glVertex3f(-1.0f,  0.0f,  10.0f);
		glVertex3f(-1.0f, 60.0f,  10.0f);
		glVertex3f(-1.0f, 60.0f, -10.0f);
	glEnd();

	//6
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.0f, 0.0f);
		glVertex3f( 1.0f,  0.0f, -10.0f);
		glVertex3f(-1.0f,  0.0f, -10.0f);
		glVertex3f(-1.0f, 60.0f, -10.0f);
		glVertex3f( 1.0f, 60.0f, -10.0f);
	glEnd();

	// FILA DE PICOS 1
	glPushMatrix();
		glTranslatef(-1.0f, 55.0f, -6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 55.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 55.0f, 6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	// FILA DE PICOS 2
	glPushMatrix();
		glTranslatef(-1.0f, 48.0f, -6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 48.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 48.0f, 6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	// FILA DE PICOS 3
	glPushMatrix();
		glTranslatef(-1.0f, 41.0f, -6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 41.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 41.0f, 6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	// FILA DE PICOS 4
	glPushMatrix();
		glTranslatef(-1.0f, 34.0f, -6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 34.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1.0f, 34.0f, 6.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(1.0f, 4, 2.5f, 1);
	glPopMatrix();
}


void dibujaEscenario(int render)
{
	if(render == 1)//sólido
		glPolygonMode(GL_FRONT,GL_FILL);
	else if(render == 2)//alambrado
		glPolygonMode(GL_FRONT,GL_LINE);

	//OBJETO 1 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.9f, 0.0f);
		glVertex3f(-30.0f,  0.0f, 25.0f);
		glVertex3f( 80.0f,  0.0f, 25.0f);
		glVertex3f( 80.0f, 40.0f, 25.0f);
		glVertex3f(-30.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 1.0f, 0.0f);
		glVertex3f(-30.0f, 40.0f, 25.0f);
		glVertex3f( 80.0f, 40.0f, 25.0f);
		glVertex3f( 80.0f, 40.0f,  0.0f);
		glVertex3f(-30.0f, 40.0f,  0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.8f, 0.0f);
		glVertex3f(80.0f,  0.0f, 25.0f);
		glVertex3f(80.0f,  0.0f,  0.0f);
		glVertex3f(80.0f, 40.0f,  0.0f);
		glVertex3f(80.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 2 (2 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.0f);
		glVertex3f( 80.0f,  0.0f, 25.0f);
		glVertex3f(200.0f,  0.0f, 25.0f);
		glVertex3f(200.0f, 10.0f, 25.0f);
		glVertex3f( 80.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f( 80.0f, 10.0f, 25.0f);
		glVertex3f(200.0f, 10.0f, 25.0f);
		glVertex3f(200.0f, 10.0f,  0.0f);
		glVertex3f( 80.0f, 10.0f,  0.0f);
	glEnd();

	//OBJETO 3 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.8f, 0.6f);
		glVertex3f(200.0f,  0.0f, 25.0f);
		glVertex3f(260.0f,  0.0f, 25.0f);
		glVertex3f(260.0f, 25.0f, 25.0f);
		glVertex3f(200.0f, 25.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.9f, 0.7f);
		glVertex3f(200.0f, 25.0f, 25.0f);
		glVertex3f(260.0f, 25.0f, 25.0f);
		glVertex3f(260.0f, 25.0f,  0.0f);
		glVertex3f(200.0f, 25.0f,  0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.7f, 0.5f);
		glVertex3f(260.0f,  0.0f, 25.0f);
		glVertex3f(260.0f,  0.0f,  0.0f);
		glVertex3f(260.0f, 25.0f,  0.0f);
		glVertex3f(260.0f, 25.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.7f, 0.5f);
		glVertex3f(200.0f,  0.0f,  0.0f);
		glVertex3f(200.0f,  0.0f, 25.0f);
		glVertex3f(200.0f, 25.0f, 25.0f);
		glVertex3f(200.0f, 25.0f,  0.0f);
	glEnd();

	//OBJETO 4 (2 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.6f, 0.7f);
		glVertex3f(260.0f,  0.0f, 25.0f);
		glVertex3f(400.0f,  0.0f, 25.0f);
		glVertex3f(400.0f, 10.0f, 25.0f);
		glVertex3f(260.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.7f, 0.8f);
		glVertex3f(260.0f, 10.0f, 25.0f);
		glVertex3f(400.0f, 10.0f, 25.0f);
		glVertex3f(400.0f, 10.0f,  0.0f);
		glVertex3f(260.0f, 10.0f,  0.0f);
	glEnd();

	//OBJETO 5 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f( 5.0f, 40.0f, 0.0f);
		glVertex3f(40.0f, 40.0f, 0.0f);
		glVertex3f(40.0f, 45.0f, 0.0f);
		glVertex3f( 5.0f, 45.0f, 0.0f);
	glEnd();

	//OBJETO 6 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.1f, 0.0f);
		glVertex3f(40.0f, 40.0f, 7.0f);
		glVertex3f(43.0f, 40.0f, 7.0f);
		glVertex3f(43.0f, 65.0f, 7.0f);
		glVertex3f(40.0f, 65.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(43.0f,  40.0f, 7.0f);
		glVertex3f(43.0f,  40.0f, 0.0f);
		glVertex3f(43.0f, 140.0f, 0.0f);
		glVertex3f(43.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(40.0f,  40.0f, 0.0f);
		glVertex3f(40.0f,  40.0f, 7.0f);
		glVertex3f(40.0f, 140.0f, 7.0f);
		glVertex3f(40.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 7 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(40.0f,  65.0f, 25.0f);
		glVertex3f(43.0f,  65.0f, 25.0f);
		glVertex3f(43.0f, 140.0f, 25.0f);
		glVertex3f(40.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(43.0f, 65.0f, 25.0f);
		glVertex3f(40.0f, 65.0f, 25.0f);
		glVertex3f(40.0f, 65.0f,  7.0f);
		glVertex3f(43.0f, 65.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(43.0f,  65.0f, 25.0f);
		glVertex3f(43.0f,  65.0f,  7.0f);
		glVertex3f(43.0f, 140.0f,  7.0f);
		glVertex3f(43.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(40.0f,  65.0f,  7.0f);
		glVertex3f(40.0f,  65.0f, 25.0f);
		glVertex3f(40.0f, 140.0f, 25.0f);
		glVertex3f(40.0f, 140.0f,  7.0f);
	glEnd();

	//OBJETO 8 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.3f, 0.6f);
		glVertex3f(53.0f, 40.0f, 7.0f);
		glVertex3f(56.0f, 40.0f, 7.0f);
		glVertex3f(56.0f, 65.0f, 7.0f);
		glVertex3f(53.0f, 65.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.2f, 0.5f);
		glVertex3f(56.0f,  40.0f, 7.0f);
		glVertex3f(56.0f,  40.0f, 0.0f);
		glVertex3f(56.0f, 140.0f, 0.0f);
		glVertex3f(56.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.2f, 0.5f);
		glVertex3f(53.0f,  40.0f, 0.0f);
		glVertex3f(53.0f,  40.0f, 7.0f);
		glVertex3f(53.0f, 140.0f, 7.0f);
		glVertex3f(53.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 9 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.9f, 1.0f, 0.7f);
		glVertex3f(53.0f,  65.0f, 25.0f);
		glVertex3f(56.0f,  65.0f, 25.0f);
		glVertex3f(56.0f, 140.0f, 25.0f);
		glVertex3f(53.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.9f, 0.6f);
		glVertex3f(56.0f, 65.0f, 25.0f);
		glVertex3f(53.0f, 65.0f, 25.0f);
		glVertex3f(53.0f, 65.0f,  7.0f);
		glVertex3f(56.0f, 65.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.8f, 0.5f);
		glVertex3f(56.0f,  65.0f, 25.0f);
		glVertex3f(56.0f,  65.0f,  7.0f);
		glVertex3f(56.0f, 140.0f,  7.0f);
		glVertex3f(56.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.8f, 0.5f);
		glVertex3f(53.0f,  65.0f,  7.0f);
		glVertex3f(53.0f,  65.0f, 25.0f);
		glVertex3f(53.0f, 140.0f, 25.0f);
		glVertex3f(53.0f, 140.0f,  7.0f);
	glEnd();

	//OBJETO 10 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.4f, 0.7f, 0.9f);
		glVertex3f( 94.0f, 44.0f, 21.0f);
		glVertex3f(196.0f, 44.0f, 21.0f);
		glVertex3f(196.0f, 52.0f, 21.0f);
		glVertex3f( 94.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.8f, 1.0f);
		glVertex3f( 94.0f, 52.0f, 21.0f);
		glVertex3f(196.0f, 52.0f, 21.0f);
		glVertex3f(196.0f, 52.0f,  4.0f);
		glVertex3f( 94.0f, 52.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.5f, 0.7f);
		glVertex3f(196.0f, 44.0f, 21.0f);
		glVertex3f( 94.0f, 44.0f, 21.0f);
		glVertex3f( 94.0f, 44.0f,  4.0f);
		glVertex3f(196.0f, 44.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.6f, 0.8f);
		glVertex3f(196.0f, 44.0f, 21.0f);
		glVertex3f(196.0f, 44.0f,  4.0f);
		glVertex3f(196.0f, 52.0f,  4.0f);
		glVertex3f(196.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.6f, 0.8f);
		glVertex3f(94.0f, 44.0f,  4.0f);
		glVertex3f(94.0f, 44.0f, 21.0f);
		glVertex3f(94.0f, 52.0f, 21.0f);
		glVertex3f(94.0f, 52.0f,  4.0f);
	glEnd();

	//OBJETO 11 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.1f, 0.0f);
		glVertex3f(137.0f, 52.0f, 7.0f);
		glVertex3f(140.0f, 52.0f, 7.0f);
		glVertex3f(140.0f, 78.0f, 7.0f);
		glVertex3f(137.0f, 78.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(140.0f,  52.0f, 7.0f);
		glVertex3f(140.0f,  52.0f, 0.0f);
		glVertex3f(140.0f, 140.0f, 0.0f);
		glVertex3f(140.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(137.0f,  52.0f, 0.0f);
		glVertex3f(137.0f,  52.0f, 7.0f);
		glVertex3f(137.0f, 140.0f, 7.0f);
		glVertex3f(137.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 12 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(137.0f,  78.0f, 25.0f);
		glVertex3f(140.0f,  78.0f, 25.0f);
		glVertex3f(140.0f, 140.0f, 25.0f);
		glVertex3f(137.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(140.0f, 78.0f, 25.0f);
		glVertex3f(137.0f, 78.0f, 25.0f);
		glVertex3f(137.0f, 78.0f,  7.0f);
		glVertex3f(140.0f, 78.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(140.0f,  78.0f, 25.0f);
		glVertex3f(140.0f,  78.0f,  7.0f);
		glVertex3f(140.0f, 140.0f,  7.0f);
		glVertex3f(140.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(137.0f,  78.0f,  7.0f);
		glVertex3f(137.0f,  78.0f, 25.0f);
		glVertex3f(137.0f, 140.0f, 25.0f);
		glVertex3f(137.0f, 140.0f,  7.0f);
	glEnd();

	//OBJETO 13 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(140.0f, 52.0f, 4.0f);
		glVertex3f(173.0f, 52.0f, 4.0f);
		glVertex3f(173.0f, 57.0f, 4.0f);
		glVertex3f(140.0f, 57.0f, 4.0f);
	glEnd();

	//OBJETO 14 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.1f, 0.0f);
		glVertex3f(233.0f, 25.0f, 7.0f);
		glVertex3f(236.0f, 25.0f, 7.0f);
		glVertex3f(236.0f, 65.0f, 7.0f);
		glVertex3f(233.0f, 65.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(236.0f,  25.0f, 7.0f);
		glVertex3f(236.0f,  25.0f, 0.0f);
		glVertex3f(236.0f, 140.0f, 0.0f);
		glVertex3f(236.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(233.0f,  25.0f, 0.0f);
		glVertex3f(233.0f,  25.0f, 7.0f);
		glVertex3f(233.0f, 140.0f, 7.0f);
		glVertex3f(233.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 15 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(233.0f,  65.0f, 25.0f);
		glVertex3f(236.0f,  65.0f, 25.0f);
		glVertex3f(236.0f, 140.0f, 25.0f);
		glVertex3f(233.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(236.0f, 65.0f, 25.0f);
		glVertex3f(233.0f, 65.0f, 25.0f);
		glVertex3f(233.0f, 65.0f,  7.0f);
		glVertex3f(236.0f, 65.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(236.0f,  65.0f, 25.0f);
		glVertex3f(236.0f,  65.0f,  7.0f);
		glVertex3f(236.0f, 140.0f,  7.0f);
		glVertex3f(236.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(233.0f,  65.0f,  7.0f);
		glVertex3f(233.0f,  65.0f, 25.0f);
		glVertex3f(233.0f, 140.0f, 25.0f);
		glVertex3f(233.0f, 140.0f,  7.0f);
	glEnd();

	//OBJETO 16 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.7f, 0.3f);
		glVertex3f(280.0f, 32.0f, 21.0f);
		glVertex3f(330.0f, 32.0f, 21.0f);
		glVertex3f(330.0f, 40.0f, 21.0f);
		glVertex3f(280.0f, 40.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.4f);
		glVertex3f(280.0f, 40.0f, 21.0f);
		glVertex3f(330.0f, 40.0f, 21.0f);
		glVertex3f(330.0f, 40.0f,  4.0f);
		glVertex3f(280.0f, 40.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.5f, 0.1f);
		glVertex3f(330.0f, 32.0f, 21.0f);
		glVertex3f(280.0f, 32.0f, 21.0f);
		glVertex3f(280.0f, 32.0f,  4.0f);
		glVertex3f(330.0f, 32.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.6f, 0.2f);
		glVertex3f(330.0f, 32.0f, 21.0f);
		glVertex3f(330.0f, 32.0f,  4.0f);
		glVertex3f(330.0f, 40.0f,  4.0f);
		glVertex3f(330.0f, 40.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.6f, 0.2f);
		glVertex3f(280.0f, 32.0f,  4.0f);
		glVertex3f(280.0f, 32.0f, 21.0f);
		glVertex3f(280.0f, 40.0f, 21.0f);
		glVertex3f(280.0f, 40.0f,  4.0f);
	glEnd();
	
	//OBJETO 17 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(280.0f, 40.0f, 4.0f);
		glVertex3f(330.0f, 40.0f, 4.0f);
		glVertex3f(330.0f, 45.0f, 4.0f);
		glVertex3f(280.0f, 45.0f, 4.0f);
	glEnd();

	//OBJETO 18 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.3f, 0.0f);
		glVertex3f(348.0f, 44.0f, 21.0f);
		glVertex3f(397.0f, 44.0f, 21.0f);
		glVertex3f(397.0f, 52.0f, 21.0f);
		glVertex3f(348.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.4f, 0.1f);
		glVertex3f(348.0f, 52.0f, 21.0f);
		glVertex3f(397.0f, 52.0f, 21.0f);
		glVertex3f(397.0f, 52.0f,  4.0f);
		glVertex3f(348.0f, 52.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.1f, 0.0f);
		glVertex3f(397.0f, 44.0f, 21.0f);
		glVertex3f(348.0f, 44.0f, 21.0f);
		glVertex3f(348.0f, 44.0f,  4.0f);
		glVertex3f(397.0f, 44.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.2f, 0.0f);
		glVertex3f(397.0f, 44.0f, 21.0f);
		glVertex3f(397.0f, 44.0f,  4.0f);
		glVertex3f(397.0f, 52.0f,  4.0f);
		glVertex3f(397.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.2f, 0.0f);
		glVertex3f(348.0f, 44.0f,  4.0f);
		glVertex3f(348.0f, 44.0f, 21.0f);
		glVertex3f(348.0f, 52.0f, 21.0f);
		glVertex3f(348.0f, 52.0f,  4.0f);
	glEnd();

	//OBJETO 19 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.9f, 0.9f, 0.0f);
		glVertex3f(370.0f, 52.0f, 7.0f);
		glVertex3f(373.0f, 52.0f, 7.0f);
		glVertex3f(373.0f, 78.0f, 7.0f);
		glVertex3f(370.0f, 78.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.0f);
		glVertex3f(373.0f,  52.0f, 7.0f);
		glVertex3f(373.0f,  52.0f, 0.0f);
		glVertex3f(373.0f, 140.0f, 0.0f);
		glVertex3f(373.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.0f);
		glVertex3f(370.0f,  52.0f, 0.0f);
		glVertex3f(370.0f,  52.0f, 7.0f);
		glVertex3f(370.0f, 140.0f, 7.0f);
		glVertex3f(370.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 20 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(370.0f,  78.0f, 25.0f);
		glVertex3f(373.0f,  78.0f, 25.0f);
		glVertex3f(373.0f, 140.0f, 25.0f);
		glVertex3f(370.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(373.0f, 78.0f, 25.0f);
		glVertex3f(370.0f, 78.0f, 25.0f);
		glVertex3f(370.0f, 78.0f,  7.0f);
		glVertex3f(373.0f, 78.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(373.0f,  78.0f, 25.0f);
		glVertex3f(373.0f,  78.0f,  7.0f);
		glVertex3f(373.0f, 140.0f,  7.0f);
		glVertex3f(373.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(370.0f,  78.0f,  7.0f);
		glVertex3f(370.0f,  78.0f, 25.0f);
		glVertex3f(370.0f, 140.0f, 25.0f);
		glVertex3f(370.0f, 140.0f,  7.0f);
	glEnd();

	//OBJETO 21 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(348.0f, 52.0f, 4.0f);
		glVertex3f(397.0f, 52.0f, 4.0f);
		glVertex3f(397.0f, 57.0f, 4.0f);
		glVertex3f(348.0f, 57.0f, 4.0f);
	glEnd();

	//OBJETO 22 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(  56.0f,  82.0f, 18.0f);
		glVertex3f(2020.0f,  82.0f, 18.0f);
		glVertex3f(2020.0f, 140.0f, 18.0f);
		glVertex3f(  56.0f, 140.0f, 18.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(2020.0f, 82.0f, 18.0f);
		glVertex3f(  56.0f, 82.0f, 18.0f);
		glVertex3f(  56.0f, 82.0f,  0.0f);
		glVertex3f(2020.0f, 82.0f,  0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(2020.0f,  82.0f, 18.0f);
		glVertex3f(2020.0f,  82.0f,  0.0f);
		glVertex3f(2020.0f, 140.0f,  0.0f);
		glVertex3f(2020.0f, 140.0f, 18.0f);
	glEnd();

	//OBJETO 23 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f( 56.0f, 10.0f, 0.0f);
		glVertex3f(900.0f, 10.0f, 0.0f);
		glVertex3f(900.0f, 82.0f, 0.0f);
		glVertex3f( 56.0f, 82.0f, 0.0f);
	glEnd();

	//OBJETO 24 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.5f, 0.5f);
		glVertex3f(400.0f,  0.0f, 25.0f);
		glVertex3f(465.0f,  0.0f, 25.0f);
		glVertex3f(465.0f, 25.0f, 25.0f);
		glVertex3f(400.0f, 25.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.6f, 0.6f);
		glVertex3f(400.0f, 25.0f, 25.0f);
		glVertex3f(465.0f, 25.0f, 25.0f);
		glVertex3f(465.0f, 25.0f,  0.0f);
		glVertex3f(400.0f, 25.0f,  0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.4f, 0.4f);
		glVertex3f(400.0f,  0.0f,  0.0f);
		glVertex3f(400.0f,  0.0f, 25.0f);
		glVertex3f(400.0f, 25.0f, 25.0f);
		glVertex3f(400.0f, 25.0f,  0.0f);
	glEnd();

	//OBJETO 25 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.2f, 0.2f, 0.8f);
		glVertex3f(465.0f,  0.0f, 25.0f);
		glVertex3f(585.0f,  0.0f, 25.0f);
		glVertex3f(585.0f, 40.0f, 25.0f);
		glVertex3f(465.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.3f, 0.3f, 0.9f);
		glVertex3f(465.0f, 40.0f, 25.0f);
		glVertex3f(585.0f, 40.0f, 25.0f);
		glVertex3f(585.0f, 40.0f,  0.0f);
		glVertex3f(465.0f, 40.0f,  0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.1f, 0.7f);
		glVertex3f(585.0f,  0.0f, 25.0f);
		glVertex3f(585.0f,  0.0f,  0.0f);
		glVertex3f(585.0f, 40.0f,  0.0f);
		glVertex3f(585.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.1f, 0.7f);
		glVertex3f(465.0f,  0.0f,  0.0f);
		glVertex3f(465.0f,  0.0f, 25.0f);
		glVertex3f(465.0f, 40.0f, 25.0f);
		glVertex3f(465.0f, 40.0f,  0.0f);
	glEnd();

	//OBJETO 26 (2 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.5f);
		glVertex3f(585.0f,  0.0f, 25.0f);
		glVertex3f(850.0f,  0.0f, 25.0f);
		glVertex3f(850.0f, 10.0f, 25.0f);
		glVertex3f(585.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.0f, 0.6f);
		glVertex3f(585.0f, 10.0f, 25.0f);
		glVertex3f(850.0f, 10.0f, 25.0f);
		glVertex3f(850.0f, 10.0f,  0.0f);
		glVertex3f(585.0f, 10.0f,  0.0f);
	glEnd();

	//OBJETO 27 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(585.0f, 10.0f, 4.0f);
		glVertex3f(850.0f, 10.0f, 4.0f);
		glVertex3f(850.0f, 15.0f, 4.0f);
		glVertex3f(585.0f, 15.0f, 4.0f);
	glEnd();

	//OBJETO 28 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.9f, 0.0f);
		glVertex3f(600.0f, 44.0f, 21.0f);
		glVertex3f(710.0f, 44.0f, 21.0f);
		glVertex3f(710.0f, 52.0f, 21.0f);
		glVertex3f(600.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.8f, 1.0f, 0.1f);
		glVertex3f(600.0f, 52.0f, 21.0f);
		glVertex3f(710.0f, 52.0f, 21.0f);
		glVertex3f(710.0f, 52.0f,  4.0f);
		glVertex3f(600.0f, 52.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.7f, 0.0f);
		glVertex3f(710.0f, 44.0f, 21.0f);
		glVertex3f(600.0f, 44.0f, 21.0f);
		glVertex3f(600.0f, 44.0f,  4.0f);
		glVertex3f(710.0f, 44.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.8f, 0.0f);
		glVertex3f(710.0f, 44.0f, 21.0f);
		glVertex3f(710.0f, 44.0f,  4.0f);
		glVertex3f(710.0f, 52.0f,  4.0f);
		glVertex3f(710.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.8f, 0.0f);
		glVertex3f(600.0f, 44.0f,  4.0f);
		glVertex3f(600.0f, 44.0f, 21.0f);
		glVertex3f(600.0f, 52.0f, 21.0f);
		glVertex3f(600.0f, 52.0f,  4.0f);
	glEnd();

	//OBJETO 29 (1 plano) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(600.0f, 52.0f, 4.0f);
		glVertex3f(710.0f, 52.0f, 4.0f);
		glVertex3f(710.0f, 57.0f, 4.0f);
		glVertex3f(600.0f, 57.0f, 4.0f);
	glEnd();

	//OBJETO 30 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.9f, 0.0f, 0.0f);
		glVertex3f(750.0f, 32.0f, 21.0f);
		glVertex3f(780.0f, 32.0f, 21.0f);
		glVertex3f(780.0f, 40.0f, 21.0f);
		glVertex3f(750.0f, 40.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(750.0f, 40.0f, 21.0f);
		glVertex3f(780.0f, 40.0f, 21.0f);
		glVertex3f(780.0f, 40.0f,  4.0f);
		glVertex3f(750.0f, 40.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.0f, 0.0f);
		glVertex3f(780.0f, 32.0f, 21.0f);
		glVertex3f(750.0f, 32.0f, 21.0f);
		glVertex3f(750.0f, 32.0f,  4.0f);
		glVertex3f(780.0f, 32.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.0f, 0.0f);
		glVertex3f(780.0f, 32.0f, 21.0f);
		glVertex3f(780.0f, 32.0f,  4.0f);
		glVertex3f(780.0f, 40.0f,  4.0f);
		glVertex3f(780.0f, 40.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.0f, 0.0f);
		glVertex3f(750.0f, 32.0f,  4.0f);
		glVertex3f(750.0f, 32.0f, 21.0f);
		glVertex3f(750.0f, 40.0f, 21.0f);
		glVertex3f(750.0f, 40.0f,  4.0f);
	glEnd();

	//OBJETO 31 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.9f, 0.0f);
		glVertex3f(780.0f, 44.0f, 21.0f);
		glVertex3f(810.0f, 44.0f, 21.0f);
		glVertex3f(810.0f, 52.0f, 21.0f);
		glVertex3f(780.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(780.0f, 52.0f, 21.0f);
		glVertex3f(810.0f, 52.0f, 21.0f);
		glVertex3f(810.0f, 52.0f,  4.0f);
		glVertex3f(780.0f, 52.0f,  4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.7f, 0.0f);
		glVertex3f(810.0f, 44.0f, 21.0f);
		glVertex3f(780.0f, 44.0f, 21.0f);
		glVertex3f(780.0f, 44.0f,  4.0f);
		glVertex3f(810.0f, 44.0f,  4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.0f);
		glVertex3f(810.0f, 44.0f, 21.0f);
		glVertex3f(810.0f, 44.0f,  4.0f);
		glVertex3f(810.0f, 52.0f,  4.0f);
		glVertex3f(810.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.0f);
		glVertex3f(780.0f, 44.0f,  4.0f);
		glVertex3f(780.0f, 44.0f, 21.0f);
		glVertex3f(780.0f, 52.0f, 21.0f);
		glVertex3f(780.0f, 52.0f,  4.0f);
	glEnd();

	//OBJETO 32 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.1f, 0.0f);
		glVertex3f(810.0f, 10.0f, 7.0f);
		glVertex3f(813.0f, 10.0f, 7.0f);
		glVertex3f(813.0f, 40.0f, 7.0f);
		glVertex3f(810.0f, 40.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(813.0f,  10.0f, 7.0f);
		glVertex3f(813.0f,  10.0f, 0.0f);
		glVertex3f(813.0f, 140.0f, 0.0f);
		glVertex3f(813.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(810.0f,  10.0f, 0.0f);
		glVertex3f(810.0f,  10.0f, 7.0f);
		glVertex3f(810.0f, 140.0f, 7.0f);
		glVertex3f(810.0f, 140.0f, 0.0f);
	glEnd();

	//OBJETO 33 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(810.0f,  40.0f, 25.0f);
		glVertex3f(813.0f,  40.0f, 25.0f);
		glVertex3f(813.0f, 140.0f, 25.0f);
		glVertex3f(810.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(813.0f, 40.0f, 25.0f);
		glVertex3f(810.0f, 40.0f, 25.0f);
		glVertex3f(810.0f, 40.0f,  7.0f);
		glVertex3f(813.0f, 40.0f,  7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(813.0f,  40.0f, 25.0f);
		glVertex3f(813.0f,  40.0f,  7.0f);
		glVertex3f(813.0f, 140.0f,  7.0f);
		glVertex3f(813.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(810.0f,  40.0f,  7.0f);
		glVertex3f(810.0f,  40.0f, 25.0f);
		glVertex3f(810.0f, 140.0f, 25.0f);
		glVertex3f(810.0f, 140.0f,  7.0f);
	glEnd();


	//Continuar creando el escenario aquí (objetos 34 a 70)
	//OBJETO 34 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.098f, 0.098f, 0.439f);
		glVertex3f(850.0f, 0.0f, 25.0f);
		glVertex3f(900.0f, 0.0f, 25.0f);
		glVertex3f(900.0f, 25.0f, 25.0f);
		glVertex3f(850.0f, 25.0f, 25.0f);	
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.098f, 0.098f, 0.439f);
		glVertex3f(850.0f, 25.0f, 25.0f);
		glVertex3f(900.0f, 25.0f, 25.0f);
		glVertex3f(900.0f, 25.0f, 0.0f);
		glVertex3f(850.0f, 25.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.098f, 0.098f, 0.439f);
		glVertex3f(850.0f, 0.0f, 25.0f);
		glVertex3f(850.0f, 25.0f, 25.0f);
		glVertex3f(850.0f, 25.0f, 0.0f);
		glVertex3f(850.0f, 0.0f, 0.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.098f, 0.098f, 0.439f);
		glVertex3f(900.0f, 0.0f, 25.0f);
		glVertex3f(900.0f, 0.0f, 0.0f);
		glVertex3f(900.0f, 25.0f, 0.0f);
		glVertex3f(900.0f, 25.0f, 25.0f);
	glEnd();

	//OBJETO 35 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(850.0f, 25.0f, 4.0f);
		glVertex3f(900.0f, 25.0f, 4.0f);
		glVertex3f(900.0f, 30.0f, 4.0f);
		glVertex3f(850.0f, 30.0f, 4.0f);
	glEnd();

	//OBJETO 36 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.118f, 0.565f, 1.000f);
		glVertex3f(900.0f, 0.0f, 25.0f);
		glVertex3f(975.0f, 0.0f, 25.0f);
		glVertex3f(975.0f, 40.0f, 25.0f);
		glVertex3f(900.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.118f, 0.565f, 1.000f);
		glVertex3f(900.0f, 40.0f, 25.0f);
		glVertex3f(975.0f, 40.0f, 25.0f);
		glVertex3f(975.0f, 40.0f, 0.0f);
		glVertex3f(900.0f, 40.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.118f, 0.565f, 1.000f);
		glVertex3f(900.0f, 0.0f, 25.0f);
		glVertex3f(900.0f, 40.0f, 25.0f);
		glVertex3f(900.0f, 40.0f, 0.0f);
		glVertex3f(900.0f, 0.0f, 0.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.118f, 0.565f, 1.000f);
		glVertex3f(975.0f, 0.0f, 25.0f);
		glVertex3f(975.0f, 0.0f, 0.0f);
		glVertex3f(975.0f, 40.0f, 0.0f);
		glVertex3f(975.0f, 40.0f, 25.0f);
		
	glEnd();

	//OBJETO 37 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(900.0f, 40.0f, 4.0f);
		glVertex3f(975.0f, 40.0f, 4.0f);
		glVertex3f(975.0f, 45.0f, 4.0f);
		glVertex3f(900.0f, 45.0f, 4.0f);
	glEnd();

	//OBJETO 38 (3 planos) ///////////////
	
	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex3f(975.0f, 0.0f, 25.0f);
		glVertex3f(1370.0f, 0.0f, 25.0f);
		glVertex3f(1370.0f, 10.0f, 25.0f);
		glVertex3f(975.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex3f(975.0f, 10.0f, 25.0f);
		glVertex3f(1370.0f, 10.0f, 25.0f);
		glVertex3f(1370.0f, 10.0f, 0.0f);
		glVertex3f(975.0f, 10.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.8f, 0.8f);
		glVertex3f(1370.0f, 0.0f, 25.0f);
		glVertex3f(1370.0f, 0.0f, 0.0f);
		glVertex3f(1370.0f, 10.0f, 0.0f);
		glVertex3f(1370.0f, 10.0f, 25.0f);
	glEnd();

	//OBJETO 39 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(975.0f, 10.0f, 0.0f);
		glVertex3f(1410.0f, 10.0f, 0.0f);
		glVertex3f(1410.0f, 82.0f, 0.0f);
		glVertex3f(975.0f, 82.0f, 0.0f);
	glEnd();

	//OBJETO 40 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(990.0f, 50.0f, 19.0f);
		glVertex3f(1050.0f, 50.0f, 19.0f);
		glVertex3f(1050.0f, 70.0f, 19.0f);
		glVertex3f(990.0f, 70.0f, 19.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(1050.0f, 50.0f, 19.0f);
		glVertex3f(1050.0f, 50.0f, 6.0f);
		glVertex3f(1050.0f, 70.0f, 6.0f);
		glVertex3f(1050.0f, 70.0f, 19.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(990.0f, 50.0f, 19.0f);
		glVertex3f(990.0f, 70.0f, 19.0f);
		glVertex3f(990.0f, 70.0f, 6.0f);
		glVertex3f(990.0f, 50.0f, 6.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(990.0f, 70.0f, 19.0f);
		glVertex3f(1050.0f, 70.0f, 19.0f);
		glVertex3f(1050.0f, 70.0f, 6.0f);
		glVertex3f(990.0f, 70.0f, 6.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(990.0f, 50.0f, 19.0f);
		glVertex3f(990.0f, 50.0f, 6.0f);
		glVertex3f(1050.0f, 50.0f, 6.0f);
		glVertex3f(1050.0f, 50.0f, 19.0f);
	glEnd();

	//OBJETO 41 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.1f, 0.0f);
		glVertex3f(1070.0f, 10.0f, 7.0f);
		glVertex3f(1073.0f, 10.0f, 7.0f);
		glVertex3f(1073.0f, 140.0f, 7.0f);
		glVertex3f(1070.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(1073.0f, 10.0f, 7.0f);
		glVertex3f(1073.0f, 10.0f, 0.0f);
		glVertex3f(1073.0f, 140.0f, 0.0f);
		glVertex3f(1073.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.0f, 0.0f);
		glVertex3f(1070.0f, 10.0f, 7.0f);
		glVertex3f(1070.0f, 140.0f, 7.0f);
		glVertex3f(1070.0f, 140.0f, 0.0f);
		glVertex3f(1070.0f, 10.0f, 0.0f);
	glEnd();

	//OBJETO 42 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.6f, 0.3f);
		glVertex3f(1070.0f, 40.0f, 25.0f);
		glVertex3f(1073.0f, 40.0f, 25.0f);
		glVertex3f(1073.0f, 140.0f, 25.0f);
		glVertex3f(1070.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.5f, 0.2f);
		glVertex3f(1073.0f, 40.0f, 25.0f);
		glVertex3f(1073.0f, 40.0f, 7.0f);
		glVertex3f(1073.0f, 140.0f, 7.0f);
		glVertex3f(1073.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(1070.0f, 40.0f, 25.0f);
		glVertex3f(1070.0f, 140.0f, 25.0f);
		glVertex3f(1070.0f, 140.0f, 7.0f);
		glVertex3f(1070.0f, 40.0f, 7.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.6f, 0.4f, 0.1f);
		glVertex3f(1070.0f, 40.0f, 25.0f);
		glVertex3f(1070.0f, 40.0f, 7.0f);
		glVertex3f(1073.0f, 40.0f, 7.0f);
		glVertex3f(1073.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 43 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.25f, 0.29f, 0.44f);
		glVertex3f(1090.0f, 50.0f, 19.0f);
		glVertex3f(1150.0f, 50.0f, 19.0f);
		glVertex3f(1150.0f, 70.0f, 19.0f);
		glVertex3f(1090.0f, 70.0f, 19.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
		glColor3f(0.25f, 0.29f, 0.44f);
		glVertex3f(1150.0f, 50.0f, 19.0f);
		glVertex3f(1150.0f, 50.0f, 6.0f);
		glVertex3f(1150.0f, 70.0f, 6.0f);
		glVertex3f(1150.0f, 70.0f, 19.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
		glColor3f(0.18f, 0.18f, 0.24f);
		glVertex3f(1090.0f, 50.0f, 19.0f);
		glVertex3f(1090.0f, 70.0f, 19.0f);
		glVertex3f(1090.0f, 70.0f, 6.0f);
		glVertex3f(1090.0f, 50.0f, 6.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
		glColor3f(0.25f, 0.29f, 0.44f);
		glVertex3f(1090.0f, 70.0f, 19.0f);
		glVertex3f(1150.0f, 70.0f, 19.0f);
		glVertex3f(1150.0f, 70.0f, 6.0f);
		glVertex3f(1090.0f, 70.0f, 6.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
		glColor3f(0.25f, 0.29f, 0.44f);
		glVertex3f(1090.0f, 50.0f, 19.0f);
		glVertex3f(1090.0f, 50.0f, 6.0f);
		glVertex3f(1150.0f, 50.0f, 6.0f);
		glVertex3f(1150.0f, 50.0f, 19.0f);
	glEnd();

	//OBJETO 44 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.1f, 0.0f);
	glVertex3f(1190.0f, 10.0f, 7.0f);
	glVertex3f(1193.0f, 10.0f, 7.0f);
	glVertex3f(1193.0f, 140.0f, 7.0f);
	glVertex3f(1190.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1193.0f, 10.0f, 7.0f);
	glVertex3f(1193.0f, 10.0f, 0.0f);
	glVertex3f(1193.0f, 140.0f, 0.0f);
	glVertex3f(1193.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1190.0f, 10.0f, 7.0f);
	glVertex3f(1190.0f, 140.0f, 7.0f);
	glVertex3f(1190.0f, 140.0f, 0.0f);
	glVertex3f(1190.0f, 10.0f, 0.0f);
	glEnd();

	//OBJETO 45 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.6f, 0.3f);
	glVertex3f(1190.0f, 40.0f, 25.0f);
	glVertex3f(1193.0f, 40.0f, 25.0f);
	glVertex3f(1193.0f, 140.0f, 25.0f);
	glVertex3f(1190.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.5f, 0.2f);
	glVertex3f(1193.0f, 40.0f, 25.0f);
	glVertex3f(1193.0f, 40.0f, 7.0f);
	glVertex3f(1193.0f, 140.0f, 7.0f);
	glVertex3f(1193.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.4f, 0.1f);
	glVertex3f(1190.0f, 40.0f, 25.0f);
	glVertex3f(1190.0f, 140.0f, 25.0f);
	glVertex3f(1190.0f, 140.0f, 7.0f);
	glVertex3f(1190.0f, 40.0f, 7.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.4f, 0.1f);
	glVertex3f(1190.0f, 40.0f, 25.0f);
	glVertex3f(1190.0f, 40.0f, 7.0f);
	glVertex3f(1193.0f, 40.0f, 7.0f);
	glVertex3f(1193.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 46 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(1275.0f, 22.0f, 21.0f);
	glVertex3f(1325.0f, 22.0f, 21.0f);
	glVertex3f(1325.0f, 30.0f, 21.0f);
	glVertex3f(1275.0f, 30.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(1325.0f, 22.0f, 21.0f);
	glVertex3f(1325.0f, 22.0f, 4.0f);
	glVertex3f(1325.0f, 30.0f, 4.0f);
	glVertex3f(1325.0f, 30.0f, 21.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.4f, 0.0f, 0.0f);
	glVertex3f(1275.0f, 22.0f, 21.0f);
	glVertex3f(1275.0f, 30.0f, 21.0f);
	glVertex3f(1275.0f, 30.0f, 4.0f);
	glVertex3f(1275.0f, 22.0f, 4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(1275.0f, 30.0f, 21.0f);
	glVertex3f(1325.0f, 30.0f, 21.0f);
	glVertex3f(1325.0f, 30.0f, 4.0f);
	glVertex3f(1275.0f, 30.0f, 4.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.0f, 0.0f);
	glVertex3f(1275.0f, 22.0f, 21.0f);
	glVertex3f(1275.0f, 22.0f, 4.0f);
	glVertex3f(1325.0f, 22.0f, 4.0f);
	glVertex3f(1325.0f, 22.0f, 21.0f);
	glEnd();

	//OBJETO 47 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.67f, 0.99f, 0.45f);
	glVertex3f(1345.0f, 34.0f, 21.0f);
	glVertex3f(1390.0f, 34.0f, 21.0f);
	glVertex3f(1390.0f, 42.0f, 21.0f);
	glVertex3f(1345.0f, 42.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.67f, 0.99f, 0.45f);
	glVertex3f(1390.0f, 34.0f, 21.0f);
	glVertex3f(1390.0f, 34.0f, 4.0f);
	glVertex3f(1390.0f, 42.0f, 4.0f);
	glVertex3f(1390.0f, 42.0f, 21.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.34f, 0.47f, 0.27f);
	glVertex3f(1345.0f, 34.0f, 21.0f);
	glVertex3f(1345.0f, 42.0f, 21.0f);
	glVertex3f(1345.0f, 42.0f, 4.0f);
	glVertex3f(1345.0f, 34.0f, 4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.67f, 0.99f, 0.45f);
	glVertex3f(1345.0f, 42.0f, 21.0f);
	glVertex3f(1390.0f, 42.0f, 21.0f);
	glVertex3f(1390.0f, 42.0f, 4.0f);
	glVertex3f(1345.0f, 42.0f, 4.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
	glColor3f(0.67f, 0.99f, 0.45f);
	glVertex3f(1345.0f, 34.0f, 21.0f);
	glVertex3f(1345.0f, 34.0f, 4.0f);
	glVertex3f(1390.0f, 34.0f, 4.0f);
	glVertex3f(1390.0f, 34.0f, 21.0f);
	glEnd();

	//OBJETO 48 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.45f, 0.99f, 0.67f);
	glVertex3f(1405.0f, 0.0f, 25.0f);
	glVertex3f(1650.0f, 0.0f, 25.0f);
	glVertex3f(1650.0f, 10.0f, 25.0f);
	glVertex3f(1405.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.45f, 0.99f, 0.67f);
	glVertex3f(1405.0f, 10.0f, 25.0f);
	glVertex3f(1650.0f, 10.0f, 25.0f);
	glVertex3f(1650.0f, 10.0f, 0.0f);
	glVertex3f(1405.0f, 10.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.24f, 0.47f, 0.43f);
	glVertex3f(1405.0f, 0.0f, 25.0f);
	glVertex3f(1405.0f, 10.0f, 25.0f);
	glVertex3f(1405.0f, 10.0f, 0.0f);
	glVertex3f(1405.0f, 0.0f, 0.0f);
	glEnd();


	//OBJETO 49 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.2f, 0.1f);
	glVertex3f(1405.0f, 10.0f, 4.0f);
	glVertex3f(1650.0f, 10.0f, 4.0f);
	glVertex3f(1650.0f, 15.0f, 4.0f);
	glVertex3f(1405.0f, 15.0f, 4.0f);
	glEnd();

	//OBJETO 50 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.14f);
	glVertex3f(1410.0f, 44.0f, 21.0f);
	glVertex3f(1600.0f, 44.0f, 21.0f);
	glVertex3f(1600.0f, 52.0f, 21.0f);
	glVertex3f(1410.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.14f);
	glVertex3f(1410.0f, 52.0f, 21.0f);
	glVertex3f(1600.0f, 52.0f, 21.0f);
	glVertex3f(1600.0f, 52.0f, 4.0f);
	glVertex3f(1410.0f, 52.0f, 4.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.14f);
	glVertex3f(1600.0f, 44.0f, 21.0f);
	glVertex3f(1410.0f, 44.0f, 21.0f);
	glVertex3f(1410.0f, 44.0f, 4.0f);
	glVertex3f(1600.0f, 44.0f, 4.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 0.14f);
	glVertex3f(1600.0f, 44.0f, 21.0f);
	glVertex3f(1600.0f, 44.0f, 4.0f);
	glVertex3f(1600.0f, 52.0f, 4.0f);
	glVertex3f(1600.0f, 52.0f, 21.0f);
	glEnd();

	//Plano 5
	glBegin(GL_QUADS);
	glColor3f(0.51f, 0.51f, 0.13f);
	glVertex3f(1410.0f, 44.0f, 4.0f);
	glVertex3f(1410.0f, 44.0f, 21.0f);
	glVertex3f(1410.0f, 52.0f, 21.0f);
	glVertex3f(1410.0f, 52.0f, 4.0f);
	glEnd();

	//OBJETO 51 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.99f, 0.45f);
	glVertex3f(1650.0f, 0.0f, 25.0f);
	glVertex3f(1705.0f, 0.0f, 25.0f);
	glVertex3f(1705.0f, 25.0f, 25.0f);
	glVertex3f(1650.0f, 25.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.99f, 0.45f);
	glVertex3f(1650.0f, 25.0f, 25.0f);
	glVertex3f(1705.0f, 25.0f, 25.0f);
	glVertex3f(1705.0f, 25.0f, 0.0f);
	glVertex3f(1650.0f, 25.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.42f, 0.46f, 0.25f);
	glVertex3f(1650.0f, 0.0f, 25.0f);
	glVertex3f(1650.0f, 25.0f, 25.0f);
	glVertex3f(1650.0f, 25.0f, 0.0f);
	glVertex3f(1650.0f, 0.0f, 0.0f);
	glEnd();
	
	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.99f, 0.45f);
	glVertex3f(1705.0f, 0.0f, 25.0f);
	glVertex3f(1705.0f, 0.0f, 0.0f);
	glVertex3f(1705.0f, 25.0f, 0.0f);
	glVertex3f(1705.0f, 25.0f, 25.0f);
	glEnd();

	//OBJETO 52 (1 planos) /////////////// NOGGGG

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(1650.0f, 25.0f, 4.0f);
		glVertex3f(1705.0f, 25.0f, 4.0f);
		glVertex3f(1705.0f, 30.0f, 4.0f);
		glVertex3f(1650.0f, 30.0f, 4.0f);
	glEnd();

	//OBJETO 53 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.71f, 1.0f, 0.83f);
	glVertex3f(1705.0f, 0.0f, 25.0f);
	glVertex3f(1815.0f, 0.0f, 25.0f);
	glVertex3f(1815.0f, 40.0f, 25.0f);
	glVertex3f(1705.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.71f, 1.0f, 0.83f);
	glVertex3f(1705.0f, 40.0f, 25.0f);
	glVertex3f(1815.0f, 40.0f, 25.0f);
	glVertex3f(1815.0f, 40.0f, 0.0f);
	glVertex3f(1705.0f, 40.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.35f, 0.47f, 0.40f);
	glVertex3f(1705.0f, 0.0f, 25.0f);
	glVertex3f(1705.0f, 40.0f, 25.0f);
	glVertex3f(1705.0f, 40.0f, 0.0f);
	glVertex3f(1705.0f, 0.0f, 0.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.71f, 1.0f, 0.83f);
	glVertex3f(1815.0f, 0.0f, 25.0f);
	glVertex3f(1815.0f, 0.0f, 0.0f);
	glVertex3f(1815.0f, 40.0f, 0.0f);
	glVertex3f(1815.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 54 (1 planos) /////////////// NOGGG

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.7f, 0.2f, 0.1f);
		glVertex3f(1705.0f, 40.0f, 4.0f);
		glVertex3f(1815.0f, 40.0f, 4.0f);
		glVertex3f(1815.0f, 45.0f, 4.0f);
		glVertex3f(1705.0f, 45.0f, 4.0f);
	glEnd();

	//OBJETO 55 (1 planos) ///////////////  CAMBIO Z

	//Plano 1
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(1600.0f, 10.0f, 0.0f);
		glVertex3f(2020.0f, 10.0f, 0.0f);
		glVertex3f(2020.0f, 82.0f, 0.0f);
		glVertex3f(1600.0f, 82.0f, 0.0f);
	glEnd();

	//OBJETO 56 (2 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.89f, 0.46f);
	glVertex3f(1815.0f, 0.0f, 25.0f);
	glVertex3f(2020.0f, 0.0f, 25.0f);
	glVertex3f(2020.0f, 10.0f, 25.0f);
	glVertex3f(1815.0f, 10.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.89f, 0.46f);
	glVertex3f(1815.0f, 10.0f, 25.0f);
	glVertex3f(2020.0f, 10.0f, 25.0f);
	glVertex3f(2020.0f, 10.0f, 0.0f);
	glVertex3f(1815.0f, 10.0f, 0.0f);
	glEnd();

	//OBJETO 57 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.2f, 0.1f);
	glVertex3f(1815.0f, 10.0f, 4.0f);
	glVertex3f(2020.0f, 10.0f, 4.0f);
	glVertex3f(2020.0f, 15.0f, 4.0f);
	glVertex3f(1815.0f, 15.0f, 4.0f);
	glEnd();

	//OBJETO 58 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.1f, 0.0f);
	glVertex3f(1870.0f, 10.0f, 7.0f);
	glVertex3f(1873.0f, 10.0f, 7.0f);
	glVertex3f(1873.0f, 140.0f, 7.0f);
	glVertex3f(1870.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1873.0f, 10.0f, 7.0f);
	glVertex3f(1873.0f, 10.0f, 0.0f);
	glVertex3f(1873.0f, 140.0f, 0.0f);
	glVertex3f(1873.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.0f, 0.0f);
	glVertex3f(1870.0f, 10.0f, 7.0f);
	glVertex3f(1870.0f, 140.0f, 7.0f);
	glVertex3f(1870.0f, 140.0f, 0.0f);
	glVertex3f(1870.0f, 10.0f, 0.0f);
	glEnd();

	//OBJETO 59 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.6f, 0.3f);
	glVertex3f(1870.0f, 40.0f, 25.0f);
	glVertex3f(1873.0f, 40.0f, 25.0f);
	glVertex3f(1873.0f, 140.0f, 25.0f);
	glVertex3f(1870.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.5f, 0.2f);
	glVertex3f(1873.0f, 40.0f, 25.0f);
	glVertex3f(1873.0f, 40.0f, 7.0f);
	glVertex3f(1873.0f, 140.0f, 7.0f);
	glVertex3f(1873.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.4f, 0.1f);
	glVertex3f(1870.0f, 40.0f, 25.0f);
	glVertex3f(1870.0f, 140.0f, 25.0f);
	glVertex3f(1870.0f, 140.0f, 7.0f);
	glVertex3f(1870.0f, 40.0f, 7.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.6f, 0.4f, 0.1f);
	glVertex3f(1870.0f, 40.0f, 25.0f);
	glVertex3f(1870.0f, 40.0f, 7.0f);
	glVertex3f(1873.0f, 40.0f, 7.0f);
	glVertex3f(1873.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 60 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.20f, 0.35f, 0.55f);
	glVertex3f(1897.0f, 10.0f, 7.0f);
	glVertex3f(1900.0f, 10.0f, 7.0f);
	glVertex3f(1900.0f, 140.0f, 7.0f);
	glVertex3f(1897.0f, 140.0f, 7.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.13f, 0.21f, 0.34f);
	glVertex3f(1897.0f, 10.0f, 0.0f);
	glVertex3f(1897.0f, 10.0f, 7.0f);
	glVertex3f(1897.0f, 140.0f, 7.0f);
	glVertex3f(1897.0f, 140.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.20f, 0.35f, 0.55f);
	glVertex3f(1900.0f, 10.0f, 7.0f);
	glVertex3f(1900.0f, 10.0f, 0.0f);
	glVertex3f(1900.0f, 140.0f, 0.0f);
	glVertex3f(1900.0f, 140.0f, 7.0f);
	glEnd();

	//OBJETO 61 (5 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.57f, 0.96f, 0.88f);
	glVertex3f(1897.0f, 40.0f, 25.0f);
	glVertex3f(1900.0f, 40.0f, 25.0f);
	glVertex3f(1900.0f, 140.0f, 25.0f);
	glVertex3f(1897.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.25f, 0.46f, 0.42f);
	glVertex3f(1897.0f, 40.0f, 25.0f);
	glVertex3f(1897.0f, 140.0f, 25.0f);
	glVertex3f(1897.0f, 140.0f, 7.0f);
	glVertex3f(1897.0f, 40.0f, 7.0f);	
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.57f, 0.96f, 0.88f);
	glVertex3f(1900.0f, 40.0f, 25.0f);
	glVertex3f(1900.0f, 40.0f, 7.0f);
	glVertex3f(1900.0f, 140.0f, 7.0f);
	glVertex3f(1900.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.57f, 0.96f, 0.88f);
	glVertex3f(1897.0f, 40.0f, 25.0f);
	glVertex3f(1897.0f, 40.0f, 7.0f);
	glVertex3f(1900.0f, 40.0f, 7.0f);
	glVertex3f(1900.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 62 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.95f, 0.72f, 1.00f);
	glVertex3f(1930.0f, 30.0f, 25.0f);
	glVertex3f(1945.0f, 30.0f, 25.0f);
	glVertex3f(1945.0f, 140.0f, 25.0f);
	glVertex3f(1930.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.95f, 0.72f, 1.00f);
	glVertex3f(1945.0f, 30.0f, 25.0f);
	glVertex3f(1945.0f, 30.0f, 7.0f);
	glVertex3f(1945.0f, 140.0f, 7.0f);
	glVertex3f(1945.0f, 140.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.45f, 0.36f, 0.47f);
	glVertex3f(1930.0f, 30.0f, 25.0f);
	glVertex3f(1930.0f, 140.0f, 25.0f);
	glVertex3f(1930.0f, 140.0f, 7.0f);
	glVertex3f(1930.0f, 30.0f, 7.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.95f, 0.72f, 1.00f);
	glVertex3f(1930.0f, 30.0f, 25.0f);
	glVertex3f(1930.0f, 30.0f, 7.0f);
	glVertex3f(1945.0f, 30.0f, 7.0f);
	glVertex3f(1945.0f, 30.0f, 25.0f);
	glEnd();

	//OBJETO 63 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.12f, 0.11f);
	glVertex3f(1970.0f, 10.0f, 25.0f);
	glVertex3f(1985.0f, 10.0f, 25.0f);
	glVertex3f(1985.0f, 65.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.12f, 0.11f);
	glVertex3f(1985.0f, 10.0f, 25.0f);
	glVertex3f(1985.0f, 10.0f, 7.0f);
	glVertex3f(1985.0f, 65.0f, 7.0f);
	glVertex3f(1985.0f, 65.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.51f, 0.12f, 0.12f);
	glVertex3f(1970.0f, 10.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 7.0f);
	glVertex3f(1970.0f, 10.0f, 7.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.12f, 0.11f);
	glVertex3f(1970.0f, 65.0f, 25.0f);
	glVertex3f(1985.0f, 65.0f, 25.0f);
	glVertex3f(1985.0f, 65.0f, 7.0f);
	glVertex3f(1970.0f, 65.0f, 7.0f);
	glEnd();

	//OBJETO 64 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.22f, 0.22f, 0.72f);
	glVertex3f(1945.0f, 30.0f, 25.0f);
	glVertex3f(1970.0f, 30.0f, 25.0f);
	glVertex3f(1970.0f, 33.0f, 25.0f);
	glVertex3f(1945.0f, 33.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.22f, 0.22f, 0.72f);
	glVertex3f(1945.0f, 30.0f, 25.0f);
	glVertex3f(1945.0f, 30.0f, 7.0f);
	glVertex3f(1970.0f, 30.0f, 7.0f);
	glVertex3f(1970.0f, 30.0f, 25.0f);	
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.22f, 0.22f, 0.72f);
	glVertex3f(1945.0f, 33.0f, 25.0f);
	glVertex3f(1970.0f, 33.0f, 25.0f);
	glVertex3f(1970.0f, 33.0f, 7.0f);
	glVertex3f(1945.0f, 33.0f, 7.0f);
	glEnd();

	//OBJETO 65 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.82f, 0.89f);
	glVertex3f(1945.0f, 46.0f, 25.0f);
	glVertex3f(1970.0f, 46.0f, 25.0f);
	glVertex3f(1970.0f, 49.0f, 25.0f);
	glVertex3f(1945.0f, 49.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.82f, 0.89f);
	glVertex3f(1945.0f, 46.0f, 25.0f);
	glVertex3f(1945.0f, 46.0f, 7.0f);
	glVertex3f(1970.0f, 46.0f, 7.0f);
	glVertex3f(1970.0f, 46.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.82f, 0.89f);
	glVertex3f(1945.0f, 49.0f, 25.0f);
	glVertex3f(1970.0f, 49.0f, 25.0f);
	glVertex3f(1970.0f, 49.0f, 7.0f);
	glVertex3f(1945.0f, 49.0f, 7.0f);
	glEnd();

	//OBJETO 66 (3 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.62f, 0.26f);
	glVertex3f(1945.0f, 62.0f, 25.0f);
	glVertex3f(1970.0f, 62.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 25.0f);
	glVertex3f(1945.0f, 65.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.62f, 0.26f);
	glVertex3f(1945.0f, 62.0f, 25.0f);
	glVertex3f(1945.0f, 62.0f, 7.0f);
	glVertex3f(1970.0f, 62.0f, 7.0f);
	glVertex3f(1970.0f, 62.0f, 25.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.55f, 0.62f, 0.26f);
	glVertex3f(1945.0f, 65.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 25.0f);
	glVertex3f(1970.0f, 65.0f, 7.0f);
	glVertex3f(1945.0f, 65.0f, 7.0f);
	glEnd();

	//OBJETO 67 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.36f, 0.65f, 0.14f);
	glVertex3f(2020.0f, 0.0f, 25.0f);
	glVertex3f(2060.0f, 0.0f, 25.0f);
	glVertex3f(2060.0f, 25.0f, 25.0f);
	glVertex3f(2020.0f, 25.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.36f, 0.65f, 0.14f);
	glVertex3f(2020.0f, 25.0f, 25.0f);
	glVertex3f(2060.0f, 25.0f, 25.0f);
	glVertex3f(2060.0f, 25.0f, 0.0f);
	glVertex3f(2020.0f, 25.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.13f, 0.24f, 0.16f);
	glVertex3f(2020.0f, 0.0f, 25.0f);
	glVertex3f(2020.0f, 25.0f, 25.0f);
	glVertex3f(2020.0f, 25.0f, 0.0f);
	glVertex3f(2020.0f, 0.0f, 0.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.36f, 0.65f, 0.14f);
	glVertex3f(2060.0f, 0.0f, 25.0f);
	glVertex3f(2060.0f, 0.0f, 0.0f);
	glVertex3f(2060.0f, 25.0f, 0.0f);
	glVertex3f(2060.0f, 25.0f, 25.0f);
	glEnd();

	//OBJETO 68 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.2f, 0.1f);
	glVertex3f(2020.0f, 25.0f, 4.0f);
	glVertex3f(2060.0f, 25.0f, 4.0f);
	glVertex3f(2060.0f, 30.0f, 4.0f);
	glVertex3f(2020.0f, 30.0f, 4.0f);
	glEnd();

	//OBJETO 69 (4 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.42f, 0.22f);
	glVertex3f(2060.0f, 0.0f, 25.0f);
	glVertex3f(2270.0f, 0.0f, 25.0f);
	glVertex3f(2270.0f, 40.0f, 25.0f);
	glVertex3f(2060.0f, 40.0f, 25.0f);
	glEnd();

	//Plano 2
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.42f, 0.22f);
	glVertex3f(2060.0f, 40.0f, 25.0f);
	glVertex3f(2270.0f, 40.0f, 25.0f);
	glVertex3f(2270.0f, 40.0f, 0.0f);
	glVertex3f(2060.0f, 40.0f, 0.0f);
	glEnd();

	//Plano 3
	glBegin(GL_QUADS);
	glColor3f(0.13f, 0.24f, 0.17f);
	glVertex3f(2060.0f, 0.0f, 25.0f);
	glVertex3f(2060.0f, 40.0f, 25.0f);
	glVertex3f(2060.0f, 40.0f, 0.0f);
	glVertex3f(2060.0f, 0.0f, 0.0f);
	glEnd();

	//Plano 4
	glBegin(GL_QUADS);
	glColor3f(0.14f, 0.42f, 0.22f);
	glVertex3f(2270.0f, 0.0f, 25.0f);
	glVertex3f(2270.0f, 0.0f, 0.0f);
	glVertex3f(2270.0f, 40.0f, 0.0f);
	glVertex3f(2270.0f, 40.0f, 25.0f);
	glEnd();

	//OBJETO 70 (1 planos) ///////////////

	//Plano 1
	glBegin(GL_QUADS);
	glColor3f(0.7f, 0.2f, 0.1f);
	glVertex3f(2060.0f, 40.0f, 4.0f);
	glVertex3f(2130.0f, 40.0f, 4.0f);
	glVertex3f(2130.0f, 45.0f, 4.0f);
	glVertex3f(2060.0f, 45.0f, 4.0f);
	glEnd();

	//Trampas de piso (1,2,7 y 8 moviles; 3,4,5 y 6 fijas)

	//Trampa 1
	glPushMatrix();
		glTranslatef(420.0f, posGrupoTramp1[0], 12.5f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 2
	glPushMatrix();
		glTranslatef(445.0f, posGrupoTramp1[1], 12.5f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 3
	glPushMatrix();
		glTranslatef(500.0f, 100.0f, 12.5f);
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 4
	glPushMatrix();
		glTranslatef(550.0f, 100.0f, 12.5f);
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 5
	glPushMatrix();
		glTranslatef(1470.0f, 112.0f, 12.5f);
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 6
	glPushMatrix();
		glTranslatef(1540.0f, 112.0f, 12.5f);
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 7
	glPushMatrix();
		glTranslatef(1740.0f, posGrupoTramp2[0], 12.5f);
		dibujaTrampa2();
	glPopMatrix();

	//Trampa 8
	glPushMatrix();
		glTranslatef(1780.0f, posGrupoTramp2[1], 12.5f);
		dibujaTrampa2();
	glPopMatrix();

	// Trampas de techo (CILINDROS)
	glPushMatrix();
		glTranslated(150.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(170.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(190.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(295.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(315.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(620.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(650.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	glPushMatrix();
		glTranslated(680.0f, 76.0f, 12.5f);
		dibujaTrampa1();
	glPopMatrix();

	//Lanzallamas

	//1
	glPushMatrix();
		glTranslatef(1968.0f, 20.0f, 12.5f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		dibujaLanzallamas();
	glPopMatrix();

	//2
	glPushMatrix();
		glTranslatef(1968.0f, 40.0f, 12.5f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		dibujaLanzallamas();
	glPopMatrix();

	//3
	glPushMatrix();
		glTranslatef(1968.0f, 55.0f, 12.5f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
		dibujaLanzallamas();
	glPopMatrix();

	//4
	glPushMatrix();
		glTranslatef(2000.0f, 80.0f, 12.5f);
		dibujaLanzallamas();
	glPopMatrix();

	//Fuego de los lanzallamas
	actualizaTrampas();

	//Se regresa al modo de dibujo sólido en caso de haber usado modo alambrado
	if(render == 2)
		glPolygonMode(GL_FRONT,GL_FILL);

	//Siempre al final se vuelve al color blanco (default de OpenGL) para que otros gráficos no se vean afectados
	glColor3f(1.0f, 1.0f, 1.0f);

	/*glPushMatrix();
		glTranslatef(30.0f, 60.0f, 12.5f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);  // ángulo, x, y, z (del eje a girar) 
		glScalef(2.25f, 2.25f, 2.25f);
		dibujaCono(3.0f, 15, 15.0f, 1);
	glPopMatrix();*/


	
}


int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//gluLookAt(80.0f, 120.0f, 170.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);
	gluLookAt(PosCam.x, PosCam.y, PosCam.z, ObjCam.x, ObjCam.y, ObjCam.z, 0, 1, 0);
	
	DibujaEjes();
	dibujaEscenario(renderModo);

	// CAMBIOS ADICIONALES
	char szWndText[300];
	sprintf(szWndText,"Color(%.3f, %.3f, %.3f)",(float)GetRValue(gColor)/255.0f, (float)GetGValue(gColor)/255.0f, (float)GetBValue(gColor)/255.0f);
	SetWindowText(hWnd,szWndText);
					
	return TRUE;
}

// CAMBIOS ADICIONALES
int RenderizaEscena2(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for(int iNumViewPort=0; iNumViewPort<4; iNumViewPort++)
	{
		if(iNumViewPort == 0)//Puerto 1
		{
			glViewport(0, glHeight*0.5f,glWidth*0.5f,glHeight*0.5f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,(GLfloat)(glWidth*0.5f)/(GLfloat)glHeight,1.0f,2000.0f);
		}
		if(iNumViewPort == 1)//Puerto 2
		{
			glViewport(glWidth*0.5f, glHeight*0.5f, glWidth*0.5f, glHeight*0.5f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,(GLfloat)(glWidth*0.5f)/(GLfloat)glHeight,1.0f,2000.0f);
		}

		if(iNumViewPort == 2)//Puerto 3
		{
			glViewport(0, 0, glWidth*0.5f, glHeight*0.5f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,(GLfloat)(glWidth*0.5f)/(GLfloat)glHeight,1.0f,2000.0f);
		}
		if(iNumViewPort == 3)//Puerto 4
		{
			glViewport(glWidth*0.5f, 0, glWidth*0.5f, glHeight*0.5f);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(45.0f,(GLfloat)(glWidth*0.5f)/(GLfloat)glHeight,1.0f,2000.0f);
		}
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glClear(GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
				
		if(iNumViewPort == 0)
		{	
			gluLookAt(PosCam.x, PosCam.y, PosCam.z, ObjCam.x, ObjCam.y, ObjCam.z, 0, 1, 0);
			DibujaEjes();
			dibujaEscenario(renderModo);
			
			char szWndText[300];
			sprintf(szWndText,"Color(%.3f, %.3f, %.3f)",(float)GetRValue(gColor)/255.0f, (float)GetGValue(gColor)/255.0f, (float)GetBValue(gColor)/255.0f);
			SetWindowText(hWnd,szWndText);
		}

		if(iNumViewPort == 1)
		{	
			gluLookAt(PosCam.x, PosCam.y, 25.0f, 1600.0f, 45.0f, 70.0f, 0, 1, 0);
			DibujaEjes();
			dibujaEscenario(renderModo);
			

			char szWndText[300];
			sprintf(szWndText,"Color(%.3f, %.3f, %.3f)",(float)GetRValue(gColor)/255.0f, (float)GetGValue(gColor)/255.0f, (float)GetBValue(gColor)/255.0f);
			SetWindowText(hWnd,szWndText);
		}

		if(iNumViewPort == 2)
		{	
			gluLookAt(PosCam.x, PosCam.y, 12.0f, 2200.0f, 20.0f, 7.0f, 0, 1, 0);
			DibujaEjes();
			dibujaEscenario(renderModo);
			

			char szWndText[300];
			sprintf(szWndText,"Color(%.3f, %.3f, %.3f)",(float)GetRValue(gColor)/255.0f, (float)GetGValue(gColor)/255.0f, (float)GetBValue(gColor)/255.0f);
			SetWindowText(hWnd,szWndText);
		}

		if(iNumViewPort == 3)
		{	
			gluLookAt(PosCam.x, PosCam.y, 30.0f, -10.0f, 20.0f, 7.0f, 0, 1, 0);
			DibujaEjes();
			dibujaEscenario(renderModo);
			

			char szWndText[300];
			sprintf(szWndText,"Color(%.3f, %.3f, %.3f)",(float)GetRValue(gColor)/255.0f, (float)GetGValue(gColor)/255.0f, (float)GetBValue(gColor)/255.0f);
			SetWindowText(hWnd,szWndText);
		}
	}
					
	return TRUE;
}

GLvoid DestruyeVentanaOGL(GLvoid)						// Elimina la ventana apropiadamente
{
	if (hRC)											// Si existe un contexto de renderizado...
	{
		if (!wglMakeCurrent(NULL,NULL))					// Si no se pueden liberar los contextos DC y RC...
		{
			MessageBox(NULL,"Falla al liberar DC y RC.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Si no se puede eliminar el RC?
		{
			MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// Se pone RC en NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// Si no se puede eliminar el DC
	{
		MessageBox(NULL,"Falla al liberar el contexto de renderizado.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Se pone DC en NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Si no se puede destruir la ventana
	{
		MessageBox(NULL,"No se pudo liberar hWnd.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Se pone hWnd en NULL
	}

	if (!UnregisterClass("OpenGL",hInstance))			// Si no se puede eliminar el registro de la clase
	{
		MessageBox(NULL,"No se pudo eliminar el registro de la clase.","Error de finalización",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Se pone hInstance en NULL
	}
}

//	Este código crea la ventana de OpenGL.  Parámetros:					
//	title			- Titulo en la parte superior de la ventana			
//	width			- Ancho de la ventana								
//	height			- Alto de la ventana								
//	bits			- Número de bits a usar para el color (8/16/24/32)	
  
BOOL CreaVentanaOGL(char* title, int width, int height, int bits)
{
	GLuint	PixelFormat;				// Guarda el resultado despues de determinar el formato a usar
	WNDCLASS	wc;						// Estructura de la clase ventana
	DWORD		dwExStyle;				// Estilo extendido de ventana
	DWORD		dwStyle;				// Estilo de ventana
	RECT		WindowRect;				// Guarda los valores Superior Izquierdo / Inferior Derecho del rectángulo
	WindowRect.left=(long)0;			// Inicia el valor Izquierdo a 0
	WindowRect.right=(long)width;		// Inicia el valor Derecho al ancho especificado
	WindowRect.top=(long)0;				// Inicia el valor Superior a 0
	WindowRect.bottom=(long)height;		// Inicia el valor Inferior al alto especificado

	hInstance			= GetModuleHandle(NULL);				// Guarda una instancia de la ventana
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redibuja el contenido de la ventana al redimensionarla
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// Maneja los mensajes para WndProc
	wc.cbClsExtra		= 0;									// Ningun dato extra para la clase
	wc.cbWndExtra		= 0;									// Ningun dato extra para la ventana
	wc.hInstance		= hInstance;							// Inicia la instancia
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Carga el ícono por defecto
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Carga el puntero de flecha
	wc.hbrBackground	= NULL;									// No se requiere ningun fondo
	wc.lpszMenuName		= NULL;									// No hay menú en la ventana
	wc.lpszClassName	= "OpenGL";								// Fija el nombre de la clase.

	if (!RegisterClass(&wc))									// Intenta registrar la clase de ventana
	{
		MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											
	}
		
	dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;					// Estilo extendido de ventana
	dwStyle=WS_OVERLAPPEDWINDOW;									// Estilo de ventana

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Ajusta la ventana al tamaño especificado

	// Crea la ventana
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// Estilo extendido para la ventana
								"OpenGL",							// Nombre de la clase
								title,								// Título de la ventana
								dwStyle |							// Definición del estilo de la ventana
								WS_CLIPSIBLINGS |					// Estilo requerido de la ventana
								WS_CLIPCHILDREN,					// Estilo requerido de la ventana
								0, 0,								// Posición de la ventana
								WindowRect.right-WindowRect.left,	// Calcula el ancho de la ventana
								WindowRect.bottom-WindowRect.top,	// Calcula el alto de la ventana
								NULL,								// No hay ventana superior
								NULL,								// No hay menú
								hInstance,							// Instancia
								NULL)))								// No se pasa nada a WM_CREATE
	{
		DestruyeVentanaOGL();										// Resetea el despliegue
		MessageBox(NULL,"Error al crear la ventana.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(hDC=GetDC(hWnd)))							// Si no se creo el contexto de dispositivo...
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de dispositivo GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Si Windows no encontró un formato de pixel compatible
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede encontrar un formato de pixel compatible.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Si no se pudo habilitar el formato de pixel
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el formato de pixel.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(hRC=wglCreateContext(hDC)))				// Si no se creo el contexto de renderizado
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede crear un contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if(!wglMakeCurrent(hDC,hRC))					// Si no se puede activar el contexto de renderizado
	{
		DestruyeVentanaOGL();						// Resetea el despliegue
		MessageBox(NULL,"No se puede usar el contexto de renderizado GL.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	ShowWindow(hWnd,SW_SHOW);				// Muestra la ventana
	SetForegroundWindow(hWnd);				// Le da la prioridad mas alta
	SetFocus(hWnd);							// Pasa el foco del teclado a la ventana
	ReDimensionaEscenaGL(width, height);	// Inicia la perspectiva para la ventana OGL

	if (!IniGL())							// Si no se inicializa la ventana creada
	{
		DestruyeVentanaOGL();				// Resetea el despliegue
		MessageBox(NULL,"Falla en la inicialización.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;							// Todo correcto
}

//CAMBIOS ADICIONALES
COLORREF ShowColorDialog(HWND hwnd)
{
	CHOOSECOLOR cc;                 
	static COLORREF crCustClr[16];     

	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hwnd;
	cc.lpCustColors = (LPDWORD) crCustClr;
	cc.rgbResult = RGB(0, 255, 0);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	ChooseColor(&cc);

	return cc.rgbResult;
}

LRESULT CALLBACK WndProc(	HWND	hWnd,	// Manejador para esta ventana
							UINT	uMsg,	// Mensaje para esta ventana
							WPARAM	wParam,	// Información adicional del mensaje
							LPARAM	lParam)	// Información adicional del mensaje
{
	switch (uMsg)							// Revisa los mensajes de la ventana
	{
		case WM_ACTIVATE:					// Revisa el mensaje de activación de ventana
		{
			if (!HIWORD(wParam))			// Revisa el estado de minimización
			{
				active=TRUE;				// El programa está activo
			}
			else
			{
				active=FALSE;				// El programa no está activo
			}

			return 0;						// Regresa al ciclo de mensajes
		}

		case WM_SYSCOMMAND:					// Intercepta comandos del sistema
		{
			switch (wParam)					// Revisa llamadas del sistema
			{
				case SC_SCREENSAVE:			// ¿Screensaver tratando de iniciar?
				case SC_MONITORPOWER:		// ¿Monitor tratando de entrar a modo de ahorro de energía?
				return 0;					// Evita que suceda
			}
			break;							// Sale del caso
		}

		case WM_CLOSE:						// Si se recibe un mensaje de cerrar...
		{
			PostQuitMessage(0);				// Se manda el mensaje de salida
			return 0;						// y se regresa al ciclo
		}

		case WM_KEYDOWN:					// Si se está presionando una tecla...
		{
			keys[wParam] = TRUE;			// Si es así, se marca como TRUE

			// CAMBIOS ADICIONALES
			if(teclaColor)
			{
			gColor = ShowColorDialog(hWnd);
			teclaColor=false;
			}

			return 0;						// y se regresa al ciclo
		}

		case WM_KEYUP:						// ¿Se ha soltado una tecla?
		{
			keys[wParam] = FALSE;			// Si es así, se marca como FALSE
			return 0;						// y se regresa al ciclo
		}

		case WM_SIZE:						// Si se redimensiona la ventana...
		{
			ReDimensionaEscenaGL(LOWORD(lParam),HIWORD(lParam));  	// LoWord=Width, HiWord=Height
			return 0;						// y se regresa al ciclo
		}
	}

	// Pasa todos los mensajes no considerados a DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

// Este es el punto de entrada al programa; la función principal 
int WINAPI WinMain(	HINSTANCE	hInstance,			// Instancia
					HINSTANCE	hPrevInstance,		// Instancia previa
					LPSTR		lpCmdLine,			// Parametros de la linea de comandos
					int			nCmdShow)			// Muestra el estado de la ventana
{
	MSG		msg;									// Estructura de mensajes de la ventana
	BOOL	done=FALSE;								// Variable booleana para salir del ciclo

	// Crea la ventana OpenGL
	if (!CreaVentanaOGL("Laboratorio de Computación Gráfica",640,480,16))
	{
		return 0;									// Salir del programa si la ventana no fue creada
	}

	while(!done)									// Mientras done=FALSE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// Revisa si hay mensajes en espera
		{
			if (msg.message==WM_QUIT)				// Si se ha recibido el mensje de salir...
			{
				done=TRUE;							// Entonces done=TRUE
			}
			else									// Si no, Procesa los mensajes de la ventana
			{
				TranslateMessage(&msg);				// Traduce el mensaje
				DispatchMessage(&msg);				// Envia el mensaje
			}
		}
		else										// Si no hay mensajes...
		{
			// Dibuja la escena. 
			if (active)								// Si está activo el programa...
			{
				if (keys[VK_ESCAPE])				// Si se ha presionado ESC
				{
					done=TRUE;						// ESC indica el termino del programa
				}
				else								// De lo contrario, actualiza la pantalla
				{
					RenderizaEscena();				// Dibuja la escena
					SwapBuffers(hDC);				// Intercambia los Buffers (Double Buffering)
				}

				if(!ManejaTeclado()) return 0;
			}
			
		}
	}

	// Finalización del programa
	DestruyeVentanaOGL();							// Destruye la ventana
	return (msg.wParam);							// Sale del programa
}

int ManejaTeclado()
{
	if(keys[VK_UP])
	{		
		PosCam.y+=0.5f;
		ObjCam.y+=0.5f;
	}
	if(keys[VK_DOWN])
	{
		PosCam.y-=0.5f;
		ObjCam.y-=0.5f;
	}
	if(keys[VK_LEFT])
	{
		PosCam.x-=2.5f;
		ObjCam.x-=2.5f;
	}
	if(keys[VK_RIGHT])
	{
		PosCam.x+=2.0f;
		ObjCam.x+=2.0f;
	}

	if(keys[VK_PRIOR])
	{
		
	}
	if(keys[VK_NEXT])
	{
		
	}

	if(keys[VK_HOME])
	{
		
	}

	if(keys[VK_END])
	{
		
	}

	if(keys['A'])
	{
		
	}
	if(keys['S'])
	{
		
	}

	// CAMBIOS ADICIONALES
	if((GetAsyncKeyState('C')&1) == 1)
	{
		teclaColor=true;
	}

	return TRUE;
}