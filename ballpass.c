#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;
#define NUM_PLAYERS_PER_TEAM 10

struct Ball {
    float x;
    float y;
    float vx;
    float vy;
    bool moving;
};

struct Player {
    float x;
    float y;
};

void updateBallPosition(struct Ball *ball, struct Player *targetPlayer, float speed);
void passBall(struct Ball *ball, struct Player *targetPlayer, float speed);

struct Ball blueBall, redBall;
struct Player blueTeam[NUM_PLAYERS_PER_TEAM], redTeam[NUM_PLAYERS_PER_TEAM];
int blueActivePlayer = NUM_PLAYERS_PER_TEAM - 1; // Index of the player currently in possession of the blue ball
int redActivePlayer = NUM_PLAYERS_PER_TEAM - 1;  // Index of the player currently in possession of the red ball

void drawSquare(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x - size / 2, y - size / 2);
    glVertex2f(x + size / 2, y - size / 2);
    glVertex2f(x + size / 2, y + size / 2);
    glVertex2f(x - size / 2, y + size / 2);
    glEnd();
}

void drawBall(float x, float y, float size, float r, float g, float b) {
    glColor3f(r, g, b);
    glTranslatef(x, y, 0.0f);
    glutSolidSphere(size, 20, 20); // Draw the ball as a solid sphere
    glTranslatef(-x, -y, 0.0f);
}

void drawPlayers(struct Player team[], float r, float g, float b, int numPlayers, float offsetX, float offsetY) {
    float centerX = offsetX;
    float centerY = offsetY;
    float radius = 0.4f;

    for (int i = 0; i < numPlayers; ++i) {
        team[i].x = centerX + radius * cos(2 * PI * i / numPlayers);
        team[i].y = centerY + radius * sin(2 * PI * i / numPlayers);
    }

    for (int i = 0; i < numPlayers; ++i) {
        if (i == 0) {
            if (r == 1.0f && g == 0.0f && b == 0.0f) // Red team
                drawSquare(team[i].x, team[i].y, 0.05f, 0.0f, 1.0f, 0.0f); // Green color for first red player
            else if (r == 0.0f && g == 0.0f && b == 1.0f) // Blue team
                drawSquare(team[i].x, team[i].y, 0.05f, 1.0f, 1.0f, 0.0f); // Yellow color for first blue player
        } else {
            drawSquare(team[i].x, team[i].y, 0.05f, r, g, b);
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw red team (below)
    drawPlayers(redTeam, 1.0f, 0.0f, 0.0f, NUM_PLAYERS_PER_TEAM, 0.0f, -0.2f); // Red color for red team players

    // Draw blue team (above)
    drawPlayers(blueTeam, 0.0f, 0.0f, 1.0f, NUM_PLAYERS_PER_TEAM, 0.0f, 0.2f); // Blue color for blue team players

    // Draw blue ball
    drawBall(blueBall.x, blueBall.y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for blue ball

    // Draw red ball
    drawBall(redBall.x, redBall.y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for red ball

    updateBallPosition(&blueBall, &blueTeam[(blueActivePlayer + 1) % NUM_PLAYERS_PER_TEAM], 0.01f);
    updateBallPosition(&redBall, &redTeam[(redActivePlayer + 1) % NUM_PLAYERS_PER_TEAM], 0.01f);

    glFlush();
}

void updateBallPosition(struct Ball *ball, struct Player *targetPlayer, float speed) {
    if (ball->moving) {
        ball->x += ball->vx;
        ball->y += ball->vy;

        // Check if ball reached the destination
        if (fabs(ball->x - targetPlayer->x) < 0.01 && fabs(ball->y - targetPlayer->y) < 0.01) {
            ball->moving = false;
            if (ball == &blueBall) {
                blueActivePlayer = (blueActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for blue team
            } else {
                redActivePlayer = (redActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for red team
            }
        }
    }
}

void passBall(struct Ball *ball, struct Player *targetPlayer, float speed) {
    ball->moving = true;
    ball->vx = (targetPlayer->x - ball->x) * speed;
    ball->vy = (targetPlayer->y - ball->y) * speed;
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'p': // 'p' key to pass the blue ball
        passBall(&blueBall, &blueTeam[(blueActivePlayer + 1) % NUM_PLAYERS_PER_TEAM], 0.01f);
        break;
    case 'P': // 'P' key to pass the red ball
        passBall(&redBall, &redTeam[(redActivePlayer + 1) % NUM_PLAYERS_PER_TEAM], 0.01f);
        break;
    }
    glutPostRedisplay();
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Ball Passing Simulation");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display); // Update display continuously
    init();

    // Initialize blue ball position
    blueBall.x = 0.0f;
    blueBall.y = blueTeam[0].y + 0.1f; // A little above the first blue player
    blueBall.vx = 0.0f;
    blueBall.vy = 0.0f;
    blueBall.moving = false;

    // Initialize red ball position
    redBall.x = 0.0f;
    redBall.y = redTeam[0].y - 0.1f; // A little below the first red player
    redBall.vx = 0.0f;
    redBall.vy = 0.0f;
    redBall.moving = false;

    glutMainLoop();
    return 0;
}

