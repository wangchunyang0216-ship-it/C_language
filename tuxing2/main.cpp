#include <iostream>
#include <cmath>
#include <vector>

// 3D点结构体
struct Point3D {
    float x, y, z;
};

// 2D点结构体
struct Point2D {
    float x, y;
};

// 透视投影计算
Point2D perspective_projection(const Point3D& p) {
    Point2D projected;
    if (p.z <= 1e-6) { // 判断点是否在近平面后面
        projected.x = -1; // 返回无效值表示裁剪
        projected.y = -1;
        return projected;
    }
    projected.x = -p.x / p.z;
    projected.y = -p.y / p.z;
    return projected;
}

// 将投影坐标转换为屏幕空间
Point2D to_screen_space(const Point2D& p, int width, int height, float s) {
    Point2D screen;
    screen.x = ((p.x + s) / (2 * s)) * width;
    screen.y = height - ((p.y + s) / (2 * s)) * height;
    return screen;
}

// 渲染三角形
void render_triangle(const Point3D& p0, const Point3D& p1, const Point3D& p2, int width, int height) {
    // 输出每个顶点的z值
    std::cout << "p0 z: " << p0.z << ", p1 z: " << p1.z << ", p2 z: " << p2.z << std::endl;

    // 透视投影
    Point2D p0_projected = perspective_projection(p0);
    Point2D p1_projected = perspective_projection(p1);
    Point2D p2_projected = perspective_projection(p2);

    if (p0_projected.x == -1 || p1_projected.x == -1 || p2_projected.x == -1) {
        std::cout << "Triangle is behind the near plane. Not rendered." << std::endl;
        return;
    }

    // 转换为屏幕空间
    Point2D p0_screen = to_screen_space(p0_projected, width, height, 1.0);
    Point2D p1_screen = to_screen_space(p1_projected, width, height, 1.0);
    Point2D p2_screen = to_screen_space(p2_projected, width, height, 1.0);

    // 输出渲染结果
    std::cout << "Triangle rendered with vertices at:" << std::endl;
    std::cout << "p0: (" << p0_screen.x << ", " << p0_screen.y << ")" << std::endl;
    std::cout << "p1: (" << p1_screen.x << ", " << p1_screen.y << ")" << std::endl;
    std::cout << "p2: (" << p2_screen.x << ", " << p2_screen.y << ")" << std::endl;
}

int main() {
    // 三角形顶点，调整Z值以确保在相机前方
    Point3D p0 = {-1, -1, -3};
    Point3D p1 = {0, 1, -3};
    Point3D p2 = {1, -1, -3};

    render_triangle(p0, p1, p2, 640, 480);

    return 0;
}
