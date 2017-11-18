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
#include "Cilindro.h"

#define PI 3.1415063

CCamera camara;
GLfloat g_lookupdown = 0.0f; // Posición en el eje Z

float rotacionX = 0.0f;
float rotacionY = 0.0f;
float rotacionZ = 0.0f;

/*
 * Texturas
 */
CTexture pasto;
CTexture tiles;
CTexture fachadaExterior;
CTexture fachadaInterior;
CTexture textureSkyBox;
CTexture cuadroDebug;
CTexture barda[6];

/*
 * Colores
 */
Color saddleBrown = Color(0.545f, 0.271f, 0.075f);
Color sienna = Color(0.627f, 0.322f, 0.176f);
Color cafeMesa = Color(0.09f, 0.06f, 0.05f);



void inicializarTexturas() {
    textureSkyBox.LoadBMP("Texturas/skybox.bmp");
	  textureSkyBox.BuildGLTexture();
	  textureSkyBox.ReleaseImage();

    pasto.LoadTGA("Texturas/pasto3.tga");
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

    barda[0] = fachadaInterior;
    barda[1] = fachadaInterior;
    barda[2] = fachadaInterior;
    barda[3] = fachadaExterior;
    barda[4] = fachadaExterior;
    barda[5] = fachadaExterior;
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
    camara.Position_Camera(10, 50, -50, 0, 30, 0, 0, 1, 0);
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
    Prisma p = Prisma(500, 5, 500, c);
    p.setRepetitionTexture(0.5);
    p.setTexture(pasto);
    p.draw();
}

void dibujarPiso() {
    Color c = Color(1.0f, 1.0f, 1.0f);
    Prisma p = Prisma(280, 1, 280, c);
    p.setTexture(tiles);
    p.draw();
}

void dibujarMesa() {
    Prisma m = Prisma(8, 0.5, 3, cafeMesa);
    m.draw();
    glTranslatef(0.0f, 0.35, 0.0f);
    m.draw(8.5, 0.25, 3.5, sienna);
    glTranslatef(0.0f, 4.1f, 0.0f);
    m.draw(7.0f, 8.0f, 3.0f, cafeMesa);
    glTranslatef(0.0f, 4.1f, 0.0f);
    m.draw(8.5, 0.2, 3.5, sienna);
    glTranslatef(0.0f, 0.35f, 0.3f);
    m.draw(15, 0.1, 10, cafeMesa);
}


void dibujarCasa(float heightWall, float scale, float textureRep=0.3f) {
    float anchoBarda = 1.1f;
    GLfloat alturaTecho = heightWall - 12.0f;
    Color n = Color(1.0f, 1.0f, 1.0f);
    Prisma c = Prisma(30 * scale, heightWall, anchoBarda, n);

    c.setRepetitionTexture(textureRep);
    c.setTexture(barda);
    glTranslatef(-50 * scale, 0, 50 * scale); // A
        c.draw();

    glTranslatef(-15 * scale, 0, -10 * scale); // B
        c.draw(anchoBarda, heightWall, 20 * scale);

    glTranslatef(30 * scale, 0, 0); // C
        c.draw(anchoBarda, heightWall, 20 * scale, fachadaInterior);
        c.setTexture(barda);

    glTranslatef(-20 * scale, 0, -10 * scale); // D
        c.draw(20*scale, heightWall, anchoBarda);

    glTranslatef(75 * scale, 0, 0); // E
        c.draw(90*scale, heightWall, anchoBarda);

    glTranslatef(-55 * scale, 0, -15 * scale); // F
        c.draw(anchoBarda, heightWall, 30 * scale);

    glTranslatef(-30 * scale, 0, -5 * scale); // G
        c.draw(anchoBarda, heightWall, 40 * scale);

    glTranslatef(130 * scale, 0, -5 * scale); // H
        c.draw(anchoBarda, heightWall, 50 * scale);

    glTranslatef(-85 * scale, 0, -5 * scale); // I
        c.draw(30 * scale, heightWall, anchoBarda);

    glTranslatef(-30 * scale, 0, -10 * scale); // J
        c.draw(30 * scale, heightWall, anchoBarda);

    glTranslatef(55 * scale, 0, 0); // K
        c.draw(40 * scale, heightWall, anchoBarda);

    glTranslatef(-25 * scale, 0, -10 * scale); // L
        c.draw(10 * scale, heightWall, anchoBarda);

    glTranslatef(52.5 * scale, 0, 0); // M
        c.draw(15 * scale, heightWall, anchoBarda);

    glTranslatef(25 * scale, 0, 0); // N
        c.draw(15 * scale, heightWall, anchoBarda);

    glTranslatef(-122.5 * scale, 0, -10 * scale); // Ñ
        c.draw(anchoBarda, heightWall, 40 * scale);

    glTranslatef(90 * scale, 0, -5 * scale); // O
        c.draw(anchoBarda, heightWall, 30 * scale);

    glTranslatef(-60 * scale, 0, 0); // P
        c.draw(anchoBarda, heightWall, 30 * scale);

    glTranslatef(20 * scale, 0, 0); // Q
        c.draw(anchoBarda, heightWall, 30 * scale);

    glTranslatef(-35 * scale, 0, -15 * scale); // R
        c.draw(30 * scale, heightWall, anchoBarda);

    glTranslatef(25 * scale, 0, 0); // S
        c.draw(20 * scale, heightWall, anchoBarda);

    glTranslatef(30 * scale, 0, 0); // T
        c.draw(40 * scale, heightWall, anchoBarda);

    glTranslatef(20 * scale, 0, 35 * scale); // Q
        c.draw(anchoBarda, heightWall, 10 * scale);

    //Secciones de Techo
    //Tres secciones: comienza en O y terminan en B
    glTranslatef(-25*scale, alturaTecho, 25*scale);
    c.draw(50*scale, 1, 40*scale); // 1
    glTranslatef(-15*scale, 0, -65*scale);
    c.draw(80*scale,  1, 90*scale); // 2
    glTranslatef(50*scale, 0, -30*scale);
    c.draw(20*scale, 1, 30*scale); // 3

}

void dibujarSkyBox() {
    Prisma p = Prisma(500, 200, 500, Color(1.0f, 1.0f, 1.0f));
    p.setTexture(textureSkyBox);
    p.drawSky();
}

/* Función para depurar los planos */
void dibujarPlano() {
    Vertex v1 = Vertex(5,0,0);
    Vertex v2 = Vertex(0,0,0);
    Vertex v3 = Vertex(0,10,0);
    Vertex v4 = Vertex(5,10 ,0);
    Plane p = Plane(v1, v2, v3, v4, cuadroDebug);
    p.draw();
}

/* Función que dibuja un sillon */
void dibujaSillon(){
    Prisma s = Prisma(20, 5, 10, cafeMesa);
    s.draw(cuadroDebug);

    glPushMatrix();
      glTranslatef(8.5, 3.5, 0);
      s.draw(2, 2, 10, cuadroDebug);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(-8.5, 3.5, 0);
      s.draw(2, 2, 10, cuadroDebug);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(0, 3.5, 4);
      s.draw(15, 3, 1.5, pasto);
    glPopMatrix();

}

void dibujaCilindro(float radio, float planes, float height){
  float ang;
  float a[3], b[3], c[3], d[3];
  float delta;

  delta = 360.0f / planes;

  for (int i = 0; i < planes; i++)  //Por la forma de renderizar de openGL se usa for
  {
    ang = i*delta;

    a[0] = radio*(float)cos(ang*PI / 180.0f);  //Conversi�n de �ngulo a RAD
    a[1] = 0.0f;
    a[2] = radio*(float)sin(ang*PI / 180.0f);

    b[0] = a[0];  // Se calcula igual y se reutiliza
    b[1] = height;  // El vertice b est� arriba por ello se usa height
    b[2] = a[2];

    ang = (i + 1)*delta;  // Se utiliza theta sig. Delta theta = i+1

    c[0] = radio*(float)cos(ang*PI / 180.0f);
    c[1] = height;
    c[2] = radio*(float)sin(ang*PI / 180.0f);

    d[0] = c[0];
    d[1] = 0.0f;
    d[2] = c[2];

    glBegin(GL_QUADS);
      glVertex3f(a[0], a[1], a[2]);
      glVertex3f(b[0], b[1], b[2]);
      glVertex3f(c[0], c[1], c[2]);
      glVertex3f(d[0], d[1], d[2]);
    glEnd();

    //Tapa superior

    glBegin(GL_TRIANGLES);
      glVertex3f(c[0], c[1], c[2]);
      glVertex3f(b[0], b[1], b[2]);
      glVertex3f(0.0f, height, 0.0f);
    glEnd();

    //Tapa inferior

    glBegin(GL_TRIANGLES);
      glVertex3f(a[0], a[1], a[2]);
      glVertex3f(d[0], d[1], d[2]);
    glEnd();
  }
}


void dibujaAvion(){

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
    gluLookAt(camara.mPos.x, camara.mPos.y + 20.0f, camara.mPos.z,
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
        glTranslatef(0, 12.5, 0);
        dibujarCasa(25.0, 2.0);
    glPopMatrix();
    glTranslatef(10, 0.025, 10);
      dibujarMesa();
    glTranslatef(0, 40, 0);

    dibujaSillon();
    dibujaAvion();
    glTranslatef(20, 20, 0);
    glDisable(GL_TEXTURE_2D);

    glFlush(); //TODO: Entender que hace
    glutSwapBuffers();
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
    glutMainLoop();

    return 0;
}
