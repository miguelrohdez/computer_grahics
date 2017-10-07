#### Flujo de creación de figura:
1.  Crear polígonos en Inkscape, y guardar archivo con extención **.svg**.
2.  Para obtener los vértices usar el script **svg2gl.py** de la
    siguiente manera:  

        $ python svg2gl.py fileIn [fileOut]  

    * **fileIn:** archivo **.svg** generado con Inkscape.  
    * **[Opcional] fileOut:** nombre del archivo donde se guardará el
    código de OpenGL. Si no se especifica será **glvertex.txt**.  

3.  Copiar el contenido del archivo **glvertex.txt**, o el nombre
selecionado, según sea el caso, en la función de dibujo del archivo
**main.cpp**.  
4.  Compilar y ejecutar **main.cpp**

        $ make run

De esta manera todo el trabajo es delegado a Inkscape. Para generar los
colores, en Inkscape se deben colorear los polígonos; **svg2gl.py**
generará el código para cambiar el color de los polígonos. 


#### Restriciones al dibujar los polígonos en Inkscape:
*   Deben ser convexos (ángulos menores a 180°)
*   Deben ser cerrados
*   Si no se rellena de algun color, se generará en color negro en
    OpenGL  

#### Dependencias script **svg2gl.py**:  
*   *numpy*
*   *svgwrite*  
*   *svgpathtools*  

Se instalan con *pip* fácilmente.
