#include <engine/collision/tree.hpp>
#include <algorithm>

void Tree::Rebuild(const std::unordered_map<int, std::shared_ptr<const Collider>> &map) {
    nodes.clear();
    objects.clear();
    for(const auto& [k, v] : map) {
        objects.push_back(Entry{k, v});
    }
    Build(0, objects.size());
}

Tree::Tree() {

}

int Tree::Build(int start, int end) {
    int node_id = nodes.size();
    nodes.push_back(Tree::Node());
    nodes[node_id].start = start;
    nodes[node_id].end = end;
    nodes[node_id].bbox = BoundingBox();
    for(int i = start; i < end; i++) {
        nodes[node_id].bbox = Merge(nodes[node_id].bbox, objects[i].collider->GetBBox());
    }
    if (start+1 >= end) {
        nodes[node_id].left = -1;
        nodes[node_id].right = -1;
        return node_id;
    }

    int axis = nodes[node_id].bbox.GetLargestDim();
    int mid = start + (end - start)/2;
    std::nth_element(objects.begin() + start, objects.begin() + end, objects.begin() + mid,
            [axis](const Tree::Entry & a, const Tree::Entry & b) {
        switch(axis) {
            case 0:
                return a.collider->GetBBox().min.x < b.collider->GetBBox().min.x;
            case 1:
                return a.collider->GetBBox().min.y < b.collider->GetBBox().min.y;
            case 2:
                return a.collider->GetBBox().min.z < b.collider->GetBBox().min.z;
        }
        return false;
    });
    nodes[node_id].left = Build(start, mid);
    nodes[node_id].right = Build(mid, end);
    return node_id;
}

void Tree::FillCandidates(int v, const BoundingBox &bbox, std::vector<int> &candidates) const {
    if (!IsIntersect(nodes[v].bbox, bbox)) {
        return;
    }
    if (nodes[v].left == -1 && nodes[v].right == -1) {
        // leaf
        for(int i = nodes[v].start; i < nodes[v].end; i++) {
            if (IsIntersect(objects[i].collider->GetBBox(), bbox)) {
                candidates.push_back(objects[i].id);
            }
        }
        return;
    }
    FillCandidates(nodes[v].left, bbox, candidates);
    FillCandidates(nodes[v].right, bbox, candidates);
}

std::vector<int> Tree::GetCandidateIds(const BoundingBox &bbox) const {
    std::vector<int> candidates;
    FillCandidates(0, bbox, candidates);
    return candidates;
}
