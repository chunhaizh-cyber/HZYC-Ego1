#ifndef EXISTENCE_FEATURE_H
#define EXISTENCE_FEATURE_H

#include <vector>
#include <memory>
#include <map>
#include <random>
#include <chrono>
#include <string>
#include "Vector3D.h"

// 简单的颜色结构体，替代OpenCV的cv::viz::Color
struct Color {
    double r, g, b;
    Color(double r = 255, double g = 255, double b = 255) : r(r), g(g), b(b) {}
};

// ==================== 全局绝对坐标系管理 ====================
class WorldCoordinateSystem {
public:
    Vector3D origin{0, 0, 0}; // 世界原点（可由SLAM初始化）
    Vector3D gravity_axis{0, 0, 1}; // 重力方向（IMU校正）
    double last_update_time = 0; // 设置世界原点（例如：启动时相机位置）
    
    void setOrigin(const Vector3D& pos, double time);
    
    // 世界坐标 → 自我坐标（减去原点）
    Vector3D worldToEgo(const Vector3D& world_pos) const;
    
    // 自我坐标 → 世界坐标
    Vector3D egoToWorld(const Vector3D& ego_pos) const;
};

extern WorldCoordinateSystem world_coords; // 全局世界坐标系

// ==================== 升级版记忆特征（v4.0支持多重验证）===================
class ExistenceMemoryV4 {
public:
    int id;
    std::wstring code = L""; // 抽象存在树编码（如"001"=妈妈）
    Vector3D world_position; // 最后一次世界坐标
    Vector3D avg_size; // 平均尺寸
    std::vector<Vector3D> trajectory; // 世界坐标轨迹（最多100帧）
    std::vector<std::vector<int64_t>> contour_history; // 轮廓特征历史
    std::vector<Vector3D> velocity_history; // 速度历史
    double first_seen = 0, last_seen = 0;
    int appearance_count = 0;
    double recognition_confidence = 0.0; // 再识别置信度（0-1）
    Color color;

    ExistenceMemoryV4(int i);
    
    // 更新记忆（每次看到时）
    void update(const Vector3D& pos, const std::vector<int64_t>& contour, double time);
    
    // 多重特征相似度评分（0-1）
    double multiFeatureSimilarity(const Vector3D& pos, const std::vector<int64_t>& contour) const;

private:
    double contourSimilarity(const std::vector<int64_t>& a, const std::vector<int64_t>& b) const;
};

// ==================== 全局记忆库（v4.0支持再识别）===================
class GlobalMemoryBankV4 {
public:
    std::map<int, std::shared_ptr<ExistenceMemoryV4>> memory;
    int next_id = 1;
    double current_time = 0;

    std::shared_ptr<ExistenceMemoryV4> recognizeOrCreate(const Vector3D& ego_pos, const std::vector<int64_t>& contour);
};

class ExistenceFeature {
public:
    int id;
    Vector3D world_position;                             // 绝对世界坐标（核心！）
    Vector3D last_ego_position;                          // 上一次在自我坐标系中的位置（用于匹配）
    Vector3D size;                                       // 尺寸
    Vector3D velocity;                                   // 速度
    std::vector<Vector3D> trajectory;                    // 世界坐标轨迹
    std::vector<std::vector<int64_t>> contour_history;  // 历史轮廓特征
    double last_seen_time;                               // 最后观测时间（秒）
    double first_seen_time;                              // 首次观测时间（秒）
    int appearance_count;                                // 出现次数（关键！）
    double confidence;                                   // 置信度
    bool is_active;                                      // 是否仍在跟踪
    std::string concept_code;                            // 抽象存在树编码
    Color color;                                         // 可视化颜色

    ExistenceFeature(int id, const Vector3D& world_pos);
    
    void update(const Vector3D& new_ego_pos, const std::vector<int64_t>& contour, double time);
    
    // 判断是否长时间未更新（终止跟踪）
    bool shouldTerminate(double current_time, double timeout = 3.0) const;
    
    void terminate(); // 终止跟踪
    
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