#ifndef CORE_GRAPHICS_OCTREE_H
#define CORE_GRAPHICS_OCTREE_H

#include <vector>

#include "core/maths/linear_algebra/vector3.h"

namespace amit::graphics
{
    namespace Physics
    {
        struct TriangleIndex
        {
            TriangleIndex() = default;
            union
            {
                uint32_t _indices[3];
                struct
                {
                    uint32_t index0;
                    uint32_t index1;
                    uint32_t index2;
                };
            };
        };

        struct OctreeNode
        {
            maths::Vector3             pos;
            maths::Vector3             extent;
            std::vector<TriangleIndex> triangles;

            maths::Vector3                    GetMin() const
            {
                return pos - extent;
            }

            maths::Vector3 GetMax() const
            {
                return pos + extent;
            }
        };

        class Octree
        {
        public:
            Octree();
            ~Octree();
            void           InitFromRange(uint32_t level, maths::Vector3 min, maths::Vector3 max);
            void           InitFromFile(const char* pFile);
            OctreeNode*    GetNodesInLevel(uint32_t levelIndex) const;
            uint32_t       GetCountOfNodesInLevel(uint32_t levelIndex) const;
            uint32_t       GetNodeCount() const;
            OctreeNode*    GetNodes() const;
            maths::Vector3 GetMin() const;
            maths::Vector3 GetMax() const;
            uint32_t       Level() const;

        private:
            uint32_t       mLevel;
            OctreeNode*    mNodes;
            uint32_t       mCountOfNode;
            maths::Vector3 min;
            maths::Vector3 max;
        };

    }  // namespace Physics
}  // namespace amit::graphics

#endif