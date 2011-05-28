#include "camera.h"

/* definizioni delle funzioni della classe Camera */


/* costruttore */
Camera::Camera(GLfloat x, GLfloat y, GLfloat z) 
{
  Point3 v = {0,0,1};
  Point3 u = {0,1,0};

  /* il costruttore permette di settare la posizione della tc;
     direzione di vista e up sono fissati, in questa versione */    
 
  eye.x = x;
  eye.y = y; 
  eye.z = z;
  
  view = v; 
  up = u;
  velocity = 0.0;
	
}

/* cambia angolo di ROLL (rollio) */
void Camera::roll(GLfloat angle) 
{  
	up = Normalize(RotateVector(angle,view,up));

}


/* cambia angolo di PITCH (beccheggio) */
void Camera::pitch(GLfloat angle) 
{  	
	/* asse x della tc, quello a cui ruotare per cambiare pitch */
	Point3 xa = Cross(view,up);
	view = Normalize(RotateVector(angle,xa,view));
	up= Normalize(RotateVector(angle,xa,up));
	
}

/* cambia angolo di YAW (imbardata)*/
void Camera::yaw(GLfloat angle) 
{  	
	Point3 xa = Cross(view,up);
	 /* asse z della tc*/
  	Point3 za = Cross(xa,view);
  	view = Normalize(RotateVector(angle,za,view));
  	up= RotateVector(angle,za,up);
	

}

/* aggiorna velocita' */
void Camera::accelerate(GLfloat step) 
{
  velocity+=step;
  if (velocity < 0) velocity = 0.0;
  if (velocity > MAXSPEED) velocity = MAXSPEED;
}


/* avanza di uno step, nella direzione di view */
void Camera::step(GLfloat s) 
{  
  eye.x+=s*view.x;
  eye.y+=s*view.y;
  eye.z+=s*view.z;
}


/* aggiorna posizione in base alla velocita' (da chiamare in idle() ) */
void Camera::advance(GLfloat t) 
{
  eye.x+=t*velocity*view.x;
  eye.y+=t*velocity*view.y;
  eye.z+=t*velocity*view.z;
}
/* chiama la gluLookAt */
void Camera::look()
{
  /* assicurarsi che agisca sulla modelview *prima* 
     della trasformazione di modellazione */
  gluLookAt(eye.x, eye.y, eye.z, 
	    eye.x+view.x, eye.y+view.y, eye.z+view.z, 
	    up.x, up.y, up.z);
}


Point3 Camera::getPos() 
{
  return(eye);
}


Point3  Camera::getView() 
{
  return(view);
}


Point3  Camera::getRight() 
{
  return(Cross(view,up));
}


Point3  Camera::getUp() 
{
  return(up);
}


GLfloat Camera::getSpeed()  
{
  return(velocity);
}

GLfloat Camera::getMaxSpeed()
{
  return(MAXSPEED);
}
