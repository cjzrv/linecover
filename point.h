#include <math.h>
#include <time.h>
#include <iostream>
#include <graphics.h>
#define M_PI 3.14159265358979323846
#define minR 7

class Vector
{
public:
    float x, y;
    Vector(float x1, float y1)
    {
        x = x1;
        y = y1;
    }

    Vector()
    {
        x = 0;
        y = 0;
    }

    float length()
    {
        return sqrt(x * x + y * y);
    }

    Vector normalize()
    {
        float len = length();
        return Vector(x / len, y / len);
    }

    float angle(Vector other)
    {
        return acos((x * other.x + y * other.y) / length() / other.length());
    }

    Vector orthY()
    {
        Vector p;
        p.x = -y;
        p.y = x;
        return p;
    }
    Vector orthX()
    {
        Vector p;
        p.x = y;
        p.y = -x;
        return p;
    }

    Vector operator+(Vector other)
    {
        return Vector(x + other.x, y + other.y);
    }

    Vector operator-(Vector other)
    {
        return Vector(x - other.x, y - other.y);
    }

    Vector operator*(float scalar)
    {
        return Vector(x * scalar, y * scalar);
    }

    Vector operator/(float scalar)
    {
        return Vector(x / scalar, y / scalar);
    }
};

class Point
{
public:
    float x, y;
    Point(float x1, float y1)
    {
        x = x1;
        y = y1;
    }
    Point()
    {
        x = 0;
        y = 0;
    }

    float distance(Point other)
    {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    Vector operator-(Point other) // Vector from other to this
    {
        return Vector(x - other.x, y - other.y);
    }

    Point operator+(Vector other) // Point + Vector = Point
    {
        return Point(x + other.x, y + other.y);
    }
};

class Segment
{
public:
    Point pstart, pend;
    float width = 1, color = 15;
    Segment(Point p1, Point p2, float w)
    {
        pstart = p1;
        pend = p2;
        width = w;
    }
    Segment(Point p1, Point p2)
    {
        pstart = p1;
        pend = p2;
    }

    void draw()
    {
        setlinestyle(0, 0, width);
        setcolor(color);
        line(pstart.x, pstart.y, pend.x, pend.y);
    }

    float calcArea()
    {
        float area = (pstart - pend).length() * width + pow(width / 2, 2) * M_PI;
        return area;
    }
};

void drawLine(Point start, Point end, float width, int color)
{
    Segment line(start, end, width);
    line.color = color;
    line.draw();
}

float calcLineArea(Point start, Point end, float width)
{
    Segment line(start, end, width);
    return line.calcArea();
}

float cot(float x)
{
    return 1 / tan(x);
}

void stop(bool quick_mode)
{
    if (!quick_mode)
        getch();
}