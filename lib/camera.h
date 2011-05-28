#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL/glut.h> 
#include "eliteutils.h"


/* ----------------------------------------------------------
Classe che implementa una telecamera in moviento.  Lo stato della
telecamera e' dato dala sua posizione dal versore up e dal versore
della direzione di vista. La classe consente di gestire il movimento
della tc aggiornando lo stato e chiamando la glLookAt con i parametri
opportuni.
------------------------------------------------------------- */


class Camera {
public:
	static const GLfloat MAXSPEED=0.01;
	// vecchia velocita'	
	// static const GLfloat MAXSPEED=0.001;
	// costruttore
	Camera(GLfloat x, GLfloat y, GLfloat z);

	/* cambia angolo di ROLL (rollio):
		inclinazione del muso verso l'alto o verso il basso
	*/
  	void roll(GLfloat angle);
 
	/* cambia angolo di YAW (imbardata):
		rotazione laterale dell'aereo
	*/
  	void yaw(GLfloat angle);	

  	/* cambia angolo di PITCH (beccheggio): 
		inclinazione laterale dell'aereo
	*/
  	void pitch(GLfloat angle);

 	// aggiorna velocita'
	void accelerate(GLfloat step);

	// avanza di uno step, nella direzione di view
  	void step(GLfloat s);

	// avanza in base alla velocita' (da chiamare in idle() ) 
	void advance(GLfloat t);
  
	// al posto della gluLookAt
  	void look();
	// restituisce la posizione
	Point3 getPos();

	// restituisce la direzione di vista
	Point3 getView();

	// restituisce la direzione a destra
	Point3 getRight();

	// restituisce la direzione alto
	Point3 getUp();

	// restituisce la velocita'
	GLfloat getSpeed();
	
	// restituisce il massimo della velocità a cui puo' arrivare
	GLfloat getMaxSpeed();
  
private:

  Point3 eye;
  Point3 view;
  Point3 up;
  float velocity;

};


#endif

