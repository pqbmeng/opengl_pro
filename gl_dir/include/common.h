#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <QDebug>

namespace gl_ns
{
    void print(const glm::vec4 &rVec4)
    {
        qDebug() << "glm::vec4";
        qDebug() << rVec4.x << "\t|" << rVec4.y << "\t|" << rVec4.z << "\t|" << rVec4.w;
        qDebug() << "\n";
    }

    void print(const glm::mat4 &rMat4)
    {
        qDebug() << "glm::mat4";
        for (auto i = 0; i < 4; ++i)
        {
            const auto &rVec4 = rMat4[i];
            qDebug() << rVec4.x << "\t|" << rVec4.y << "\t|" << rVec4.z << "\t|" << rVec4.w;
        }
        qDebug() << "\n";
    }
}