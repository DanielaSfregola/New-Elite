/*
*	drawCockpit
*		cockpitImage
*		drawControlPanel
*			drawPoint
*			drawBar
*/

// pos telecamera
Point3	pos_present=g_camera.getPos(), 
	pos_past=g_camera.getPos();
// variabile che tiene conto del tempo
clock_t precClock, precClock1=clock(); 
char msgShoot[1024];

void cockpitImage();
void drawControlPanel();
void drawBar();
void drawPoint(GLfloat centerX, GLfloat centerY);
void drawSight(GLfloat centerX, GLfloat centerY);
void drawRectMissiles(int num);
//
void drawEllipse(GLfloat centerX, GLfloat centerY, GLfloat radiusX, GLfloat radiusY);



/*
* gluOrtho2D(  left, right, bottom, up ) - setta la matrice di proiezione ortografica 2D
* CONVENZIONE:
* 		lo schermo è suddiviso in un insieme infinito di valori nell'intervallo [0,1]
*		i 4 valori permettono di fissare la porzione dello schermo sulla quale si vuole lavorare
* 
*		up		(1,0)		(1,1)
*
*		bottom		(0,0)		(0,1)
*
*				left		right
* per settare tutto lo schermo:
*				gluOrtho2D(0, 1, 0, 1);
*		da 0 a sinistra a 1 a destra, da 0 in basso a 1 in alto
*/

// main: DRAW COCKPIT
void drawCockpit(){
	// disabilitiamo queste opzioni in modo da poter lavorare in 2D
	glDisable(GL_LIGHTING); 	// lighting
	glDisable(GL_DEPTH_TEST); 	// z buffer
	glDisable(GL_CULL_FACE); 	// face culling

		glPushAttrib(GL_ALL_ATTRIB_BITS);
			// attiva il viewport del pannello (a tutto schermo)
			glViewport(0, 0, width, height);
			// CHIAMATA ALLA FUNZIONE COCKPITIMAGE: inserimento dell'immagine del cockpit
			cockpitImage();
	    		// CHIAMATA A DRAWCONTROLPANEL: rendering delle info sullo schermo  
			drawControlPanel();
		glPopAttrib();
	
	// riabilitiamo quanto precedentemente disabilitato	
	glEnable(GL_LIGHTING);	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	// per non vedere il frame buffer mentre viene disegnato
	glutSwapBuffers();
}


// COCKPIT IMAGE: inserisce l'immagine del cockpit in primo piano
void cockpitImage(){
	GLfloat x1,x2,y1,y2;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1,0,1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();
			// BARRA DI PRECISIONE/VELOCITA' MOUSE
			x1=0.4738,
			x2=x1+0.01058;
			y1=0.048,
			y2=y1+0.2;			
			glColor3f(0,0,0);
			glRectf(x1,y1,x2,y2);	
			GLfloat	h_bar=y2-y1;
			glColor3f(0, 2-(5*0.2), 0.2*5);
			glLineWidth(1);			
			for(float i=0;i<(precMouse-1)*0.01;i+=0.03){
				glBegin(GL_LINES);
					glVertex2f(x1, y1+(h_bar-0.001)*i);
					glVertex2f(x2, y1+(h_bar-0.001)*i);
				glEnd();
			}
			
			glColor3f(1,1,1);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			// per attivare la trasparenza nelle regioni dell'immagine dove e' presente
			glBindTexture(GL_TEXTURE_2D,g_textureArray[0]);
			// per settare l'altezza dell'immagine del cockpit
			GLfloat imageHeight=0.42;
			// per settare la larghezza del cockpit (eliminare le parti nere ai lati)			
			x1=-0.034;
			x2=1.0369;
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);glVertex3f(x1,-0.002,0);
				glTexCoord2f(1,0);glVertex3f(x2,-0.002,0);
				glTexCoord2f(1,1);glVertex3f(x2,imageHeight,0);
				glTexCoord2f(0,1);glVertex3f(x1,imageHeight,0);
			glEnd();

			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


// DRAW CONTROL PANEL: disegna il pannello di controllo
void drawControlPanel(){
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0, 1, 0, 1);
		// resetta modelview 
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPushMatrix();		
			// MIRINO al centro dello schermo
			switch(sight){			
				case REDSIGHT:		drawPoint(0.5, 0.5); 	break;
				case GREENSIGHT:	drawSight(0.5,0.5);	break;
				case NO: 					break;
			}
			// rettangoli che indicano il numero di missili presenti			
			drawRectMissiles(nSuperShoot);
			// RADAR
			GLfloat centreX, centreY, radiusX, radiusY; // coordinate cerchio/ellisse
			centreX=0.3355;
			centreY=0.14;
			radiusX=0.1;
			radiusY=0.08;	
			// perimetro dell'ellisse
			glColor3f( 1, 1, 1);
			glLineWidth(0.02);
			glBegin(GL_LINES);
				for(int t=0; t<720; t++)
		      			glVertex3f(centreX+(radiusX*cos(t*(PI/180.0))), centreY+(radiusY*sin(t*(PI/180.0))),0.1);
		  	glEnd();
			//drawEllipse(centreX,centreY,radiusX,radiusY);
			// croce dentro il radar
			glColor3f( 1, 1, 1);
			glLineWidth(0.01);
			glBegin(GL_LINES); 
				//linea orizzontale
				glVertex3f(centreX-radiusX, centreY,0.1);
				glVertex3f(centreX+radiusX, centreY,0.1);
				//linea verticale
				glVertex3f(centreX, centreY-radiusY,0.1);
		 		glVertex3f(centreX, centreY+radiusY,0.1);
			glEnd();
			
			// puntino verde (fronte) o rosso (retro)
			/*
			* x,y 		coordinate che permettono di visualizzare la posizione dell'oggetto sul radar
			* z		coordinata che permette di capire se l'oggetto è davanti o dietro alla telecamera
			*
			*/
			float x,y,z; 
			Point3 u; 
			//visualizzazione dei 9 pianeti sul radar
			for(int i=0;i<9;i++){			
				u=g_camera.getPos();
				// prendiamo la posizione di ogni pianeta
				u.x -= posPlanet[i].x;
				u.y -= posPlanet[i].y;
				u.z -= posPlanet[i].z;
				u = Normalize(u);
				x = -ProdScal(u,g_camera.getRight());
				y = -ProdScal(u,g_camera.getUp());
				z = -ProdScal(u,g_camera.getView());
				//printf("x=%f\ty=%f\tz=%f\n",x,y,z); fflush(stdout);
				glPointSize(6);
				// HENRY: colore modificato 
				glBegin(GL_POINTS);  
					if (z>=0)	glColor3f(0, 1, 0);	// se e' davanti: verde 
					else		glColor3f(0, 0.4, 0); 	// se e' dietro: verde scuro
					glVertex2f(centreX+(radiusX*x), centreY+(radiusY*y)); 
				glEnd();
					
			}
			// posizione nemici sul radar
			for(int i=0;i<NUM_ENEMY;i++){
				u=g_camera.getPos();
				if(enemy[i].life>0 && distance(u,enemy[i].pos)<=DIST_RADAR){
				//if(true){
					// prendiamo la posizione di ogni nemico
					u.x -= enemy[i].pos.x;
					u.y -= enemy[i].pos.y;
					u.z -= enemy[i].pos.z;
					u = Normalize(u);
					x = -ProdScal(u,g_camera.getRight());
					y = -ProdScal(u,g_camera.getUp());
					z = -ProdScal(u,g_camera.getView());
					glPointSize(3.5);
					glBegin(GL_POINTS);
						if (z>=0)	glColor3f(1, 0, 0); 	// se e' davanti: rosso
						else		glColor3f(0.5, 0, 0);	// se e' dietro:  rosso scuro  
					
						glVertex2f(centreX+(radiusX*x), centreY+(radiusY*y)); 
					glEnd();
				}
			}
			//polizia
			for(int i=0;i<NUM_POLICE;i++){
				u=g_camera.getPos();
				if(police[i].life>0 && distance(u,police[i].pos)<=DIST_RADAR){
				//if(true){
					// prendiamo la posizione di ogni nemico
					u.x -= police[i].pos.x;
					u.y -= police[i].pos.y;
					u.z -= police[i].pos.z;
					u = Normalize(u);
					x = -ProdScal(u,g_camera.getRight());
					y = -ProdScal(u,g_camera.getUp());
					z = -ProdScal(u,g_camera.getView());
					glPointSize(3.5);
					glBegin(GL_POINTS);
						if (z>=0)	glColor3f(0, 0, 1); 	// se e' davanti: blu 
						else 		glColor3f(0, 0.5, 0.5);	// se e' dietro: blu scuro
						glVertex2f(centreX+(radiusX*x), centreY+(radiusY*y)); 
					glEnd();
				}
			}

			// MESSAGGI DI BORDO
			int n=checkDistPlanet(g_camera.getPos()); // indice pianeta se abb vicino
			//printf("pianeta %d", n);
			if(n>=0){
				glColor3f(1,1,1);		
				sprintf(temp ,"Welcome to %s: press T", getPlanetName(n));
				glRasterPos2f(0.515, 0.325);	
				renderBitmapString(GLUT_BITMAP_HELVETICA_12, temp);
			}

			//ALBERTO
			if( (hitEnemy!=-1 || hitPolice!=-1) && abs(clock()-precClock)>=CLOCKS_PER_SEC*1){
				precClock=clock();
				if(hitEnemy!=-1) sprintf(msgShoot ,"Hit Enemy!  Energy remained: %d%%", 
								(int) (enemy[hitEnemy].life>=0? enemy[hitEnemy].life : 0));
				if(hitPolice!=-1) sprintf(msgShoot ,"Hit Police! Energy remained: %d%%", 
								(int) (police[hitPolice].life>=0? police[hitPolice].life : 0));
			}
			else if( (hitEnemy==-1 && hitPolice==-1) && abs(clock()-precClock1)>=CLOCKS_PER_SEC*2){
				precClock1=clock();
				sprintf(msgShoot ,"");
			}
			glColor3f(1,1,1);			
			glRasterPos2f(0.515, 0.3);			
			renderBitmapString(GLUT_BITMAP_HELVETICA_12, msgShoot);

			// si disegnano le barre di velocita', energia e carburante
			drawBar();
		glPopMatrix();  
    	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

// DRAW RECTANGLE MISSILE: disegna sul cockpit il numero di missili disponibili
void drawRectMissiles(int num){
	GLfloat x1=0.2715,
		y1=0.2935,
		w=0.0323,
		h=0.0265;
	if(num>0)	glColor3f(0, 0.14, 0);
	else		glColor3f(0.5, 0, 0);
	glRectf(x1,y1,x1+w,y1+h);
	
	x1+=0.082;
	if(num>1)	glColor3f(0, 0.14, 0);
	else		glColor3f(0.5, 0, 0);
	glRectf(x1,y1,x1+w,y1+h);
	
	x1+=0.082;
	if(num>2)	glColor3f(0, 0.14, 0);
	else		glColor3f(0.5, 0, 0);
	glRectf(x1,y1,x1+w,y1+h);
}

// DRAW BAR: disegna le barre di velocita', energia e carburante
void drawBar(){
	GLfloat x1, x2, y1, y2;
	/* coordinate quadrilatero
	*
	*	y2	*		*
	*
	*	y1	*		*
	*
	*	0	x1		x2 
	*
	*/	
	// la coordinata x e la lunghezza sono uguali per tutte			
	x1=0.575;
	x2=x1+0.175; //x2=0.75;

	// BARRA DELLA VELOCITA'
	// settiamo la coordinata y
	y1=0.175;
	y2=y1+0.015;
	// posizionamento e inserimento della scritta SPEED
	glColor3f( 1, 1, 1);
	glRasterPos2f(x1, y2+0.01);
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, "SPEED");
	// si determina la velocita'
	GLfloat v = (g_camera.getSpeed())/(g_camera.MAXSPEED);
	// si setta il colore a seconda della velocita' che varia tra 0 e 1			
	glColor3f(2*v, 2-(v*2), 0);
	GLfloat w_bar=x2-x1;
	//GLfloat h_bar=y2-y1;
	glLineWidth(2);
	glBegin(GL_QUADS);
		glVertex3f(x1+0.001, y1+0.0015, 0.1);
		glVertex3f(x1+0.001, y2-0.0015, 0.1);
		glVertex3f(x1+(w_bar-0.001)*v, y2-0.0015, 0.1);
		glVertex3f(x1+(w_bar-0.001)*v, y1+0.0015, 0.1);
	glEnd();
	// quadrilatero esterno
	glLineWidth(2);
	glColor3f( 1, 1, 1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
	// posizioniamo la percentuale di velocita'
	sprintf(temp ,"%d%%", (int)(v*100));
	glRasterPos2f(x2+0.006, y1);			
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, temp);


	// BARRA DELL'ENERGIA
	// settiamo la coordinata y
	y1=y1-0.05;
	y2=y1+0.015;
	// posizionamento e inserimento della scritta ENERGY
	glColor3f( 1, 1, 1);
	glRasterPos2f(x1, y2+0.01);
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, "ENERGY");
	

	// per intanto è l'inverso della velocità	
	//energy=1-v;
	GLfloat en= energy/100.0;
	glColor3f(2*(1-en), 2-((1-en)*2), 0);
	w_bar=x2-x1;
	//GLfloat h_bar=y2-y1;
	glLineWidth(2);
	glBegin(GL_QUADS);
		glVertex3f(x1+0.001, y1+0.0015, 0.1);
		glVertex3f(x1+0.001, y2-0.0015, 0.1);
		glVertex3f(x2-(w_bar-0.001)*(1-en), y2-0.0015, 0.1);
		glVertex3f(x2-(w_bar-0.001)*(1-en), y1+0.0015, 0.1);
	glEnd();

	// quadrilatero esterno
	glLineWidth(2);
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
	// posizioniamo la percentuale di velocita'
	sprintf(temp ,"%d%%", (int) energy);
	glRasterPos2f(x2+0.006, y1);			
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, temp);			


	// BARRA DEL CARBURANTE
	// settiamo la coordinata y
	y1=y1-0.05; //y1=0.125;
	y2=y1+0.015;		
	// posizionamento e inserimento della scritta FUEL
	glColor3f( 1, 1, 1);
	glRasterPos2f(x1, y2+0.01);
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, "FUEL");
	// consumo del carburante
	pos_present=g_camera.getPos();
	if(abs((int) (fuel*100))>0 && (abs(pos_present.x-pos_past.x)>5.0 || abs(pos_present.y-pos_past.y)>5.0 || abs(pos_present.z-pos_past.z)>5.0)){
		// diminuiamo un po' il carburante
		fuel-= 0.002;
		//fuel-= 0.2;
		// aggiorniamo la posizione passata
		pos_past=g_camera.getPos();	
	}
	// se finisce il carburante si porta diminuisce la velocita' fino a fermare la navetta
	if(abs((int)(fuel*100))<=0 && v>0.0 && abs(clock()-precClock)>=CLOCKS_PER_SEC*0.1){
			g_camera.accelerate(-CAM_SPEED_STEP);
	       	 	glutPostRedisplay();
			// calcoliamo il tempo precedente, per diminuire la velocita' con gradualita'
			precClock=clock();
	}
	glColor3f(2*(1-fuel), 2-((1-fuel)*2), 0);
	w_bar=x2-x1;
	//GLfloat h_bar=y2-y1;
	glLineWidth(2);
	glBegin(GL_QUADS);
		glVertex3f(x1+0.001, y1+0.0015, 0.1);
		glVertex3f(x1+0.001, y2-0.0015, 0.1);
		glVertex3f(x2-(w_bar-0.001)*(1-fuel), y2-0.0015, 0.1);
		glVertex3f(x2-(w_bar-0.001)*(1-fuel), y1+0.0015, 0.1);
	glEnd();

	// quadrilatero esterno
	glLineWidth(2);
	glColor3f( 1, 1, 1);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x1, y1);
		glVertex2f(x1, y2);
		glVertex2f(x2, y2);
		glVertex2f(x2, y1);
	glEnd();
	// posizioniamo la percentuale di velocita'
	sprintf(temp ,"%d%%", (int) (fuel*100));
	glRasterPos2f(x2+0.006, y1);			
	renderBitmapString(GLUT_BITMAP_HELVETICA_12, temp);

}

// DRAW POINT: disegna il punto che indica dove si spara
void drawPoint(GLfloat centerX, GLfloat centerY){
	// puntino rosso
	glColor3f(1, 0, 0);
	GLfloat radiusX=0.0023,
		radiusY=0.0042;
		//radiusY=radiusX+0.0005;
	drawEllipse(centerX,centerY,radiusX,radiusY);
}

void drawSight(GLfloat centerX, GLfloat centerY){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0,1,0,1);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
			glLoadIdentity();

			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
			// per attivare la trasparenza nelle regioni dell'immagine dove e' presente
			glBindTexture(GL_TEXTURE_2D,g_textureArray[6]);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);glVertex3f(centerX-0.5,centerY-0.51,0);
				glTexCoord2f(1,0);glVertex3f(centerX+0.5,centerY-0.51,0);
				glTexCoord2f(1,1);glVertex3f(centerX+0.5,centerY+0.49,0);
				glTexCoord2f(0,1);glVertex3f(centerX-0.5,centerY+0.49,0);
			glEnd();
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

// DISEGNO DI UN ELLISSE: se si vuole un cerchio radiusX=radiusY
void drawEllipse(GLfloat centerX, GLfloat centerY, GLfloat radiusX, GLfloat radiusY){
	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(centerX, centerY);
		for(int angle=0;angle<720;angle+=1)
			glVertex2f(centerX + radiusX*sin(angle/2.0), centerY + radiusY*cos(angle/2.0));
	glEnd();
}
