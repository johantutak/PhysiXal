#include "lnx_example_game.h"

#ifdef PX_PLATFORM_LINUX
    #include <GLFW/glfw3.h>
    #include <glad/include/glad/glad.h>
#endif
#include <iostream>

#ifdef PX_PLATFORM_LINUX

LnxExampleGame::LnxExampleGame()
    : CoreLayer("LnxExampleGame")
{
    // Draw
}

LnxExampleGame::~LnxExampleGame()
{
}

void LnxExampleGame::OnAttach()
{
    // SHADER
    m_Shader = PhysiXal::OpenGLShader::FromGLSLTextFiles(
            "../../../Example/assets/shaders/triangle.vert.glsl",
            "../../../Example/assets/shaders/triangle.frag.glsl"
            );
    
    // VERTEX ARRAY/BUFFER
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes
    glGenVertexArrays(1, &m_VertexArray);
    glBindVertexArray(m_VertexArray);
    
    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer); 
    
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
        };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // position attribute
    glEnableVertexAttribArray(0); 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    
    // You can unbind the Vertex array afterwards so other Vertex array calls won't accidentally modify this Vertex array, but this rarely happens. Modifying other
    // Vertex arrays requires a call to glBindVertexArray anyways so we generally don't unbind Vertex arrays (nor Vertex buffers) when it's not directly necessary.
    glBindVertexArray(0);
    
    // INDEX BUFFER
    // Reorder the vertex data, and reuse existing data for multiple vertices.
    glGenBuffers(1, &m_IndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

    unsigned int indices[3] = { 0, 1, 2 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void LnxExampleGame::OnDetach()
{
    glDeleteVertexArrays(1, &m_VertexArray);
    glDeleteBuffers(1, &m_VertexBuffer);
}

void LnxExampleGame::OnUpdate(PhysiXal::Timestep ts)
{   
    // Update
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    // Draw our first triangle
    glUseProgram(m_Shader->GetRendererID());
    glBindVertexArray(m_VertexArray); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void LnxExampleGame::OnEvent(PhysiXal::Event& e)
{
}
#endif