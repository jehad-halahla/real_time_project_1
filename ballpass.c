#include "includes/include.h"
#include <GL/glut.h>
#include <GL/gl.h>


struct sigaction sa_usr1, sa_usr2, sa_ui;


const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;
#define NUM_PLAYERS_PER_TEAM 6

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

int sender, receiver;

struct Ball balls[MAX_NUM_BALLS];

void updateBallPosition(struct Ball* ball, struct Player* targetPlayer, float speed);
void new_round();
struct Ball blueBall, redBall;

struct Player blueTeam[NUM_PLAYERS_PER_TEAM], redTeam[NUM_PLAYERS_PER_TEAM];
int blueActivePlayer = 5; // Index of the player currently in possession of the blue ball
int redActivePlayer = 0;  // Index of the player currently in possession of the red ball

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

void drawPlayers(struct Player team[], float r, float g, float b) {
    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        if (i == 5) {
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
    // Draw blue team
    drawPlayers(blueTeam, 0.0f, 0.0f, 1.0f); // Blue color for blue team players

    // Draw red team
    drawPlayers(redTeam, 1.0f, 0.0f, 0.0f); // Red color for red team players

    updateBallPosition(&balls[0], &blueTeam[blueActivePlayer], SPEED);

    drawBall((balls[0].x) , balls[0].y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for balls
    glFlush();
}

void updateBallPosition(struct Ball* ball, struct Player* targetPlayer, float speed) {
    if (ball->moving) {
        ball->x += ball->vx;
        ball->y += ball->vy;

        // Check if ball reached the destination
        if (fabs(ball->x - targetPlayer->x) < 0.01 && fabs(ball->y - targetPlayer->y) < 0.01) {
            ball->moving = false;
            if (ball == &blueBall){
                blueActivePlayer = (blueActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for blue team
            } else {
                redActivePlayer = (redActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for red team
            }
        }
    }
}

void passBall(struct Ball* ball, struct Player* targetPlayer, float speed) {
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

void signal_handler(int signum){
    if(signum == SIGUSR1){

        // open FIFO 
        int fd = open(GUI_FIFO, O_RDONLY);
        
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // read from FIFO

        char buffer[6];
        //sender#receiver
        
        if (read(fd, buffer, sizeof(buffer)) == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        close(fd);

        if (sscanf(buffer, "%d#%d", &sender, &receiver) != 2) {
            fprintf(stderr, "Invalid input format\n");
            exit(EXIT_FAILURE);
        }

        printf("sender: %d, receiver: %d\n", sender, receiver);

        if (sender <= 5)
            passBall(&balls[0], &blueTeam[blueActivePlayer], SPEED);

    }

    else if(signum == SIGUI){
        printf("SIGUI received\n");
        new_round();
    }
    else if(signum == SIGUSR2){

    }
}

void init() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {

    //set the signal handler for SIGUSR1 and SIGUSR2
    sa_usr1.sa_handler = signal_handler;
    sigemptyset(&sa_usr1.sa_mask);
    sa_usr1.sa_flags = 0;

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    sa_usr2.sa_handler = signal_handler;
    sigemptyset(&sa_usr2.sa_mask);
    sa_usr2.sa_flags = 0;

    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction for SIGUSR2");
        exit(EXIT_FAILURE);
    }

    sa_ui.sa_handler = signal_handler;
    sigemptyset(&sa_ui.sa_mask);
    sa_ui.sa_flags = 0;

    if (sigaction(SIGUI, &sa_ui, NULL) == -1) {
        perror("sigaction for SIGUI");
        exit(EXIT_FAILURE);
    }

    
    new_round();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Ball Passing Simulation");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display); // Update display continuously
    init();
    glutMainLoop();
    
    return 0;
}

void new_round() {

    balls[0].moving = false;

     // Initialize blue team and red team player positions
    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        blueTeam[i].x = cos(2 * PI * (5-i) / NUM_PLAYERS_PER_TEAM) * 0.5f;
        blueTeam[i].y = sin(2 * PI * (5-i) / NUM_PLAYERS_PER_TEAM) * 0.5f - 0.5f;
        redTeam[i].x = cos(2 * PI * (5-i) / NUM_PLAYERS_PER_TEAM) * 0.5f;
        redTeam[i].y = sin(2 * PI * (5-i) / NUM_PLAYERS_PER_TEAM) * 0.5f + 0.5f;
    }


    balls[0].x = 0.8f;
    balls[0].y = 0.0f;
    balls[0].vx = 0.0f;
    balls[0].vy = 0.0f;

}


