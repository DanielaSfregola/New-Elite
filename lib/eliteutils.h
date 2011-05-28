#ifndef _ELITEUTILS_H
#define _ELITEUTILS_H

#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

#define  PI acos(-1.0)

typedef struct  MaterialStruct 
    {
      float emission[4];
      float ambient[4];
      float diffuse[4];
      float specular[4];
      float shininess[1];
} Material;

typedef struct Point2Struct {
  float x, y;
} Point2;

typedef struct Point3Struct {
  float x, y, z;
} Point3;


GLvoid drawEliteObject(int numberOfFaces, Point3 vertex[], long face[], Material material[]);

GLvoid drawEliteObject2(int numberOfFaces, Point3 vertex[], long face[], Material material[]);

GLvoid renderBitmapString(void *font, char *string);

// routa il vettore vView attorno all'asse axis dell'angolo angle in gradi
extern Point3 RotateVector(float  angle, Point3 axis, Point3 vView);

// calcola prodotto esterno
extern Point3 Cross(Point3 vVector1, Point3 vVector2);

// calcola la normale ad un triangolo di vertici v1,v2,v3
extern Point3 Calc_normal(Point3 v1, Point3 v2, Point3 v3) ;

/* Prodotto scalare tra due vettori */
extern float ProdScal(Point3 v1, Point3 v2);

extern Point3 Normalize(Point3 v);

Point3  Scalar(Point3 v, float s);

//void drawSight(float x, float y);



// FUNZIONI NOSTRE

// sottrae al vettore a il vettore b
Point3 minus(Point3 a, Point3 b);
// somma elemento per elemento 'b' ad 'a': 'a+b'
Point3 plus(Point3 a, Point3 b);
// restituisce il punto a distanza 'module' da 'start' nella direzione 'dir'
Point3 pointDir(Point3 start, Point3 dir, GLfloat module);
// somma 'num' al modulo di 'start', lasciandone inalterato il verso
Point3 addPos(Point3 start, GLfloat num);
// distanza tra due punti
float distance(Point3 me, Point3 planet);
// restituisce la direzione del vettore (cioe' il versore) che va da 'origin' a 'destination'
Point3 directionRel(Point3 origin, Point3 destination);

/*
	Funzione che dato un vettore, una sfera, e il suo raggio, restituisce TRUE sse il segmento inserseca la sfera.
	
	origin:		punto di applicazione del vettore
	destination:	coordinate del vettore
	posSph:		posizione della sfera
	radius:		raggio della sfera
	intersection:	se c'è intersezione restituisce il punto sul segmento [origin,destination] che e' piu' vicino a posSph

	return 		TRUE se il vettore interseca la sfera, FALSE altrimenti
*/
GLboolean RaySphereIntersection(Point3 origin, Point3 destination, Point3 posSph, GLfloat radius, Point3 *intersection);
/*
*	Se esiste (true) restituice:
*			il punto 'Intersection' che e' il punto piu' vicino a 'Point' sul segmento che va da 'LineStart' a 'LineEnd'
*			la distanza del punto dal segmento
*	Se non esiste (false) restituisce:
*			non modifica le variabili 'Insersection' e 'Distance'
*/
bool DistancePointSegment(Point3 *LineStart, Point3 *LineEnd, Point3 *Point, Point3 *Intersection, float *Distance);

// trova il punto piu' vicino a 'p' sulla retta che passa da 'a' e da 'b'
Point3 distancePointLine(Point3 a, Point3 b, Point3 p);
#endif
