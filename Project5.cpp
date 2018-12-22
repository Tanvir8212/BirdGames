#include<windows.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
#include <unistd.h>
#include <algorithm>
#include <list>
#include <fstream>
#include <iostream>
#include<mmsystem.h>


int score =0;

using namespace std;


int windowX = 1200, windowY = 600 ;
//
GLfloat positionX = 0.0f;
GLfloat positionY = 0.0f;
GLfloat speed = .1f;
GLfloat speedEnvironment = .05f;

list <int> rX;
list <double> rY;
list <int> rXLife;
list <double> rYLife;
list <int> life;

GLfloat birdX = -5.00f;
GLfloat birdY = 0.50f;
GLfloat positionCloudX = 5.75f;
GLfloat birdEyeX= -5.00f;
GLfloat birdEyeY= 0.50f;
GLfloat adjustWingX= 0.00f;

GLfloat upWingY[13];
GLfloat downWingY[13];

GLfloat wingZ = 0.3f;
bool checkOnlyOneTime = true;

int level = 0;

char text[] = "Your score is: ";

void Sprint( float x, float y, char *st)
{
    int l,i;


    l=strlen( st ); // see how many characters are in text string.
    glColor3f(0.0,0.0,0.0);
    //glDisable(GL_LIGHTING);
    glRasterPos3f( x, y,2); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);

    }
}

void Sprint1( float x, float y, char *st)
{
    int l,i;


    l=strlen( st ); // see how many characters are in text string.
    glColor3f(0.0,0.0,0.0);
    //glDisable(GL_LIGHTING);
    glRasterPos3f( x, y,2.5); // location to start printing text
    for( i=0; i < l; i++) // loop until i is greater then l
    {
       glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, st[i]);

    }
}


void initRendering()
{
    gluOrtho2D(0.0, 1200.0, 0, 600.0);
    glEnable(GL_DEPTH_TEST);
}

//Called when the window is resized
void handleResize(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void generateRandomNumber(){
    double ryArr[9] = {-1, -.5, 0, .5, 1, 1.5, 2, 2.5, 3};
    /// For obstacles
     for(int i=0; i<30; i++){
        int rx= rand() % 55 + 0;
        rX.push_back(rx);
        /// 0 to 55..

        int ryi= rand() % 9 + 0;

        rY.push_back(ryArr[ryi]);
        /// -1 to 3
    }

    /// For life balls
     for(int i=0; i<5; i++){
        int rxLife= rand() % 55 + 0;
        rXLife.push_back(rxLife);
        /// 0 to 55..

        int ryi= rand() % 9 + 0;

        rYLife.push_back(ryArr[ryi]);
        /// -1 to 3
    }


}


bool collision()
{
    /// Check collision of obstacles
    rX.sort();
    list<double>::iterator ity = rY.begin();
    for(list<int>::iterator it = rX.begin(); it != rX.end(); ++it){
        ++ity;
        if((positionX+(*it)-.55)<=birdX){
            if(positionY+(*ity)==birdY){
                return true;
            }
            else{
                rX.erase(it);
                rY.erase(ity);
                return false;
            }
        }
        else{
            return false;
        }
    }
}

int checkLife = -6;

bool collisionLife()
{
    /// Check collision of life balls
    rXLife.sort();
    list<double>::iterator ity = rYLife.begin();
    for(list<int>::iterator it = rXLife.begin(); it != rXLife.end(); ++it){
        ++ity;
        if((positionX+(*it)-.55)<=birdX){
            if(positionY+(*ity)==birdY){

                return true;
            }
            else{
                rXLife.erase(it);
                rYLife.erase(ity);

                return false;
            }
        }
        else{
            return false;
        }
    }
}

float _angle = 0.0;
float _angle1 = 30.0f;
float _cameraAngle = 0.0;
float _ang_tri = 0.0;

float colorSky1 = .2, colorSky2 = 0.765, colorSky3= 1.0;

static bool  startingPage = true;
static bool endingPage = false;



//Draws the 3D scene
void drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
    glLoadIdentity(); //Reset the drawing perspective
    glRotatef(-_cameraAngle, 0.0, 1.0, 0.0); //Rotate the cameraC:\Users\User\Documents\Camtasia
    glTranslatef(0.0, 0.0, -7.0); //Move forward 5 units

    /// Starting Page
    if(startingPage){
            glPushMatrix();
            glColor3f(.5, 1, .5);
            glBegin(GL_QUADS);
            glVertex3f(-30, -3.0, 1.0);
            glVertex3f(30, -3.0, 1.0);
            glVertex3f(30, 5, 1.0);
            glVertex3f(-30,  5, 1.0);
            glEnd();
            glPopMatrix();

            char startWriting[100] = "Press n to start the game...";
            Sprint(-1,0,startWriting);

            char startWriting1[100] = "***Control***";
            Sprint(-.5,-.5,startWriting1);

            char startWriting2[100] = "# Use UP, DOWN, LEFT, RIGHT button to move";
            Sprint(-1.5,-.65,startWriting2);

            char startWriting3[100] = "# Use s button to speed up";
            Sprint(-1.5,-.8,startWriting3);

            char startWriting4[100] = "# Use w button for day-night mode";
            Sprint(-1.5,-.95,startWriting4);

    }


    /// LIFE
   glColor3f(1, 0, 0);
    for(list<int>::iterator it1 = life.begin(); it1 != life.end(); ++it1){
    glPushMatrix();
        glBegin(GL_QUADS);
        glVertex3f((*it1),2.5,0);
        glVertex3f((*it1)+1,2.5,0);
        glVertex3f((*it1)+1,2.7,0);
        glVertex3f((*it1),2.7,0);
        glEnd();
    glPopMatrix();
    }

    /// SKY
    glColor3f(colorSky1, colorSky2, colorSky3);  ///20, 76.5, 100
    glBegin(GL_QUADS);
    glVertex3f(-30, -3.0, -1.0);
    glVertex3f(30, -3.0, -1.0);
    glVertex3f(30, 5, -1.0);
    glVertex3f(-30,  5, -1.0);
    glEnd();


    /// GROUND
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
    glVertex3f(-30, -1.0, 0.0);
    glVertex3f(30, -1.0, 0.0);
    glVertex3f(30, -3, 0.0);
    glVertex3f(-30,  -3, 0.0);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);

    /// GRASS on the ground
    glPointSize(5);
    glBegin(GL_POINTS);
    for(double j=-30; j<30; j+=.5)
    {
        for(double i=-1; i>-3; i-=.5)
        {
            glVertex3f(j, i, 1);
        }
    }
    glEnd();



    /// bird
    ///Body
    glColor3f(1, 1, 0);
    glPushMatrix();
    glTranslatef(birdX, birdY, 0.0);
    glutSolidSphere(.3,50,50);
    glPopMatrix();

    glColor3f(1, .5, 0);
    /// Head
    glPushMatrix();
    glTranslatef(birdX+.4, birdY+.07, 0.0);
    glutSolidSphere(.15,50,50);
    glPopMatrix();

    /// Eye
    glColor3f(0, 0, 0);
    glPushMatrix();
    glTranslatef(birdEyeX+.65, birdEyeY+.09, 0.25);
    glutSolidSphere(.02,50,50);
    glEnd();
    glPopMatrix();


    glColor3f(0, .5, 0);
    /// Lips
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(birdX+.55,birdY+.1,0.0);
    glVertex3f(birdX+.7,birdY+.04,0.0);
    glVertex3f(birdX+.55,birdY+.01,0.0);
    glEnd();
    glPopMatrix();

    /// BackWings
    glColor3f(0, .5, 0);
    glPushMatrix();
    glBegin(GL_TRIANGLES);
    glVertex3f(birdX-.1,birdY+.03,0.0);
    glVertex3f(birdX-.7,birdY+.4,0.0);
    glVertex3f(birdX-.3,birdY+.00,0.0);
    glEnd();
    glPopMatrix();


    /// Wings
    glColor3f(1.0, 0.4, 0.1);

    upWingY[1] =   birdY+.03;
    upWingY[2] =   birdY+.6;
    upWingY[3] =   birdY+.55;
    upWingY[4] =   birdY+.6;
    upWingY[5] =   birdY+.55;
    upWingY[6]=   birdY+.6;
    upWingY[7] =   birdY+.55;
    upWingY[8] =   birdY+.6;
    upWingY[9] =   birdY+.55;
    upWingY[10] =   birdY+.6;
    upWingY[11] =   birdY+.55;
    upWingY[12] =   birdY+.6;
    upWingY[13] =   birdY+.03;

    downWingY[1] =   birdY+.03;
    downWingY[2] =   birdY-.6;
    downWingY[3] =   birdY-.55;
    downWingY[4] =   birdY-.6;
    downWingY[5] =   birdY-.55;
    downWingY[6] =   birdY-.6;
    downWingY[7] =   birdY-.55;
    downWingY[8] =   birdY-.6;
    downWingY[9] =   birdY-.55;
    downWingY[10] =   birdY-.6;
    downWingY[11]=   birdY-.55;
    downWingY[12] =   birdY-.6;
    downWingY[13] =   birdY+.03;




    glPushMatrix();
    glBegin(GL_POLYGON );
    glVertex3f(birdX-.1+.2-adjustWingX,upWingY[1],0.3);
    glVertex3f(birdX-.25+.2-adjustWingX,upWingY[2],wingZ);
    glVertex3f(birdX-.2+.2-adjustWingX,upWingY[3],wingZ);
    glVertex3f(birdX-.15+.2-adjustWingX,upWingY[4],wingZ);
    glVertex3f(birdX-.1+.2-adjustWingX,upWingY[5],wingZ);
    glVertex3f(birdX-.05+.2-adjustWingX,upWingY[6],wingZ);
    glVertex3f(birdX-0+.2-adjustWingX,upWingY[7],wingZ);
    glVertex3f(birdX+.05+.2-adjustWingX,upWingY[8],wingZ);
    glVertex3f(birdX+.1+.2-adjustWingX,upWingY[9],wingZ);
    glVertex3f(birdX+.15+.2-adjustWingX,upWingY[10],wingZ);
    glVertex3f(birdX+.2+.2-adjustWingX,upWingY[11],wingZ);
    glVertex3f(birdX+.25+.2-adjustWingX,upWingY[12],wingZ);
    glVertex3f(birdX+.1+.2-adjustWingX,upWingY[13],0.3);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glBegin(GL_POLYGON );
    glVertex3f(birdX-.1+.2-adjustWingX,downWingY[1]-.025,0.3);
    glVertex3f(birdX-.25+.2-adjustWingX,downWingY[2],wingZ);
    glVertex3f(birdX-.2+.2-adjustWingX,downWingY[3],wingZ);
    glVertex3f(birdX-.15+.2-adjustWingX,downWingY[4],wingZ);
    glVertex3f(birdX-.1+.2-adjustWingX,downWingY[5],wingZ);
    glVertex3f(birdX-.05+.2-adjustWingX,downWingY[6],wingZ);
    glVertex3f(birdX-0+.2-adjustWingX,downWingY[7],wingZ);
    glVertex3f(birdX+.05+.2-adjustWingX,downWingY[8],wingZ);
    glVertex3f(birdX+.1+.2-adjustWingX,downWingY[9],wingZ);
    glVertex3f(birdX+.15+.2-adjustWingX,downWingY[10],wingZ);
    glVertex3f(birdX+.2+.2-adjustWingX,downWingY[11],wingZ);
    glVertex3f(birdX+.25+.2-adjustWingX,downWingY[12],wingZ);
    glVertex3f(birdX+.1+.2-adjustWingX,downWingY[13]-.025,0.3);
    glEnd();
    glPopMatrix();


    glColor3f(0.0, 0.0, 1.0);

    /// OBSTACLES

    list<double>::iterator ity = rY.begin();
    for(list<int>::iterator it = rX.begin(); it != rX.end(); ++it){
        ++ity;
        glPushMatrix();

        glTranslatef(positionX+(*it), positionY+(*ity), 0.0);
        glutSolidSphere(.3,50,50);

        glPopMatrix();
    }




    /// LifeBalls
    glColor3f(1, 0.0, 0.0);

    list<double>::iterator ity1 = rYLife.begin();
    for(list<int>::iterator it1 = rXLife.begin(); it1 != rXLife.end(); ++it1){
        ++ity1;
        glPushMatrix();

        glTranslatef(positionX+(*it1), positionY+(*ity1), 0.0);
        glutSolidTorus(0.05, .15, 15, 35);

        glPopMatrix();
    }



    /// CLOUDS
    glColor3f(1.0, 1.0, 1.0);

    glPushMatrix();
    glTranslatef(positionCloudX, 2, -1);
    glutSolidSphere(.6,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX-.8, 2, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX+.8, 2, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(positionCloudX+5, 1.25, -1);
    glutSolidSphere(.6,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX-.8+5, 1.25, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX+.8+5, 1.25, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(positionCloudX+15, 2.25, -1);
    glutSolidSphere(.6,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX-.8+15, 2.25, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(positionCloudX+.8+15, 2.25, -1);
    glutSolidSphere(.4,50,50);
    glPopMatrix();


    /// Check Collision with life balls
    if(collisionLife()){
        if(life.size()<13){
            int p = life.size();
            for(int i=p; i<=13; i++){
                life.push_back(i-7);
            }
        }

    }

    /// Check Collision
    if(collision()){
    /// Collision Window
        life.pop_back();


        checkLife++;
        if(life.empty()){
                endingPage= true;

            glColor3f(0.0, 1.0, 0.0);
            glBegin(GL_QUADS);
            glVertex3f(-6, -3.0, 2.0);
            glVertex3f(6, -3.0, 2.0);
            glVertex3f(6, 3, 2.0);
            glVertex3f(-6,  3, 2.0);
            glEnd();
            Sprint1(-.5,0,"GAME OVER...");

            char Score_string[100];
            sprintf(Score_string,"%d",score);

            char print_string[100] = "Your score: ";
            strcat(print_string,Score_string);

            Sprint1(-0.5,-.5,print_string);



            speed=0.0;
            speedEnvironment = 0.0;

            int getLastHighScore;

        ifstream infile;
        infile.open("Score.txt");

        infile>>getLastHighScore;

        if(score>getLastHighScore){
            ofstream scoreFile;
            scoreFile.open("Score.txt");
            scoreFile << score << endl;
            checkOnlyOneTime= false;
        }
        }


    }
    else{
        if(startingPage==false && endingPage==false){
            score++;
        }
    }

    int getLastHighScore;

    ifstream infile;
    infile.open("Score.txt");

    infile>>getLastHighScore;

    if(score>getLastHighScore){
            char highScoreDone[100] = "Congratulations !!! NEW HIGH SCORE... <3";
            Sprint(-1.3,-1.6,highScoreDone);
        }


    char highScore_string[100];
    sprintf(highScore_string,"%d",getLastHighScore);

    char print_high_string[100] = "High score: ";
    strcat(print_high_string,highScore_string);

    Sprint(-0.5,1.95,print_high_string);


    char score_string[100];
    sprintf(score_string,"%d",score);

    char print_string[100] = "Your score: ";
    strcat(print_string,score_string);

    Sprint(-4,1.95,print_string);

    char level_string[100];
    sprintf(level_string,"%d",level);

    char print_string1[100] = "Level: ";
    strcat(print_string1,level_string);

    Sprint(3.5,1.95,print_string1);



    glutSwapBuffers();
}


int checkS=0;
int checkJump=0;

int birdYa=0;


void handleKeypress(int key, int x, int y)
{

    switch (key)
    {

        /// UP
    case GLUT_KEY_UP:
        if(birdY<2.5){
            birdY+=0.5;
            birdEyeY+=0.48;
        }
//}while(posx-=2);
        break;
        /// DOWN
    case GLUT_KEY_DOWN:
        if(birdY>-1){
             birdY-=0.5;
             birdEyeY-=0.48;
        }
        //elbow = (elbow + 200) % 360;
        glutPostRedisplay();
//}while(posx-=2);
        break;
        /// LEFT
    case GLUT_KEY_LEFT:
        if(birdX>-5.5){
            birdX-=0.5;
            birdEyeX-=0.48;
            adjustWingX-=0.025;
        }
        //elbow = (elbow + 200) % 360;
        glutPostRedisplay();
//}while(posx-=2);
        break;
    case GLUT_KEY_RIGHT:
        if(birdX<5.5){
            birdX+=0.5;
            birdEyeX+=0.48;
            adjustWingX+=0.025;

        }

        //elbow = (elbow + 200) % 360;
        glutPostRedisplay();
//}while(posx-=2);
        break;
    }
}

bool checkDay = true;

void handleKeypress1(unsigned char key, int x, int y)
{
    switch (key)
    {
    case ' ':
        if(checkS==0){
            speed = 0.0f;
            checkS=1;
        }
        else{
            speed = 0.05f;
            checkS=0;
        }
        break;

    case 's':
        speed += 0.02f;
        break;
    case 'w':
        if(checkDay){
            colorSky1=0;
            colorSky2=0;
            colorSky3=0;
            checkDay= false;
        }
        else{
            colorSky1 = .2;
            colorSky2 = 0.765;
            colorSky3= 1.0;
            checkDay= true;
        }
        break;
    case 'n':
        startingPage=false;
        positionX=0;
        glutPostRedisplay();
        break;
    }
}





void update(int value)
{

        if(positionX < -60){
            positionX =5.0f;
            speed += 0.05;
            generateRandomNumber();
            level++;
        }

        if(wingZ>.7){
            wingZ=.3;
        }

        wingZ+= .01;


        positionX -= speed;

        if(positionCloudX<-25){
            positionCloudX = 10;
        }
        positionCloudX -= speedEnvironment;

        glutPostRedisplay();


        glutTimerFunc(50, update, 0);



}

void lightSetting()
{
	float zpos = -0.0;


	GLfloat ambientIntensity[4] = {.8, 1, .7, 1.0}; // 6. ambient property
	GLfloat diffuseIntensity[4] = {.8, 1, .7, 1.0}; // 8. ambient property
	GLfloat position[4] = {0, 3, zpos, 0.0}; // 10. position property

	glEnable(GL_DEPTH_TEST); // 4. just keep
	glEnable(GL_COLOR_MATERIAL); // 3. Materals' color

	glEnable(GL_LIGHTING); // 1. enable lighting
	glEnable(GL_LIGHT0); // 2. enable light 0
	glEnable(GL_NORMALIZE);

	// set up light 0 properties
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientIntensity); // 7. ambient property adding to Light0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseIntensity); // 9. ambient property adding to Light0
	glLightfv(GL_LIGHT0, GL_POSITION, position); // 11. position property adding to Light0
}





int main(int argc, char** argv)
{

   generateRandomNumber();

   for(int i= -6; i<=6; i++){
        life.push_back(i);
   }

    //Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 600);
    glutInitWindowPosition(200,100);



    //Create the window
    glutCreateWindow("Transformations");
    initRendering();
    lightSetting();

    //Set handler functions
    glutDisplayFunc(drawScene);

    glutReshapeFunc(handleResize);
    glutSpecialFunc(handleKeypress);
    glutKeyboardFunc(handleKeypress1);
    //sndPlaySound("Music.wav",SND_ASYNC|SND_LOOP);

    glutTimerFunc(200, update, 0); //Add a timer

    glutMainLoop();



    return 0;
}











