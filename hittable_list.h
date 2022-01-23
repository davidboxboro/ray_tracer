#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    HittableList() {}
    HittableList(std::shared_ptr<Hittable> object) {
        add(object);
    }
    virtual ~HittableList() = default;

    void clear() {
        objects.clear();
    }
    void add(std::shared_ptr<Hittable> object) {
        objects.push_back(object);
    }

    virtual bool hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const override;

private:
    std::vector<std::shared_ptr<Hittable>> objects;
};

inline bool HittableList::hit(const Ray& r, double tMin, double tMax, HitRecord& rec) const {
    HitRecord tmpRec;
    bool hitAnything = false;
    double closestSoFar = tMax;
    for (const auto& object : objects) {
        if (object->hit(r, tMin, closestSoFar, tmpRec)) {
            hitAnything = true;
            closestSoFar = tmpRec.t;
            rec = tmpRec;
        }
    }
    return hitAnything;
}

#endif