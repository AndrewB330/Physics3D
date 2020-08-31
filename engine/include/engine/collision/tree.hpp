#pragma once

#include <unordered_map>
#include "bounding_volume.hpp"
#include "collider.hpp"

class Tree {
public:
    Tree();

    void Rebuild(const std::unordered_map<int, std::shared_ptr<const Collider>> & map);

    std::vector<int> GetCandidateIds(const BoundingBox & bbox) const;

private:
    int Build(int start, int end);

    void FillCandidates(int v, const BoundingBox& bbox, std::vector<int> & candidates) const;

    struct Node {
        int left;
        int right;

        int start;
        int end;

        BoundingBox bbox;
    };

    struct Entry {
        int id;
        std::shared_ptr<const Collider> collider;
    };

    std::vector<Node> nodes;
    std::vector<Entry> objects;
};
