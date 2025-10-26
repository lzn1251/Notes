#include <glm/glm.hpp>
#include <array>

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

class Frustum {
public:
    Frustum(const glm::mat4& viewMatrix, const glm::mat4& projMatrix) {
        SetPlanes(projMatrix * viewMatrix);
    }
    
    void SetPlanes(const glm::mat4& matrix) {
        planes[LEFT].x = matrix[0].w + matrix[0].x;
        planes[LEFT].y = matrix[1].w + matrix[1].x;
        planes[LEFT].z = matrix[2].w + matrix[2].x;
        planes[LEFT].w = matrix[3].w + matrix[3].x;

        planes[RIGHT].x = matrix[0].w - matrix[0].x;
        planes[RIGHT].y = matrix[1].w - matrix[1].x;
        planes[RIGHT].z = matrix[2].w - matrix[2].x;
        planes[RIGHT].w = matrix[3].w - matrix[3].x;

        planes[TOP].x = matrix[0].w - matrix[0].y;
        planes[TOP].y = matrix[1].w - matrix[1].y;
        planes[TOP].z = matrix[2].w - matrix[2].y;
        planes[TOP].w = matrix[3].w - matrix[3].y;

        planes[BOTTOM].x = matrix[0].w + matrix[0].y;
        planes[BOTTOM].y = matrix[1].w + matrix[1].y;
        planes[BOTTOM].z = matrix[2].w + matrix[2].y;
        planes[BOTTOM].w = matrix[3].w + matrix[3].y;

        planes[BACK].x = matrix[0].w + matrix[0].z;
        planes[BACK].y = matrix[1].w + matrix[1].z;
        planes[BACK].z = matrix[2].w + matrix[2].z;
        planes[BACK].w = matrix[3].w + matrix[3].z;

        planes[FRONT].x = matrix[0].w - matrix[0].z;
        planes[FRONT].y = matrix[1].w - matrix[1].z;
        planes[FRONT].z = matrix[2].w - matrix[2].z;
        planes[FRONT].w = matrix[3].w - matrix[3].z;

        for (auto i = 0; i < planes.size(); i++) {
            float length = sqrtf(planes[i].x * planes[i].x + planes[i].y * planes[i].y + planes[i].z * planes[i].z);
            planes[i] /= length;
        }
    }
    
    bool checkAABB(const AABB& aabb) {
        for (int i = 0; i < planes.size(); i++) {
            glm::vec3 normal = glm::vec3(planes[i].x, planes[i].y, planes[i].z);
            
            glm::vec3 p;
            p.x = (normal.x >= 0.0f) ? aabb.min.x : aabb.max.x;
            p.y = (normal.y >= 0.0f) ? aabb.min.y : aabb.max.y;
            p.z = (normal.z >= 0.0f) ? aabb.min.z : aabb.max.z;
            
            float dist = glm::dot(normal, p) + planes[i].w;
            if (dist < 0.0f) {
                return false;
            }
        }
        return true;
    }
    
    bool checkSphere(glm::vec3 pos, float radius)
    {
        for (auto i = 0; i < planes.size(); i++) {
             if ((planes[i].x * pos.x) + (planes[i].y * pos.y) + (planes[i].z * pos.z) + planes[i].w <= -radius) {
                return false;
             }
        }
        return true;
    }
    
    
private:
    enum side { LEFT = 0, RIGHT = 1, TOP = 2, BOTTOM = 3, BACK = 4, FRONT = 5 };
    std::array<glm::vec4, 6> planes;
};
