#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <math.h> 
using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)

// Function Declarations
void drawScene();
void update(int value);
void drawBox(float len);
void initRendering();
void handleKeypress1(unsigned char key, int x, int y);
void handleResize(int w, int h) ;
void collisions();
void friction(int i, int j);

// -------------------------------------- Variables ---------------------------

// Lengths and Radii
float len=5.3f;
float str_rad=0.2f;
float coin_rad=0.13f;

// Velocities
float str_velx=0.01f;
float str_vely=0.024f;
float coin_velx=0.0f;
float coin_vely=0.0f;


// Positions
float str_x=0.0f;
float str_y=(-len/2.9f);

float coin_x=0.0f;
float coin_y=-0.25f;
//--------------------------------- End Variables ------------------------

// Classes

class Box{
	public:
		float len;
		void set(float len_in)
		{
			len=len_in;
		}
		void drawBox() {

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_QUADS);
			glVertex2f(-len / 2, -len / 2);
			glVertex2f(len / 2, -len / 2);
			glVertex2f(len / 2, len / 2);
			glVertex2f(-len / 2, len / 2);
			glEnd();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

};

Box Box1,Box2;

class Circle{
	public:
		float rad,len,coin_x,coin_y,coin_velx,coin_vely;
		void set(float ra,float l, float coin_x, float coin_y, float coin_velx, float coin_vely)
		{
			rad=ra;
			len=l;// Board's length
			this->coin_x=coin_x;
			this->coin_y=coin_y;
			this->coin_vely=coin_vely;
			this->coin_velx=coin_velx;

		}
		void drawBall() {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBegin(GL_TRIANGLE_FAN);
			for(int i=0 ; i<360 ; i++) {
				glVertex2f(rad * cos(DEG2RAD(i)), rad * sin(DEG2RAD(i)));
			}
			glEnd();
		}
};

Circle Cir1,Cir2[10],hole[4];

int main(int argc, char **argv) {

	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

	int w = glutGet(GLUT_SCREEN_WIDTH);
	int h = glutGet(GLUT_SCREEN_HEIGHT);
	int windowWidth = w * 2 / 3;
	int windowHeight = h * 2 / 3;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	glutCreateWindow("Carrom");  // Setup the window
	initRendering();

	// Register callbacks

	//void set(float ra,float l, float coin_x, float coin_y, float coin_velx, float coin_vely)
	Cir2[0].set(coin_rad,len,0.0f, 0.0f,0.0f, 0.0f);// Queen Coin Set
	Cir2[1].set(coin_rad,len,coin_x, coin_y-0.05f,coin_velx,coin_vely);	// Black Coin Set
	float t=2.0*coin_y;	
	Cir2[2].set(coin_rad,len,coin_x,t-0.1 ,coin_velx,coin_vely); // White Coin Set
	Cir2[3].set(coin_rad,len,coin_x,-coin_y+0.05f ,coin_velx,coin_vely);
	Cir2[4].set(coin_rad,len,coin_x,-t+0.1f ,coin_velx,coin_vely);
	Cir2[5].set(coin_rad,len,coin_y-0.05f,0.0 ,coin_velx,coin_vely);
	Cir2[6].set(coin_rad,len,coin_y+coin_y-0.1f,0.0 ,coin_velx,coin_vely);
	Cir2[7].set(coin_rad,len,-coin_y+0.05f,0.0 ,coin_velx,coin_vely);
	Cir2[8].set(coin_rad,len,-coin_y-coin_y+0.1f,0.0 ,coin_velx,coin_vely);	
	Cir2[9].set(str_rad,len,str_x,str_y,str_velx,str_vely);	// Striker Set

	//cout << "Cir2[0].coin_x:" << Cir2[0].coin_x << endl;
	//cout << "Cir2[0].coin_y:" << Cir2[0].coin_y << endl;

	//hole[0].set(coin_rad,len,(-5.7f/2)+0.4f, (5.7f/2)-0.4f,0.0 ,0.0);

	glutDisplayFunc(drawScene);
	glutIdleFunc(drawScene);
	// glutKeyboardFunc(handleKeypress1);
	// glutSpecialFunc(handleKeypress2);
	// glutMouseFunc(handleMouseclick);
	glutReshapeFunc(handleResize);
	glutTimerFunc(1, update, 0);

	glutMainLoop();
	return 0;
}

// Function to draw objects on the screen

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();


	//_______________________________________________________________________________________
	// Draw Box 1
	glTranslatef(0.0f, 0.0f, -8.0f);
	glColor3f(0.502f, 0.000f, 0.000f);	
	Box1.set(6.0f);
	Box1.drawBox();

	// Draw Box 2
	glColor3f(1.000f, 0.871f, 0.678f);
	Box2.set(5.7f);
	Box2.drawBox();
	// _______________________________________________________________________________________

	// Drawing 4 Holes

	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	for(int i=0;i<4;i++)
	{
		glPushMatrix();
		glTranslatef((-5.7/2)+0.4f, (5.7/2)-0.4f, 0.0f);
		Cir1.set(0.25f,5.8f,(-5.7/2)+0.4f, (5.7/2)-0.4f,0.0,0.0 );
		Cir1.drawBall();
		glPopMatrix();
		glRotatef(90,0.0f,0.0f,-0.8f);
	}
	glPopMatrix();

	
	//_______________________________________________________________________________________

	// Drawing Center Disc

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	Cir1.set(0.7f,4.3f, 0.0f, 0.0f, 0.0f, 0.0f);
	Cir1.drawBall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.1f);
	glColor3f(1.000f, 0.871f, 0.678f);
	Cir1.set(0.65f,4.3f,0.0f, 0.0f, 0.0f, 0.0f);
	Cir1.drawBall();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -0.3f);
	glColor3f(1.000f, 0.0f, 0.0f);
	Cir1.set(0.15f,4.3f, 0.0f, 0.0f, 0.0f, 0.0f);
	Cir1.drawBall();
	glPopMatrix();

	// ______________________________________________________________________________________
	// Drawing 4 lines & 8 position circles
	for(int i=0;i<4;i++)
	{
		glPushMatrix();
		glLineWidth(4.0f); // Line Below
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f((-len/3.5), (-len/2.5f), 0.0);
		glVertex3f((len/3.5), (-len/2.5f), 0.0);
		glEnd();
		glPopMatrix();

		glPushMatrix();
		glLineWidth(2.0f); // Line Above
		glColor3f(0.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f((-len/3.5), (-len/2.8f), 0.0);
		glVertex3f((len/3.5), (-len/2.8f), 0.0);
		glEnd();
		glPopMatrix();

		glPushMatrix(); // Position Circle 1
		glTranslatef((-len/3.5)-0.1, (-len/2.6f), -0.3f);
		glColor3f(1.000f, 0.0f, 0.0f);
		Cir1.set(0.15f,4.3f,(-len/3.5)-0.1, (-len/2.6f),0.0,0.0);
		Cir1.drawBall();
		glPopMatrix();

		glPushMatrix(); // Position Circle 2
		glTranslatef((len/3.5)+0.1, -(len/2.6f), -0.3f);
		glColor3f(1.000f, 0.0f, 0.0f);
		Cir1.set(0.15f,4.3f, (len/3.5)+0.1, -(len/2.6f),0.0, 0.0);
		Cir1.drawBall();
		glPopMatrix();

		glRotatef(90,0.0f,0.0f,-0.8f);
	}
	//_____________________________________________________________________________________
	//Drawing 4 white + 4 Black +1 Queen Coin	

	glPushMatrix(); // Queen Coin
	glColor3f(1.0, 1.0, 0.447);
	glTranslatef(Cir2[0].coin_x, Cir2[0].coin_y, 0.6f);
	//cout << "Cir2[0].coin_x:" << Cir2[0].coin_x << endl;
	//cout << "Cir2[0].coin_y:" << Cir2[0].coin_y << endl;				
	Cir2[0].drawBall();
	glPopMatrix();

	//void set(float ra,float l, float coin_x, float coin_y, float coin_velx, float coin_vely)
	for(int i=1;i<11;i=i+2)
	{
		glPushMatrix(); // Black Coin
		glTranslatef(Cir2[i].coin_x, Cir2[i].coin_y, 0.6f);
		glColor3f(0.439, 0.502, 0.565);
		Cir2[i].drawBall();
		glPopMatrix();		

		
		if(i+1<11)
		{
			// White Coin
			glPushMatrix();	
			glTranslatef(Cir2[i+1].coin_x, Cir2[i+1].coin_y, 0.6f);
			glColor3f(0.741, 0.718, 0.420);
			 
			Cir2[i+1].drawBall();
			glPopMatrix();	
		}
		
	}
	//cout << endl;
	//cout << "Cir2[0].coin_x:" << Cir2[0].coin_x << endl;
	//cout << "Cir2[0].coin_y:" << Cir2[0].coin_y << endl;

	//exit(0);

	// Drawing Striker
	glPushMatrix(); // 
	glTranslatef(Cir2[9].coin_x,Cir2[9].coin_y, 0.6);
	glColor3f(0.545, 0.000, 0.545);
	Cir2[9].drawBall();
	glPopMatrix();
	// ______________________________________________________________________________________

	glPopMatrix();
	glutSwapBuffers();

}

void collisions()
{
	float vx1t,vy1t, vx2t, vy2t, ang, swap, px, py;
	for (int i = 0; i <11; i++)
	{
		for (int j = i + 1; j < 11; j++)  
		{  

			ang=atan2((Cir2[j].coin_y-Cir2[i].coin_y),(Cir2[j].coin_x-Cir2[i].coin_x));
			px=abs(Cir2[i].coin_x)-abs(Cir2[j].coin_x);
			px=px*px;
			py=abs(Cir2[i].coin_y)-abs(Cir2[j].coin_y);
			py=py*py;
			//friction(i,j);// Add friction
			if (sqrt(px+py)+0.1f<= Cir2[i].rad+Cir2[j].rad)  
			{
			//	cout << sqrt(px+py) <<endl;
 	
				// exit(0);
				// Resolving Velocities
				vx1t=Cir2[i].coin_velx * cos(ang) + Cir2[i].coin_vely* sin(ang);
				vy1t=Cir2[i].coin_vely * cos(ang) - Cir2[i].coin_velx * sin(ang);

				vx2t=Cir2[j].coin_velx * cos(ang)+Cir2[j].coin_vely* sin(ang);
				vy2t=Cir2[j].coin_vely* cos(ang) - Cir2[j].coin_velx * sin(ang);

				// Swapping		            
				swap=vx1t;
				vx1t=vx2t;
				vx2t=swap;

				// Re-Resolving Velocities
				Cir2[i].coin_velx=vx1t * cos(ang) -vy1t*sin(ang);
				Cir2[i].coin_vely=vx1t * sin(ang) +vy1t*cos(ang);

				Cir2[j].coin_velx=vx2t * cos(ang) -vy2t*sin(ang);
				Cir2[j].coin_vely=vx2t * sin(ang) +vy2t*cos(ang);

				
				/*while(sqrt(px+py)<= Cir2[i].rad+Cir2[j].rad)
				{
					cout << i <<endl;
					cout << j <<endl;
					cout << sqrt(px + py) << endl;
					cout << Cir2[i].rad + Cir2[j].rad << endl;
					px=abs(Cir2[i].coin_x)-abs(Cir2[j].coin_x);
					px=px*px;
					py=abs(Cir2[i].coin_y)-abs(Cir2[j].coin_y);
					py=py*py;
					Cir2[i].coin_x += Cir2[i].coin_velx;
					Cir2[i].coin_y += Cir2[i].coin_vely;
					Cir2[j].coin_x += Cir2[j].coin_velx;
					Cir2[j].coin_y += Cir2[j].coin_vely;			
				}*/
			}
		}

		// Handle Coin- Wall Collisions
		if(Cir2[i].coin_x + coin_rad >= (len)/2 || Cir2[i].coin_x - coin_rad <= -(len/2))
			Cir2[i].coin_velx *= -1; //cout << "Coin-" << i << ", STR_Y: " << str_y
		if(Cir2[i].coin_y + coin_rad >= (len)/2 || Cir2[i].coin_y - coin_rad <= -(len)/2)
			Cir2[i].coin_vely *= -1;

		// Update position of Coins after Wall Collision
		Cir2[i].coin_x += Cir2[i].coin_velx;
		Cir2[i].coin_y += Cir2[i].coin_vely;
	}
}
void friction(int i, int j)
{
				float fr=0.005f;
				if(fr>Cir2[i].coin_velx)// i->x
				{
					Cir2[i].coin_velx +=fr;
				}
				else if(fr<Cir2[i].coin_velx)
				{
					Cir2[i].coin_velx -=fr;
				}
				else
				{
					Cir2[j].coin_velx +=0.0f;
				}

				if(fr>Cir2[i].coin_vely)// i->y
				{
					Cir2[i].coin_vely +=fr;
				}
				else if(fr<Cir2[i].coin_vely)
				{
					Cir2[i].coin_vely -=fr;
				}
				else
				{
					Cir2[j].coin_vely =0.0f;
				}

				//====

				if(fr>Cir2[j].coin_velx)// j->x
				{
					Cir2[j].coin_velx +=fr;
				}
				else if(fr<Cir2[j].coin_velx)
				{
					Cir2[j].coin_velx -=fr;
				}
				else
				{
					Cir2[j].coin_velx +=0.0f;
				}


				if(fr>Cir2[j].coin_vely)// j->y
				{
					Cir2[j].coin_vely +=fr;
				}
				else if(fr<Cir2[j].coin_vely)
				{
					Cir2[j].coin_vely -=fr;
				}
				else
				{
					Cir2[j].coin_vely =0.0f;
				}

}
// updated evry 1 millisecond
void update(int value) {
	
	collisions();// Handling Collisions 	

	glutTimerFunc(10, update, 0);
}

// Initializing some openGL 3D rendering options
void initRendering() {

	glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
	glEnable(GL_COLOR_MATERIAL);    // Enable coloring
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

	if (key == 27) {
		exit(0);     // escape key is pressed
	}
}
