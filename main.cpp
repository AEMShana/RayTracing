#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#include <iostream>
#include "utils.h"
#include "hittable_list.h"
#include "sphere.h"
#include "triangle.h"
#include "camera.h"
#include "material.h"
using namespace std;
using namespace Eigen;

const int image_width = 400;        // 图像宽度
const int image_height = 250;       // 图像高度
const int samples_per_pixel = 5;   // 每个像素采样的光线数
const int max_depth = 5;            // 光线弹射的最大次数

char image_buf[image_height * image_width * 3];


// 显示法线颜色
// Vector3d ray_color(const ray& r, const hittable& world) {
//     hit_record rec;
//     if (world.hit(r, 0, infinity, rec))
//         return 0.5 * (rec.normal + Vector3d(1, 1, 1));
//     Vector3d unit_direction = r.direction().normalized();
//     double t = 0.5 * (unit_direction.y() + 1.0);
//     return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
// }


// 漫反射材质
Vector3d ray_color(const ray& r, const hittable& world, int depth) {
    // depth 光线弹射次数
    if (depth <= 0) return Vector3d(0, 0, 0);
    hit_record rec;
    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        Vector3d attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation.cwiseProduct(ray_color(scattered, world, depth - 1));
        return Vector3d(0, 0, 0);
    }
    Vector3d unit_direction = r.direction().normalized();
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Vector3d(1.0, 1.0, 1.0) + t * Vector3d(0.5, 0.7, 1.0);
}


void write_color(char* buf, Vector3d vec, int x, int y, int samples_per_pixel) {
    double scale = 1.0 / samples_per_pixel;
    double r = sqrt(scale * vec.x());
    double g = sqrt(scale * vec.y());
    double b = sqrt(scale * vec.z());

    int index = (image_height - x - 1) * image_width * 3 + y * 3;
    image_buf[index + 0] = static_cast<char>(256 * clamp(r, 0.0, 0.999));
    image_buf[index + 1] = static_cast<char>(256 * clamp(g, 0.0, 0.999));
    image_buf[index + 2] = static_cast<char>(256 * clamp(b, 0.0, 0.999));
}

void show_progress(int line) { // 显示进度
    cerr << "\rprogress: ";
    int cur = 50 * ((double)(image_height - line) / (double)image_height);
    for (int i = 1;i <= cur;++i) cerr << '=';
    cerr << ' ' << cur * 2 << '%' << flush;
}

int main() {
    Vector3d lower_left_corner(-(double)image_width / 200.0, -(double)image_height / 200.0, -1.5);
    Vector3d horizontal((double)image_width / 100.0, 0, 0);
    Vector3d vertical(0, (double)image_height / 100.0, 0);
    Vector3d origin(0, 0, 0);

    hittable_list world;
    hittable_list cube;


    cube.load_obj("./model/cube.obj"); //读取obj

    // 添加物体
    world.add(make_shared<sphere>(Vector3d(0, 0, -1), 0.5, make_shared<lambertian>(Vector3d(0.0, 0.9, 0.1))));
    world.add(make_shared<sphere>(Vector3d(0, -100.5, -1), 100, make_shared<lambertian>(Vector3d(0.0, 0.3, 0.6))));
    // world.add(make_shared<triangle>(Vector3d(-1, -1, -1), Vector3d(1, -1, -1), Vector3d(0, 1, -1), make_shared<metal>(Vector3d(0.8, 0.1, 0.1), 0.3)));
    // world.add(make_shared<triangle>(Vector3d(-1.1, -1, -0.7), Vector3d(-0.1, -1.4, -1.4), Vector3d(-0.8, 1, -1.2), make_shared<metal>(Vector3d(0.8, 0.4, 0.0), 0.0)));

    world.add(make_shared<sphere>(Vector3d(1, 0, -1), 0.5, make_shared<metal>(Vector3d(0.8, 0.6, 0.2), 0.0)));
    // world.add(make_shared<hittable_list>(world2));

    world.add(make_shared<hittable_list>(cube));


    camera cam;

    for (int i = image_height - 1;i >= 0;--i) {
        show_progress(i);
        for (int j = 0;j < image_width;++j) {
            Vector3d color(0, 0, 0);
            for (int s = 0;s < samples_per_pixel;++s) {
                double u = double(j + random_double()) / image_width;
                double v = double(i + random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                color += ray_color(r, world, max_depth);
            }
            write_color(image_buf, color, i, j, samples_per_pixel);
        }
    }
    stbi_write_png("image.png", image_width, image_height, 3, image_buf, 3 * image_width);
    cerr << "\nDone.\n";

    return 0;
};