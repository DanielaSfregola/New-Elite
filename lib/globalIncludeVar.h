#ifndef _GLOBAL_H
#define _GLOBAL_H


/** INCLUSIONE DELLE LIBRERIE **/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
// piccola libreria con alcune definizioni di tipo e utils
#include "lib/eliteutils.h"
// libreria gestione telecamera in C++
#include "lib/camera.h"
#include "lib/camera.cpp"
// libreria per caricare le texture
#include "lib/sgi.h"
#include "lib/sgi.c"
// funzione per caricare i file obj
#include "lib/glm.c"
#include <time.h> // libreria per gestire il tempo



/** DEFINIZIONI COSTANTI **/
#define KEY_ESC		27		// ascii value for the escape key
#define KEY_ENTER	13		// ascii value for the enter key
#define CAM_SPEED_STEP	0.00005f 	// step di incremento velocita'  
#define CAM_ROT_STEP	1.0f    	// step di incremento angolo
#define CAM_POS_STEP	0.005f   	// step di incremento posizione
#define MAX_DIST	500		// distanza massima dal centro della scena
#define MAX_RADIUS	80		// raggio massimo dei pianeti
#define NUM_SAT		3		// numero satelliti
#define MAX_LASER	120		// numero massimo laser
#define NUM_ENEMY 	10		// numero totale nemici
#define NUM_POLICE	9		// numero totale navicelle polizia
#define DIST_RADAR	300		// HENRY la distanza massima tra noi e i nemici che permette di visualizzare la posizione sul radar
#define MAX_SHOOT	3		// numero massimo di super-spari



/** 
*			DEFINIZIONI VARIABILI GLOBALI 
**/

/* VARIABILI RELATIVE ALLA VISUALIZZAZIONE */
Camera g_camera(-1,-1,-5);
//Camera g_camera(0,0, 210);
GLfloat radiusCamera=1;
Point3 centerCamera=pointDir(g_camera.getPos(), g_camera.getView(), -radiusCamera);

GLsizei width, height;
int pixelDepth, refreshRate;
long g_frames, dtime = 0;
bool not_pressF1=true;		// gestine della modalita' schermo intero
// variabili che gestiscono le varie visualizzazioni
enum sceneEnum {TEXT, SOLAR, PAUSE, GAMEOVER};
static GLint scene=SOLAR;

enum sightEnum {GREENSIGHT, REDSIGHT, NO};
static GLint sight=GREENSIGHT;

/* VARIABILI DI GIOCO */
GLfloat fuel=1; 	// carburante:	varia tra 0 e 1
GLfloat energy=100;	// energia:	varia tra 0 e 100
int nSuperShoot=3;	// numero missili 	max 3
// se si trova il tesoro diventa true
GLboolean treasure=false;
// Variabili per il movimento del mouse
int xRot = 0;
int yRot = 0;
GLfloat precMouse=100;	// precisione mouse:	1-->prec max, movimenti lenti	100-->prec min, movimenti veloci

/* TEXTURE */
#define TOT_TEXTURE			21	// N.B. DA AGGIORNARE OGNI VOLTA CHE SI AGGINUGE UNA NUOVA TEXTURE
// variabile che raccoglie tutte le immagini sgi caricate
unsigned char **img= (unsigned char**) malloc(sizeof(unsigned char*)*TOT_TEXTURE);
// array contenenente gli ID delle texture caricate e dello sfondo
GLuint g_textureSfondo[1];		// texture sfondo
GLuint g_texturePlanet[9];		// texture pianeti
GLuint g_textureRing[1];		// texture anelli
GLuint g_textureSatellite[NUM_SAT];	// texture satelliti	
GLuint g_textureArray[7];		// array per tutte le altre texture
/* g_textureArray:
* 	pos 0		cockpit
* 	pos 1		txtelite
*	pos 2		scudo
*	pos 3		game over
* 	pos 4		particle
*	pos 5		superShoot
*	pos 6		sight
*
*/
/* OBJ */
GLMmodel* enemyModel;
GLMmodel* policeModel;

/* VARIABILI CHE DESCRIVONO GLI OGGETTI DEL SISTEMA SOLARE */
// struttura che contiene le informazioni su ogni navicella
typedef struct SpaceShip{
	Point3		pos;		// posizione
	Point3		dir;		// direzione verso cui punta
	GLboolean	dirCorr;	// TRUE solo se la navetta e' orientata verso la telecamera
	GLboolean 	move;		// TRUE solo se la navetta si sta muovendo
	GLboolean	drawExp;	// FALSE solo se e' morto ed e' gia' stata inizializzata l'esplosione
	GLint		life;		// quanta vita gli e' rimasta (a seconda del numero di volte che e' stata colpita)
	GLfloat 	rad;		// raggio della sfera occupata dalla navicella	
	char		type;		// tipo di navetta (enemy: e, police: p)
	int		index;		// indice del nemico	
} SpaceShip;
// struttura che contiene le informazioni su ogni satellite
typedef struct Satellite{
	GLfloat	rad;		// raggio del satellite;	
	Point3 	pos;		// posizione nello spazio
	Point3 	posRel;		// posizione del satellite rispetto al pianeta attorno a cui gira
	GLint	nPlanet;	// numero del pianeta attorno a cui gira
	GLfloat distPlanet;	// distanza dal pianeta
} Satellite;
// struttura laser
enum typeLaser {ENEMY,POLICE,ME};

typedef struct Laser{
	Point3 start;		// dove inizia il segmento
	Point3 origin;		// da dove inizia il laser
	Point3 end;		// dove finisce il segmento
	Point3 dir;		// direzione
	GLfloat color[3];	// colore
	GLfloat size;		// lunghezza laser
	GLboolean draw;		// se TRUE deve essere disegnato
	GLint type;


} Laser;

SpaceShip enemy[NUM_ENEMY];	// navette nemiche
SpaceShip police[NUM_POLICE];	// navette della polizia
Satellite satellite[NUM_SAT];	// satelliti
Laser laser[MAX_LASER];		// numero di oggetti laser
int nLaser=0;			// contatore al numero dei laser
// posizione dei nove pianeti del sistema solare (x,y,z)
Point3	posPlanet[9];
GLfloat radius[9];	//raggio dei pianeti
int 	hitEnemy, hitPolice =-1;
GLfloat		radShootEnemy=2.5,
		radShootPolice=2.5;	

/* POSIZIONI SORGENTI LUMINOSE (x,y,z,w) */
// Posizione della luce che simula il sole
GLfloat g_light0_pos[] = { 0.0, 0.0, 0.0, 1.0};
// Direzioniamo le luci per NPR
GLfloat g_light1_pos[] = { 1.0, 0.1, 0.0, 0.0}; // come se provenisse dal sole
GLfloat g_light2_pos[] = {-1.0,-0.1, 0.0, 0.0};
// la direzione 1 e' una riscalatura delle coordinate del sole in modo che la luce provenga proprio dal sole
// la 2 ha direzione inversa della 1 


char temp[1024];	//  array di caratteri per molteplici usi
#endif
