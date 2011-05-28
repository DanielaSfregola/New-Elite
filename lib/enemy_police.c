/*
*	initEnemy
*	initPolice
*
*	enemy
*		drawEnemy
*		setPosDir
*	police
*		drawPolice
*		setPosDir			
*
*	firstShip
*	freezeSpaceShip
*
*/
GLvoid initEnemy();
GLvoid initPolice();
GLvoid drawEnemy(GLMmodel* model, Point3 pos, Point3 direction);
GLvoid drawPolice(GLMmodel* model, Point3 pos, Point3 direction);
GLboolean firstShip(SpaceShip *spsh, int *nShip, char *typeShip, Point3 *intersection);
GLvoid setPosDir(SpaceShip *spSh);
GLvoid drawShield(Point3 pos, GLfloat radius);
GLvoid freezeSpaceShip(GLboolean b);
/* VARIABILI GLOBALI */
Point3 		me=g_camera.getPos(), extraPos;
GLfloat		dist,
		sp=20*g_camera.getMaxSpeed(),
		angleEnemy=glutGet(GLUT_ELAPSED_TIME)*0.032,
		anglePolice=glutGet(GLUT_ELAPSED_TIME)*0.032,
		radiusEnemy=4,			
		radiusPolice=4.2;
// setPosDir
clock_t 	pastClock=clock();
char 		oldType=' ';
int		oldNum=-1;
clock_t 	enemyClock, policeClock=clock();
//drawPolice, drawEnemy
int		numPoliceDraw, numEnemyDraw=0;		
GLboolean 	freezeShip=false;

// funzione per la freeze
GLvoid freezeSpaceShip(GLboolean b){
	freezeShip=b;
}

/* FUNZIONI DI INIZIALIZZAZIONE (melice.c)*/
GLvoid initEnemy(){
	/* POS per le prove
	enemy[0].pos=g_camera.getPos();
	enemy[0].pos.x -= 20;
	enemy[0].pos.z += 40;
	*/

	enemy[0].pos=posPlanet[2];
	enemy[0].pos.x += radius[2]+60;
	enemy[0].pos.y += radius[2]+40;
	
	enemy[2].pos=posPlanet[4];
	enemy[2].pos.y -= radius[4]+40;
	enemy[2].pos.z += radius[4]+60;
	
	enemy[4].pos = posPlanet[1];
	enemy[4].pos.x += radius[1]+200;
	
	enemy[6].pos = posPlanet[5];
	enemy[6].pos.x += radius[5]+200;
	
	enemy[8].pos = posPlanet[5];
	enemy[8].pos.x -= radius[5]+200;
	enemy[8].pos.z -= radius[5]+50;
	
	for(int i=0;i<NUM_ENEMY;i++){
		if(i%2==1){
			enemy[i].pos=enemy[i-1].pos;
			enemy[i].pos.x -= 2*radiusEnemy+0.1;
			enemy[i].pos.y -= 2*radiusEnemy+0.1;	
		}
		enemy[i].dir=directionRel(enemy[i].pos,me);
		enemy[i].dirCorr=true;
		enemy[i].drawExp=true;
		enemy[i].rad=radiusEnemy;
		enemy[i].index=i;
		enemy[i].type='e';
		enemy[i].life=100;
		enemy[i].move=false;
	}
}
GLvoid initPolice(){
	/* POS per le prove	
	police[0].pos=g_camera.getPos();
	police[0].pos.x +=10;
	police[0].pos.z +=40;
	
	police[0].dir=directionRel(police[0].pos,me);
	police[0].dirCorr=true;
	police[0].drawExp=true;
	police[0].rad=radiusPolice;
	police[0].index=0;
	police[0].type='p';
	police[0].life=100;
	police[0].move=false;
	*/
	
	for(int i=0;i<NUM_POLICE;i++){
		police[i].pos=pointDir(posPlanet[i], directionRel(posPlanet[i],me), radius[i]+radius[i]/5);
		if(i==2 || i==4)	police[i].pos=pointDir(police[i].pos, g_camera.getUp(), 15);
		if(i==8)		police[i].pos=pointDir(posPlanet[i], directionRel(posPlanet[i],me), radius[i]*2);
		police[i].dir=directionRel(police[i].pos,me);
		police[i].dirCorr=true;
		police[i].drawExp=true;
		police[i].rad=radiusPolice;
		police[i].index=i;
		police[i].type='p';
		police[i].life=100;
		police[i].move=false;
	}
}

/* DISEGNO DELLE NAVICELLE SPAZIALI (solar_system.c) */
GLvoid insertEnemy(){
	
	// FOR PER TUTTI I NEMICI
	numEnemyDraw=0;	// se il numero di polizia è minore di 2 se ne disegna un'altra fuori dalla sfera dello sfondo
				// questo per evitare che i movimenti (laser, satelliti) siano troppo veloci
	me=g_camera.getPos();
	for(int i=0;i<NUM_ENEMY;i++){
		dist=distance(enemy[i].pos,me);
		if(dist<=DIST_RADAR && enemy[i].life>0){
		//if(enemy[i].life>0){
			setPosDir(&enemy[i]);	// gli passo l'indirizzo di memoria dove sta quella variabile
			drawEnemy(enemyModel,enemy[i].pos,enemy[i].dir);
			numEnemyDraw++;
			//drawShield(enemy[i].pos,radShootEnemy);
			// sparo dei nemici
			if(enemy[i].dirCorr && abs(clock()-enemyClock)>=CLOCKS_PER_SEC*1) 
			{
				newEnemyLaser(i);
				enemyClock=clock();
				glutPostRedisplay();
			}
		}
	}
	while (numEnemyDraw<2){ // disegno tante navette fuori dalla sfera dello sfondo
		extraPos=pointDir(me, g_camera.getUp(), 1200);
		drawEnemy(enemyModel,extraPos,g_camera.getUp());
		numEnemyDraw++;
	}
}

GLvoid insertPolice(){
	
	// FOR PER TUTTE LE NAVETTE DELLA POLIZIA
	numPoliceDraw=0;	// se il numero di polizia è minore di 2 se ne disegna un'altra fuori dalla sfera dello sfondo
				// questo per evitare che i movimenti (laser, satelliti) siano troppo veloci
	me=g_camera.getPos();
	for(int i=0;i<NUM_POLICE;i++){
		dist=distance(police[i].pos,me);
		if(dist<=DIST_RADAR && police[i].life>0){
		//if(police[i].life>0){	
			if ( (isIllegal() || police[i].life<100) )
			{	// la polizia si muove solo se abbiamo commerciato illegalmente o gli stiamo sparando
				setPosDir(&police[i]);
			}
			drawPolice(policeModel,police[i].pos,police[i].dir);
			numPoliceDraw++;
			//drawShield(police[i].pos,radShootPolice);
			if( (isIllegal() || police[i].life<100) && police[i].dirCorr){
				if(abs(clock()-policeClock)>=CLOCKS_PER_SEC*1) {
					newPoliceLaser(i);
					policeClock=clock();
					glutPostRedisplay();
				}
				//newEnemyLaser(i);
			}
		}
	}
	while (numPoliceDraw<2){ // disegno tante navette fuori dalla sfera dello sfondo
		extraPos=pointDir(me, g_camera.getUp(), 1200);
		drawPolice(policeModel,extraPos,g_camera.getUp());
		numPoliceDraw++;
	}
}

/*
*	FUNZIONE CHE DATO IL PUNTATORE ALLA POSIZIONE E ALLA DIREZIONE ATTUALI
*	SETTA LA NUOVA POSIZIONE E LA NUOVA DIREZIONE 
*/
GLvoid setPosDir(SpaceShip *spSh){
	me=g_camera.getPos();
	dist=distance(spSh->pos,me);
	GLfloat rad=spSh->rad;
	
	if(dist>35){
	//if(dist>10){
		spSh->move=true;	
		int i=checkDistPlanet(spSh->pos);
		int j=checkDistSatellite(spSh->pos);
		Point3 *tmp=(Point3 *) malloc(sizeof(Point3));
		
		/* ALGORITMO CHE PERMETTE ALLE NAVETTE SPAZIALI DI EVITARE PIANETI, SATELLITI E ALTRE NAVETTE */
			
			if( (i!=-1) && (j==-1) && RaySphereIntersection(spSh->pos,me,posPlanet[i],radius[i]+rad+rad*0.1,tmp) )
			{	// se stiamo andando addosso ad un pianeta ma non c'e' un satellite nelle vicinanze		
			
				// troviamo il nuovo punto verso cui dirigerci per evitare il pianeta
				*tmp=addPos(*tmp, radius[i]+rad);			
				spSh->dir=directionRel(spSh->pos,*tmp);
				spSh->dirCorr=false;		// perche' non ci stiamo muovendo verso la telecamera
			}
			// se stiamo andando addosso ad un satellite ma non siamo ancora nelle vicinanze di un pianeta
			else if( (i==-1) && (j!=-1) && RaySphereIntersection(spSh->pos,me,satellite[j].pos,satellite[j].rad+rad+rad*0.1,tmp) )
			{
				*tmp=addPos(*tmp, satellite[j].rad);
				spSh->dir=directionRel(spSh->pos,*tmp);
				spSh->dirCorr=false;		// perche' non ci stiamo muovendo verso la telecamera
			}
			// se siamo nelle vicinanze sia di un satellite che di un pianeta e vi e' intesersezione con almeno uno dei due
			else if( (i!=-1) && (j!=-1) && 
				( 	RaySphereIntersection(spSh->pos,me,satellite[j].pos,satellite[j].rad+rad+rad*0.1,tmp) ||  
					RaySphereIntersection(spSh->pos,me,posPlanet[i],radius[i]+rad+rad*0.1,tmp))	)
				{
				
				Point3 *tmpSat=(Point3 *) malloc(sizeof(Point3));
				Point3 *tmpPl=(Point3 *) malloc(sizeof(Point3));
				bool intersectionSat=RaySphereIntersection(spSh->pos,me,satellite[j].pos,satellite[j].rad+rad+rad*0.1,tmpSat);
				bool intersectionPl=RaySphereIntersection(spSh->pos,me,posPlanet[i],radius[i]+rad+rad*0.1,tmpPl);
				float distPl=distance(spSh->pos,*tmpPl);
				float distSat=distance(spSh->pos,*tmpSat);
				// se interseca solo il pianeta, o se gli interseca entrambi, ma prima il pianeta
				if( (intersectionPl && !intersectionSat) || ((intersectionPl && intersectionSat) && distPl<distSat) )
				{
					*tmpPl=addPos(*tmpPl, radius[i]+rad);
					spSh->dir=directionRel(spSh->pos,*tmpPl);
					spSh->dirCorr=false;		// perche' non ci stiamo muovendo verso la telecamera
				} 
				// se interseca solo il satellite, o se gli interseca entrambi, ma prima il satellite
				else if(!intersectionPl && intersectionSat || ((intersectionPl && intersectionSat) && distSat<=distPl) )
				{
					*tmpSat=addPos(*tmpSat, satellite[j].rad);
					//*tmpSat=addPos(*tmpSat, satellite[j].rad+rad);
					spSh->dir=directionRel(spSh->pos,*tmpSat);
					spSh->dirCorr=false;		// perche' non ci stiamo muovendo verso la telecamera
				}
				free(tmpPl);
				free(tmpSat);
			}
			else	// se non c'e' rischio collisione ne con un satellite ne con un pianeta
			{	
				// le variabili di tipo clock servono per non far "impazzire" il cambio di direzione delle navicelle 
				//quando abbengono rapidi cambiamenti di posizione della telecamera
				int num;
				char type;
				Point3 intShip;
				GLboolean collision=firstShip(spSh,&num,&type,&intShip);
				// restituisce se esiste il numero e il tipo della prima navicella che c'e' nella direzione verso la telecamera
				if(collision && abs(clock()-pastClock)>=CLOCKS_PER_SEC*0.3)
				{ 	// si controlla se c'e' un altra navetta sulla traiettoria
					pastClock=clock();					
					
					GLboolean mv;
					switch(type){	case 'p': mv=police[num].move; break;
							case 'e': mv=enemy[num].move; break;	}
					
					// si settano queste variabili in modo che si mantenga la stessa direzione 
					// finche' continua ad esserci insersezione con la stessa navetta		
					if(!mv && type!=oldType && num!=oldNum)
					{	// si trova la nuova direzione per evitarla						
						oldType=type;
						oldNum=num;	
						switch(type){
							case 'p': *tmp=addPos(intShip, (spSh->rad+police[num].rad)); break;
							case 'e': *tmp=addPos(intShip, (spSh->rad+enemy[num].rad)); break;
						}
						spSh->dir= directionRel(spSh->pos,*tmp);
						spSh->dirCorr=false;	
					}
					//else if(mv && abs(clock()-pastClock)>=CLOCKS_PER_SEC*0.3)
					else if(mv) 
					{
						oldType=' ';
						oldNum=-1;
						switch(type){
							case 'p': *tmp=addPos(intShip, (spSh->rad+police[num].rad)); break;
							case 'e': *tmp=addPos(intShip, (spSh->rad+enemy[num].rad)); break;
						}
						spSh->dir= directionRel(spSh->pos,*tmp);
						spSh->dirCorr=false;
					}
				}
				// se non ci sono ostacoli
				else if(!collision && abs(clock()-pastClock)>=CLOCKS_PER_SEC*0.3)
				//else
				{
					pastClock=clock();
					oldType=' ';
					oldNum=-1;
					spSh->dir=directionRel(spSh->pos,me); 	// si aggiorna nella direzione tra oggetto e me
					spSh->dirCorr=true;			// perche' ci stiamo muovendo verso la telecamera
				}
			}
		
			// ora che abbiamo trovato la direzione di movimento, muoviamo la navicella
			if (!freezeShip){			
				spSh->pos.x += spSh->dir.x*sp;
				spSh->pos.y += spSh->dir.y*sp;
				spSh->pos.z += spSh->dir.z*sp;
				glutPostRedisplay();
			}
		free(tmp);
	}
	// se non ci si deve muovere, la direzione deve essere sempre rivolta verso la telecamera
	else if (!freezeShip){
		spSh->move=false;
		spSh->dir=directionRel(spSh->pos,me); 		// si aggiorna nella direzione tra oggetto e me
		spSh->dirCorr=true; 				// perche' ci stiamo muovendo verso la telecamera
	}
}


// se c'e' intersezione con almeno un elemento ritorna TRUE e restituisce il numero, il tipo e il punto di intersezione
// con la prima navicella che c'e' nella direzione della telecamera, altrimenti ritorna FALSE 
GLboolean firstShip(SpaceShip *spSh, int *nShip,char *typeShip,Point3 *intersection){
	Point3		me=g_camera.getPos(),
			tmp;
	GLfloat 	dist,
			minDist=MAX_DIST+1000;
	GLboolean	anyIntersection=false;

	for(int i=0;i<NUM_ENEMY;i++){
		if ( enemy[i].life>0 && (spSh->type!='e' || spSh->index!=i) &&		// per non far il controllo con la navetta stessa
			RaySphereIntersection(spSh->pos,me,enemy[i].pos,enemy[i].rad+spSh->rad+0.1, &tmp))
		{
			dist=distance(spSh->pos,tmp);
			if(dist<minDist){
				anyIntersection=true;
				minDist=dist;
				*nShip=i;
				*typeShip='e';
				*intersection=tmp;
			}
		}
	}
	for(int i=0;i<NUM_POLICE;i++){
		if ( police[i].life>0 && (spSh->type!='p' || spSh->index!=i) &&		// per non far il controllo con la navetta stessa
			RaySphereIntersection(spSh->pos,me,police[i].pos,police[i].rad+spSh->rad+0.1, &tmp))
		{
			dist=distance(spSh->pos,tmp);
			if(dist<minDist){
				anyIntersection=true;
				minDist=dist;
				*nShip=i;
				*typeShip='p';
				*intersection=tmp;
			}
		}
	}
	return anyIntersection;
}





/* DRAW ENEMY
*	funzione che disegna le navette nemiche caricate come obj
* 		model		modello del nemico caricato
*		pos		posizione del nemico
*		direcion (r3)	orientazione (versore direzione di movimento)	
*
*/
GLvoid drawEnemy(GLMmodel* model, Point3 pos, Point3 r3){	
	// vettore temporaneo: ci serve per definire una terna di assi ortogonali tra loro di cui uno di essi e' r3  	
	Point3 temp;
	temp.x=1;
	temp.y=1;
	temp.z=1;
	
	// CI TROVIAMO LA TERNA DI ASSI ORTOGONALI TRA LORO:	r1,r2,r3
	// normalizziamo r3, anche se dovrebbe essere gia' stato normalizzato	
	r3=Normalize(r3);
	// il prodotto esterno del vettore 'r3' per un qualsiasi vettore 'temp', mi fornisce il vettore 'r1' ortogonale a r3
	Point3 r1=Normalize(Cross(r3, temp));
	// ci troviamo il versore r2, ortogonale a 'r1' e quindi anche a 'r3'  
	Point3 r2=Normalize(Cross(r3,r1));
	
	// ci creiamo un vettore che definisce la matrice (4x4) di rototraslazione che dobbiamo applicare
	/* MATRICE DI ROTO-TRASLAZIONE
		Mappatura indici del vettore sulla matrice:
			0	1	2	3
			4	5	6	7
			8	9	10	11
			12	13	14	15
		POSIZIONI:		
			0:2 		coordinate di r1		(prime tre posizioni della prima riga)
			4:6		coordinate di r2		(prime tre posizioni della seconda riga)
			8:10		coordinate di r3		(prime tre posizioni della terza riga)
			12:14		coordinate della posizione	(prime tre posizioni della quarta riga)
			3,7,11		impostate a 0			(prime tre posizioni dell'ultima colonna)
			15		impostata a 1
	*/
	GLfloat r[16];
	r[0]=r1.x;
	r[1]=r1.y;
	r[2]=r1.z;

	r[4]=r2.x;
	r[5]=r2.y;
	r[6]=r2.z;
	
	r[8]=r3.x;
	r[9]=r3.y;
	r[10]=r3.z;
	
	r[3]=0;
	r[7]=0;
	r[11]=0;
	
	r[12]=pos.x;
	r[13]=pos.y;
	r[14]=pos.z;
	r[15]=1;	
	
	/*glDisable(GL_LIGHTING);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();

		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		glEnable(GL_LIGHT2);
		glEnable(GL_COLOR_MATERIAL);			// abilita il materiale
		glColorMaterial(GL_FRONT, GL_EMISSION);	// tipo di materiale
*/

	glPushMatrix();
		// applichiamo la roto-traslazione
		glMultMatrixf(r);
		if(!freezeShip)
			angleEnemy += 0.896000;
		glRotatef(angleEnemy, 0, 0, 1);	
		glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
		//glmDraw(model, GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);		
	glPopMatrix();
/*		
		glDisable(GL_COLOR_MATERIAL);			// disabilita il materiale
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);

	glPopMatrix();
	glPopAttrib();


	//glEnable(GL_LIGHTING);}
*/
}

/* DRAW POLICE
*	funzione che disegna le navette nemiche caricate come obj
* 		model		modello del nemico caricato
*		pos		posizione del nemico
*		direcion (r1)	orientazione (versore direzione di movimento)	
*
*/
GLvoid drawPolice(GLMmodel* model, Point3 pos, Point3 r1){
	// vettore temporaneo: ci serve per definire una terna di assi ortogonali tra loro di cui uno di essi e' r3  	
	Point3 temp;
	temp.x=1;
	temp.y=1;
	temp.z=1;
	
	// CI TROVIAMO LA TERNA DI ASSI ORTOGONALI TRA LORO:	r1,r2,r3
	// normalizziamo r1, anche se dovrebbe essere gia' stato normalizzato	
	r1=Normalize(r1);
	// il prodotto esterno del vettore 'r1' per un qualsiasi vettore 'temp', mi fornisce il vettore 'r2' ortogonale a r1
	Point3 r2=Normalize(Cross(r1, temp));
	// ci troviamo il versore r3, ortogonale a 'r2' e quindi anche a 'r1'  
	Point3 r3=Normalize(Cross(r1,r2));
	
	// ci creiamo un vettore che definisce la matrice (4x4) di rototraslazione che dobbiamo applicare
	/* MATRICE DI ROTO-TRASLAZIONE
		Mappatura indici del vettore sulla matrice:
			0	1	2	3
			4	5	6	7
			8	9	10	11
			12	13	14	15
		POSIZIONI:		
			0:2 		coordinate di r1		(prime tre posizioni della prima riga)
			4:6			coordinate di r2		(prime tre posizioni della seconda riga)
			8:10		coordinate di r3		(prime tre posizioni della terza riga)
			12:14		coordinate della posizione	(prime tre posizioni della quarta riga)
			3,7,11		impostate a 0			(prime tre posizioni dell'ultima colonna)
			15			impostata a 1
	*/
	GLfloat r[16];
	r[0]=r1.x;
	r[1]=r1.y;
	r[2]=r1.z;

	r[4]=r2.x;
	r[5]=r2.y;
	r[6]=r2.z;
	
	r[8]=r3.x;
	r[9]=r3.y;
	r[10]=r3.z;
	
	r[3]=0;
	r[7]=0;
	r[11]=0;
	
	r[12]=pos.x;
	r[13]=pos.y;
	r[14]=pos.z;
	r[15]=1;	
		
	glPushMatrix();
		glMultMatrixf(r);
		//printf("diff=%f\n", (glutGet(GLUT_ELAPSED_TIME)-anglePolice)*0.032); fflush(stdout);
		// se non sono nel freeze		
		if(!freezeShip)
			anglePolice += 0.896000;
		glRotatef(anglePolice, 1, 0, 0);
		//ROTAZIONE perche' stia dritto:		
		//glRotatef(-135, 1, 0, 0);
		glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);	
		//glmDraw(model, GLM_SMOOTH|GLM_TEXTURE|GLM_MATERIAL);		
	glPopMatrix();
}

