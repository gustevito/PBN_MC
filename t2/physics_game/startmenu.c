#include "opengl.h"
#include <GL/glut.h>
#include <SOIL.h>
#include <string.h>

extern int gameState;
extern void restartCM(void);

GLuint titleTexture;
GLuint pressSpaceTexture;

GLuint loadComps(const char *imgFile);
void loadMenuComponents();

GLuint loadComps(const char *imgFile)
{
    GLuint t = SOIL_load_OGL_texture(
        imgFile,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_DDS_LOAD_DIRECT
    );
    if (!t)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    return t;
}

void loadMenuComponents()
{
    titleTexture = loadComps("images/titlemenu.png");
    pressSpaceTexture = loadComps("images/buttonmenu.png");
}

// Função para carregar textura com SOIL
// GLuint loadComps(const char *img) {
//     GLuint texID = SOIL_load_OGL_texture(
//         img,
//         SOIL_LOAD_AUTO,
//         SOIL_CREATE_NEW_ID,
//         SOIL_FLAG_DDS_LOAD_DIRECT
//     );
//     if (!texID)
//         printf("Erro carregando textura: %s\n", SOIL_last_result());
//     return texID;
// }

// // Inicializa as texturas do menu
// void loadMenuComponents() {
//     titleTexture = loadComps("images/titlemenu.png");
//     pressSpaceTexture = loadComps("images/buttonmenu.png");
// }

void drawTexture(GLuint tex, float x, float y, float w, float h) {
    if (!tex) return;
    
    // Habilita blending para transparência
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Reseta a cor para branco puro (sem tingir a textura)
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

// Tela do menu principal
void drawMenu() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Fundo simples
    glColor3f(0.2f, 0.1f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(LARGURA_JAN, 0);
        glVertex2f(LARGURA_JAN, ALTURA_JAN);
        glVertex2f(0, ALTURA_JAN);
    glEnd();

    // Desenha o título (centralizado)
    drawTexture(titleTexture,
        LARGURA_JAN / 2 - 200,
        ALTURA_JAN / 2 - 150,
        400, 150
    );

    // Desenha o "Press SPACE to Start"
    drawTexture(pressSpaceTexture,
        LARGURA_JAN / 2 - 150,
        ALTURA_JAN / 2 + 50,
        300, 80
    );

    glutSwapBuffers();
    glutPostRedisplay(); // mantém o redesenho ativo
}

// Controle de teclado no menu
void menuKeyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC
        exit(0);
    } else if (key == ' ') {
        gameState = 1;
        restartCM();
        glutPostRedisplay();
    }
}
