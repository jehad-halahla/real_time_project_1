#include <GL/glut.h>
#include <math.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;
const int NUM_PLAYERS_PER_TEAM = 6;

void drawSquare(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Change background color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Draw blue team
    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        float angle = 2 * PI * i / NUM_PLAYERS_PER_TEAM;
        float x = cos(angle) * 0.5f;
        float y = sin(angle) * 0.5f + 0.5f;
        if (i == 0)
            drawSquare(x, y, 0.05f, 1.0f, 1.0f, 0.0f); // Yellow color for leftmost blue square
        else
            drawSquare(x, y, 0.05f, 0.0f, 0.0f, 1.0f); // Blue color for other blue squares
    }

    // Draw red team
    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        float angle = 2 * PI * i / NUM_PLAYERS_PER_TEAM;
        float x = cos(angle) * 0.5f;
        float y = sin(angle) * 0.5f - 0.5f;
        if (i == 0)
            drawSquare(x, y, 0.05f, 0.0f, 1.0f, 0.0f); // Green color for leftmost red square
        else
            drawSquare(x, y, 0.05f, 1.0f, 0.0f, 0.0f); // Red color for other red squares
    }

    glFlush();
}

void init() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("OpenGL Player Formation");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

