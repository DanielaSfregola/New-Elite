#define MAX_PARTICLES		600

// Struttura per la singola particella
typedef struct Particle			
{
	GLboolean	active;		// Active (Yes/No)
	GLfloat		life;		// Vita -> luminosita'
	GLfloat		fade;		// Velocita' di schieramento 
	GLfloat 	color[3];	// colore particella
	Point3		pos;		// posizione particella
	Point3		dir;		// direzione particella
} Particle;				
		       
// Struttura che gestisce una "nuvola" di particelle
typedef struct Particles_system
{	
	GLboolean	active;				// Active (Yes/No)
	Point3		pos;				// posizione 	
	Particle 	particle[MAX_PARTICLES];		
} Particles_system;


Particles_system explosionEnemy[NUM_ENEMY];
Particles_system explosionPolice[NUM_POLICE];


GLboolean 	freezeExpl=false;

void freezeExplosion(GLboolean b){
	freezeExpl=b;	
}
void drawExplosion(Particles_system *partSys);
// inizializzazione sistemi di particelle per le collisioni
void initPartSys(){
	for(int i=0;i<NUM_ENEMY;i++)	explosionEnemy[i].active=false;
	for(int i=0;i<NUM_POLICE;i++)	explosionPolice[i].active=false;
}

void newParticles(Particles_system *partSys, Point3 position){
	partSys->active = true;
	partSys->pos	= position;
	Particle *part;
  	for(int i=0;i<MAX_PARTICLES;i++)
	{
		part = &(partSys->particle[i]);
		part->active=true;
		part->life=0.0f;				// Diamo vita alle particelle
		/*part->pos.x=0.0f;	 			// Riposizione la particella al centro 
		part->pos.y=0.0f;	       
		part->pos.z=0.0f;*/
		part->pos=position;
		// dir casuale
		part->dir.x=(GLfloat) ((rand()%50)-30.0f);	
		part->dir.y=(GLfloat) ((rand()%50)-30.0f);
		part->dir.z=(GLfloat) ((rand()%50)-30.0f);
		part->dir=Normalize(part->dir);
		  
		part->fade= (GLfloat) dtime/2000 + (rand()%100)/1000.0f;	
		part->color[0]=1;
		part->color[1]=1;
		part->color[2]=0;
		      
	}
  	
	glEnable(GL_TEXTURE_2D);
	  	glBindTexture(GL_TEXTURE_2D, g_textureArray[5]);	// carichiamo la texture		
	glDisable(GL_TEXTURE_2D);
}

void insertExplosion(){
	for(int i=0;i<NUM_ENEMY;i++){
		if(explosionEnemy[i].active)
		{
			drawExplosion(&explosionEnemy[i]);
		}
		// questo if e' per non fare un altro ciclo for
		// poiche' NUM_POLICE<NUM_ENEMY usiamo lo stesso ciclo e se l'indice 
		// e' utilizzabile anche per il vettore della polizia, lo usiamo		
		if(i<NUM_POLICE && explosionPolice[i].active)
		{
			drawExplosion(&explosionPolice[i]);
			
		}
	}
}


void drawExplosion(Particles_system *partSys){
	//glDisable(GL_LIGHTING);	
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_BLEND);				// abilita blending (superficie traslucida)
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// tipo di blending
		glDisable(GL_CULL_FACE);			// disiabilita il taglio delle superfici nascoste
		glEnable(GL_COLOR_MATERIAL);			// abilita il materiale
		glEnable(GL_TEXTURE_2D);			// attiviamo texture
		glColorMaterial(GL_FRONT, GL_EMISSION);	// tipo di materiale
			//glTranslatef(partSys->pos.x,partSys->pos.y,partSys->pos.z);	// ci spostiamo nella posizione della navicella
			GLfloat lifeRate, moveRate;
			Particle* part;
			GLfloat *x,*y,*z;
			GLboolean allOff=true;		
			for (int i=0;i<MAX_PARTICLES;i++)	
    			{
				part= &(partSys->particle[i]);
				if(part->active)
				{
					allOff=false;
					x=&(part->pos.x);
					y=&(part->pos.y);
					z=&(part->pos.z);
					// disegnamo la particella					
					glColor4f(part->color[0],part->color[1],part->color[2],part->life);
					glBindTexture(GL_TEXTURE_2D, g_textureArray[4]);
					
					/*
					glBegin(GL_TRIANGLE_STRIP);   	// Costruisco un quadrato da una striscia di triangoli
						glTexCoord3d(1,0,1); glVertex3f(*x+0.05f,*y-0.05f,*z); // inferiore destro
						glTexCoord3d(0,0,1); glVertex3f(*x-0.05f,*y-0.05f,*z); // inferiore sinistro
						glTexCoord3d(1,1,1); glVertex3f(*x+0.05f,*y+0.05f,*z); // superiore destro
						glTexCoord3d(0,1,1); glVertex3f(*x-0.05f,*y+0.05f,*z); // superiore sinistro
	  				glEnd();
					*/
					glBegin(GL_TRIANGLE_STRIP);   	// Costruisco un quadrato da una striscia di triangoli
						glTexCoord3d(1,0,1); glVertex3f(*x+0.1f,*y-0.1f,*z+0.1f); // inferiore destro
						glTexCoord3d(0,0,1); glVertex3f(*x-0.1f,*y-0.1f,*z-0.1f); // inferiore sinistro
						glTexCoord3d(1,1,1); glVertex3f(*x+0.1f,*y+0.1f,*z-0.1f); // superiore destro
						glTexCoord3d(0,1,1); glVertex3f(*x-0.1f,*y+0.1f,*z+0.1f); // superiore sinistro
	  				glEnd();
					if(!freezeExpl){					
						// muoviamo la particella
						lifeRate = (0.5*dtime)/40;
	    					moveRate = pow(dtime,2)/2500;
						
						*x +=part->dir.x*moveRate;	// muoviamo la particella su X utilizzando anche il framerate
						*y +=part->dir.y*moveRate;	// muoviamo la particella su Y utilizzando anche il framerate
						*z +=part->dir.z*moveRate;	// muoviamo la particella su X utilizzando anche il framerate
						part->fade=(float)dtime/10000000 + (rand()%100)/1000.0f;
						part->life-=part->fade;	    // Riduciamo la vita della particella
					}
					//printf("X: %f\tY: %f\tZ: %f, moverate: %f\n", *x,*y,*z, moveRate);fflush(stdout);
					//printf("\tX: %f\tY: %f\tZ: %f\n", part->pos.x,part->pos.y,part->pos.z);fflush(stdout);
					// se e' tanto distante dal punto di esplosione non la visualizziamo piu'			
					if(distance(partSys->pos,part->pos)>120){ 
							part->active=false;
					}
				}
			}
			// se nessuna particella e' attiva disattiviamo il sistema di particelle
			if(allOff) partSys->active=false;

		glDisable(GL_COLOR_MATERIAL);			// disabilita il materiale
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_CULL_FACE);				// abilita il taglio delle superfici nascoste
		glDisable(GL_BLEND);				// disabilita blending
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);
	glPopMatrix();
	glPopAttrib();
	//glEnable(GL_LIGHTING);	
}
