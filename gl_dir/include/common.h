#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <QDebug>
#pragma execution_character_set("utf-8")
namespace gl_ns
{
    void print(const glm::vec3 &rVec3)
    {
        qDebug() << "glm::vec3";
        qDebug() << rVec3.x << "\t|" << rVec3.y << "\t|" << rVec3.z;
        qDebug() << "\n";
    }

    void print(const glm::vec4 &rVec4)
    {
        qDebug() << "glm::vec4";
        qDebug() << rVec4.x << "\t|" << rVec4.y << "\t|" << rVec4.z << "\t|" << rVec4.w;
        qDebug() << "\n";
    }

    void print(const glm::mat4 &rMat4)
    {
        qDebug() << "glm::mat4";

        const auto &rCol0 = rMat4[0];
        const auto &rCol1 = rMat4[1];
        const auto &rCol2 = rMat4[2];
        const auto &rCol3 = rMat4[3];
        qDebug() << rCol0.x << "\t|" << rCol1.x << "\t|" << rCol2.x << "\t|" << rCol3.x;
        qDebug() << rCol0.y << "\t|" << rCol1.y << "\t|" << rCol2.y << "\t|" << rCol3.y;
        qDebug() << rCol0.z << "\t|" << rCol1.z << "\t|" << rCol2.z << "\t|" << rCol3.z;
        qDebug() << rCol0.w << "\t|" << rCol1.w << "\t|" << rCol2.w << "\t|" << rCol3.w;

        qDebug() << "\n";
    }
}