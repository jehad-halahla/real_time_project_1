#include "includes/include.h"
#include <GL/glut.h>
#include <GL/gl.h>

struct sigaction sa_usr1, sa_usr2, sa_ui;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PI = 3.14159265359f;
#define NUM_PLAYERS_PER_TEAM 6

int current_ball_index = 0;

struct Ball {
    float x;
    float y;
    float vx;
    float vy;
    bool moving;

    struct Player* holder;
};

struct Player {
    float x;
    float y;

    struct Ball* ball;
};

struct Ball balls[MAX_NUM_BALLS];

struct Player blueTeam[NUM_PLAYERS_PER_TEAM], redTeam[NUM_PLAYERS_PER_TEAM];


void updateBallPosition(struct Ball* ball, struct Player* targetPlayer, float speed);
void new_round();
void passBall(struct Ball* ball, struct Player* targetPlayer, float speed);
void signal_handler(int signum);
void drawSquare(float x, float y, float size, float r, float g, float b);
void drawBall(float x, float y, float size, float r, float g, float b);
void drawPlayers(struct Player team[], float r, float g, float b);

void init() {
    //glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black background color
    glClearColor(.8f, .8f, 0.1f, .8f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void signal_handler_usr1(int signum, siginfo_t *si, void *ptr);
void signal_handler_usr2(int signum, siginfo_t *si, void *ptr);
void signal_handler_ui(int signum, siginfo_t *si, void *ptr);

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // Draw blue team
    drawPlayers(blueTeam, 0.0f, 0.0f, 1.0f); // Blue color for blue team players
    // Draw red team
    drawPlayers(redTeam, 1.0f, 0.0f, 0.0f); // Red color for red team players

    //keep track of the balls
    for (int i = 0; i < PLAYERS_PER_TEAM; i++) {
        if (blueTeam[i].ball != NULL) {
            updateBallPosition(blueTeam[i].ball, &blueTeam[i], SPEED);
           // drawBall(blueTeam[i].ball->x, blueTeam[i].ball->y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for balls
        }
        if (redTeam[i].ball != NULL) {
            updateBallPosition(redTeam[i].ball, &redTeam[i], SPEED);
           // drawBall(redTeam[i].ball->x, redTeam[i].ball->y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for balls
        }
    }

    
    for (int i = 0; i < MAX_NUM_BALLS; i++) {

        drawBall(balls[i].x, balls[i].y, 0.03f, 1.0f, 1.0f, 1.0f); // White color for balls
    }
    

    glFlush();
}

void updateBallPosition(struct Ball* ball, struct Player* targetPlayer, float speed) {
    if (!ball->moving) return;
    //the rest if the ball is moving
    ball->x += ball->vx;
    ball->y += ball->vy;

    // Calculate the distance only once
    float dx = fabs(ball->x - targetPlayer->x);
    float dy = fabs(ball->y - targetPlayer->y);

    // Check if ball reached the destination
    if(dx < 0.01 && dy < 0.01) {
       ball->moving = false;
        /*
        if (ball == &balls[0]) {
            blueActivePlayer = (blueActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for blue team to next player
        } else if (ball == &balls[1]){
            redActivePlayer = (redActivePlayer + 1) % NUM_PLAYERS_PER_TEAM; // Update active player for red team
        }
        */
    }
}

void passBall(struct Ball* ball, struct Player* targetPlayer, float speed) {
    ball->moving = true;
    ball->vx = (targetPlayer->x - ball->x) * speed;
    ball->vy = (targetPlayer->y - ball->y) * speed;
}

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

/*
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'p': // 'p' key to pass the blue ball
            passBall(&balls[0], &blueTeam[blueActivePlayer], SPEED*2);
            break;
        case 'P': // 'P' key to pass the red ball
            passBall(&balls[0], &redTeam[redActivePlayer], SPEED*2);
            break;
    }
    glutPostRedisplay();
}

*/

// sigui -> new round
//
//


void signal_handler_usr1(int signum, siginfo_t *si, void *ptr) {
    
        // recieved integer.

        int received = si->si_value.sival_int;
        
        // pass to the other team leader. (team1 -> team2)
        if (received == 511) {

                redTeam[5].ball = blueTeam[5].ball;
                blueTeam[5].ball = NULL;
                passBall(redTeam[5].ball, &redTeam[5], SPEED);

        }

        // pass to the other team leader. (team2 -> team1)
        else if (received == 115) {

                blueTeam[5].ball = redTeam[5].ball;
                redTeam[5].ball = NULL;
                passBall(blueTeam[5].ball, &blueTeam[5], SPEED);
        }

        // ball received from parent
        else if (received == -1) {
            // TODO
            
            printf("TESSTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n");
            // tell the next player which ball to pass next time
            blueTeam[5].ball = &balls[current_ball_index];
            passBall(&balls[current_ball_index], &blueTeam[5], SPEED);
            current_ball_index++;
        }

        else if (received == -2) {
            // TODO
            // tell the next player which ball to pass next time
            redTeam[5].ball = &balls[current_ball_index];
            passBall(&balls[current_ball_index], &redTeam[5], SPEED);
            current_ball_index++;
        }

        // pass between player 10 and 11
        else if (received == 1011) {
            
            redTeam[5].ball = redTeam[4].ball;
            redTeam[4].ball = NULL;
            passBall(redTeam[5].ball, &redTeam[5], SPEED);
        }

        else if (received == 910) {
        
            redTeam[4].ball = redTeam[3].ball;
            redTeam[3].ball = NULL;
            passBall(redTeam[4].ball, &redTeam[4], SPEED);
        }

        else {

            int sender = received / 10;
            int receiver = received % 10;


            // initially (just to test)
            // 
            //
            //
            //
            if (receiver <= 5) {

                printf("sender: %d, receiver: %d-------------------------------------------------------------------\n", sender, receiver);
                blueTeam[receiver].ball = blueTeam[sender].ball;
                blueTeam[sender].ball = NULL;
                passBall(blueTeam[receiver].ball, &blueTeam[receiver], SPEED);
            }

            else {

                 sender = sender - 6;
                 receiver = receiver - 6;

                printf("sender: %d, receiver: %d+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n", sender+6, receiver+6);
                redTeam[receiver].ball = redTeam[sender].ball;
                redTeam[sender].ball = NULL;
                passBall(redTeam[receiver].ball, &redTeam[receiver], SPEED);
            }
            
            // Pass the ball to the receiver

        }


    glutPostRedisplay();
}

void signal_handler_usr2(int signum, siginfo_t *si, void *ptr) {
    //TODO
}

void signal_handler_ui(int signum, siginfo_t *si, void *ptr) {
    new_round();
}




void new_round() {

    current_ball_index = 0;

    // Initialize blue team and red team player positions
    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        blueTeam[i].x = cos(2 * PI * (5 - i) / NUM_PLAYERS_PER_TEAM) * 0.5f;
        blueTeam[i].y = sin(2 * PI * (5 - i) / NUM_PLAYERS_PER_TEAM) * 0.5f - 0.5f;
        redTeam[i].x = cos(2 * PI * (5 - i) / NUM_PLAYERS_PER_TEAM) * 0.5f;
        redTeam[i].y = sin(2 * PI * (5 - i) / NUM_PLAYERS_PER_TEAM) * 0.5f + 0.5f;
    }

    for (int i = 0; i < MAX_NUM_BALLS; ++i) {
        balls[i].holder = NULL;
        balls[i].moving = false;
        balls[i].x = 0.8f;
        balls[i].y = 0.0f;
        balls[i].vx = 0.0f;
        balls[i].vy = 0.0f;
    }

    for (int i = 0; i < NUM_PLAYERS_PER_TEAM; ++i) {
        blueTeam[i].ball = NULL;
        redTeam[i].ball = NULL;
    }
}

int main(int argc, char** argv) {
    //set the signal handler for SIGUSR1 and SIGUSR2
    sa_usr1.sa_sigaction = signal_handler_usr1;
    sa_usr1.sa_flags = SA_SIGINFO;
    sigemptyset(&sa_usr1.sa_mask);

    if (sigaction(SIGUSR1, &sa_usr1, NULL) == -1) {
        perror("sigaction for SIGUSR1");
        exit(EXIT_FAILURE);
    }

    sa_usr2.sa_sigaction = signal_handler_usr2;
    sa_usr2.sa_flags = SA_SIGINFO;
    sigemptyset(&sa_usr2.sa_mask);

    if (sigaction(SIGUSR2, &sa_usr2, NULL) == -1) {
        perror("sigaction for SIGUSR2");
        exit(EXIT_FAILURE);
    }

    sa_ui.sa_sigaction = signal_handler_ui;
    sigemptyset(&sa_ui.sa_mask);
    sa_ui.sa_flags = SA_SIGINFO;

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
//    glutKeyboardFunc(keyboard);
    glutIdleFunc(display); // Update display continuously
    init();
    glutMainLoop();

    return 0;
}
