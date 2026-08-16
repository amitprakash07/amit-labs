
#include <fstream>
#include <iostream>

#include "octree.h"

using namespace amit::graphics;

Physics::Octree::Octree()
{
    mLevel       = 0;
    mNodes       = nullptr;
    mCountOfNode = 0;
    min          = maths::Vector3(0, 0, 0);
    max          = maths::Vector3(0, 0, 0);
}

Physics::Octree::~Octree()
{
    if (mNodes)
        delete mNodes;
}

maths::Vector3 Physics::Octree::GetMax() const
{
    return max;
}

maths::Vector3 Physics::Octree::GetMin() const
{
    return min;
}

uint32_t Physics::Octree::GetNodeCount() const
{
    return mCountOfNode;
}

Physics::OctreeNode* Physics::Octree::GetNodes() const
{
    return mNodes;
}

uint32_t Physics::Octree::Level() const
{
    return mLevel;
}

uint32_t Physics::Octree::GetCountOfNodesInLevel(uint32_t levelIndex) const
{
    return static_cast<uint32_t>(std::pow(8.0f, levelIndex));
}

Physics::OctreeNode* Physics::Octree::GetNodesInLevel(uint32_t levelIndex) const
{
    uint32_t baseOfNodesInLevel  = static_cast<uint32_t>((std::pow(8.0f, levelIndex) - 1) / (8 - 1));
    uint32_t countOfNodesInLevel = static_cast<uint32_t>(std::pow(8.0f, levelIndex));
    return &mNodes[baseOfNodesInLevel];
}

void Physics::Octree::InitFromRange(uint32_t level, maths::Vector3 min, maths::Vector3 max)
{
    mLevel           = level;
    mCountOfNode     = static_cast<uint32_t>((std::pow(8.0f, mLevel) - 1) / (8 - 1));
    mNodes           = new OctreeNode[mCountOfNode];
    min              = min;
    max              = max;
    mNodes[0].pos    = (min + max) * 0.5f;
    mNodes[0].extent = (max - min) * 0.5f;
    for (uint32_t levelIndex = 0; levelIndex < mLevel - 1; ++levelIndex)
    {
        uint32_t       baseOfNodesInLevel  = static_cast<uint32_t>(std::pow(8.0f, levelIndex) - 1) / (8 - 1);
        uint32_t       countOfNodesInLevel = static_cast<uint32_t>(std::pow(8.0f, levelIndex));
        maths::Vector3 extentInNextLevel   = mNodes[baseOfNodesInLevel].extent * 0.5f;
        for (uint32_t nodeIndex = 0; nodeIndex < countOfNodesInLevel; ++nodeIndex)
        {
            uint32_t       indexOfNode = baseOfNodesInLevel + nodeIndex;
            maths::Vector3 centerPos   = mNodes[indexOfNode].pos;
            OctreeNode*    pChildren   = &mNodes[indexOfNode * 8 + 1];
            pChildren[0].pos =
                centerPos + maths::Vector3(-extentInNextLevel.x(), -extentInNextLevel.y(), -extentInNextLevel.z());
            pChildren[1].pos =
                centerPos + maths::Vector3(-extentInNextLevel.x(), -extentInNextLevel.y(), +extentInNextLevel.z());
            pChildren[2].pos =
                centerPos + maths::Vector3(+extentInNextLevel.x(), -extentInNextLevel.y(), +extentInNextLevel.z());
            pChildren[3].pos =
                centerPos + maths::Vector3(+extentInNextLevel.x(), -extentInNextLevel.y(), -extentInNextLevel.z());
            pChildren[4].pos =
                centerPos + maths::Vector3(-extentInNextLevel.x(), +extentInNextLevel.y(), -extentInNextLevel.z());
            pChildren[5].pos =
                centerPos + maths::Vector3(-extentInNextLevel.x(), +extentInNextLevel.y(), +extentInNextLevel.z());
            pChildren[6].pos =
                centerPos + maths::Vector3(+extentInNextLevel.x(), +extentInNextLevel.y(), +extentInNextLevel.z());
            pChildren[7].pos =
                centerPos + maths::Vector3(+extentInNextLevel.x(), +extentInNextLevel.y(), -extentInNextLevel.z());
            for (uint32_t childIndex = 0; childIndex < 8; ++childIndex)
            {
                pChildren[childIndex].extent = extentInNextLevel;
            }
        }
    }
}

void Physics::Octree::InitFromFile(const char* pFile)
{
    std::ifstream infile(pFile, std::ifstream::binary);
    // get size of file
    infile.seekg(0, infile.end);
    size_t size = infile.tellg();
    infile.seekg(0);
    // allocate memory for file content

    char* pBuffer = new char[size];
    // read content of infile
    infile.read(pBuffer, size);
    {
        uint32_t offset = 0;
        memcpy(reinterpret_cast<uint8_t*>(pBuffer) + offset, reinterpret_cast<uint8_t*>(&mLevel), sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(
            reinterpret_cast<uint8_t*>(pBuffer) + offset, reinterpret_cast<uint8_t*>(&mCountOfNode), sizeof(uint32_t));
        offset += sizeof(uint32_t);
        memcpy(reinterpret_cast<uint8_t*>(pBuffer) + offset, reinterpret_cast<uint8_t*>(&min), sizeof(maths::Vector3));
        offset += sizeof(maths::Vector3);
        memcpy(reinterpret_cast<uint8_t*>(pBuffer) + offset, reinterpret_cast<uint8_t*>(&max), sizeof(maths::Vector3));
        offset += sizeof(maths::Vector3);

        // Build the Octree
        InitFromRange(mLevel, min, max);
        // Now let's fill in the trianlges information
        OctreeNode* pLeaves       = GetNodesInLevel(mLevel - 1);
        uint32_t    countOfLeaves = GetCountOfNodesInLevel(mLevel - 1);

        for (uint32_t leafIndex = 0; leafIndex < countOfLeaves; ++leafIndex)
        {
            // record count of trianlges in this node
            uint32_t triangleCountInLeaf = 0;
            memcpy(reinterpret_cast<uint8_t*>(pBuffer + offset),
                   reinterpret_cast<uint8_t*>(&triangleCountInLeaf),
                   sizeof(uint32_t));
            offset += sizeof(uint32_t);
            if (triangleCountInLeaf > 0)
            {
                for (uint32_t trianlgeIndex = 0; trianlgeIndex < triangleCountInLeaf; ++trianlgeIndex)
                {
                    TriangleIndex tempTraiangle;
                    memcpy(reinterpret_cast<uint8_t*>(pBuffer + offset),
                           reinterpret_cast<uint8_t*>(&tempTraiangle),
                           sizeof(TriangleIndex));
                    pLeaves[leafIndex].triangles.push_back(tempTraiangle);
                    offset += sizeof(TriangleIndex);
                }
            }
        }
    }

    delete[] pBuffer;
    infile.close();
}
