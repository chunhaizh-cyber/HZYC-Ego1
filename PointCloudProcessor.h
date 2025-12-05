#ifndef POINT_CLOUD_PROCESSOR_H
#define POINT_CLOUD_PROCESSOR_H

#include <vector>
#include <memory>
#include "Vector3D.h"

class PointCloudProcessor {
public:
    struct Cluster {
        std::vector<size_t> point_indices;
        Vector3D center;
        double size;
        
        Cluster() : size(0) {}
    };

    PointCloudProcessor();
    
    std::vector<Cluster> clusterPoints(const std::vector<Vector3D>& points);
    
    void setClusterDistanceThreshold(double threshold);
    void setMinClusterSize(size_t min_size);
    void setMaxClusterSize(size_t max_size);
    
private:
    double cluster_distance_threshold_;
    size_t min_cluster_size_;
    size_t max_cluster_size_;
    
    std::vector<bool> visited_;
    std::vector<int> cluster_ids_;
    
    void dbscan(const std::vector<Vector3D>& points);
    std::vector<size_t> getNeighbors(const std::vector<Vector3D>& points, size_t point_idx);
    void expandCluster(const std::vector<Vector3D>& points, size_t point_idx, int cluster_id);
    double calculateDistance(const Vector3D& p1, const Vector3D& p2);
    Vector3D calculateCenter(const std::vector<Vector3D>& points, const std::vector<size_t>& indices);
};

#endif // POINT_CLOUD_PROCESSOR_H