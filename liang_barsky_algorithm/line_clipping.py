#!/usr/bin/python
# -*- coding: utf-8 -*-
# Universidad Nacional Autonoma de Mexico / Facultad de Ingenieria
# Computación Gráfica
# Alumnos:
# Barriga Martínez Diego Alberto
# Vilchis Oropeza Luis Alberto
# Hecho en Python 3.5


class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def get_x(self):
        return self.x

    def get_y(self):
        return self.y

    def to_string(self):
        return "({0}, {1})".format(self.x, self.y)


class Window:
    def __init__(self, point, width=0, height=0):
        self.point = point
        self.width = width
        self.height = height

    def get_x_left(self):
        return self.point.get_x()

    def get_x_right(self):
        return self.point.get_x() + self.width

    def get_y_bottom(self):
        return self.point.get_y()

    def get_y_top(self):
        return self.point.get_y() + self.height

    def is_inside_window(self, point):
        if point.get_x() < self.get_x_left() or point.get_x() > self.get_x_right():
            return False
        elif point.get_y() < self.get_y_bottom() or point.get_y() > self.get_y_top():
            return False
        else:
            return True


class Line:
    def __init__(self, point_1, point_2):
        self.point_1 = point_1
        self.point_2 = point_2

    def get_x_i(self):
        return self.point_1.get_x()

    def get_x_f(self):
        return self.point_2.get_x()

    def get_y_i(self):
        return self.point_1.get_y()

    def get_y_f(self):
        return self.point_2.get_y()

    def is_vertical(self):
        return True if (self.get_x_f() - self.get_x_i()) == 0 else False

    def is_horizontal(self):
        return True if (self.get_y_f() - self.get_y_i()) == 0 else False

    def is_parallel_to_axis(self):
        return True if self.is_vertical() or self.is_horizontal() else False


def get_data():
    points = list()
    with open('input.txt', 'r') as file:
        for line in file:
            data = list(map(int, line.rstrip("\n").split(" ")))
            points.append(Point(data[0], data[1]))
    return points


def get_lines(points):
    lines = []
    for (p1, p2) in zip(points[::2], points[1::2]):
        lines.append(Line(p1, p2))
    return lines


def left_analysis(line, w):
    if line.is_vertical():
        return None
    u = (w.get_x_left() - line.get_x_i())/(line.get_x_f() - line.get_x_i())
    if u < 0 or u > 1:
        return None
    y = line.get_y_i() + u*(line.get_y_f() - line.get_y_i())
    clipping_point = Point(w.get_x_left(), y)
    if not w.is_inside_window(clipping_point):
        return None
    return clipping_point


def right_analysis(line, w):
    if line.is_vertical():
        return None
    u = (w.get_x_right() - line.get_x_i())/(line.get_x_f() - line.get_x_i())
    if u < 0 or u > 1:
        return None
    y = line.get_y_i() + u*(line.get_y_f() - line.get_y_i())
    clipping_point = Point(w.get_x_right(), y)
    if not w.is_inside_window(clipping_point):
        return None
    return clipping_point


def bottom_analysis(line, w):
    if line.is_horizontal():
        return None
    u = (w.get_y_bottom() - line.get_y_i())/(line.get_y_f() - line.get_y_i())
    if u < 0 or u > 1:
        return None
    x = line.get_x_i() + u*(line.get_x_f() - line.get_x_i())
    clipping_point = Point(x, w.get_y_bottom())
    if not w.is_inside_window(clipping_point):
        return None
    return clipping_point


def top_analysis(line, w):
    if line.is_horizontal():
        return None
    u = (w.get_y_top() - line.get_y_i())/(line.get_y_f() - line.get_y_i())
    if u < 0 or u > 1:
        return None
    x = line.get_x_i() + u*(line.get_x_f() - line.get_x_i())
    clipping_point = Point(x, w.get_y_top())
    if not w.is_inside_window(clipping_point):
        return None
    return clipping_point


def liang_barsky(line, window):
    intersections = dict()
    intersections["left"] = left_analysis(line, window)
    intersections["right"] = right_analysis(line, window)
    intersections["top"] = top_analysis(line, window)
    intersections["bottom"] = bottom_analysis(line, window)
    return intersections


def line_clipper(lines, window):
    line_counter = 1
    with open('output.txt', "w") as file:
        for l in lines:
            clipping_points = liang_barsky(l, window)
            store_information(clipping_points["left"], "Left", line_counter,file)
            store_information(clipping_points["right"], "Right", line_counter,file)
            store_information(clipping_points["top"], "Top", line_counter,file)
            store_information(clipping_points["bottom"], "Bottom", line_counter,file)
            line_counter += 1
            file.write('\n')


def store_information(clipping_point, side, line_id, file):
    message = "{0} limit, line {1}: {2}\n"
    if clipping_point:
        message = message.format(side, line_id, clipping_point.to_string())
    else:
        message = message.format(side, line_id, "it has no clipping point.")
    file.write(message)


def main():
    lines = get_lines(get_data())
    print("===Enter Window data===")
    w_point = list(map(int, input("Point (separate by space): ").split(" ")))
    width = int(input("Width: "))
    height = int(input("Height: "))
    w = Window(Point(w_point[0], w_point[1]), width, height)
    line_clipper(lines, w)


if __name__ == '__main__':
    main()
