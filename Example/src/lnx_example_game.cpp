#include "lnx_example_game.h"

#include <GLFW/glfw3.h>
#include <glad/include/glad/glad.h>

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
    
    // Vertex array
    // Set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
        // first triangle 
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 
    }; 
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); 
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
    
     // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
    
    // Uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void LnxExampleGame::OnDetach()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void LnxExampleGame::OnUpdate(PhysiXal::Timestep ts)
{
    // Update
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    // Draw our first triangle
    glUseProgram(m_Shader->GetRendererID());
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void LnxExampleGame::OnEvent(PhysiXal::Event& e)
{
}
#endif