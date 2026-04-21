/*没有规范文件（头文件、源文件），因为懒*/
#pragma once
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <Eigen/Core>

using namespace std;

/**
 * 滑动窗口IQR计算器
 * 使用双堆（中位数流）方法高效计算IQR
 */
class SlidingWindowIQR {
private:
    int windowSize;            // 窗口大小
    deque<double> window;      // 滑动窗口存储数据
    
    // 计算四分位数的辅助函数
    double getPercentile(const vector<double>& sortedData, double percentile) {
        if (sortedData.empty()) return 0.0;
        
        int n = sortedData.size();
        double pos = (n - 1) * percentile;
        int idx = static_cast<int>(pos);    
        double frac = pos - idx;
        
        if (idx == n - 1) {
            return sortedData[idx];
        } else {
            return sortedData[idx] + frac * (sortedData[idx + 1] - sortedData[idx]);
        }
    }
    
public:
    SlidingWindowIQR(int size = 36) : windowSize(size) {
        if (size <= 0) {
            throw invalid_argument("Window size must be positive");
        }
    }

    /*
    // 添加新值到滑动窗口
    void add(double value) {
        window.push_back(value);
        
        // 保持窗口大小固定
        if (window.size() > windowSize) {
            window.pop_front();
        }
    }
    */

    void add(const vector<double>& values) {  
        for (double value : values) {
            window.push_back(value);
            if (window.size() > windowSize) {
                window.pop_front();
            }
        }
    }

    // Eigen-compatible overload: accepts column or row Eigen vectors
    template <typename Derived>
    void add(const Eigen::MatrixBase<Derived>& values) {
        for (int i = 0; i < values.size(); ++i) {
            window.push_back(static_cast<double>(values(i)));
            if (window.size() > windowSize) {
                window.pop_front();
            }
        }
    }

    // 获取当前窗口的所有数据
    vector<double> getWindowData() const {
        return vector<double>(window.begin(), window.end());
    }
    
    // 计算IQR（四分位距）
    double calculateIQR() {
        if (window.size() < 4) {
            cerr << "Warning: Window size too small for IQR calculation" << endl;
            return 0.0;
        }
        
        // 复制数据并排序
        vector<double> sortedData(window.begin(), window.end());
        sort(sortedData.begin(), sortedData.end());
        
        // 计算Q1（第25百分位数）和Q3（第75百分位数）
        double q1 = getPercentile(sortedData, 0.25);
        double q3 = getPercentile(sortedData, 0.75);
        
        return q3 - q1;
    }
    
    // 获取详细的统计信息
    struct Statistics {
        double min;
        double q1;
        double median;
        double q3;
        double max;
        double iqr;
        double Td;
        //double mean;
        //vector<double> outliers;  // 异常值
    };
    
    Statistics getDetailedStatistics() {
        Statistics stats;
        
        if (window.empty()) {
            return stats;
        }
        
        // 排序数据
        vector<double> sortedData(window.begin(), window.end());
        sort(sortedData.begin(), sortedData.end());
        
        // 计算基本统计量
        stats.min = sortedData.front();
        stats.max = sortedData.back();
        stats.q1 = getPercentile(sortedData, 0.25);
        stats.median = getPercentile(sortedData, 0.5);
        stats.q3 = getPercentile(sortedData, 0.75);
        stats.iqr = stats.q3 - stats.q1;
        stats.Td = stats.q1 - 1.12 * stats.iqr;//动态阈值
        
        /*
        // 计算平均值
        double sum = 0.0;
        for (double val : window) {
            sum += val;
        }
        stats.mean = sum / window.size();

        // 检测异常值（1.5 * IQR法则）
        double lowerBound = stats.q1 - 1.5 * stats.iqr;
        double upperBound = stats.q3 + 1.5 * stats.iqr;
        

        for (double val : window) {
            if (val < lowerBound || val > upperBound) {
                stats.outliers.push_back(val);
            }
        }
        */
       
        return stats;
    }

    // Alias for compatibility
    Statistics getStatistics() { return getDetailedStatistics(); }
    
    // 判断窗口是否已满
    bool isFull() const {
        return window.size() == windowSize;
    }
    
    // 清空窗口
    void clear() {
        window.clear();
    }
    
    // 获取当前窗口大小
    int size() const {
        return window.size();
    }
    
    // 获取窗口容量
    int capacity() const {
        return windowSize;
    }
};

/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <queue>

using namespace std;

class FixedSizeSlidingWindow {
private:
    int windowSize;
    int left, right;
    vector<int> data;//动态数组，存储滑动窗口内的元素（把特征值全部保存，不做删除处理，只滑动窗口）
    
public:
    FixedSizeSlidingWindow(int size) : windowSize(size), left(0), right(0) {}
    
    // 添加元素并返回窗口内的最大值
    int addAndGetMax(int val) {
        data.push_back(val);
        right++;
        
        // 如果窗口超过大小，移动左指针
        if (right - left > windowSize) {
            left++;
        }
        
        // 计算当前窗口内的最大值
        int maxVal = data[left];
        for (int i = left + 1; i < right; i++) {
            maxVal = max(maxVal, data[i]);
        }
        
        return maxVal;
    }
    
    // 获取当前窗口内的所有元素
    vector<int> getWindow() {
        vector<int> result;
        for (int i = left; i < right; i++) {
            result.push_back(data[i]);
        }
        return result;
    }
    
    int getSize() {
        return right - left;
    }
};
*/