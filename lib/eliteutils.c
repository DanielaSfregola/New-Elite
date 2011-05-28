/* Funzioni ausiliarie per i vettori */


#include "eliteutils.h"

/* Prodotto per uno scalare */
Point3  Scalar(Point3 v, float s)
{

  v.x = v.x*s;
  v.y = v.y*s;
  v.z = v.z*s;
  
  return v;
  
}


/* Funzione per normalizzare un vettore */
Point3  Normalize(Point3 v)
{
  float nor;

  nor = sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
  v.x = v.x/nor;
  v.y = v.y/nor;
  v.z = v.z/nor;
  
  return v;
  
}

/* Funzione per normalizzare un vettore */
void m_Normalize(Point3 *v)
{
  float nor;

  nor = sqrt(v->x*v->x + v->y*v->y + v->z*v->z);
  v->x = v->x/nor;
  v->y = v->y/nor;
  v->z = v->z/nor;
}

/* Prodotto scalare tra due vettori */
float ProdScal(Point3 v1, Point3 v2)
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

/* Prodotto vettore tra due vettori */
void Prod_Vett(Point3 *v1, Point3 *v2, Point3 *v)
{
  v->x = v1->y*v2->z - v2->y*v1->z;
  v->y = v1->z*v2->x - v2->z*v1->x;
  v->z = v1->x*v2->y - v2->x*v1->y;
}

/* Normale a due vettori */
void Normal(Point3 *v1, Point3 *v2, Point3 *v)
{
  Prod_Vett(v1, v2, v);
  m_Normalize(v);
}

/* Normale ad un triangolo */
Point3 Calc_normal(Point3 v1, Point3 v2, Point3 v3) 
{
  Point3 a, b, v;

  a.x = v2.x - v1.x;
  a.y = v2.y - v1.y;
  a.z = v2.z - v1.z;

  b.x = v3.x - v1.x;
  b.y = v3.y - v1.y;
  b.z = v3.z - v1.z;

  Normal(&a, &b, &v);

  return (v);
  
}

/* ruota un vettore attorno ad un asse */
Point3 RotateVector(float angle, Point3 axis, Point3 oVec)
{
  Point3 nVec;
	
  float x=axis.x;
  float y=axis.y;
  float z=axis.z;

  /* Calculate the sine and cosine of the angle once */
  float cosTheta = (float)cos(angle * PI/180.0);
  float sinTheta = (float)sin(angle * PI/180.0);
        
  /* Find the new x position for the new rotated point */
  nVec.x  = (cosTheta + (1 - cosTheta) * x * x)	* oVec.x;
  nVec.x += ((1 - cosTheta) * x * y - z * sinTheta)	* oVec.y;
  nVec.x += ((1 - cosTheta) * x * z + y * sinTheta)	* oVec.z;

  /* Find the new y position for the new rotated point */
  nVec.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* oVec.x;
  nVec.y += (cosTheta + (1 - cosTheta) * y * y)	* oVec.y;
  nVec.y += ((1 - cosTheta) * y * z - x * sinTheta)	* oVec.z;

  /* Find the new z position for the new rotated point */
  nVec.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* oVec.x;
  nVec.z += ((1 - cosTheta) * y * z + x * sinTheta)	* oVec.y;
  nVec.z += (cosTheta + (1 - cosTheta) * z * z)	* oVec.z;

  return nVec;
}

/* calcola prodotto esterno */
Point3 Cross(Point3 vVector1, Point3 vVector2)
{
  Point3 vNormal;	

  /* Calculate the cross product with the non communitive equation */
  vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
  vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
  vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

  return vNormal;	
}

// restituisce il punto a distanza 'module' da 'start' nella direzione 'dir'
Point3 pointDir(Point3 start, Point3 dir, GLfloat module){
	// per prima cosa controlliamo che dir sia un versore
	dir=Normalize(dir);
	Point3 end, tmp;
	tmp=Scalar(dir,module);
	return plus(start,tmp);
}
// somma al modulo di start num, lasciando inalterato il verso
Point3 addPos(Point3 start, GLfloat num){
	start.x>=0? (start.x += num) : (start.x -= num);
	start.y>=0? (start.y += num) : (start.y -= num);
	start.z>=0? (start.z += num) : (start.z -= num);
	return start;
}



//calcola la distanza tra 2 punti 
float distance(Point3 me, Point3 planet){

	//calcolo la norma euclidea di due punti
	return sqrt(pow(me.x-planet.x,2)+pow(me.y-planet.y,2)+pow(me.z-planet.z,2));
}


/**
* Moti relativi: posizione di DESTINATION rispetto a ORIGIN
* 	si applica la definizione di moti relativi
* 	alla fine si torna il vettore normalizato perche' esso altro non e' che il versore direzione
*/
Point3 directionRel(Point3 origin, Point3 destination){
	Point3 vector;
	vector.x=destination.x-origin.x;
	vector.y=destination.y-origin.y;
	vector.z=destination.z-origin.z;
	return Normalize(vector);
	//return vector;
}

Point3 minus(Point3 a, Point3 b){
	Point3 c;
	c.x=a.x-b.x;
	c.y=a.y-b.y;
	c.z=a.z-b.z;
	return c;
}

Point3 plus(Point3 a, Point3 b){
	Point3 c;
	c.x=a.x+b.x;
	c.y=a.y+b.y;
	c.z=a.z+b.z;
	return c;
}

float Magnitude(Point3 *Point1, Point3 *Point2 )
{
    Point3 Vector;

    Vector.x = Point2->x - Point1->x;
    Vector.y = Point2->y - Point1->y;
    Vector.z = Point2->z - Point1->z;

    return (float)sqrt( Vector.x * Vector.x + Vector.y * Vector.y + Vector.z * Vector.z );
}
/*
* Cerca il punto piu' vicino ad un segmento.
*
*	Se esiste (return true) restituice:
*			il punto 'Intersection' che e' il punto piu' vicino a 'Point' sul segmento che va da 'LineStart' a 'LineEnd'
*			la distanza del punto dal segmento
*	Se non esiste (return false) restituisce:
*			non modifica le variabili 'Insersection' e 'Distance'
*/
bool DistancePointSegment(Point3 *LineStart, Point3 *LineEnd, Point3 *Point, Point3 *Intersection, float *Distance )
{
    float LineMag;
    float U;
 
    LineMag = Magnitude( LineEnd, LineStart );
 
    U = ( ( ( Point->x - LineStart->x ) * ( LineEnd->x - LineStart->x ) ) +
        ( ( Point->y - LineStart->y ) * ( LineEnd->y - LineStart->y ) ) +
        ( ( Point->z - LineStart->z ) * ( LineEnd->z - LineStart->z ) ) ) /
        ( LineMag * LineMag );
 
    if( U < 0.0f || U > 1.0f )
        return false;   // closest point does not fall within the line segment
 
    Intersection->x = LineStart->x + U * ( LineEnd->x - LineStart->x );
    Intersection->y = LineStart->y + U * ( LineEnd->y - LineStart->y );
    Intersection->z = LineStart->z + U * ( LineEnd->z - LineStart->z );
 
    *Distance = Magnitude( Point, Intersection );
 
    return true;
}

/*
	Funzione che dato un vettore, una sfera, e il suo raggio, restituisce TRUE sse il segmento inserseca la sfera.
	
	origin:		punto di applicazione del vettore
	destination:	coordinate del vettore
	posSph:		posizione della sfera
	radius:		raggio della sfera
	intersection:	se c'è intersezione restituisce il punto sul segmento [origin,destination] che e' piu' vicino a posSph

	return 		TRUE se il vettore interseca la sfera, FALSE altrimenti
*/
GLboolean RaySphereIntersection(Point3 origin, Point3 destination, Point3 posSph, GLfloat radius, Point3 *intersection){
			float dist;
			if (DistancePointSegment(&origin, &destination, &posSph, intersection, &dist) )
				// controlliamo che la distanza tra il punto e la sfera sia minore del raggio
				if(dist<=radius) return true;
			return false;
}
/**
	calculates minimum distance vector between point p and straight line throught a and b
	a,b 	estremi del segmento
	p	punto rispetto al quale bisogna calcolare la minima distanza

*/
Point3 distancePointLine(Point3 a, Point3 b, Point3 p){
	double t0 = ProdScal(minus(p,a),minus(b,a)) / ProdScal(minus(b,a),minus(b,a));	
	Point3 d;
	d.x = ( a.x + (b.x-a.x)*t0 );
	d.y = ( a.y + (b.y-a.y)*t0 );
	d.z = ( a.z + (b.z-a.z)*t0 );
	return d;
}











/* ----------------------------------------------------------*/
GLvoid drawEliteObject(int  numberOfFaces, Point3 vertex[], long face[], Material material[])
{
  /* funzione che disegna un oggetto nel formato C salvato da Crossroads
     e modificato a mano (vedi p.es. coriolis.h).
     Bisogna aver incluso il file .h con la definizione  */

  /* In questa versione il materiale viene ignorato; servira' piu' avanti */

  int i =0;
 
  while (i < numberOfFaces)
  {
    /* Disegno il poligono */
    glBegin( GL_LINE_LOOP ); 
    while(face[i] > -1) {
      glVertex3f(vertex[face[i]].x/1000.0, 
		   vertex[face[i]].y/1000.0,
		   vertex[face[i]].z/1000.0);
	  i++;
    }
     
    glEnd(); 
    i++; 
  }
}

/* ----------------------------------------------------------*/
void drawEliteObject2(int  numberOfFaces, Point3 vertex[], 
		     long face[], Material material[])
{
  /* funzione che disegna un oggetto nel formato C salvato da Crossroads
     e modificato a mano (vedi p.es. coriolis.h).
     Bisogna aver incluso il file .h con la definizione  */

  /* NEW: salva attributi (colori, dimensioni linee, ecc...) */
  glPushAttrib(GL_ALL_ATTRIB_BITS);

  glEnable(GL_POLYGON_OFFSET_FILL); /* NEW: contro z-fighting */

  int j, i=0;
  Point3 normale;
 
  while (i < numberOfFaces)
  {    
      /* Qui i punta al primo vertice di una faccia */
  
      /* NEW: Il calcolo della normale si dovrebbe fare solo la prima volta */
      /* Per la normale uso i primi 3 vertici della faccia */
      normale = Calc_normal(vertex[face[i]],
			    vertex[face[i+1]],
			    vertex[face[i+2]]);
      
      /* Guardo avanti per recuperare l'indice del materiale */
      j = i;
      while(face[j]>-1)
        j++;
 
      /* NEW: Definisco le proprieta' del materiale per la faccia frontale dei poligoni */
      glMaterialfv(GL_FRONT, GL_EMISSION, material[-face[j]-1].emission);
      glMaterialfv(GL_FRONT, GL_DIFFUSE, material[-face[j]-1].diffuse);
      glMaterialfv(GL_FRONT, GL_AMBIENT, material[-face[j]-1].ambient);
      glMaterialfv(GL_FRONT, GL_SPECULAR, material[-face[j]-1].specular);
      glMaterialfv(GL_FRONT, GL_SHININESS, material[-face[j]-1].shininess);

      /* NEW: se il materiale non e' quello di default potrebbe trattarsi di
      un dettaglio superficiale (porte, motori, etc...) quindi
      attivo il PolygonOffset per evitare z-fighting */
      if (face[j]!=-1)
        glPolygonOffset(-1.0, -2.0); 
        /* offset = M * (-1) + R * (-2) 
           where M is the maximum depth slope of the polygon and 
           R is the smallest value guaranteed to produce a resolvable difference 
           in window coordinate depth values. 
           The value R is an implementation-specific constant.*/

      /* Finalmente disegno il poligono */
      glBegin( GL_POLYGON ); 
         while(face[i] > -1)
            {
              glNormal3f(normale.x, normale.y, normale.z);
              glVertex3f(vertex[face[i]].x/1000.0, 
                 vertex[face[i]].y/1000.0,
                 vertex[face[i]].z/1000.0);
              i++;
            }
      glEnd(); 
	  
      i++; 
  }

  glDisable(GL_POLYGON_OFFSET_FILL);

  glPopAttrib();
}

/* ----------------------------------------------------------*/
void renderBitmapString(void *font, char *string) 
{  
  char *c;
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

/* ----------------------------------------------------------
// disegna il mirino nella posizione specificata
void drawSight(float x, float y)
{
  glColor3f( 0, 0, 1);
  glLineWidth( 2 );
  glBegin( GL_LINES);
    glVertex3f(x-0.05, y+0.03, 0.99);
    glVertex3f(x-0.02, y+0.01, 0.99);
    glVertex3f(x+0.02, y-0.01, 0.99);
    glVertex3f(x+0.05, y-0.03, 0.99);
    glVertex3f(x+0.05, y+0.03, 0.99);
    glVertex3f(x+0.02, y+0.01, 0.99);
    glVertex3f(x-0.02, y-0.01, 0.99);
    glVertex3f(x-0.05, y-0.03, 0.99);  
  glEnd();
}*/
