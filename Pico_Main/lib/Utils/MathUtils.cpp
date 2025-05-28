// #include "MathUtils.h"

// using namespace Eigen;


// Matrix<float, 3, 3>  skewMatix(Vector<float, 3> v)
// {
//     Matrix<float, 3, 3> skew = {
//         0, -v(2), v(1),
//         v(2), 0, -v(0),
//         -v(1), v(0), 0};

//     return skew;
// };

// Matrix<float, 3, 3> quat2rotm(Quaternion q)
// {

//     float qw = q(0);
//     float qx = q(1);
//     float qy = q(2);
//     float qz = q(3);

//     // Quaternion to rotation matrix conversion
//     float qxqx = qx * qx;
//     float qxqy = qx * qy;
//     float qxqz = qx * qz;
    
//     float qyqy = qy * qy;
//     float qyqz = qy * qz;

//     float qwqx = qw * qx;
//     float qwqy = qw * qy;
//     float qwqw = qw * qw;
//     float qwqz = qw * qz;

//     float qzqz = qz * qz;

//     Matrix<float, 3, 3> rotm = {
//         qwqw + qxqx - qyqy - qzqz, 2 * (qxqy - qwqz), 2 * (qxqz + qwqy),
//         2 * (qxqy + qwqz), qwqw - qxqx + qyqy - qzqz, 2 * (qyqz - qwqx),
//         2 * (qxqz - qwqy), 2 * (qwqx + qyqz), qwqw - qxqx - qyqy + qzqz};

//     return rotm;
// };

// Vector4f quatMult(Vector4f q1, Vector4f q2)
// {

//     float w1 = q1(0);
//     float i1 = q1(1);
//     float j1 = q1(2);
//     float k1 = q1(3);

//     float w2 = q2(0);
//     float i2 = q2(1);
//     float j2 = q2(2);
//     float k2 = q2(3);

//     Vector4f res;

//     res <<  w1 * w2 - i1 * i2 - j1 * j2 - k1 * k2,
//             w1 * i2 + i1 * w2 + j1 * k2 - k1 * j2,
//             w1 * j2 - i1 * k2 + j1 * w2 + k1 * i2, 
//             w1 * k2 + i1 * j2 - j1 * i2 + k1 * w2;

//     return res;
// };

// Vector4f quatNorm(Vector4f q)
// {
    
//     return q / q.norm();
// };
