//#include "tgaimage.h"
//#include <iostream>
//
//const TGAColor white = TGAColor(255, 255, 255, 255);
//const TGAColor red   = TGAColor(255, 0,   0,   255);
//
//int main(int argc, char** argv) {
//
////    //1
////        // 创建一个 640x360 的 TGAImage 对象
////    TGAImage image(640, 360, TGAImage::RGBA);
////
////    // 设置背景颜色：红色0.5，绿色0.5，蓝色0.5
////   // TGAColor background(128, 128, 128, 255);  // 灰色背景 (128,128,128)
////    image.clear();
////
////    // --- 绘制第一个圆形：有填充和无填充 ---
////    // 填充颜色为红(0.3), 绿(0.5), 蓝(0.8)
////    TGAColor circleFillColor(77, 128, 204, 255); // 填充颜色
////    // 无填充时，边框颜色为红(0.3), 绿(0.5), 蓝(0.8)
////    TGAColor circleBorderColor(77, 128, 204, 255); // 边框颜色
////
////    // 绘制填充圆形，圆心(320, 180)，半径160
////    image.draw_circle(320, 180, 160, circleFillColor, true);  // 填充圆形
////
////    // 绘制无填充圆形，圆心(320, 180)，半径160
////    image.draw_circle(320, 180, 160, circleBorderColor, false);  // 边框圆形
////
////    // --- 绘制第二个圆形：有填充和无填充 ---
////    // 填充颜色为红(0.8), 绿(0.3), 蓝(0.5)
////    TGAColor circle2FillColor(204, 77, 128, 255); // 填充颜色
////    // 无填充时，边框颜色为红(0.8), 绿(0.3), 蓝(0.5)
////    TGAColor circle2BorderColor(204, 77, 128, 255); // 边框颜色
////
////    // 绘制填充圆形，圆心(150, 80)，半径80
////    image.draw_circle(150, 80, 80, circle2FillColor, true);  // 填充圆形
////
////    // 绘制无填充圆形，圆心(150, 80)，半径80
////    image.draw_circle(150, 80, 80, circle2BorderColor, false);  // 边框圆形
////
////    // --- 绘制矩形：有填充和无填充 ---
////    TGAColor rectangleFillColor(0, 255, 0, 255);  // 填充颜色：绿色
////    TGAColor rectangleBorderColor(0, 255, 0, 255);  // 边框颜色：绿色
////
////    // 绘制填充矩形，左上角(50, 250)，右下角(200, 300)
////    image.draw_rectangle(50, 250, 200, 300, rectangleFillColor, true);  // 填充矩形
////
////    // 绘制无填充矩形，左上角(50, 250)，右下角(200, 300)
////    image.draw_rectangle(50, 250, 200, 300, rectangleBorderColor, false);  // 边框矩形
////
////    // --- 绘制三角形：有填充和无填充 ---
////    TGAColor triangleFillColor(0, 0, 255, 255);  // 填充颜色：蓝色
////    TGAColor triangleBorderColor(0, 0, 255, 255);  // 边框颜色：蓝色
////
////    // 绘制填充三角形，顶点为(400, 250), (500, 350), (600, 250)
////    image.draw_triangle(400, 250, 500, 350, 600, 250, triangleFillColor, true);  // 填充三角形
////
////    // 绘制无填充三角形，顶点为(400, 250), (500, 350), (600, 250)
////    image.draw_triangle(400, 250, 500, 350, 600, 250, triangleBorderColor, false);  // 边框三角形
////
////    // 保存图像为 tga 文件
////    if (image.write_tga_file("output_with_both_filled_and_borders.tga", true)) {
////        std::cout << "图像保存成功！" << std::endl;
////    } else {
////        std::cout << "图像保存失败！" << std::endl;
////    }
//
//
//
//
////    TGAImage image(640, 360, TGAImage::RGB);
////    TGAColor background(128, 128, 128, 255); // 背景颜色为灰色
////    image.clear();
////    image.fill(background);
////
////    // 绘制第一个圆
////    TGAColor circle1Color(77, 128, 204, 255); // 填充颜色
////    image.draw_circle(320, 180, 160, circle1Color, true);
////
////    // 绘制第二个圆
////    TGAColor circle2Color(204, 77, 128, 255); // 填充颜色
////    image.draw_circle(150, 80, 80, circle2Color, true);
////
////    // 绘制第三个圆
////    TGAColor circle3Color(204, 77, 128, 255); // 填充颜色
////    image.draw_circle(490, 80, 80, circle3Color, true);
////
////    // 保存文件
////    image.write_tga_file("output.tga");
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////  // 创建一个640x360的图像，RGBA格式（每个像素4字节）
////    TGAImage image(640, 360, 4);
////
////    // 定义填充颜色
////    TGAColor fill_color1(76, 127, 204, 255);  // 填充颜色（红：0.3, 绿：0.5, 蓝：0.8）
////    TGAColor fill_color2(204, 76, 127, 255);  // 填充颜色（红：0.8, 绿：0.3, 蓝：0.5）
////
////    // 绘制圆形
////    image.draw_circle(320, 180, 160, fill_color1);  // 第一个圆：填充，圆心(320, 180)，半径160，填充颜色(76, 127, 204)
////    image.draw_circle(150, 80, 80, fill_color2);   // 第二个圆：填充，圆心(150, 80)，半径80，填充颜色(204, 76, 127)
////    image.draw_circle(490, 80, 80, fill_color2);   // 第三个圆：填充，圆心(490, 80)，半径80，填充颜色(204, 76, 127)
////
////    // 保存图像到文件
////    image.write_tga_file("output1_1.tga");
////
////    std::cout << "图像已保存为 output.tga1_1" << std::endl;
//
//
//
//
//
//
//        // 创建一个640x360的图像，RGBA格式（每个像素4字节）
//    TGAImage image(640, 360, 4);
//
//    // 定义背景颜色为红（0.5），绿（0.5），蓝（0.5）
//    TGAColor backgroundColor(128, 128, 128, 255);  // RGB(0.5, 0.5, 0.5) -> (128, 128, 128)
//    image.clear();
//    image.fill(backgroundColor); // 填充背景颜色
//
//    // 定义填充颜色
//    TGAColor fillColor1(77, 128, 204, 255);  // 填充颜色（红：0.3, 绿：0.5, 蓝：0.8）
//    TGAColor fillColor2(204, 77, 128, 255);  // 填充颜色（红：0.8, 绿：0.3, 蓝：0.5）
//
//    // 绘制第一个圆：圆心(320, 180)，半径160，填充颜色（红：0.3，绿：0.5，蓝：0.8），不填充颜色时只绘制边框
//    image.draw_circle(320, 180, 160, fillColor1, false);  // 第一个圆：不填充（仅边框）
//
//    // 绘制第二个圆：圆心(150, 80)，半径80，填充颜色（红：0.8，绿：0.3，蓝：0.5），不填充颜色时只绘制边框
//    image.draw_circle(150, 80, 80, fillColor2, false);   // 第二个圆：不填充（仅边框）
//
//    // 绘制第三个圆：圆心(490, 80)，半径80，填充颜色（红：0.8，绿：0.3，蓝：0.5），不填充颜色时只绘制边框
//    image.draw_circle(490, 80, 80, fillColor2, false);   // 第三个圆：不填充（仅边框）
//
//    // 保存图像到文件
//    image.write_tga_file("output1_1.tga");
//
//    std::cout << "图像已保存为 output1_1.tga" << std::endl;
//
//
//
//
////    //2
////    // 创建一个640x360的图像，RGBA格式（每个像素4字节）
////    TGAImage image(640, 360, 4);
////
////    // 定义填充颜色
////    TGAColor fill_color1(76, 127, 204, 255);  // 填充颜色（红：0.3, 绿：0.5, 蓝：0.8）
////    TGAColor fill_color2(204, 76, 127, 255);  // 填充颜色（红：0.8, 绿：0.3, 蓝：0.5）
////
////    // 绘制圆形（填充）
////    image.draw_circle(320, 180, 160, fill_color1, true);  // 第一个圆：填充，圆心(320, 180)，半径160，填充颜色(76, 127, 204)
////    image.draw_circle(150, 80, 80, fill_color2, true);   // 第二个圆：填充，圆心(150, 80)，半径80，填充颜色(204, 76, 127)
////    image.draw_circle(490, 80, 80, fill_color2, true);   // 第三个圆：填充，圆心(490, 80)，半径80，填充颜色(204, 76, 127)
////
////    // 保存图像到文件
////    image.write_tga_file("output1.tga");
////void TGAImage::draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor color, bool fill = false) {
//
//
//
////    TGAImage image(800, 450, TGAImage::RGB); // 创建图像，宽800，高450，RGB模式
////    TGAColor background(128, 128, 128, 255); // 背景颜色为灰色（RGB(0.5, 0.5, 0.5)）
////    image.clear();
////    image.fill(background); // 用背景色填充整个画布
////
////    // 绘制第一个圆形
////    TGAColor circleColor(77, 204, 77, 255); // 填充颜色为绿色（RGB(0.3, 0.8, 0.3)）
////    image.draw_circle(400, 225, 200, circleColor, true); // 圆心在(400, 225)，半径为200，填充颜色为绿色
////
////    // 绘制第二个矩形
////    TGAColor rectColor(204, 77, 204, 255); // 填充颜色为紫色（RGB(0.8, 0.3, 0.8)）
////    image.draw_rectangle(100, 100, 300, 350, rectColor, true); // 左上坐标(100, 100)，右下坐标(300, 350)，填充颜色为紫色
////
////    // 绘制第三个三角形
////   TGAColor triColor(204, 204, 128, 255); // 填充颜色为黄色（RGB(0.8, 0.8, 0.5)）
////    image.draw_triangle(750, 50, 700, 400, 200, 250, triColor, true); // 顶点坐标分别为(750, 50)，(700, 400)，(200, 250)，填充颜色为黄色
////
////    // 保存文件
////    image.write_tga_file("output3.tga"); // 将结果保存为output3.tga
//
//
//
//
//
//// // 创建一个图像对象，画布大小800x450，使用RGB模式
////    TGAImage image(800, 450, TGAImage::RGB);
////
////    // 背景颜色为灰色 (0.5, 0.5, 0.5)
////    TGAColor background(128, 128, 128, 255);  // RGB(0.5, 0.5, 0.5) = 128, 128, 128
////    image.clear();
////    image.fill(background);
////
////    // 第一个图形：圆形（不填充），圆心(400, 225)，半径200，边框颜色为(0.3, 0.8, 0.3)
////    TGAColor circleBorderColor(77, 204, 77, 255); // RGB(0.3, 0.8, 0.3)
////    image.draw_circle(400, 225, 200, circleBorderColor, false); // 不填充
////
////    // 第二个图形：矩形（不填充），左上坐标(100, 100)，右下坐标(300, 350)，边框颜色为(0.8, 0.3, 0.8)
////    TGAColor rectBorderColor(204, 77, 204, 255);  // RGB(0.8, 0.3, 0.8)
////    image.draw_rectangle(100, 100, 300, 350, rectBorderColor, false); // 不填充
////
////    // 第三个图形：三角形（不填充），三个顶点坐标：p0(750, 50), p1(700, 400), p2(200, 250)，边框颜色为(0.8, 0.8, 0.5)
////    TGAColor triBorderColor(204, 204, 128, 255);  // RGB(0.8, 0.8, 0.5)
////    image.draw_triangle(750, 50, 700, 400, 200, 250, triBorderColor, false); // 不填充
////
////    // 保存文件
////    image.write_tga_file("output4.tga");
////
////    std::cout << "图像已保存为 output4.tga" << std::endl;
//
//
//
//
//    return 0;
//}

















//-----------------------------------------------------------
#include "tgaimage.h"
#include <iostream>
#include <vector>
#include <algorithm>



// 主函数
int main() {
    // 创建一个640x480的图像，RGBA格式（每个像素4字节）
    TGAImage image(640, 480, 4);

    // 定义背景颜色为灰色
    TGAColor backgroundColor(128, 128, 128, 255);  // RGB(0.5, 0.5, 0.5) -> (128, 128, 128)
    image.clear();
    image.fill(backgroundColor); // 填充背景颜色

    // 三角形1：顶点坐标(-1, -1, -3), (0, 1, -3), (1, -1, -3), 填充颜色RGB(0.7, 0.3, 0.4)
    TGAColor color1(178, 76, 102, 255);
    image.project_and_draw_triangle(-1, -1, -3, 0, 1, -3, 1, -1, -3, color1, true);

    // 三角形2：顶点坐标(-1, 0, -2), (0, 2, -4), (1, -2, -5), 填充颜色RGB(0.7, 0.3, 0.8)
    TGAColor color2(178, 76, 204, 255);
    image.project_and_draw_triangle(-1, 0, -2, 0, 2, -4, 1, -2, -5, color2, true);

    // 三角形3：顶点坐标(2, 2, -1), (-1, -1, -2), (2, 0, -3), 填充颜色RGB(0.7, 0.8, 0.2)
    TGAColor color3(178, 204, 51, 255);
    image.project_and_draw_triangle(2, 2, -1, -1, -1, -2, 2, 0, -3, color3, true);

    // 三角形4：顶点坐标(200, 200, -100), (1, 3, -4), (-2, -2, -2), 填充颜色RGB(0.8, 0.1, 0.3)
    TGAColor color4(204, 25, 77, 255);
    image.project_and_draw_triangle(200, 200, -100, 1, 3, -4, -2, -2, -2, color4, true);

    // 保存图像到文件
    image.write_tga_file("output3d.tga");

    std::cout << "图像已保存为 output3d.tga" << std::endl;

    return 0;
}

