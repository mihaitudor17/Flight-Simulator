#include "Terrain.h"

Terrain::Terrain(const std::string& TextureString)
{
    texture = Texture::CreateTexture(TextureString);
}

static unsigned int planeVAO = 0;
static unsigned int planeVBO = 0;

void Terrain::Render()
{

    if (planeVAO == 0) {

        float planeVertices[] = {
            // positions            // normals         // texcoords
            250.0f, -0.5f,  250.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
            -250.0f, -0.5f,  250.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -250.0f, -0.5f, -250.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

            250.0f, -0.5f,  250.0f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
            -250.0f, -0.5f, -250.0f,  0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
            250.0f, -0.5f, -250.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f
        };

        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindVertexArray(0);
    }

    glBindVertexArray(planeVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}
