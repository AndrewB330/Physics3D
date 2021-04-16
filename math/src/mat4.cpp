#include <math/mat4.hpp>

Mat4 Mat4::Transformation(Vec3 translation, Quat rotation) {
    Mat3 r = rotation.Mat();
    Mat4 res;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            res.val[i][j] = r[i][j];
        }
    }
    res.val[3][3] = 1;
    res.val[0][3] = translation.x;
    res.val[1][3] = translation.y;
    res.val[2][3] = translation.z;
    return res;
}

Mat4 Mat4::TransformationInv(Vec3 translation, Quat rotation) {
    Mat3 r = rotation.Mat().T(); // inverse
    Mat4 res;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            res.val[i][j] = r[i][j];
        }
    }
    res.val[3][3] = 1;
    translation = r * (-translation); // inverse translation
    res.val[0][3] = -translation.x;
    res.val[1][3] = -translation.y;
    res.val[2][3] = -translation.z;
    return res;
}
