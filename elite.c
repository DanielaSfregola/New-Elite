/*
*	ELITE
*	Author: Daniela Sfregola
*	
*/

#include "lib/globalIncludeVar.h"	// inclusione liberie di supportoe e varabili utilizzate in tutti gli altri file
#include "lib/textureOBJ.c" 		// caricamento texture e OBJ
#include "lib/explosion.c"		// gestione esplosioni
#include "lib/txtelite.c"		// gestione textelite
#include "lib/shooting.c"		// gestione spari
#include "lib/collision.c" 		// gestione collisioni 
#include "lib/cockpit.c"		// gestione cockpit
#include "lib/enemy_police.c"		// gestione navette nemici/polizia
#include "lib/solarSystem.c"		// gestione degli oggetti nel sistema solare

#include "lib/pause.c"			// gestione menu pause
#include "lib/gameover.c"		// gestione schermata gameover

/** FUNCTION PROTOTYPE **/
void initgfx( void );
int init(void);
void openGLinitializations(void);
// gestione delle periferiche di input (mouse e tastiera)
void keyboard( GLubyte, GLint, GLint );
void specialkeys( GLint, GLint, GLint );
void mouse(GLint button, GLint state, GLint x, GLint y);
void motion(GLint x, GLint y);
//
void drawScene(void);
void checkError( char * );
void printHelp( char * );
void reshape ( GLsizei, GLsizei );
void animate(void);
void visibility( GLint );
void gameOver();
// blocca la scena
void freezeAll(GLboolean b);

// FUNZIONI ESTERNE
// file:	lib/solarSystem.c
void solarInit();	// inizializzazione delle luci e delle texture del sistema solare 
void solarScene();	// rendering del sistema solare
// file:	lib/cockpit.c
void drawCockpit();	// rendering dell'interfaccia
// file:	lib/textureOBJ.c
void loadTexture();	// caricamento delle texture
void loadOBJ();		// caricamento dei modelli OBJ

// DEFINIZIONI VARIABILI GLOBALI INTERNE A QUESTO FILE
bool fullscreen;
char *first_param;
char size[16];
int windowID;

int main( int argc, char *argv[] ){
	glutInit( &argc, argv);
	// per settare se all'inizio si vuole la modalita' a schermo intero o no	
	fullscreen=true;
	not_pressF1=true;
	first_param=argv[0];
	// per settare il numero di missili corretto nel text elite
	addMissiles();
	addMissiles();
	addMissiles();

	initPlanet();		// inizializzazione posizione pianeti e loro raggi
	initSatellite();	// inizializzazione dei satelliti
	initEnemy();		// inizializzazione dei nemici
	initPolice();		// inizializzazione della polizia
	initPartSys();		// inizializzazione sistemi di particelle per le collisioni
	//HENRY
	for(int i=0; i<MAX_LASER; i++) laser[i].draw=false;	// si inizializza a 0 il disegno dei laser
	for(int i=0; i<nSuperShoot;i++) superShoot[i]=false;
	return init();
}
// FUNZIONE DI INIZIALIZZAZIONE DELLE LIBRERIE DI openGL E DELLE GLUT
int init(void){

	// add the double buffering (to animations) and the z-zuffer
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	//save the screen's current size 
	width = glutGet( GLUT_SCREEN_WIDTH ); 
	height = glutGet( GLUT_SCREEN_HEIGHT );
	//set sizes and posizion of window    
	glutInitWindowPosition( width / 2, height / 2 );
	glutInitWindowSize(width, height);
    	sprintf(size,"%dx%d",width, height);
	
	if (not_pressF1)
		windowID=glutCreateWindow(first_param);
	else{
		glutDestroyWindow(windowID);
		windowID=glutCreateWindow(first_param);
	}
	if(fullscreen && glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
		glutGameModeString(size);
		glutEnterGameMode();
	}
	//necessary callbacks as well as perform the openGL required initializations	
	openGLinitializations();
	glutMainLoop();
    	return(0);
}
// necessary callbacks as well as perform the openGL required initializations
void openGLinitializations(void){
	initgfx();
	glutDisplayFunc(drawScene); 
	// aggiornameto di width e height per il corretto aspect ratio
    	glutReshapeFunc(reshape);
    	// funzione per animare pianeta e satellite
    	glutIdleFunc(animate); 
    	// funzione per gestire la visibilita' della finestra
    	glutVisibilityFunc(visibility);   
}

/* ----------------------------------------------------------*/
void initgfx( void ){
	if(not_pressF1) printf("\n________________________________\n\n	LOADING \n________________________________\n\n");
	loadTexture();	// caricamento texture	
	if(not_pressF1){ 
		loadOBJ();	// caricamento modelli OBJ		
		//printHelp(first_param); // stampa sul terminale i tasti da usare
	}
	// setta la telecamera
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// senza il fattore corretto gli angoli non si mantengono
	gluPerspective( 45.0, (GLdouble) width / (GLdouble) height, 0.1, 1e3 );
}

/* ----------------------------------------------------------*/
void drawScene( void ){
	switch(scene){
		case TEXT:
			// funzioni di textelite
			glutKeyboardFunc(textkeyboard);
			glutSpecialFunc(textspecialkeys);
			textScene(checkDistPlanet(g_camera.getPos()));
		break;
		case SOLAR:
			glutKeyboardFunc(keyboard);
			glutSpecialFunc(specialkeys);
			
			glutMouseFunc(mouse);
			// 	glutMotionFunc(motion); (funziona solo quando è premuto un tasto del mouse)
			glutPassiveMotionFunc(motion);		//funziona anche quando non sono premuti tasti
			glutSetCursor(GLUT_CURSOR_NONE);	// per non mostrare il puntatore del mouse
			
			solarInit(); 	// inizializzazione delle luci del sistema solare
			solarScene();	// rendering del sistema solare
			drawCockpit();	// rendering del cockpit
			gameOver();	// per gestire le collisioni
		break;
		case PAUSE:
			glutKeyboardFunc(pausekeyboard);
			glutSpecialFunc(pausespecialkeys);
			pauseScene();
		break;
		case GAMEOVER:
			gameover();
			glutKeyboardFunc(gameoverkeyboard);
		break;
	}
	checkError("drawScene");
}

/* ----------------------------------------------------------*/
void visibility( int state ){
// restart the animation function if we were animated when the window was hidden
if (state == GLUT_VISIBLE)
	glutIdleFunc(animate);
else
	glutIdleFunc(NULL);
}


/* ----------------------------------------------------------*/
void reshape( GLsizei w, GLsizei h ){
	// aggiorniamo dimensioni con i valori correnti
	width = w;
	height = h;
	// aggiornamento settaggi relativi alla visione in soggettiva del gioco
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective( 45.0, (GLdouble) width / (GLdouble) (height-ctrl_panel_height), 0.1, 1e3 );
	gluPerspective( 45.0, (GLdouble) width / (GLdouble) height, 0.1, 1e3 );
}


/* ----------------------------------------------------------*/
void animate( void ){ 

	static long currentTime=0, previousTime=0;
 	static long nearTime=0;
  
	/* restituisce il tempo in ms da glutInit o dall'ultima chiamata */
  	currentTime = glutGet(GLUT_ELAPSED_TIME);
  	
	dtime = currentTime - previousTime;
  	/*//questo serve per calcolare il fps ogni secondo (circa) 
  	if (dtime > 1000){
		fprintf(stdout, "\r%d fps", (int)(1e3 * (double) g_frames/(double) dtime) );
		fflush(stdout);
		previousTime = currentTime;
  	}*/

 
  	/* muoviamo la nostra navetta spaziale (ossia la telecamera) in base
   		al tempo trascorso dall'ultimo rendering */
  	dtime = currentTime - nearTime;
  	nearTime = currentTime;
  	g_camera.advance(dtime);
	// aggiorniamo il centro della navetta
	centerCamera=pointDir(g_camera.getPos(), g_camera.getView(), -radiusCamera);
	//centerCamera=g_camera.getPos();
	glutPostRedisplay();
}

/* ----------------------------------------------------------*/
void keyboard(GLubyte key, GLint x, GLint y){   

	switch (key) {
	case 'w':
	case 'W': 
		if(abs((int)(fuel*100))>0)
	        	g_camera.accelerate(CAM_SPEED_STEP);
	        glutPostRedisplay();
        break;

	case 's':
	case 'S': 
	        g_camera.accelerate(-CAM_SPEED_STEP);
	        glutPostRedisplay();
        break;
	// turbo
	case 'q':
	case 'Q':
		if(abs((int)(fuel*100))>0)
			g_camera.accelerate(CAM_SPEED_STEP*10);	
		glutPostRedisplay();
	break;
	// freno d'emergenza
	case 'e':
	case 'E':
		g_camera.accelerate(-CAM_SPEED_STEP*10);	
		glutPostRedisplay();
	break;
    	case 'a':
	case 'A': 
	        g_camera.roll(-CAM_ROT_STEP);
        	glutPostRedisplay(); 
        break;

	case 'd': 
	case 'D':
	        g_camera.roll(CAM_ROT_STEP);
	        glutPostRedisplay();
        break;
	
	case 'f':
	case 'F':
		if (nSuperShoot>0){	
			initSuperShoot();
		}	
	break;
	
	case 't':
	case 'T':
		//printf("controllo %d 	",checkDistPlanet(g_camera.getPos()));
		if(checkDistPlanet(g_camera.getPos())>=0){
			// azzero la velocita' anche dopo l'uscita del txtelite
			g_camera.accelerate(-1);			
			glutPostRedisplay();
			scene=TEXT;
			freezeAll(true);
		}
	break;
	
	case '+':
		if(precMouse<100)	precMouse++;
	break;
	case '-':
		if(precMouse>1) 	precMouse--;
	break;
	case ' ':
		newLaser();
	break;
	case 'p':
	case 'P':
	case KEY_ESC:	// exit when the Escape key is pressed
        	glutPostRedisplay();
		scene=PAUSE;
		freezeAll(true);
	break;

	/*
	case 'o':
	case 'O':
		freezeAll(true);
	break;
	case 'i':
	case 'I':
		freezeAll(false);
	break;
	case 'l':
	case 'L':
		for(int i=0;i<NUM_ENEMY;i++) newParticles(&explosionEnemy[i],enemy[i].pos);
		for(int i=0;i<NUM_POLICE;i++) newParticles(&explosionPolice[i],police[i].pos);
	break;
	*/	
	}
}
/* ----------------------------------------------------------*/
void specialkeys( GLint key, GLint u, GLint v ){   
	
	switch (key) {
	
	case GLUT_KEY_UP: 
        	g_camera.pitch(CAM_ROT_STEP);
        	glutPostRedisplay(); 
        break;

	case GLUT_KEY_DOWN:
	        g_camera.pitch(-CAM_ROT_STEP);
	        glutPostRedisplay();
        break;
	// HENRY modifica all'argomento di yaw
	case GLUT_KEY_LEFT: 
		//printf("x=%f y=%f z=%f\t",g_camera.getView().x,g_camera.getView().y,g_camera.getView().z);fflush(stdout);
        	g_camera.yaw(CAM_ROT_STEP);
        	glutPostRedisplay(); 
        break;
	case GLUT_KEY_RIGHT:
		//printf("x=%f y=%f z=%f\t",g_camera.getView().x,g_camera.getView().y,g_camera.getView().z);fflush(stdout);
	        g_camera.yaw(-CAM_ROT_STEP);
	        glutPostRedisplay();
        break;

	// per attivare la modalita' GAME
	case GLUT_KEY_F1:  
		fullscreen = !fullscreen;
		not_pressF1=false;
      		if (fullscreen && glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)){
			glutGameModeString(size);	// define resolution
			glutEnterGameMode();		// enter full screen
			init();
			//openGLinitializations();
		}
      		else{
			glutLeaveGameMode();			
			init();			
      		}
	break;
	case GLUT_KEY_F2:		
		sight=(sight+1)%3;
	break;	
	// trucco per avere gratis un super-sparo :-)
	case GLUT_KEY_F12:
		if (nSuperShoot<3){
			nSuperShoot++;	
			addMissiles();
		}	
	break;		
	}
}

// compatibility with original GLUT
#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif

// Funzione che gestice la pressione dei tasti del mouse
GLvoid mouse(GLint button, GLint state, GLint x, GLint y){	
	switch(button){
		case GLUT_LEFT_BUTTON:
			newLaser();
		break;
		// per diminuire la precisione del puntatore, ma aumentarne la velocita'
		case GLUT_WHEEL_UP:
			//printf("Wheel Up\n"); fflush(stdout);
			if(precMouse<100)	precMouse++;
		break;
		// per aumentare la precisione del puntatore, ma diminuirne la velocita'
		case GLUT_WHEEL_DOWN:
			//printf("Wheel Down\n"); fflush(stdout);
			if(precMouse>1) 	precMouse--;
		break;
		case GLUT_RIGHT_BUTTON:
			if (nSuperShoot>0)	initSuperShoot();
		break;
	}


}

// Funzione che gestisce il movimento del mouse
GLvoid motion(GLint x, GLint y){
	float i=0.01;
	if(abs(x-xRot)>0){	
		// HENRY modifica all'argomento di yaw	
		if(x>xRot)	g_camera.yaw(-CAM_ROT_STEP*precMouse*i);
		else		g_camera.yaw(CAM_ROT_STEP*precMouse*i);
		// quando il puntatore del mouse va nei pixel sul bordo viene riportato al centro dello schermo
		if(x<=10 || x>=width-10)	glutWarpPointer((int)width/2,(int)height/2);		
		xRot=x;
	}
	if(abs(y-yRot)>0){
		if(y>yRot)	g_camera.pitch(-CAM_ROT_STEP*precMouse*i);
		else		g_camera.pitch(CAM_ROT_STEP*precMouse*i);
		// quando il puntatore del mouse va nei pixel sul bordo viene riportato al centro dello schermo
		if(y<=10 || y>=height-10)	glutWarpPointer((int)width/2,(int)height/2);
		yRot=y;
	}
	//printf("x=%d y=%d\t\t", x,y); fflush(stdout);
	glutPostRedisplay();	
}
/* ----------------------------------------------------------*/
void checkError( char *label )
{
	GLenum error;
	while ( (error = glGetError()) != GL_NO_ERROR )
		printf( "%s: %s\n", label, gluErrorString(error) );
}

// funzione che blocca completamente la scena in modalita' SOLAR
void freezeAll(GLboolean b){

	// nemici e polizia
	freezeSpaceShip(b);	

	// pianeti e satelliti
	freezeSolarSystem(b);

	// spari
	freezeShooting(b);
	
	//esplosioni
	freezeExplosion(b);

	// congelo la velocita' 
	freezeVelocity(b);		
}


// funzione che controlla eventuali collisioni con pianeti (da fare con asteroidi, e satelliti)
void gameOver()
{			
	GLfloat speed = (g_camera.getSpeed())/(g_camera.MAXSPEED);
	// GAME OVER se: (collisione con pianeta o satellire, con police o enemy) OR (fuel finito e distante da qualsiasi pianeta e speed a zero)
	if(  (checkCollision() || checkCollisionPolice()!=-1 || checkCollisionEnemy()!=-1)   ||   
			( fuel<=0.1 && checkDistPlanet(g_camera.getPos())==-1 && speed==0 ) || energy<=0 ){
	
		scene=GAMEOVER;		
	}
	
}

/* ----------------------------------------------------------*/
void printHelp( char *progname )
{
	printf("\n________________________________\n\n	CONTROLS \n________________________________\n\n");
	printf("1. MOVEMENT\n");	
	fprintf(stdout,
		"\tW key\t\t		- speed up\n"
		"\tS key\t\t		- slow down\n"
		"\tQ key\t\t		- extra speed up\n"
		"\tE key\t\t		- extra slow down\n"
		"\tA/D key\t\t		- ROLL\n"
		"\tUP/DOWN arrow\n"
		"\t\"     \" MOTION MOUSE\t	- PITCH\n"
		"\tLEFT/RIGHT arrow\n"
		"\t\"        \" MOTION MOUSE\t	- YAW\n"
		"\tWHEEL UP\n"
		"\t+ key\t\t		- more velocity motion\n"
		"\tWHEEL DOWN\n"
		"\t- key\t\t		- less velocity motion\n" );
	printf("2. FIRE\n");
	fprintf(stdout,	
		"\tSPACE key\n"
		"\tMOUSE LEFT BUTTON\t	- fire\n"
		"\tF key\n"
		"\tMOUSE RIGHT BUTTON\t	- extra fire\n"
		"\tWHEEL UP\n"
		"\t+ key\t\t		- less precision\n"
		"\tWHEEL DOWN\n"
		"\t- key\t\t		- more precision\n" );
	printf("3. OTHER\n");
	fprintf(stdout,	
		"\tT key\t\t		- enter/exit textelite\n"	
		"\tESC Key\t\t		- exit the program\n");
}

