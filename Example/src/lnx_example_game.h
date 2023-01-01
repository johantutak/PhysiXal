#pragma once

#include "core/physixal.h"

#ifdef PX_PLATFORM_LINUX

class LnxExampleGame : public PhysiXal::CoreLayer
{
public:
    LnxExampleGame();
    virtual ~LnxExampleGame();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(PhysiXal::Timestep ts) override;

    void OnEvent(PhysiXal::Event& e) override;
private:
    // SHADER
    PhysiXal::OpenGLShader* m_Shader;
    
    // VERTEX ARRAY
    unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
};
#endif
