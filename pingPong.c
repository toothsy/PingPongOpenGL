
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
/* A simple ping pong game using opengl library and glut
   The game starts on left mouse click, and stops on middle mouse click
   Player 1 can control the game using the keys q and a
   Player 2 can control the game using the keys o and l
*/
// global variables
static GLint windowSizeX = 1200,
                    windowSizeY = 800;
static GLint orthoSizeX = 600,
                    orthoSizeY = 400;
// game variables
static char score_1[20], score_2[20];
static GLint player1_score = 0,
                    player2_score = 0;
static GLint player1_life = 3,
                     player2_life = 3;
static GLint paddle_height = 300,//changable
                    paddle_boundary = windowSizeY/2-paddle_height/2, 
                    paddle_velocity = 8.0;
static GLint player1_paddle_y = 0,
                    player2_paddle_y = 0, 
                    paddle_x = 595;//paddle initial position
static GLfloat ball_velocity_x = 0, 
                    ball_velocity_y = 0, 
                    speed_increment = 1.0;
static GLfloat  ballColor[][6] = {{1.0,1.0,1.0},{1.0,0.0,0.0},{0.0,1.0,1.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{0.0,1.0,0.0}}; //paddle colors
static GLfloat  paddleColor[][6] = {{1.0,1.0,1.0},{1.0,0.0,0.0},{0.0,1.0,1.0},{0.0,0.0,1.0},{1.0,1.0,0.0},{0.0,1.0,0.0}}; 

static int  ballColorIndex=0;
static int  paddleColorIndex=0;
static GLint minHeight=100;
static GLint expert=9, hard = 7,medium = 5 ,easy = 3 ;
static GLint level = 0;
static GLint ball_pos_x = 0, ball_pos_y = 0, ball_radius = 40;
// draw text on screen
void drawStrokeText(char*string, int x, int y, int z)
{
    char *c;
    glPushMatrix();
    glTranslatef(x, y+8,z);
    // glScalef(0.09f,-0.08f,z);
    glColor3f(1.0,1.0,1.0);
    for (c=string; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN , *c);
    }
    glPopMatrix();
}
// draw the center lines spaces 20 pixels apart and with a width of 4 px
void drawCenterLines() {
    // center lines start
    int y= -410;
    for(int i =0;i<25;i++){
    glBegin(GL_QUADS);
        glColor3f(1.0, 1.0, 1.0);
        glVertex2f(-2 , y);
        glVertex2f(2 , y);
        y+=20;
        glVertex2f(2 , y);
        glVertex2f(-2 , y);
        y+=20;
    glEnd();
    }
    // center lines end
}
// x, y is the top left corodinate of the paddle
void drawPaddle(int x, int y,GLfloat paddleColor[]) {
    glPushMatrix();

    glTranslatef(x, y, 0);
    GLint paddleWidth = 5;
    int height = paddle_height / 2;
    glColor3fv(paddleColor);
    glBegin(GL_QUADS);
        glVertex2f(-paddleWidth , height);
        glVertex2f(paddleWidth , height);
        glVertex2f(paddleWidth , -height);
        glVertex2f(-paddleWidth, -height);
    glEnd();

    glPopMatrix();
}
void drawBall(int x, int y,GLfloat ballColor[]) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glColor3fv(ballColor);
    glutSolidSphere (ball_radius, 20, 16);
    glPopMatrix();
}
// main display functions
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    // create center lines
    drawCenterLines();
    // draw left paddle at (-paddle_x, player1_paddle_y)
    drawPaddle(-paddle_x, player1_paddle_y,paddleColor[paddleColorIndex]);
    // draw right paddle at (paddle_x, player2_paddle_y)
    drawPaddle(paddle_x, player2_paddle_y,paddleColor[paddleColorIndex]);
    // draw the ball (ball_pos_x, ball_pos_y) - varies in each frame
    drawBall(ball_pos_x, ball_pos_y,ballColor[ballColorIndex]);
    // draw the score on the left for player 1
    snprintf (score_1, sizeof(score_1), "%d", player1_score);
    drawStrokeText(score_1, -300, 200, 0);
    // draw the score on the left for player 1
    snprintf (score_2, sizeof(score_2), "%d", player2_score);
    drawStrokeText(score_2, 200, 200, 0);
    // swap the current frame with the drawn frame
    glutSwapBuffers();
    glFlush();
}
void startGame(void) {
    // move the ball
    ball_pos_x += ball_velocity_x;
    ball_pos_y += ball_velocity_y;
    // ball hits the top or bottom
    if (ball_pos_y + ball_radius > orthoSizeY || ball_pos_y - ball_radius < -orthoSizeY)
        ball_velocity_y = -ball_velocity_y;
    // ball hits the left paddle
    if (ball_pos_x - ball_radius - 5 < -paddle_x )//5 is paddle 
    //so that it doesnt go inside paddle
        if (ball_pos_y < player1_paddle_y + paddle_height/2 && ball_pos_y > player1_paddle_y - paddle_height/2) {
            ball_velocity_x = -ball_velocity_x;
            paddle_velocity += speed_increment;
        }
    // ball hits the right paddle
    if (ball_pos_x + ball_radius + 5 > paddle_x)
        if (ball_pos_y < player2_paddle_y + paddle_height/2 && ball_pos_y > player2_paddle_y - paddle_height/2){
            ball_velocity_x = -ball_velocity_x;
            }
// 
    // player 1 scores
    if (ball_pos_x + ball_radius > orthoSizeX) {
        player1_score++;
        printf("Player 1 = %d \n", player1_score);
        ball_velocity_x = -ball_velocity_x;
    }

    // player 2 scores
    if (ball_pos_x - ball_radius < -orthoSizeX) {
        player2_score++;
        printf("Player 2 = %d \n", player2_score);
        ball_velocity_x = -ball_velocity_x;
    }
    printf("ball velo x  = %f \n", ball_velocity_x);
    printf("ball velo y = %f \n", ball_velocity_y);
    glutPostRedisplay();
}
// reshape the display
void reshape(int w, int h) {
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-orthoSizeX, orthoSizeX, -orthoSizeY, orthoSizeY, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
// on mouse button click
void mouse(int button, int state, int x, int y) {
    switch (button) {
        // left button - initialize random velocity between (ran(5) - rand(3))
        case GLUT_LEFT_BUTTON:
            if (state == GLUT_DOWN)
            ball_velocity_x = easy;
            ball_velocity_y = 1;
            // keep on calling the callback to move the ball and check boundary conditions
            glutIdleFunc(startGame);
            break;
        // middle button to reset the ball, paddle and score
        case GLUT_MIDDLE_BUTTON:
            // reset ball, paddle and player scores
            ball_pos_x = ball_pos_y = 0;
            player1_paddle_y = player2_paddle_y = 0;
            player1_score = player2_score = 0;
            if (state == GLUT_DOWN)
                // remove the call back so that game stops
                glutIdleFunc(NULL);
            break;
        default:
        break;
    }
}
void keyboard (unsigned char key, int x, int y) {
        // printf("%d %d\n",x,y);
    switch (key) {
        // move player 1 paddile up
        case 'q':
            if (player1_paddle_y < paddle_boundary){
                player1_paddle_y += paddle_velocity;
                // printf("%d\t%d\n",player1_paddle_y,paddle_boundary);
                }
                glutPostRedisplay();
            break;
        // move player 1 paddile down
        case 'a':
            if (player1_paddle_y > -paddle_boundary){
                player1_paddle_y -= paddle_velocity;
                // printf("%d\t%d\n",player1_paddle_y,paddle_boundary);

                }
            glutPostRedisplay();
            break;
        // move player 2 paddile up
        case 'o':
            if (player2_paddle_y < paddle_boundary)
                player2_paddle_y += paddle_velocity;
            glutPostRedisplay();
            break;
        // move player 2 paddile down
        case 'l':
            if (player2_paddle_y > -paddle_boundary)
                player2_paddle_y -= paddle_velocity;
            glutPostRedisplay();
            break;
        // exit on esc
        case 27:
            exit(0);
            break;
        default:
            break;
    }
}
void init(void) {
    // initalise display with black colors
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel (GL_SMOOTH);
    srand(2);   // should only be called once
}
/*
* Request double buffer display mode.
* Register mouse input callback functions
*/
void paddleSizeHandle(int id){
    switch(id){
        case 1 : paddle_height = paddle_height>=windowSizeY/2 ? windowSizeY/2 : paddle_height+50;break;
        case 2 : paddle_height =paddle_height<=minHeight ? minHeight: paddle_height-50;break;
    }
    paddle_boundary=windowSizeY/2-paddle_height/2;
    glutPostRedisplay();
}
void levelHandle(int id){
     switch(id){
         case 1 :  ball_velocity_x = easy;break;
         case 2 :  ball_velocity_x = medium;break;
         case 3 :  ball_velocity_x = hard;break;
         case 4 :  ball_velocity_x = expert;break;
    }
    glutPostRedisplay();

}
void mainHandle(int id){
    switch(id){
        case 1: break;
    }
}
void ballSizeHandle(int id){
    //;)
    switch(id){
        case 1:ball_radius= ball_radius<=80?ball_radius + 10:ball_radius; break;
        case 2: ball_radius=ball_radius>=20?ball_radius - 10: ball_radius;break;
    }
    glutPostRedisplay();
}
void ballColorHandle(int id){
    switch(id){
        case 0: ballColorIndex = 0;break;
        case 1: ballColorIndex = 1;break;
        case 2: ballColorIndex = 2;break;
        case 3: ballColorIndex = 3;break;
        case 4: ballColorIndex = 4;break;
        case 5: ballColorIndex = 5;break;
    }
    glutPostRedisplay();

}
void paddleColorHandle(int id){
    switch(id){
        case 0: paddleColorIndex = 0;break;
        case 1: paddleColorIndex = 1;break;
        case 2: paddleColorIndex = 2;break;
        case 3: paddleColorIndex = 3;break;
        case 4: paddleColorIndex = 4;break;
        case 5: paddleColorIndex = 5;break;
    }
    glutPostRedisplay();
    }

void makeMenu(){
    int paddleSize = glutCreateMenu(paddleSizeHandle);
        glutAddMenuEntry("increase paddle height",1);
        glutAddMenuEntry("decrease paddle height",2);
    int gameLevel = glutCreateMenu(levelHandle);
        glutAddMenuEntry("easy",1);
        glutAddMenuEntry("medium",2);
        glutAddMenuEntry("hard",3);
        glutAddMenuEntry("expert",4);
    int ballSize = glutCreateMenu(ballSizeHandle);
        glutAddMenuEntry("increase ball size",1);
        glutAddMenuEntry("decrease ball size",2);
    int ballColor  = glutCreateMenu(ballColorHandle);
        glutAddMenuEntry("white",0);
        glutAddMenuEntry("red",1);
        glutAddMenuEntry("cyan",2);
        glutAddMenuEntry("blue",3);
        glutAddMenuEntry("yellow",4);
        glutAddMenuEntry("green",5);
    int paddleColor  = glutCreateMenu(paddleColorHandle);
        glutAddMenuEntry("white",0);
        glutAddMenuEntry("red",1);
        glutAddMenuEntry("cyan",2);
        glutAddMenuEntry("blue",3);
        glutAddMenuEntry("yellow",4);
        glutAddMenuEntry("green",5);

   int mainMenu = glutCreateMenu(mainHandle);
        glutAddSubMenu("game level",gameLevel);
        glutAddSubMenu("paddle size change",paddleSize);
        glutAddSubMenu("ball size",ballSize);
        glutAddSubMenu("ball color",ballColor);
        glutAddSubMenu("paddle color",paddleColor);
        


    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize (windowSizeX, windowSizeY);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);
    init ();
    // call back functions for rendering, reshape
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    makeMenu();
    // callback on mouse click and keyboard input
    glutMouseFunc(mouse);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
