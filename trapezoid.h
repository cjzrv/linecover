#include <math.h>
#include <time.h>
#include <iostream>
#include <graphics.h>
#include "point.h"

using namespace std;

class trapezoid
{
public:
    Point p[4];
    float width = 1, minAngle, area = 0;
    int Min, color = 10;
    int rcount = 0;
    float angle[4];
    float totalLength = 0;
    bool quick_mode = false;

    trapezoid(Point pp1, Point pp2, Point pp3, Point pp4)
    {
        p[0] = pp1;
        p[1] = pp2;
        p[2] = pp3;
        p[3] = pp4;
        calcAngle();
    }

    trapezoid()
    {
        readFile();
        calcAngle();
    }

    void readFile()
    {
        FILE *fp = fopen("vertex.txt", "r");
        if (fp == NULL)
            return;
        for (int i = 0; i < 4; i++)
            fscanf(fp, "%f %f", &p[i].x, &p[i].y);
        fclose(fp);
    }

    void calcAngle()
    {
        Vector vec;
        minAngle = M_PI;
        Min = 0;
        for (int i = 0; i < 4; i++)
        {
            vec = p[(i + 1) % 4] - p[i];
            angle[i] = vec.angle(p[(i + 3) % 4] - p[i]);
            if (minAngle > angle[i])
            {
                minAngle = angle[i];
                Min = i;
            }
        }
    }

    void draw()
    {
        rcount = 0;
        totalLength = 0;
        color = 10;
        width = 3;
        for (int i = 0; i < 4; i++)
            drawLine(p[i], p[(i + 1) % 4], width, color);
    }

    Point calcCenter(int i, int radius)
    {
        Vector vec1 = p[(i + 1) % 4] - p[i];
        Vector vec2 = p[(i + 3) % 4] - p[i];

        vec1 = vec1.normalize();
        vec2 = vec2.normalize();

        float angleBetween = vec1.angle(vec2);
        Vector bisector = (vec1 + vec2) / 2;

        Point circleCenter;
        circleCenter = p[i] + (bisector.normalize() * radius / sin(angleBetween / 2));

        setlinestyle(0, 0, 3);
        setcolor(color);
        return circleCenter;
    }

    void cover3()
    {
        rcount = area = 0;
        Point centers[4];
        int next;
        float r, width;

        if ((p[Min].y) == (p[(Min + 1) % 4].y))
        {
            next = (Min + 1) % 4;
            r = 0.5 * fabs((p[Min].y) - (p[(Min + 3) % 4].y));
        }
        else
        {
            next = (Min + 3) % 4;
            r = 0.5 * fabs((p[Min].y) - (p[(Min + 1) % 4].y));
        }

        float nextR = (2 * minR) / (1 - sin(minAngle / 2));

        for (float d, e, f, g, x = nextR; x < r;)
        {
            rcount++;
            width = 2 * x;
            color = 1 + color % 14;
            centers[0] = calcCenter(Min, x);
            centers[1] = calcCenter(next, x);
            drawLine(centers[0], centers[1], width, color);
            area += calcLineArea(centers[0], centers[1], width);
            color = 1 + color % 14;
            centers[2] = calcCenter((Min + 2) % 4, x);
            centers[3] = calcCenter((next + 2) % 4, x);
            drawLine(centers[2], centers[3], width, color);
            area += calcLineArea(centers[0], centers[1], width);
            stop(quick_mode);

            d = x / tan(minAngle / 2) + sqrt(4 * x * minR - 4 * minR * minR);
            e = 1 / tan(minAngle / 2) / tan(minAngle / 2);
            f = 2 * d / tan(minAngle / 2) + 4 * minR;
            g = d * d + 4 * minR * minR;
            x = (f + sqrt(f * f - 4 * e * g)) / (2 * e);
        }

        width = 2 * r;
        color = 1 + color % 14;
        centers[0] = calcCenter(Min, r);
        centers[1] = calcCenter(next, r);
        drawLine(centers[0], centers[1], width, color);
        area += calcLineArea(centers[0], centers[1], width);
        rcount++;
        stop(quick_mode);

        centers[0] = calcCenter(Min, minR);
        centers[1] = calcCenter(next, minR);
        centers[2] = calcCenter((Min + 2) % 4, minR);
        centers[3] = calcCenter((next + 2) % 4, minR);

        width = 2 * minR + 1;
        color = 1 + color % 14;
        for (int i = 0; i < 4; i++)
        {
            drawLine(centers[i], centers[(i + 1) % 4], width, color);
            area += calcLineArea(centers[i], centers[(i + 1) % 4], width);
        }
        rcount++;
        stop(quick_mode);
        printf("--------------\nHuang's strategy:\nRadius count: %d\nCovered area: %.2f%%\n", rcount, area / totalArea() * 100);
    }

    void cover2()
    {
        color = 9;
        rcount = area = 0;
        int next;
        float r;
        if ((p[Min].y) == (p[(Min + 1) % 4].y))
        {
            next = (Min + 1) % 4;
            r = 0.5 * fabs((p[Min].y) - (p[(Min + 3) % 4].y));
        }
        else
        {
            next = (Min + 3) % 4;
            r = 0.5 * fabs((p[Min].y) - (p[(Min + 1) % 4].y));
        }

        Point centers[4];
        float width;

        centers[0] = calcCenter(Min, minR);
        centers[1] = calcCenter(next, minR);
        centers[2] = calcCenter((Min + 2) % 4, minR);
        centers[3] = calcCenter((next + 2) % 4, minR);
        width = 2 * minR;
        color = 1 + color % 14;
        for (int i = 0; i < 4; i++)
        {
            drawLine(centers[i], centers[(i + 1) % 4], width, color);
            area += calcLineArea(centers[i], centers[(i + 1) % 4], width);
        }
        rcount++;
        stop(quick_mode);

        int gen = 1;
        float x1;

        for (float x = minR * 2; x < r; x = x * 2)
        {
            float lastR = pow(2, gen - 1);
            cout << lastR << " and " << 2 * lastR << ": ";
            float gap = (12 * lastR * pow(cot(angle[Min] / 2), 2) - sqrt(128 * lastR * lastR * pow(cot(angle[Min] / 2), 4) - 16 * lastR * lastR * pow(cot(angle[Min] / 2), 6))) / (8 * (pow(cot(angle[Min] / 2), 2) + 1));
            cout << gap << endl;
            if (2 < gap)
                cout << "exceed!!!!!!!!" << endl;
            gen++;

            rcount++;
            width = 2 * x;
            color = 1 + color % 14;
            centers[0] = calcCenter(Min, x);
            centers[1] = calcCenter(next, x);
            drawLine(centers[0], centers[1], width, color);
            color = 1 + color % 14;
            centers[2] = calcCenter((Min + 2) % 4, x);
            centers[3] = calcCenter((next + 2) % 4, x);
            drawLine(centers[2], centers[3], width, color);
            area += calcLineArea(centers[0], centers[1], width) + calcLineArea(centers[2], centers[3], width);

            stop(quick_mode);
        }

        width = 2 * r;
        color = 1 + color % 14;
        centers[0] = calcCenter(Min, r);
        centers[1] = calcCenter(next, r);
        drawLine(centers[0], centers[1], width, color);
        area += calcLineArea(centers[0], centers[1], width);
        rcount++;
        stop(quick_mode);

        printf("--------------\nTraditional strategy:\nRadius count: %d\nCovered area: %.2f%%\n", rcount, area / totalArea() * 100);
    }

    void cover4()
    {
        totalLength = rcount = area = 0;
        int height = ((p[Min].y) == (p[(Min + 1) % 4].y))
                         ? fabs(p[Min].y - p[(Min + 3) % 4].y)
                         : fabs(p[Min].y - p[(Min + 1) % 4].y);

        float bigR = height / 4 + 1;
        Point circleCenters[4];

        for (int i = 0; i < 4; i++)
        {
            color = 1 + color % 14;
            circleCenters[i] = calcCenter(i, bigR);
        }
        stop(quick_mode);

        for (int i = 0; i < 4; i++)
        {
            color = 1 + color % 14;
            drawLine(circleCenters[i], circleCenters[(i + 1) % 4], 2 * bigR, color);
            area += calcLineArea(circleCenters[i], circleCenters[(i + 1) % 4], 2 * bigR);
            stop(quick_mode);
        }

        for (int i = 0; i < 4; i++)
            fragCover(bigR, circleCenters[i], i);

        printf("--------------\nZhuang's strategy:\nRadius count: %d\nCovered area: %.2f%%\n", rcount + 1, area / totalArea() * 100);
    }

    void fragCover(float refR, Point center, int ai)
    {
        float cR = refR * (1 - sin(angle[ai] / 2)) / 2;
        cR = round(cR);
        if (cR < minR)
            cR = minR;

        Point right, mid, left;
        mid = calcCenter(ai, cR);

        Vector refv = p[ai] - p[(ai + 1) % 4];
        Vector PO = center - p[ai];
        Vector tv1 = PO.normalize() + refv.orthX();
        Vector tv2 = PO.normalize() + refv.orthY();

        if (tv1.length() > tv2.length())
            right = center + refv.normalize().orthY() * (refR - cR);
        else
            right = center + refv.normalize().orthX() * (refR - cR);

        refv = p[ai] - p[(ai + 3) % 4];
        tv1 = PO.normalize() + refv.orthX();
        tv2 = PO.normalize() + refv.orthY();

        if (tv1.length() > tv2.length())
            left = center + refv.normalize().orthY() * (refR - cR);
        else
            left = center + refv.normalize().orthX() * (refR - cR);

        color = 1 + color % 14;
        setlinestyle(0, 0, 1);
        setcolor(color);

        drawLine(mid, right, 2 * cR + 1, color);
        area += calcLineArea(mid, right, 2 * cR);
        stop(quick_mode);
        drawLine(mid, left, 2 * cR + 1, color);
        area += calcLineArea(mid, left, 2 * cR);
        stop(quick_mode);
        rcount++;
        if (cR <= minR)
            return;
        fragCover(cR, mid, ai);
        return;
    }

    float marginArea()
    {
        float marginArea = 0;
        for (int i = 0; i < 4; i++)
            marginArea += minR * minR * (1 / tan(angle[i] / 2) - (M_PI - angle[i]) / 2);
        return marginArea;
    }

    float totalArea()
    {
        float totalArea = 0, height, bottom1, bottom2;
        if ((p[Min].y) == (p[(Min + 1) % 4].y))
        {
            height = fabs(p[Min].y - p[(Min + 3) % 4].y);
            bottom1 = fabs(p[Min].x - p[(Min + 1) % 4].x);
            bottom2 = fabs(p[(Min + 2) % 4].x - p[(Min + 3) % 4].x);
        }
        else
        {
            height = fabs(p[Min].y - p[(Min + 1) % 4].y);
            bottom1 = fabs(p[Min].x - p[(Min + 3) % 4].x);
            bottom2 = fabs(p[(Min + 2) % 4].x - p[(Min + 1) % 4].x);
        }
        totalArea = (bottom1 + bottom2) * height / 2;
        return totalArea;
    }

    void random()
    {
        for (int i = 0; i < 4; i++)
            p[i] = Point(0, 0);
        srand(time(NULL));
        while (totalArea() < 200000)
        {
            p[0] = Point(rand() % 800, 50 + rand() % 100);
            p[1] = p[0] + Vector(rand() % 800, 0);
            p[3] = Point(rand() % 800, p[0].y + rand() % 400);
            p[2] = p[3] + Vector(rand() % 800, 0);
        }
        calcAngle();
        return;
    }

    void scaling(float scale)
    {
        for (int i = 0; i < 4; i++)
            p[i] = Point(p[i].x * scale, p[i].y * scale);
        calcAngle();
        return;
    }

    void save()
    {
        FILE *fp = fopen("save4.txt", "w");
        for (int i = 0; i < 4; i++)
            fprintf(fp, "%f %f\n", p[i].x, p[i].y);
        fclose(fp);
    }
    void load()
    {
        FILE *fp = fopen("save4.txt", "r");
        if(fp == NULL)
        {
            fclose(fp);
            random();
            return;
        }
        for (int i = 0; i < 4; i++)
            fscanf(fp, "%f %f", &p[i].x, &p[i].y);
        fclose(fp);
        calcAngle();
    }
};