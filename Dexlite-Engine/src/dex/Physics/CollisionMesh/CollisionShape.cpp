#include "CollisionShape.hpp"

namespace dex
{
    ConvexHullCollisionShape::ConvexHullCollisionShape(const Mesh::Default3D& mesh, const glm::vec3& transform_scale)
    {
        std::vector<glm::vec3> vertices;
        vertices.reserve(mesh.m_Indices.size());

        glm::mat3 scalar = glm::mat3(glm::scale(glm::mat4(1.0f), transform_scale * glm::vec3(.99)));

        for (auto index : mesh.m_Indices)
        {
            //DEX_LOG_INFO(index);
            vertices.push_back(scalar * mesh.m_Vertices.at(index).Position);
        }

        btConvexHullShape convex_hull = btConvexHullShape((btScalar*)vertices.data(), vertices.size(), sizeof(glm::vec3));

        btShapeHull* hull = new btShapeHull(&convex_hull);
        hull->buildHull(0, true);
        m_BtShape = new btConvexHullShape((const btScalar*)hull->getVertexPointer(), hull->numVertices(), sizeof(btVector3));
        m_BtShape->setMargin(.025);

        static_cast<btConvexHullShape*>(m_BtShape)->optimizeConvexHull();

        delete hull;
    }

    TriangleMeshCollisionShape::TriangleMeshCollisionShape(const Mesh::Default3D& mesh, const glm::vec3& transform_scale)
    {
        std::vector<glm::vec3> vertices;

        glm::mat3 scalar = glm::mat3(glm::scale(glm::mat4(1.0f), transform_scale * glm::vec3(.99)));

        vertices.reserve(mesh.m_Indices.size());
        for (auto& v : mesh.m_Vertices)
        {
            vertices.push_back(scalar * v.Position);
        }

        auto m = new btTriangleMesh();

        for (size_t i = 0; i < mesh.m_Indices.size(); i += 3)
        {
            m->addTriangle(
                btVector3(
                    vertices[mesh.m_Indices[i]].x,
                    vertices[mesh.m_Indices[i]].y,
                    vertices[mesh.m_Indices[i]].z),

                btVector3(
                    vertices[mesh.m_Indices[i + 1]].x,
                    vertices[mesh.m_Indices[i + 1]].y,
                    vertices[mesh.m_Indices[i + 1]].z),

                btVector3(
                    vertices[mesh.m_Indices[i + 2]].x,
                    vertices[mesh.m_Indices[i + 2]].y,
                    vertices[mesh.m_Indices[i + 2]].z));
        }

        m_BtShape = new btBvhTriangleMeshShape(m, true);

        m_BtShape->setMargin(.025);
    }
}
