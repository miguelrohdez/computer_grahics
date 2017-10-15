import sys
from svgpathtools import svg2paths

class Poligono():

    def __init__(self, vertices, color=(0,0,0)):
        self.vertices = vertices
        self.color = color

    def __str__(self):
        cabecera = '* Poligono\n'
        color = 'Color {0}\n'.format(self.color)
        vertices = ["".join([str(v), '\n']) for v in self.vertices]
        return "".join([cabecera, color] + vertices)

    def to_opengl(self):
        begin = 'glBegin(GL_POLYGON);\n'
        end = 'glEnd();\n'
        color = '\tglColor3f({0}, {1}, {2});\n'.format(self.color[0],\
                                                       self.color[1],\
                                                       self.color[2])
        vertices = ["".join(['\t', p.to_opengl()]) for p in self.vertices]
        return "".join([begin, color] + vertices + [end])

class Vertice():

    def __init__(self, x, y, z=-1.2, presicion=2):
        self.x = round(x, presicion)
        self.y = round(y, presicion)
        self.z = z

    def __str__(self):
        return "({0}, {1})".format(self.x, self.y)

    def __eq__(self, other):
        return (isinstance(other, self.__class__)) and \
        (self.x==other.x and self.y==other.y)

    def to_opengl(self):
        return 'glVertex3f({0}, {1}, {2});\n'.format(self.x, self.y, self.z)

def svg2gl(nombre_archivo):
    poligonos = []
    puntos = []
    objetos, atributos = svg2paths(nombre_archivo)
    paths = zip(objetos, atributos)
    for p in paths:
        for linea in p[0]:
            puntos += [Vertice(v.real, v.imag)\
                          for v in linea if v not in puntos]
        color = p[1]['style'].split(';')[0].split(':')[1]
        poligonos.append(Poligono(puntos,
                                  hex2float(color) if '#' in color else (0)*3))
        puntos = []
    return poligonos

def hex2rgb(hex):
    tmp = hex.strip('#')
    rgb = tuple(int(tmp[i:i+2], 16) for i in (0, 2 ,4))
    return rgb

def rgb2float(rgb):
    return tuple(i/256 for i in rgb)

def hex2float(hex):
    rgb = hex2rgb(hex)
    return rgb2float(rgb)

def main():
    if len(sys.argv) < 2:
        print("Forma de uso: $python svg2gl fileIn [fileOut]")
        print("fileOut: por defecto es glvertex.txt")
    elif len(sys.argv) < 3:
        fsalida = open('glvertex.txt', 'w')
    else:
        fsalida = open(sys.argv[2], 'w')
    poligonos = svg2gl(sys.argv[1])
    for i in poligonos:
        fsalida.write(i.to_opengl())
    fsalida.close()

if __name__ == '__main__':
    main()
