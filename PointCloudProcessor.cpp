#include "PointCloudProcessor.h"
#include <algorithm>
#include <cmath>
#include <map>

PointCloudProcessor::PointCloudProcessor() 
    : cluster_distance_threshold_(0.1),  // 10cm
      min_cluster_size_(10),
      max_cluster_size_(10000) {
}

std::vector<PointCloudProcessor::Cluster> PointCloudProcessor::clusterPoints(const std::vector<Vector3D>& points) {
    std::vector<Cluster> clusters;
    
    if (points.empty()) {
        return clusters;
    }
    
    // 初始化
    visited_.assign(points.size(), false);
    cluster_ids_.assign(points.size(), -1);
    
    // 执行DBSCAN聚类
    dbscan(points);
    
    // 收集聚类结果
    std::map<int, std::vector<size_t>> cluster_map;
    for (size_t i = 0; i < cluster_ids_.size(); ++i) {
        if (cluster_ids_[i] != -1) {
            cluster_map[cluster_ids_[i]].push_back(i);
        }
    }
    
    // 创建Cluster对象
    for (const auto& pair : cluster_map) {
        if (pair.second.size() >= min_cluster_size_ && pair.second.size() <= max_cluster_size_) {
            Cluster cluster;
            cluster.point_indices = pair.second;
            cluster.center = calculateCenter(points, pair.second);
            cluster.size = pair.second.size();
            clusters.push_back(cluster);
        }
    }
    
    return clusters;
}

void PointCloudProcessor::dbscan(const std::vector<Vector3D>& points) {
    int cluster_id = 0;
    
    for (size_t i = 0; i < points.size(); ++i) {
        if (visited_[i]) {
            continue;
        }
        
        visited_[i] = true;
        std::vector<size_t> neighbors = getNeighbors(points, i);
        
        if (neighbors.size() < min_cluster_size_) {
            cluster_ids_[i] = -1;  // 标记为噪声
        } else {
            expandCluster(points, i, cluster_id++);
        }
    }
}

std::vector<size_t> PointCloudProcessor::getNeighbors(const std::vector<Vector3D>& points, size_t point_idx) {
    std::vector<size_t> neighbors;
    
    for (size_t i = 0; i < points.size(); ++i) {
        if (i == point_idx) {
            continue;
        }
        
        double distance = calculateDistance(points[point_idx], points[i]);
        if (distance <= cluster_distance_threshold_) {
            neighbors.push_back(i);
        }
    }
    
    return neighbors;
}

void PointCloudProcessor::expandCluster(const std::vector<Vector3D>& points, size_t point_idx, int cluster_id) {
    std::vector<size_t> seeds = getNeighbors(points, point_idx);
    
    cluster_ids_[point_idx] = cluster_id;
    
    for (size_t i = 0; i < seeds.size(); ++i) {
        size_t current_point = seeds[i];
        
        if (!visited_[current_point]) {
            visited_[current_point] = true;
            std::vector<size_t> current_neighbors = getNeighbors(points, current_point);
            
            if (current_neighbors.size() >= min_cluster_size_) {
                seeds.insert(seeds.end(), current_neighbors.begin(), current_neighbors.end());
            }
        }
        
        if (cluster_ids_[current_point] == -1) {
            cluster_ids_[current_point] = cluster_id;
        }
    }
}

double PointCloudProcessor::calculateDistance(const Vector3D& p1, const Vector3D& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;
    return std::sqrt(dx*dx + dy*dy + dz*dz);
}

Vector3D PointCloudProcessor::calculateCenter(const std::vector<Vector3D>& points, const std::vector<size_t>& indices) {
    if (indices.empty()) {
        return Vector3D(0, 0, 0);
    }
    
    Vector3D center(0, 0, 0);
    for (size_t idx : indices) {
        center.x += points[idx].x;
        center.y += points[idx].y;
        center.z += points[idx].z;
    }
    
    center.x /= indices.size();
    center.y /= indices.size();
    center.z /= indices.size();
    
    return center;
}

void PointCloudProcessor::setClusterDistanceThreshold(double threshold) {
    cluster_distance_threshold_ = threshold;
}

void PointCloudProcessor::setMinClusterSize(size_t min_size) {
    min_cluster_size_ = min_size;
}

void PointCloudProcessor::setMaxClusterSize(size_t max_size) {
    max_cluster_size_ = max_size;
}