#include "opengl.h"
#include <GL/glut.h>
#include <SOIL.h>
#include <string.h>

extern int gameState;
extern void restartCM(void);

GLuint titleTexture;
GLuint pressSpaceTexture;

// Função para carregar textura com SOIL
GLuint loadTexture(const char *filename) {
    GLuint texID = SOIL_load_OGL_texture(
        filename,
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_DDS_LOAD_DIRECT
    );
    if (!texID)
        printf("Erro carregando textura: %s\n", SOIL_last_result());
    return texID;
}

// Inicializa as texturas do menu
void initMenuAssets() {
    titleTexture = loadTexture("images/titlemenu.png");
    pressSpaceTexture = loadTexture("images/buttonmenu.png");
}

// Desenha textura em um retângulo
void drawTexture(GLuint tex, float x, float y, float w, float h) {
    if (!tex) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
        glTexCoord2f(1, 0); glVertex2f(x + w, y);
        glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
        glTexCoord2f(0, 1); glVertex2f(x, y + h);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Tela do menu principal
void drawMenu() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Fundo simples
    glColor3f(0.8f, 0.6f, 0.01f);
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
    } else if (key == ' ') { // Barra de espaço
        gameState = 1;   // muda para modo jogo
        restartCM();     // reinicia a simulação
        glutPostRedisplay();
    }
}
