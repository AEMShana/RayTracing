#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "OBJ_Loader.h"
#include "triangle.h"
#include "material.h"
#include <memory>
#include <vector>
using std::string;
using std::shared_ptr;
using std::make_shared;

class hittable_list :public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

public:
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }
    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    void load_obj(string obj_path) {
        // 在blender中物体要移动到y轴正方向
        objl::Loader objLoader;
        objLoader.LoadFile(obj_path);
        for (auto& mesh : objLoader.LoadedMeshes) {
            for (int i = 2;i < mesh.Indices.size();i += 3) {
                Vector3d a(
                    mesh.Vertices[mesh.Indices[i - 2]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 2]].Position.Z);

                Vector3d b(
                    mesh.Vertices[mesh.Indices[i - 1]].Position.X,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Y,
                    mesh.Vertices[mesh.Indices[i - 1]].Position.Z);

                Vector3d c(
                    mesh.Vertices[mesh.Indices[i]].Position.X,
                    mesh.Vertices[mesh.Indices[i]].Position.Y,
                    mesh.Vertices[mesh.Indices[i]].Position.Z);

                add(make_shared<triangle>(a, b, c, make_shared<metal>(Vector3d(0.8, 0.1, 0.1), 0.3)));
            }
        }
    }

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closet_so_far = t_max; // 到目前为止最近的hit点

    for (const auto& object : objects) {
        if (object->hit(r, t_min, closet_so_far, temp_rec)) {
            hit_anything = true;
            closet_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif