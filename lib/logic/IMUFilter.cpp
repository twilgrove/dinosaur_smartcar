#include "IMUFilter.h"
template <typename T>
T clamp(T value, T minVal, T maxVal)
{
    if (value < minVal)
        return minVal;
    if (value > maxVal)
        return maxVal;
    return value;
}

IMUFilter::IMUFilter() {}

void IMUFilter::update(const int16 &gyro_x, const int16 &gyro_y, const int16 &gyro_z, const int16 &acc_x, const int16 &acc_y, const int16 &acc_z)
{
    // 1.传感器数据滤波处理
    //  加速度计一阶低通滤波
    ax = acc_x * ALPHA + (1 - ALPHA) * ax_prev;
    ay = acc_y * ALPHA + (1 - ALPHA) * ay_prev;
    az = acc_z * ALPHA + (1 - ALPHA) * az_prev;
    // 更新加速度历史值
    ax_prev = ax;
    ay_prev = ay;
    az_prev = az;

    // 2.陀螺仪数据处理
    //  陀螺仪一阶低通滤波
    gx = (GYRO_SENSITIVITY * gyro_x) * DEG_TO_RAD * ALPHA + (1 - ALPHA) * gx_prev;
    gy = (GYRO_SENSITIVITY * gyro_y) * DEG_TO_RAD * ALPHA + (1 - ALPHA) * gy_prev;
    gz = (GYRO_SENSITIVITY * gyro_z) * DEG_TO_RAD * ALPHA + (1 - ALPHA) * gz_prev;
    // 更新角速度历史值
    gx_prev = gx;
    gy_prev = gy;
    gz_prev = gz;

    // 3. 加速度归一化处理（将加速度向量转换为单位向量）
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax /= norm;
    ay /= norm;
    az /= norm;

    // 异常处理：加速度过小时跳过计算
    if (norm < 1e-6)
    {
        ax = 0;
        ay = 0;
        az = 0;
        return;
    }

    // 4. 计算预测的重力方向（通过当前四元数）
    // 重力向量在机体坐标系中的投影
    vx = 2 * (q1 * q3 - q0 * q2);               // 2(q1q3 - q0q2)
    vy = 2 * (q0 * q1 + q2 * q3);               // 2(q0q1 + q2q3)
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3; // q0² - q1² - q2² + q3²

    // 5. 计算加速度计测量值与预测值的误差（叉积）
    ex = ay * vz - az * vy; // 横滚轴误差
    ey = az * vx - ax * vz; // 俯仰轴误差
    ez = ax * vy - ay * vx; // 偏航轴误差

    // 6. 误差积分（用于消除稳态误差）
    exInt += ex * Ki * DT;
    eyInt += ey * Ki * DT;
    ezInt += ez * Ki * DT;

    exInt = clamp(exInt, -ERROR_INT_LIMIT, ERROR_INT_LIMIT);
    eyInt = clamp(eyInt, -ERROR_INT_LIMIT, ERROR_INT_LIMIT);
    ezInt = clamp(ezInt, -ERROR_INT_LIMIT, ERROR_INT_LIMIT);

    // 7. 陀螺仪数据修正（比例-积分反馈）
    gx_corr = gx + Kp * ex + exInt;
    gy_corr = gy + Kp * ey + eyInt;
    gz_corr = gz + Kp * ez + ezInt;

    // 8. 四元数更新（微分方程离散化）
    // 四元数导数计算
    q0_dot = -q1 * gx_corr - q2 * gy_corr - q3 * gz_corr;
    q1_dot = q0 * gx_corr + q2 * gz_corr - q3 * gy_corr;
    q2_dot = q0 * gy_corr - q1 * gz_corr + q3 * gx_corr;
    q3_dot = q0 * gz_corr + q1 * gy_corr - q2 * gx_corr;

    // 四元数积分更新（0.5*DT为离散化系数）
    q0 += q0_dot * DT * 0.5f;
    q1 += q1_dot * DT * 0.5f;
    q2 += q2_dot * DT * 0.5f;
    q3 += q3_dot * DT * 0.5f;

    // 9. 四元数归一化（避免数值误差累积）
    pp = sqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 /= pp;
    q1 /= pp;
    q2 /= pp;
    q3 /= pp;

    // 10. 四元数转欧拉角（Z-Y-X旋转顺序）
    roll = atan2(2 * (q2 * q3 + q0 * q1), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * RAD_TO_DEG;

    pitch = asin(-2 * (q1 * q3 - q0 * q2)) * RAD_TO_DEG;

    yaw = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * RAD_TO_DEG;
}