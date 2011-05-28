// caricamento della texture da porre sul pianeta
void initTexture(GLuint textureArray[], char *imageFileName, int textureID);
// procedura per la creazione dei satelliti dei pianeti
void drawSatellite(Satellite *sat, char rotation_axis, float speed, GLuint texture);
// procedura per la creazione dei pianeti
void drawPlanet(Point3 posPlanet, GLfloat radius, GLuint texture);
// procedura per disegnare l'anello attorno ai pianeti
void drawRing(Point3 posPlanet, double in_ring, double out_ring);
// scudi
GLvoid drawShield(Point3 pos, GLfloat radius);
// procedura che blocca i movimenti di tutti i pianeti
void freezeSolarSystem(GLboolean b);
// procedura che blocca la velocita'
void freezeVelocity(GLboolean b);

GLfloat anglePlanet, angleSatellite;//=(glutGet(GLUT_ELAPSED_TIME))*0.0012;
GLfloat rotSat;
GLboolean 	freezeSystem=false;
GLfloat 	oldV = 0.0f;
void freezeSolarSystem(GLboolean b){
	freezeSystem=b;
}
// INIZIALIZZAZIONE DELLE LUCI E DELLE TEXTURE DEL SISTEMA SOLARE
void solarInit(void){   
	// impostazioni generali della luce
	{
		GLfloat ambient_light[]={ 0.0, 0.0, 0.0, 1.0};
    		GLfloat infinite[] = {0.0};
		GLfloat one_side[] = {0.0};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_light); 
		/* poniamo a zero per non intaccare NPR, sara' LIGHT0 ad avere una componente ambiente */
		glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, infinite); 
		/* nella galassia possiamo supporre distanze elevate alleggerendo cosi' la computazione */
		glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, one_side); /* non ha senso illuminare l'interno degli oggetti */
	}

  	/* impostazione luce principale (sole) */
  	{
    		GLfloat ambient_light[]={ 0.15, 0.15, 0.15, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light); 
		/* LIGHT0 dovra' essere disabilitata durante il NPR per usare solo la luce controllata */
	}
  
	/* Seguono impostazioni delle luci controllate per il NPR (v. note)
	Supponendo che coriolis sia grigio kd=(0.5 0.5 0.5) e prendendo
     yellow = 0.5, blue = 0.5, alfa = 0.5 e beta = 0.5 si ha
	 
     Kwarm = [0.5, 0.5, 0.0] + 0.5 * [0.5, 0.5, 0.5] = [0.75, 0.75, 0.25]
     Kcool = [0.0, 0.0, 0.5] + 0.5 * [0.5, 0.5, 0.5] = [0.25, 0.25, 0.75] 
     Kwarm - Kcool = [0.5  0.5 -0.5] */      
	{
		GLfloat light_diffuse1[]={ 0.25,  0.25, -0.25, 1.0}; /* (Kwarm - Kcool)/2 */ 
		GLfloat light_diffuse2[]={-0.25, -0.25,  0.25, 1.0}; /* (Kcool - Kwarm)/2 */
		GLfloat light_ambient[] ={ 0.25,  0.25,  0.25, 1.0}; /* (Kcool + Kwarm)/4 anziche' 
      2 perche' ci sono due luci che contribuisco alla luce ambientale */  
    /* Impostazione della componente diffusa delle luci 1 e 2 */
		glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
		glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
    /* Luci che contribuiscono all'ambiente per PNR */  
		glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
 	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
	    /* NB. GL_SPECULAR delle luci diverse da LIGHT0 e' per default [0.0, 0.0, 0.0] */
	}


  /* -- abilita funzionalita' di OpenGL */  
	glShadeModel(GL_SMOOTH); /* enable smooth shading */
	glEnable(GL_DEPTH_TEST); /* enable z buffer */
	glEnable(GL_CULL_FACE); /* enable cull face */
	glCullFace(GL_BACK);

  /* abilitiamo l'utilizzo della luce */
	glEnable(GL_LIGHTING); /* enable lighting */
	glEnable(GL_LIGHT0);  /* accende luce 0 */
 	/* -- set clear color to black */
  	
	glClearColor( 0.0, 0.0, 0.0, 1.0 );
}

// funzione che inizializza la pos dei pianeti
void initPlanet(){
// POSIZIONI PIANETI
	//mercurio --> (-150, -150, 150)
	// posizione nello spazio

	posPlanet[0].x=MAX_DIST-650;
	posPlanet[0].y=MAX_DIST-650;
	posPlanet[0].z=MAX_DIST-350;
	// raggio pianeta
	radius[0]=MAX_RADIUS-60;
	
	//venere --> (200, 0, -200)
	// posizione nello spazio

	posPlanet[1].x=MAX_DIST-300;
	posPlanet[1].y=MAX_DIST-MAX_DIST;
	posPlanet[1].z=MAX_DIST-700;
	// raggio pianeta
	radius[1]=MAX_RADIUS-50;

	//terra --> (250, 300, 300)
	// posizione nello spazio
	
	posPlanet[2].x=MAX_DIST-MAX_DIST;
	posPlanet[2].y=MAX_DIST-MAX_DIST;
	posPlanet[2].z=MAX_DIST-250;
	// raggio pianeta
	radius[2]=MAX_RADIUS-50;

	//marte --> (-250, -250, -250)
	// posizione nello spazio

	posPlanet[3].x=MAX_DIST-750;
	posPlanet[3].y=MAX_DIST-750;
	posPlanet[3].z=MAX_DIST-750;
	// raggio pianeta
	radius[3]=MAX_RADIUS-60;

	//giove --> (-500, 0, 0)
	// posizione nello spazio

	posPlanet[4].x=MAX_DIST-1000;
	posPlanet[4].y=MAX_DIST-MAX_DIST;
	posPlanet[4].z=MAX_DIST-MAX_DIST;
	// raggio pianeta
	radius[4]=MAX_RADIUS;

	//saturno --> (-200, 100, -350)
	// posizione nello spazio

	posPlanet[5].x=MAX_DIST-700;
	posPlanet[5].y=MAX_DIST-400;
	posPlanet[5].z=MAX_DIST-850;
	// raggio pianeta
	radius[5]=MAX_RADIUS-10;

	//urano --> (0, 0, 500)
	// posizione nello spazio
	posPlanet[6].x=MAX_DIST-250;
	posPlanet[6].y=MAX_DIST-200;
	posPlanet[6].z=MAX_DIST-200;

	// raggio pianeta
	radius[6]=MAX_RADIUS-40;

	//nettuno --> (500, 0, 0)
	// posizione nello spazio

	posPlanet[7].x=MAX_DIST;
	posPlanet[7].y=MAX_DIST-MAX_DIST;
	posPlanet[7].z=MAX_DIST-MAX_DIST;
	// raggio pianeta
	radius[7]=MAX_RADIUS-40;

	//plutone --> (0, 0, 500)
	// posizione nello spazio

	posPlanet[8].x=MAX_DIST-MAX_DIST;
	posPlanet[8].y=MAX_DIST-MAX_DIST;
	posPlanet[8].z=MAX_DIST-1000;
	// raggio pianeta
	radius[8]=MAX_RADIUS-70;
}
void initSatellite(){
	// moon (earth)
	satellite[0].rad=5;
	satellite[0].nPlanet=2;
	satellite[0].distPlanet=10;
	// io (jupiter)
	satellite[1].rad=5;
	satellite[1].nPlanet=4;
	satellite[1].distPlanet=7;
	// europe (jupiter)
	satellite[2].rad=5;
	satellite[2].nPlanet=4;
	satellite[2].distPlanet=15;	
}

// RENDERING DEL SISTEMA SOLARE
void solarScene(void){

	

	// ogni volta che disegno incremento il numero dei frame
	g_frames++;
    
	// Do all your OpenGL rendering here
	
 	
	// cancelliamo il  buffer video e lo z buffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* ------------------------------------------------------------------------- */
  	// attiva il viewport principale
  	glViewport( 0, 0, width, height); 
        
    	glMatrixMode(GL_MODELVIEW); 
    	glLoadIdentity();

    	glPushMatrix();

        g_camera.look();    // chiama la gluLookAt con i parametri opportuni
    
        // posizionando la luce dopo la telecamera la luce risulta fissa nel mondo 
        glLightfv(GL_LIGHT0, GL_POSITION, g_light0_pos);
        glLightfv(GL_LIGHT1, GL_POSITION, g_light1_pos);
        glLightfv(GL_LIGHT2, GL_POSITION, g_light2_pos);
	
	glDisable(GL_CULL_FACE);
	
	// texture dello sfondo
	{
		GLUquadric* a = gluNewQuadric();
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_TEXTURE_2D);
		Point3 poscam = g_camera.getPos();
		glPushMatrix();
			glTranslatef(poscam.x, poscam.y, poscam.z);
			gluQuadricOrientation(a, GLU_INSIDE);
			glBindTexture(GL_TEXTURE_2D, g_textureSfondo[0]);
			gluQuadricTexture(a, GL_TRUE);						
			gluSphere(a, 900, 24, 24);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glPopAttrib();
	}
	glEnable(GL_CULL_FACE);
	
	/* PIANETI */
	drawPlanet(posPlanet[0], radius[0], g_texturePlanet[0]);	//	mercury
	drawPlanet(posPlanet[1], radius[1], g_texturePlanet[1]);	//	venus
	drawPlanet(posPlanet[2], radius[2], g_texturePlanet[2]);	//	earth
	drawPlanet(posPlanet[3], radius[3], g_texturePlanet[3]);	//	mars
	drawPlanet(posPlanet[4], radius[4], g_texturePlanet[4]);	//	jupiter
	drawPlanet(posPlanet[5], radius[5], g_texturePlanet[5]);	//	saturn
	drawPlanet(posPlanet[6], radius[6], g_texturePlanet[6]);	//	uranus
	drawPlanet(posPlanet[7], radius[7], g_texturePlanet[7]);	//	neptune
	drawPlanet(posPlanet[8], radius[8], g_texturePlanet[8]);	//	pluto


	/* SATELLITI */
	drawSatellite(&satellite[0], 'y', 0.00012, g_textureSatellite[0]);	// moon
	//drawShield(satellite[0].pos, satellite[0].rad+1);
	drawSatellite(&satellite[1], 'z', 0.00024, g_textureSatellite[1]);	// io
	drawSatellite(&satellite[2], 'y', 0.00012, g_textureSatellite[2]);	// europa
	
	/* ANELLI DI SATURNO */
	drawRing(posPlanet[5], 85, 120);				//	anello saturno

	// HENRY 
	//	gestione dei nemici: posizione, movimenti, inseguimenti
	//	FILE:		enemy_police.c
	insertEnemy();
	insertPolice();
	
	//ALBERTO
	// 	disegno di tutte le collisioni se attivate
	//	FILE		explosion.c
	insertExplosion();
		
	//ALBERTO LASER
	//	disegno di tutti i laser e controllo collisioni
	//	FILE		shooting.c
	hitEnemy=laserHitEnemy();
	hitPolice=laserHitPolice();
	insertRemoveLaser();
	// se siamo stati colpiti dal laser nemico si decrementa la vita
	shooted();
	// inseriamo il super-sparo
	insertSuperShoot();
		
    glPopMatrix();
}

void drawSatellite(Satellite *sat, char rotation_axis, float speed, GLuint texture){
	GLUquadric* a = gluNewQuadric();
	int num=sat->nPlanet;
	float rotation_ray=radius[num]+sat->distPlanet;
	// salva attributi (colori, dimensioni linee, ecc...) 
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
			
			glTranslatef(posPlanet[num].x, posPlanet[num].y, posPlanet[num].z); 
			// mi posiziono al centro del pianeta attorno a cui giro
			if (!freezeSystem){
				rotSat += 25.0;
				switch(rotation_axis){
					case 'x':
						sat->posRel.x=0;
						sat->posRel.y=rotation_ray*sin(rotSat*speed);
						sat->posRel.z=rotation_ray*cos(rotSat*speed);	
					break;
					case 'y':	
						sat->posRel.x=-rotation_ray*sin(rotSat*speed);
						sat->posRel.y=0;
						sat->posRel.z=rotation_ray*cos(rotSat*speed);
					break;
					case 'z':
						sat->posRel.x=-rotation_ray*sin(rotSat*speed);
						sat->posRel.y=rotation_ray*cos(rotSat*speed);
						sat->posRel.z=0;
					break;
				}	
				
			}
			glTranslatef(sat->posRel.x,sat->posRel.y,sat->posRel.z);
			glRotatef( 90, 1.0, 0.0, 0.0 );		//rotazione del satellite su se stesso
			if(!freezeSystem)
				angleSatellite += 0.016;			
			glRotatef(angleSatellite, 0.0, 0.0, 1.0); 

			glBindTexture(GL_TEXTURE_2D, texture);
			gluQuadricTexture( a, GL_TRUE );
			gluSphere(a,sat->rad, 60, 60); 

		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	glPopAttrib();
	
	// posizione (assoluta) dei satelliti nello spazio
	sat->pos=plus(posPlanet[num],sat->posRel);
}

void drawPlanet(Point3 posPlanet, GLfloat radius, GLuint texture) {
	
	GLUquadric* a = gluNewQuadric();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(posPlanet.x, posPlanet.y, posPlanet.z);
		glRotatef(-90, 1.0, 0.0, 0.0 );
		if(!freezeSystem)
				anglePlanet += 0.016;
		glRotatef(anglePlanet, 0.0, 0.0, 1.0);
		glBindTexture(GL_TEXTURE_2D, texture);
		gluQuadricTexture(a, GL_TRUE);
		gluSphere(a, radius, 60, 60); 
        glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}

void drawRing(Point3 posPlanet, double in_ring, double out_ring) {
	
	GLUquadric* b = gluNewQuadric();
		
	/* salva attributi (colori, dimensioni linee, ecc...) */
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		
	glEnable(GL_TEXTURE_2D); /* NEW: enable 2D texture mapping */
		
	glPushMatrix();
						
	//glDisable(GL_BLEND);
	glTranslatef(posPlanet.x, posPlanet.y, posPlanet.z);
	glRotatef( 90, 1.0, 0.0, 0.0 );
		
	// Ora usiamo la texture... 
	glBindTexture(GL_TEXTURE_2D, g_textureRing[0]);
	gluQuadricTexture( b, GL_TRUE );

	gluDisk(b, in_ring, out_ring, 50, 50);
	
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glPopAttrib();
}

GLvoid drawShield(Point3 pos, GLfloat radius){
	
	GLfloat color[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLUquadric* a = gluNewQuadric();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);    
		glEnable(GL_TEXTURE_2D);   
		glPushMatrix();
			glTranslatef(pos.x,pos.y,pos.z);
			glMaterialfv(GL_FRONT, GL_EMISSION, color);
			//			
			gluQuadricOrientation(a, GLU_INSIDE);
			glBindTexture(GL_TEXTURE_2D, g_textureArray[2]);
			gluQuadricTexture( a, GL_TRUE );
			gluSphere(a, radius, 60, 60);
		glPopMatrix(); 
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);   
		glDisable(GL_BLEND);  
	glPopAttrib();
}


void freezeVelocity(GLboolean b){
	
	if(b){ 
		oldV = (g_camera.getSpeed())/(g_camera.MAXSPEED);	
		//printf(" velocità %f", oldV);		
		g_camera.accelerate(-1);	
	}else{ 
		//printf(" vecchia velocità %f", oldV);		
		g_camera.accelerate(+(oldV/100.0));
	}		
}


