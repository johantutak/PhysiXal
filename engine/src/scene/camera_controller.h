#pragma once

namespace PhysiXal {
    
    class CameraController
    {
    public:
        static void MoveInPlaneXZ(/*GLFWwindow* window, float ts, LveGameObject& gameObject*/);

        float m_MoveSpeed{ 3.f };
        float m_LookSpeed{ 1.5f };
    };
}