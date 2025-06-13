
#include "headfile.h"
using namespace std;
// 一阶
pair<double, double> calc_rss_linear(const vector<double> &y)
{
    int n = y.size();
    double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0;

    for (int i = 0; i < n; ++i)
    {
        sum_x += i;
        sum_y += y[i];
        sum_x2 += i * i;
        sum_xy += i * y[i];
    }

    double a = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x); // 斜率
    double b = (sum_y - a * sum_x) / n;

    double rss = 0;
    for (int i = 0; i < n; ++i)
    {
        double pred = a * i + b;
        rss += (y[i] - pred) * (y[i] - pred);
    }

    return {rss, a};
}

// 二阶
double calc_rss_quadratic(const vector<double> &y)
{
    int n = y.size();
    double sum_x = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0;
    double sum_y = 0, sum_xy = 0, sum_x2y = 0;

    for (int i = 0; i < n; ++i)
    {
        double x = i;
        double x2 = x * x;
        sum_x += x;
        sum_x2 += x2;
        sum_x3 += x2 * x;
        sum_x4 += x2 * x2;
        sum_y += y[i];
        sum_xy += x * y[i];
        sum_x2y += x2 * y[i];
    }

    // 解二次回归的正规方程组：Ax = B
    double A[3][3] = {
        {n, sum_x, sum_x2},
        {sum_x, sum_x2, sum_x3},
        {sum_x2, sum_x3, sum_x4}};
    double B[3] = {sum_y, sum_xy, sum_x2y};
    double coef[3] = {0};

    // 使用高斯消元法求解
    for (int i = 0; i < 3; ++i)
    {
        // 选主元
        for (int j = i + 1; j < 3; ++j)
        {
            if (abs(A[j][i]) > abs(A[i][i]))
            {
                swap(A[i], A[j]);
                swap(B[i], B[j]);
            }
        }

        // 消元
        for (int j = i + 1; j < 3; ++j)
        {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < 3; ++k)
                A[j][k] -= factor * A[i][k];
            B[j] -= factor * B[i];
        }
    }

    // 回代
    for (int i = 2; i >= 0; --i)
    {
        coef[i] = B[i];
        for (int j = i + 1; j < 3; ++j)
            coef[i] -= A[i][j] * coef[j];
        coef[i] /= A[i][i];
    }

    // RSS
    double rss = 0;
    for (int i = 0; i < n; ++i)
    {
        double pred = coef[0] + coef[1] * i + coef[2] * i * i;
        rss += (y[i] - pred) * (y[i] - pred);
    }

    return rss;
}

std::vector<double> first_order_filter(const std::vector<double> &input_i, double alpha = 0.2)
{
    int n = input_i.size();
    std::vector<double> output(n);
    if (n == 0)
        return output;

    output[0] = input_i[0]; // 初始输出值设为首项

    for (int i = 1; i < n; ++i)
    {
        output[i] = alpha * input_i[i] + (1.0 - alpha) * output[i - 1];
    }

    return output;
}

void transpose_matrix(const unsigned char *src, unsigned char *dst, int rows, int cols)
{
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            dst[j * rows + i] = src[i * cols + j];
        }
    }
}
