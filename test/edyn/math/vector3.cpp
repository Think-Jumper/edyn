#include <random>
#include <gtest/gtest.h>
#include <edyn/math/vector3.hpp>

class vector3_test: public ::testing::Test {
protected:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<edyn::scalar> dist;

    vector3_test() :
        gen(rd()),
        dist(-1e10, 1e10)
    {}

public:
    edyn::scalar random() {
        return dist(gen);
    }
    edyn::vector3 randomvec() {
        return {random(), random(), random()};
    }
};

TEST_F(vector3_test, fundamental) {
    auto a = randomvec();
    auto b = randomvec();

    ASSERT_EQ((a + b).x, a.x + b.x);
    ASSERT_EQ((a + b).y, a.y + b.y);
    ASSERT_EQ((a + b).z, a.z + b.z);

    ASSERT_EQ((a - b).x, a.x - b.x);
    ASSERT_EQ((a - b).y, a.y - b.y);
    ASSERT_EQ((a - b).z, a.z - b.z);

    ASSERT_EQ((-b).x, -b.x);
    ASSERT_EQ((-b).y, -b.y);
    ASSERT_EQ((-b).z, -b.z);

    auto s = random();

    ASSERT_EQ((a * s).x, a.x * s);
    ASSERT_EQ((a * s).y, a.y * s);
    ASSERT_EQ((a * s).z, a.z * s);

    ASSERT_EQ((s * b).x, s * b.x);
    ASSERT_EQ((s * b).y, s * b.y);
    ASSERT_EQ((s * b).z, s * b.z);
    
    ASSERT_EQ((a / s).x, a.x / s);
    ASSERT_EQ((a / s).y, a.y / s);
    ASSERT_EQ((a / s).z, a.z / s);

    ASSERT_EQ((s / b).x, s / b.x);
    ASSERT_EQ((s / b).y, s / b.y);
    ASSERT_EQ((s / b).z, s / b.z);
}

TEST_F(vector3_test, dot) {
    auto a = randomvec();
    auto b = randomvec();
    auto x = edyn::dot(a, b);
    auto y = a.x * b.x + a.y * b.y + a.z * b.z;
    ASSERT_EQ(x, y);
}

TEST_F(vector3_test, length) {
    auto v = randomvec();
    auto l2 = edyn::length2(v);
    ASSERT_EQ(l2, v.x * v.x + v.y * v.y + v.z * v.z);
    auto l = edyn::length(v);
    ASSERT_EQ(l, std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}   

TEST_F(vector3_test, normalize) {
    auto v = randomvec();
    auto nv = edyn::normalize(v);
    auto l = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    ASSERT_EQ(nv.x, v.x / l);
    ASSERT_EQ(nv.y, v.y / l);
    ASSERT_EQ(nv.z, v.z / l);
}

TEST_F(vector3_test, comparison) {
    auto v = randomvec();
    auto w = randomvec();
    ASSERT_TRUE(v == v);
    ASSERT_FALSE(v == w);
    ASSERT_FALSE(v != v);
    ASSERT_TRUE(v != w);
}