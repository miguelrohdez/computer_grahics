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

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaracion de WndProc (Procedimiento de ventana)

GLvoid ReDimensionaEscenaGL(GLsizei width, GLsizei height)	// Redimensiona e inicializa la ventana
{
	if (height==0)							// Para que no se presente una division por cero
	{
		height=1;							// la altura se iguala a 1
	}

	glViewport(0,0,width,height);					// Resetea el puerto de vista

	glMatrixMode(GL_PROJECTION);					// Selecciona la Matriz de Proyeccion
	glLoadIdentity();								// Resetea la Matriz de Proyeccion

	// Calcula el radio de aspecto o proporcion de medidas de la ventana
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,2000.0f);
	
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

int IniGL(GLvoid)										// Aqui se configuran los parametros iniciales de OpenGL
{
	glShadeModel(GL_SMOOTH);							// Activa Smooth Shading
	glClearColor(0.3f, 0.3f, 0.3f, 0.5f);				// Fondo negro
	glClearDepth(1.0f);									// Valor para el Depth Buffer
	glEnable(GL_DEPTH_TEST);							// Activa Depth Testing
	glDepthFunc(GL_LEQUAL);								// Tipo de Depth Testing a usar
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Correccion de cálculos de perspectiva

	glCullFace(GL_BACK);								// Configurado para eliminar caras traseras
	glEnable(GL_CULL_FACE);								// Activa eliminacion de caras ocultas

	e=gluNewQuadric();

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

void DibujaPrimitivas(){
	static int tiempoEstrella = 0;

	// Techo
	glBegin(GL_TRIANGLES);
		glColor3f(0.6f, 0.6f, 0.6f);
		glVertex3f(-60.0f, 0.0f, 0.0f);
		glVertex3f(60.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 30.0f, 0.0f);
	glEnd();

	// Pilares
	glBegin(GL_LINE_LOOP);
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex3f(-50.0f, 0.0f, 0.0f);
		glVertex3f(-30.0f, 0.0f, 0.0f);
		glVertex3f(-30.0f, -40.0f, 0.0f);
		glVertex3f(-50.0f, -40.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(-10.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, 0.0f, 0.0f);
		glVertex3f(10.0f, -40.0f, 0.0f);
		glVertex3f(-10.0f, -40.0f, 0.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(30.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, 0.0f, 0.0f);
		glVertex3f(50.0f, -40.0f, 0.0f);
		glVertex3f(30.0f, -40.0f, 0.0f);
	glEnd();

	// Base 1
	glBegin(GL_QUADS);
		glColor3f(0.8f, 0.8f, 0.8f);
		glVertex3f(-60.0f, -40.0f, 0.0f);
		glVertex3f(-60.0f, -55.0f, 0.0f);
		glVertex3f(60.0f, -55.0f, 0.0f);
		glVertex3f(60.0f, -40.0f, 0.0f);
	glEnd();

	// Base 2
	glBegin(GL_QUADS);
		glColor3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-70.0f, -55.0f, 0.0f);
		glVertex3f(-70.0f, -75.0f, 0.0f);
		glVertex3f(70.0f, -75.0f, 0.0f);
		glVertex3f(70.0f, -55.0f, 0.0f);
	glEnd();

	// Estrellas
	if(tiempoEstrella < 30){
		glPointSize(3.0);
		glBegin(GL_POINTS);
			glColor3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-25.0f, 45.0f, 0.0f);
			glVertex3f(-38.0f, 40.0f, 0.0f);
			glVertex3f(-15.0f, 37.0f, 0.0f);
			glVertex3f(0.0f, 56.0f, 0.0f);
			glVertex3f(16.0f, 30.0f, 0.0f);
			glVertex3f(28.0f, 55.0f, 0.0f);
			glVertex3f(37.0f, 68.0f, 0.0f);
			glVertex3f(40.0f, 44.0f, 0.0f);
		glEnd();
		glPointSize(1.0);
	}

	if(tiempoEstrella < 50)
		tiempoEstrella++;
	else
		tiempoEstrella = 0;
	
}

void DibujaPiramide()
{
	//Pirámide sólida interior
	glBegin(GL_TRIANGLES);  //PRIMITIVA GRAFICA DE GL
		//Cara delantera
		glColor3f(0.0f, 0.0f, 0.8f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
		glVertex3f(-30.0f, 20.0f, -20.0f);
		glVertex3f(-10.0f, 20.0f, -20.0f);
		glVertex3f(-20.0f, 40.0f, -30.0f);
		//Cara derecha
		glColor3f(0.0f,0.0f,0.9f);
		glVertex3f(-10.0f, 20.0f, -20.0f);
		glVertex3f(-10.0f, 20.0f, -40.0f);
		glVertex3f(-20.0f, 40.0f, -30.0f);
		//Cara izquierda
		glColor3f(0.0f,0.0f,1.0f);
		glVertex3f(-30.0f, 20.0f, -40.0f);
		glVertex3f(-30.0f, 20.0f, -20.0f);
		glVertex3f(-20.0f, 40.0f, -30.0f);
		//Cara trasera
		glColor3f(0.0f,0.0f,0.7f);
		glVertex3f(-10.0f, 20.0f, -40.0f);
		glVertex3f(-30.0f, 20.0f, -40.0f);
		glVertex3f(-20.0f, 40.0f, -30.0f);
	glEnd();

	//Cara inferior
	glBegin(GL_QUADS);
		glColor3f(0.0f,0.0f,0.6f);
		glVertex3f(-10.0f, 20.0f, -20.0f);
		glVertex3f(-30.0f, 20.0f, -20.0f);
		glVertex3f(-30.0f, 20.0f, -40.0f);
		glVertex3f(-10.0f, 20.0f, -40.0f);
	glEnd();

	glColor3f(1.0f,1.0f,1.0f);

	//Piramide alambrada exterior

	//Cara delantera
	glBegin(GL_LINE_LOOP);
		glVertex3f(-35.0f, 15.0f, -15.0f);
		glVertex3f(- 5.0f, 15.0f, -15.0f);
		glVertex3f(-20.0f, 45.0f, -30.0f);
	glEnd();

	//Cara derecha
	glBegin(GL_LINE_LOOP);
		glVertex3f(- 5.0f, 15.0f, -15.0f);
		glVertex3f(- 5.0f, 15.0f, -45.0f);
		glVertex3f(-20.0f, 45.0f, -30.0f);
	glEnd();

	//Cara izquierda
	glBegin(GL_LINE_LOOP);
		glVertex3f(-35.0f, 15.0f, -45.0f);
		glVertex3f(-35.0f, 15.0f, -45.0f);
		glVertex3f(-20.0f, 45.0f, -30.0f);
	glEnd();

	//Cara trasera
	glBegin(GL_LINE_LOOP);
		glVertex3f(- 5.0f, 15.0f, -45.0f);
		glVertex3f(-35.0f, 15.0f, -45.0f);
		glVertex3f(-20.0f, 45.0f, -30.0f);
	glEnd();

	//Cara inferior
	glBegin(GL_LINE_LOOP);
		glVertex3f(- 5.0f, 15.0f, -15.0f);
		glVertex3f(-35.0f, 15.0f, -15.0f);
		glVertex3f(-35.0f, 15.0f, -45.0f);
		glVertex3f(- 5.0f, 15.0f, -45.0f);
	glEnd();

	//Lineas de union entre las dos piramides

	glColor3f(0.0f,1.0f,0.0f);

	glBegin(GL_LINES);
		//Linea entre los vértices superiores
		glVertex3f(-20.0f, 45.0f, -30.0f);
		glVertex3f(-20.0f, 40.0f, -30.0f);

		//Linea entre los vértices frontal inferior izquierda
		glVertex3f(-35.0f, 15.0f, -15.0f);
		glVertex3f(-30.0f, 20.0f, -20.0f);

		//Linea entre los vértices frontal inferior derecha
		glVertex3f(-10.0f, 20.0f, -20.0f);
		glVertex3f(- 5.0f, 15.0f, -15.0f);

		//Linea entre los vértices trasera inferior izquierda
		glVertex3f(-35.0f, 15.0f, -45.0f);
		glVertex3f(-30.0f, 20.0f, -40.0f);

		//Linea entre los vértices trasera inferior derecha
		glVertex3f(-10.0f, 20.0f, -40.0f);
		glVertex3f(- 5.0f, 15.0f, -45.0f);
	glEnd();
	
	glColor3f(1.0f,1.0f,1.0f);

}

void DibujaCubo(){
	/*Cubo Solido*/
	//Cara frontal
	glColor3f(0.6f, 0.1f, 0.0f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(-5.0f, -5.0f, 5.0f);
		glVertex3f(5.0f, -5.0f, 5.0f);
		glVertex3f(5.0f, 5.0f, 5.0f);
		glVertex3f(-5.0f, 5.0f, 5.0f);
	glEnd();

	//Cara derecha
	glColor3f(0.2f, 0.65f, 0.0f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(5.0f, -5.0f, 5.0f);
		glVertex3f(5.0f, -5.0f, -5.0f);
		glVertex3f(5.0f, 5.0f, -5.0f);
		glVertex3f(5.0f, 5.0f, 5.0f);
	glEnd();

	//Cara trasera
	glColor3f(0.6f, 0.1f, 0.0f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(5.0f, -5.0f, -5.0f);
		glVertex3f(-5.0f, -5.0f, -5.0f);
		glVertex3f(-5.0f, 5.0f, -5.0f);
		glVertex3f(5.0f, 5.0f, -5.0f);
	glEnd();

	//Cara izquierda 
	glColor3f(0.2f, 0.65f, 0.0f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(-5.0f, -5.0f, -5.0f);
		glVertex3f(-5.0f, -5.0f, 5.0f);
		glVertex3f(-5.0f, 5.0f, 5.0f);
		glVertex3f(-5.0f, 5.0f, -5.0f);
	glEnd();

	//Cara superior
	glColor3f(0.0f, 0.0f, 0.9f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(-5.0f, 5.0f, -5.0f);
		glVertex3f(-5.0f, 5.0f, 5.0f);
		glVertex3f(5.0f, 5.0f, 5.0f);
		glVertex3f(5.0f, 5.0f, -5.0f);
	glEnd();

	//Cara inferior
	glColor3f(0.0f, 0.0f, 0.9f);  // INSTRUCCION PARA EL COLOR (R,G,B): Aplica a todo hasta que se declare d enuevo la funcion
	glBegin(GL_QUADS);
		glVertex3f(-5.0f, -5.0f, -5.0f);
		glVertex3f(5.0f, -5.0f, -5.0f);
		glVertex3f(5.0f, -5.0f, 5.0f);
		glVertex3f(-5.0f, -5.0f, 5.0f);
	glEnd();

	/*Cubo Externo (HUECO)*/
	glColor3f(1.0f, 1.0f, 1.0f);
	//Cara frontal
	glBegin(GL_LINE_LOOP);
		glVertex3f(-7.0f, -7.0f, 7.0f);
		glVertex3f(7.0f, -7.0f, 7.0f);
		glVertex3f(7.0f, 7.0f, 7.0f);
		glVertex3f(-7.0f, 7.0f, 7.0f);
	glEnd();

	//Cara derecha
	glBegin(GL_LINE_LOOP);
		glVertex3f(7.0f, -7.0f, 7.0f);
		glVertex3f(7.0f, -7.0f, -7.0f);
		glVertex3f(7.0f, 7.0f, -7.0f);
		glVertex3f(7.0f, 7.0f, 7.0f);
	glEnd();

	//Cara trasera
	glBegin(GL_LINE_LOOP);
		glVertex3f(7.0f, -7.0f, -7.0f);
		glVertex3f(-7.0f, -7.0f, -7.0f);
		glVertex3f(-7.0f, 7.0f, -7.0f);
		glVertex3f(7.0f, 7.0f, -7.0f);
	glEnd();

	//Cara izquierda 
	glBegin(GL_LINE_LOOP);
		glVertex3f(-7.0f, -7.0f, -7.0f);
		glVertex3f(-7.0f, -7.0f, 7.0f);
		glVertex3f(-7.0f, 7.0f, 7.0f);
		glVertex3f(-7.0f, 7.0f, -7.0f);
	glEnd();

	//Cara superior
	glBegin(GL_LINE_LOOP);
		glVertex3f(-7.0f, 7.0f, -7.0f);
		glVertex3f(-7.0f, 7.0f, 7.0f);
		glVertex3f(7.0f, 7.0f, 7.0f);
		glVertex3f(7.0f, 7.0f, -7.0f);
	glEnd();

	//Cara inferior

	glBegin(GL_LINE_LOOP);
		glVertex3f(-7.0f, -7.0f, -7.0f);
		glVertex3f(7.0f, -7.0f, -7.0f);
		glVertex3f(7.0f, -7.0f, 7.0f);
		glVertex3f(-7.0f, -7.0f, 7.0f);
	glEnd();

}

int RenderizaEscena(GLvoid)								// Aqui se dibuja todo lo que aparecera en la ventana
{
	static float ang = 0.0f;
	static float desp = 0.0f, aux=0.0f;
	static int direccion = 1;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(80.0f, 120.0f, 170.0f, 0.0f, 0.0f, 0.0f, 0, 1, 0);
	
	DibujaEjes();
	DibujaPiramide();
	//DibujaPrimitivas();
	//DibujaCubo();
	glPushMatrix();		
		glTranslatef(0.0f, desp, 0.0f);
		glRotatef(ang, 0.0f, 1.0f, 0.0f);
		DibujaCubo();
	glPopMatrix();
	
	aux += 0.01f;
	ang += aux;
	if(ang > 360){
		ang -= 360;
	}

	// Manejador de direccion
	if(direccion == 1){
		if(desp < 20)
			desp += 0.1f;
		else
			direccion = 2;
	}else if(direccion == 2){
		if(desp > -20.0f)
			desp -= 0.1f;
		else
			direccion = 1;
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
		
	}
	if(keys[VK_DOWN])
	{
		
	}
	if(keys[VK_LEFT])
	{
		
	}
	if(keys[VK_RIGHT])
	{
		
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

	return TRUE;
}