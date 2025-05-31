#ifndef IMU_FILTER_H
#define IMU_FILTER_H

#include <cmath>
typedef signed short int int16;
class IMUFilter
{

public:
    IMUFilter();
    void update(const int16 &gyro_x, const int16 &gyro_y, const int16 &gyro_z, const int16 &acc_x, const int16 &acc_y, const int16 &acc_z);
    float roll = 0, pitch = 0, yaw = 0; // 欧拉角

private:
    const float ERROR_INT_LIMIT = 0.5f;     // 误差积分限幅
    const float Kp = 0.3f;                  // 加速度权重
    const float Ki = 0.003f;                // 误差积分增益
    const float DT = 0.01f;                 // 采样时间间隔
    const float ALPHA = 0.85f;              // 一阶低通滤波系数，新数据权重
    const float GYRO_SENSITIVITY = 0.07f;   // 陀螺仪灵敏度（LSB/dps），用于将原始数据转换为角速度(±2000dps)
    const float DEG_TO_RAD = 0.0174532925f; // 角度转弧度系数（π / 180）
    const float RAD_TO_DEG = 57.295779513f; // 弧度转角度系数（180 / π）

    float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;                               // 加速度计和陀螺仪测量值
    float ax_prev = 0, ay_prev = 0, az_prev = 0, gx_prev = 0, gy_prev = 0, gz_prev = 0; // 加速度计和陀螺仪上一次测量值

    float norm = 0;                          // 加速度归一化处理
    float q0 = 1.0f, q1 = 0, q2 = 0, q3 = 0; // 四元数
    float vx = 0, vy = 0, vz = 0;            // 重力向量在机体坐标系中的投影

    float ex = 0, ey = 0, ez = 0;                         // 加速度计测量值与预测值的误差
    float exInt = 0, eyInt = 0, ezInt = 0;                // 误差积分
    float gx_corr = 0, gy_corr = 0, gz_corr = 0;          // 陀螺仪数据修正
    float q0_dot = 0, q1_dot = 0, q2_dot = 0, q3_dot = 0; // 四元数导数
    float pp = 0;                                         // 四元数归一化
};

#endif // IMU_FILTER_H