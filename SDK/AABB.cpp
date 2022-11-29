#include "AABB.h"


AABB::AABB(const AABB& aabb) {
    min = vec3_t(aabb.min);
    max = vec3_t(aabb.max);
}

//IDA Pro AABB::getsize();
float AABB::getsize() {
    return (float)((float)((float)(*((float*)this + 4) - *((float*)this + 1))
        + (float)(*((float*)this + 3) - *(float*)this))
        + (float)(*((float*)this + 5) - *((float*)this + 2)))
        * 0.33333334f;
}

//IDA Pro AABB::isValid();
bool AABB::isValid() {
    return *((float*)this + 3) >= *(float*)this
        && *((float*)this + 4) >= *((float*)this + 1)
        && *((float*)this + 5) >= *((float*)this + 2);
}

vec3_t AABB::getCenter() {
    vec3_t center;
    center.x = (this->max.x - this->min.x) * 0.5f + this->min.x;
    center.y = (this->max.y - this->min.y) * 0.5f + this->min.y;
    center.z = (this->max.z - this->min.z) * 0.5f + this->min.z;
    return center;
}

//IDA Pro AABB::getVolume;
float AABB::getVolume() {
    return (float)((float)(*((float*)this + 4) - *((float*)this + 1)) * (float)(*((float*)this + 3) - *(float*)this))
        * (float)(*((float*)this + 5) - *((float*)this + 2));
}

vec3_t AABB::getBounds() {
    vec3_t bounds;
    bounds.x = this->max.x - this->min.x;
    bounds.y = this->max.y - this->min.y;
    bounds.z = this->max.z - this->min.z;
    return bounds;
}

inline bool AABB::operator==(const AABB& rhs) const {
    return min == rhs.min && max == rhs.max;
}

inline bool AABB::operator!=(const AABB& rhs) const {
    return min != rhs.min || max != rhs.max;
}