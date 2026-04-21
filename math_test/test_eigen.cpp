#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>

int main() {
    // 示例数据矩阵 (4个样本, 2个特征)
    Eigen::MatrixXd X(4, 2);
    X << 1, 2,
         3, 4,
         5, 6,
         7, 8;
    
    // 1. 中心化 (每列减去均值)
    Eigen::VectorXd mean = X.colwise().mean();
    Eigen::MatrixXd X_centered = X.rowwise() - mean.transpose();
    
    // 2. 计算协方差矩阵
    int n = X.rows();
    Eigen::MatrixXd cov = (X_centered.transpose() * X_centered) / (n - 1);
    
    std::cout << "协方差矩阵:\n" << cov << std::endl;
    
    // 3. 特征值分解
    Eigen::EigenSolver<Eigen::MatrixXd> solver(cov);
    Eigen::VectorXd eigenvalues = solver.eigenvalues().real();  // 取实部
    Eigen::MatrixXd eigenvectors = solver.eigenvectors().real();
    
    std::cout << "\n特征值:\n" << eigenvalues.transpose() << std::endl;
    std::cout << "\n特征向量（列）:\n" << eigenvectors << std::endl;
    
    return 0;
}