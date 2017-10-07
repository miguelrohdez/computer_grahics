import sys
from svgpathtools import svg2paths

class Poligono():

    def __init__(self, vertices, color=(0,0,0)):
        self.vertices = vertices
        self.color = color

    def __str__(self):
        rep = '* Poligono\n'
        for punto in self.vertices:
            rep += str(punto)
            rep += '\n'
        rep += 'Color {0}\n'.format(self.color)
        rep += '* Fin Poligono\n'
        return rep

    def to_gl(self):
        rep = 'glBegin(GL_POLYGON);\n'
        rep += '\tglColor3f({0}, {1}, {2});\n'.format(self.color[0],\
                                                     self.color[1],\
                                                     self.color[2])
        for punto in self.vertices:
            rep += '\t'
            rep += punto.to_gl()
        rep += 'glEnd();\n'
        return rep

class Vertice():

    def __init__(self, x, y):
        self.x = round(x, 2)
        self.y = round(y, 2)

    def __str__(self):
        return "({0}, {1})".format(self.x, self.y)

    def __eq__(self, other):
        return (isinstance(other, self.__class__)) and \
        (self.x==other.x and self.y==other.y)

    def to_gl(self):
        return 'glVertex3f({0}, {1}, {2});\n'.format(self.x, self.y, -1.2)


def svg2gl(archivo):
    poligonos = []
    puntos = []
    objetos, atributos = svg2paths(archivo)
    zipped = zip(objetos, atributos)
    for objeto in zipped:
        for linea in objeto[0]:
            for punto in linea:
                vertice = Vertice(punto.real, punto.imag)
                if vertice not in puntos:
                    puntos.append(vertice)
        color = objeto[1]['style'].split(';')[0].split(':')[1]
        if '#' in color:
            poligonos.append(Poligono(puntos, hex2rgb(color)))
        else:
            poligonos.append(Poligono(puntos))
        puntos = []
    return poligonos

def hex2rgb(hex):
    tmp = hex.strip('#')
    rgb = tuple(int(tmp[i:i+2], 16) for i in (0, 2 ,4))
    return rgb2float(rgb)

def rgb2float(rgb):
    return tuple(i/256 for i in rgb)

def main():
    if len(sys.argv) < 2:
        print("Forma de uso: $python svg2gl fileIn [fileOut]")
        print("fileOut: por defecto es glvertex.txt")
    elif len(sys.argv) < 3:
        fsalida = open('glvertex.txt', 'w')
    else:
        fsalida = open(sys.argv[2], 'w')
    pols = svg2gl(sys.argv[1])
    for i in pols:
        fsalida.write(i.to_gl())
    fsalida.close()

if __name__ == '__main__':
    main()
