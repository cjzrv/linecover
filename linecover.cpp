#include <iostream>
#include <graphics.h>
#include <unistd.h>
#include "triangle.h"

using namespace std;

void show_usage()
{
    cerr << "Usage: linecover [-3] [-r] [-q] [-h] [-b] [-s]" << endl;
    cerr << "Options:" << endl;
    cerr << "  -3\tProcess triangle" << endl;
    cerr << "  -r\tRandom mode" << endl;
    cerr << "  -q\tQuick mode" << endl;
    cerr << "  -h\tUse cover3 algorithm" << endl;
    cerr << "  -b\tUse both cover3 and cover4 algorithms" << endl;
    cerr << "  -s\tSave figure" << endl;
    cerr << "Example: linecover -3rb" << endl;
}

int main(int argc, char **argv)
{
    trapezoid trapezoid;
    triangle triangle;
    bool is_triangle = false;
    bool random_mode = false;
    bool quick_mode = false;
    bool use_cover3 = false;
    bool use_both = false;
    bool save_file = false;
    int opt;
    float scale;

    initwindow(1600, 800);

    // 檢查輸入參數，如果沒有 dash 則自動加上 dash
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i][0] != '-')
        {
            string new_arg = "-" + string(argv[i]);
            argv[i] = strdup(new_arg.c_str());
        }
    }

    while ((opt = getopt(argc, argv, "34rqhbs")) != -1)
    {
        switch (opt)
        {
        case '3': // 三角形
            is_triangle = true;
            break;
        case '4': // 四邊形
            break;
        case 'r': // Random
            random_mode = true;
            break;
        case 'q': // quick mode (不使用 getch() 一口氣做完)
            quick_mode = true;
            break;
        case 'h': // 黃老師的覆蓋法
            use_cover3 = true;
            break;
        case 'b': // 使用兩種覆蓋法（做比較用）
            use_both = true;
            break;
        case 's': // 保存圖形
            save_file = true;
            break;

        default:
            show_usage();
            return 1;
        }
    }

    if (is_triangle)
    {
        if (random_mode)
            triangle.random();
        else
            triangle.load();
        triangle.draw();
        triangle.quick_mode = quick_mode;
        if (use_both)
        {
            triangle.cover3();
            cleardevice();
            triangle.draw();
            triangle.cover4();
        }
        else if (use_cover3)
            triangle.cover3();
        else
            triangle.cover4();

        if (save_file)
            triangle.save();
    }
    else
    {
        if (random_mode)
            trapezoid.random();
        else
            trapezoid.load();
        trapezoid.draw();
        trapezoid.quick_mode = quick_mode;
        if (use_both)
        {
            trapezoid.cover3();
            cleardevice();
            trapezoid.draw();
            trapezoid.cover4();
        }
        else if (use_cover3)
            trapezoid.cover3();
        else
            trapezoid.cover4();

        if (save_file)
            trapezoid.save();
    }

    closegraph();
    cout << endl;
    return 0;
}