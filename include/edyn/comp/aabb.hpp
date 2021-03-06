#ifndef EDYN_COMP_AABB_HPP
#define EDYN_COMP_AABB_HPP

#include "edyn/math/vector3.hpp"
#include "edyn/math/geom.hpp"

namespace edyn {

/**
 * @brief Axis-aligned bounding box.
 */
struct AABB {
    vector3 min;
    vector3 max;

	inline AABB inset(const vector3 &v) const {
		return {min + v, max - v};
	}

    inline vector3 center() const {
        return (min + max) * scalar(0.5);
    }
};

inline bool intersect(const AABB &b0, const AABB &b1) {
    return intersect_aabb(b0.min, b0.max, b1.min, b1.max);
}

inline AABB enclosing_aabb(const AABB &b0, const AABB &b1) {
    return {
        min(b0.min, b1.min),
        max(b0.max, b1.max)
    };
}

template<typename VectorIterator>
AABB calculate_aabb(VectorIterator vector_begin, VectorIterator vector_end) {
    // TODO: implement and use `parallel_reduce`.
    auto aabb = AABB{vector3_max, -vector3_max};

    for (auto it = vector_begin; it != vector_end; ++it) {
        aabb.min = min(aabb.min, *it);
        aabb.max = max(aabb.max, *it);
    }

    return aabb;
}

}

#endif // EDYN_COMP_AABB_HPP