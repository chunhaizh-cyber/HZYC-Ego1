#ifndef EXISTENCE_FEATURE_H
#define EXISTENCE_FEATURE_H

#include <vector>
#include <memory>
#include <map>
#include <random>
#include "Vector3D.h"

// 简单的颜色结构体，替代OpenCV的cv::viz::Color
struct Color {
    double r, g, b;
    Color(double r = 255, double g = 255, double b = 255) : r(r), g(g), b(b) {}
};

class ExistenceFeature {
public:
    int id;
    std::vector<Vector3D> trajectory;                    // 历史轨迹（最多50帧）
    std::vector<std::vector<int64_t>> contour_history;  // 历史轮廓特征（N=9，L=3，81点）
    Vector3D avg_size;                                   // 平均尺寸
    Vector3D avg_velocity;                               // 平均速度
    double last_seen_time;                               // 最后观测时间（秒）
    double confidence;                                   // 置信度
    std::string concept_code;                            // 抽象存在树编码（如"001"=妈妈）
    Color color;                                         // 可视化颜色

    ExistenceFeature(int id);
    
    void update(const Vector3D& pos, const std::vector<int64_t>& contour, double time);
    void decay(double current_time);
    
private:
    void calculateAverageVelocity();
};

class ExistenceMemory {
public:
    std::map<int, std::shared_ptr<ExistenceFeature>> database;
    int next_id;

    ExistenceMemory();
    
    std::shared_ptr<ExistenceFeature> getOrCreate(int temp_id, const Vector3D& pos, 
                                                  const std::vector<int64_t>& contour, double time);
    
private:
    double contourSimilarity(const std::vector<int64_t>& a, const std::vector<int64_t>& b);
    double calculateMatchScore(std::shared_ptr<ExistenceFeature> memory, const Vector3D& pos, 
                              const std::vector<int64_t>& contour, double time);
};

#endif // EXISTENCE_FEATURE_H