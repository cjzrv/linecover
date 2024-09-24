#include <math.h>
#include <time.h>
#include <iostream>
#include <graphics.h>
#include "trapezoid.h"

using namespace std;

class triangle
{
public:
    Point p[3];
    float angle[3];
    float width = 1;
    int color = 10;
    float area0 = 0;
    int rcount = 0;
    float cvArea=0;
    float tvLength=0;
    float totalLength=0;
    bool quick_mode = false;

    triangle()
    {
        readFile();
        calcAngle();
    }

    triangle(Point pp1, Point pp2, Point pp3)
    {
        p[0] = pp1;
        p[1] = pp2;
        p[2] = pp3;
        calcAngle();
    }

    void readFile()
    {
        FILE *fp = fopen("vertex3.txt", "r");
        if (fp == NULL)
            return;
        for (int i = 0; i < 3; i++)
            fscanf(fp, "%f %f", &p[i].x, &p[i].y);
        fclose(fp);
    }

    void calcAngle()
    {
        Vector vec;
        for (int i = 0; i < 3; i++)
        {
            vec = p[(i + 1) % 3] - p[i];
            angle[i] = vec.angle(p[(i + 2) % 3] - p[i]);
        }
    }

    float totalArea()
    {
        float totalArea = 0;
        totalArea = 0.5 * fabs(p[0].x * (p[1].y - p[2].y) + p[1].x * (p[2].y - p[0].y) + p[2].x * (p[0].y - p[1].y));
        return totalArea;
    }

    float cornerMargin(float cR, int i)
    {
        return cR * cR * (cot(angle[i] / 2) - (M_PI - angle[i]) / 2);
    }

    void draw()
    {
        rcount = 0;
        totalLength=0;
        color = 10;
        width = 3;
        for (int i = 0; i < 3; i++)
            drawLine(p[i], p[(i + 1) % 3], width, color);
    }

    Point calcCenter(int i, int radius)
    {
        Vector vec1 = p[(i + 1) % 3] - p[i];
        Vector vec2 = p[(i + 2) % 3] - p[i];

        vec1 = vec1.normalize();
        vec2 = vec2.normalize();

        float angleBetween = vec1.angle(vec2);
        Vector bisector = (vec1 + vec2) / 2; // bisector of two vectors
        Point circleCenter = p[i] + (bisector.normalize() * radius / sin(angleBetween / 2));

        setlinestyle(0, 0, 3);
        setcolor(color);
        circle(circleCenter.x, circleCenter.y, radius);
        return circleCenter;
    }

    void cover3()
    {
        color = 11;
        rcount = 0;
        float cR, maxR;
        Point circleCenter;
        Vector vec1(p[1] - p[0]), vec2(p[2] - p[1]), vec3(p[0] - p[2]);

        maxR = (vec3.length() * vec1.length() * sin(angle[0])) / (vec1.length() + vec2.length() + vec3.length());
        float area0 = 0;

        for (int i = 0; i < 3; i++)
        {
            color = 1 + color % 14;

            circleCenter = calcCenter(i, minR);
            area0 += minR * minR * M_PI;
            cR = round((2 * minR) / (1 - sin(angle[i] / 2)));

            for (float d, e, f, g; cR < maxR;)
            {
                circleCenter = calcCenter(i, cR);
                area0 += cR * cR * M_PI;
                rcount++;
                stop(quick_mode);

                d = cR / tan(angle[i] / 2) + sqrt(4 * cR * minR - 4 * minR * minR);
                e = 1 / (tan(angle[i] / 2) * tan(angle[i] / 2));
                f = 2 * d / tan(angle[i] / 2) + 4 * minR;
                g = d * d + 4 * minR * minR;

                cR = (f + sqrt(f * f - 4 * e * g)) / (2 * e);
            }
        }
        color = 3 + color % 14;
        calcCenter(0, maxR);
        calcCenter(0, maxR-1);
        area0 += maxR * maxR * M_PI;
        rcount++;
        stop(quick_mode);
        color = 4 + color % 14;
        Point circleCenters[3];
        for (int i = 0; i < 3; i++)
        {
            circleCenters[i] = calcCenter(i, minR);
            stop(quick_mode);
        }

        for (int i = 0; i < 3; i++)
        {
            drawLine(circleCenters[i], circleCenters[(i + 1) % 3], 2 * minR + 1, color);
            area0 += calcLineArea(circleCenters[i], circleCenters[(i + 1) % 3], 2 * minR);
            stop(quick_mode);
        }
        rcount++;
        printf("--------------\nHuang's strategy:\nRadius count: %d\nCovered area: %.3f%%\n", rcount, area0 / totalArea() * 100);
    }

    void cover4()
    {
        rcount = 0;
        float cR, maxR;
        Vector vec1 = p[1] - p[0], vec2 = p[2] - p[1], vec3 = p[0] - p[2];

        maxR = (vec3.length() * vec1.length() * sin(angle[0])) / (vec1.length() + vec2.length() + vec3.length());
        Point circleCenters[3];

        int bigR = maxR / 2;

        color = 6;
        for (int i = 0; i < 3; i++)
        {
            circleCenters[i] = calcCenter(i, bigR);
        }
        stop(quick_mode);
        color = 1 + color % 14;
        for (int i = 0; i < 3; i++)
        {
            drawLine(circleCenters[i], circleCenters[(i + 1) % 3], 2 * bigR + 1, color);
            area0 += calcLineArea(circleCenters[i], circleCenters[(i + 1) % 3], 2 * bigR);
            color = 1 + color % 14;
            stop(quick_mode);
        }

        for (int i = 0; i < 3; i++)
            fragCover(bigR, circleCenters[i], i);

        printf("--------------\nZhuang's strategy:\nRadius count: %d\nCovered area: %.3f%%\n", rcount + 1, area0 / totalArea() * 100);
    }

    void fragCover(int refR, Point center, int ai)
    {
        int cR = refR * (1 - sin(angle[ai] / 2)) / 2;
        if (cR < minR)
            cR = minR;
        rcount++;
        Point right, mid, left;

        mid = calcCenter(ai, cR);

        Vector refv = p[ai] - p[(ai + 1) % 3];
        Vector PO = center - p[ai];

        if ((PO.normalize() + refv.orthX()).length() > (PO.normalize() + refv.orthY()).length())
            right = center + refv.orthY().normalize() * (refR - cR);
        else
            right = center + refv.orthX().normalize() * (refR - cR);

        refv = p[ai] - p[(ai + 2) % 3];

        if ((PO.normalize() + refv.orthX()).length() > (PO.normalize() + refv.orthY()).length())
            left = center + refv.orthY().normalize() * (refR - cR);
        else
            left = center + refv.orthX().normalize() * (refR - cR);

        setlinestyle(0, 0, 3);
        setcolor(color);
        circle(mid.x, mid.y, cR);
        circle(right.x, right.y, cR);
        circle(left.x, left.y, cR);
        stop(quick_mode);

        drawLine(mid, right, 2 * cR + 1, color);
        area0 += calcLineArea(mid, right, 2 * cR);
        stop(quick_mode);
        drawLine(mid, left, 2 * cR + 1, color);
        area0 += calcLineArea(mid, left, 2 * cR);
        stop(quick_mode);
        color = 1 + color % 14;

        if (cR <= minR)
            return;
        fragCover(cR, mid, ai);
        return;
    }

    void random()
    {
        for (int i = 0; i < 3; i++)
            p[i] = Point(0, 0);
        srand(time(NULL));
        while (totalArea() < 100000)
        {
            p[1] = Point(rand() % 1600, rand() % 800);
            p[2] = Point(rand() % 1600, rand() % 800);
            p[0] = Point(rand() % 1600, rand() % 800);
        }
        calcAngle();
        return;
    }

    void scaling(float scale)
    {
        for (int i = 0; i < 3; i++)
            p[i] = Point(p[i].x * scale, p[i].y * scale);
        calcAngle();
        return;
    }

    void save()
    {
        FILE *pFile = fopen("save3.txt", "w");
        for (int i = 0; i < 3; i++)
        {
            fprintf(pFile, "%f %f\n", p[i].x, p[i].y);
        }
        fprintf(pFile, "\n");
        fclose(pFile);
        return;
    }

    void load()
    {
        FILE *pFile = fopen("save3.txt", "r");
        if(pFile == NULL)
        {
            fclose(pFile);
            random();
            return;
        }
        for (int i = 0; i < 3; i++)
        {
            fscanf(pFile, "%f %f", &p[i].x, &p[i].y);
        }
        fclose(pFile);
        calcAngle();
        return;
    }
};