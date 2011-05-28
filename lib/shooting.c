GLfloat speed1=150*g_camera.getMaxSpeed(),
	speed2=g_camera.getSpeed();
clock_t 	laserClock, superShootClock=clock();
GLboolean 	freezeShoot=false,
		superShoot[3];
GLfloat		ampiezza[3]={0,0,0};


void drawLaser(Laser *l);
// disegno del super-sparo
void drawSuperShoot(int i);

void freezeShooting(GLboolean b){
	freezeShoot=b;	
}

// decurta la vita se siamo colpiti dai laser nemici
void shooted(){
	for(int i=0;i<MAX_LASER; i++){
		if(laser[i].draw && laser[i].type!=ME && distance(laser[i].end,centerCamera)-radiusCamera<=0)
		{
			energy-=1;
		}
	}
}

// inizializza il laser della nostra navetta
void newLaser(){

	if(abs(clock()-laserClock)>=CLOCKS_PER_SEC*0.5) {
		laserClock=clock();
	
		if(nLaser==MAX_LASER) nLaser=0;
		
		// LASER DESTRO
		//printf("Num laser: %i\n", nLaser);
		laser[nLaser].draw=true;
		//laser[nLaser].start=g_camera.getPos();
		//laser[nLaser].start=pointDir(g_camera.getPos(),Scalar(g_camera.getUp(),-1),3);
		laser[nLaser].start=pointDir(g_camera.getPos(),g_camera.getRight(),2);
		laser[nLaser].origin=laser[nLaser].start;
		//laser[nLaser].dir=directionRel(laser[nLaser].start,Scalar(g_camera.getView(),100));
		laser[nLaser].dir=g_camera.getView();
		laser[nLaser].size=6;
		laser[nLaser].end = pointDir(laser[nLaser].start, laser[nLaser].dir, laser[nLaser].size);
		//laser[nLaser].color = {0,2,0};
		laser[nLaser].color[0]=1;
		laser[nLaser].color[1]=1;
		laser[nLaser].color[2]=0;
		laser[nLaser].type=ME;
	
		nLaser++;
		if(nLaser==MAX_LASER) nLaser=0;		
		
		// LASER SINISTRO
		//printf("Num laser: %i\n", nLaser);
		laser[nLaser].draw=true;
		//laser[nLaser].start=g_camera.getPos();
		//laser[nLaser].start=pointDir(g_camera.getPos(),Scalar(g_camera.getUp(),-1),3);
		laser[nLaser].start=pointDir(g_camera.getPos(),g_camera.getRight(),-2);
		laser[nLaser].origin=laser[nLaser].start;
		//laser[nLaser].dir=directionRel(laser[nLaser].start,Scalar(g_camera.getView(),100));
		laser[nLaser].dir=g_camera.getView();
		laser[nLaser].size=6;
		laser[nLaser].end = pointDir(laser[nLaser].start, laser[nLaser].dir, laser[nLaser].size);
		//laser[nLaser].color = {0,2,0};
		laser[nLaser].color[0]=1;
		laser[nLaser].color[1]=1;
		laser[nLaser].color[2]=0;
		laser[nLaser].type=ME;
		
		nLaser++;

		glutPostRedisplay();
	}
}

// inizializza il laser dei nemici
void newEnemyLaser(int i){
	if(nLaser==MAX_LASER) nLaser=0;
	
	laser[nLaser].draw=true;
	
	//laser[nLaser].start=enemy[i].pos;
	laser[nLaser].start=pointDir(enemy[i].pos, enemy[i].dir, 4);
	laser[nLaser].origin=laser[nLaser].start;
	laser[nLaser].dir=directionRel(laser[nLaser].start,centerCamera);
	laser[nLaser].size=4;
	laser[nLaser].end = pointDir(laser[nLaser].start, laser[nLaser].dir, laser[nLaser].size);
	laser[nLaser].color[0]=1;
	laser[nLaser].color[1]=0;
	laser[nLaser].color[2]=0;
	laser[nLaser].type=ENEMY;

	nLaser++;
}

// inizializza il laser della polizia
void newPoliceLaser(int i){
	if(nLaser==MAX_LASER) nLaser=0;
	
	laser[nLaser].draw=true;
	
	laser[nLaser].start=police[i].pos;
	//laser[nLaser].start=pointDir(police[i].pos, police[i].dir, 4.2);
	laser[nLaser].origin=laser[nLaser].start;
	laser[nLaser].dir=directionRel(laser[nLaser].start,centerCamera);
	laser[nLaser].size=4;
	laser[nLaser].end = pointDir(laser[nLaser].start, laser[nLaser].dir, laser[nLaser].size);
	laser[nLaser].color[0]=0;
	laser[nLaser].color[1]=0;
	laser[nLaser].color[2]=1;
	laser[nLaser].type=POLICE;

	nLaser++;
}

void insertRemoveLaser(){
	GLboolean interSection;
	Point3 tmp;
	for(int i=0;i<MAX_LASER;i++)
		if(laser[i].draw){
			if(distance(laser[i].origin,laser[i].start)>DIST_RADAR)
				laser[i].draw=false;
			else
			{	
				// controllo se un laser e' entrato in un pianeta o in un asteroide		
				interSection=false;			
				for(int j=0; j<9  && !interSection; j++)
					interSection=RaySphereIntersection(laser[i].start, laser[i].end, 
										posPlanet[j], radius[j], &tmp);		
				for(int j=0;j<NUM_SAT && !interSection;j++)
					interSection=RaySphereIntersection(laser[i].start, laser[i].end,
										 satellite[j].pos, satellite[j].rad, &tmp);
				if(interSection)
					laser[i].draw=false;
				else
					drawLaser(&laser[i]);
			}
		}
}

void drawLaser(Laser *l){

	glDisable(GL_LIGHTING);

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	
	
	//glColor3fv(laser[nLaser].color[0], laser[nLaser].color[1],laser[nLaser].color[2]);
	glColor3fv(l->color);
	/*glPushAttrib(GL_ALL_ATTRIB_BITS);
   		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, l->color);*/
			glLineWidth(2);
			glBegin(GL_LINES);
				glVertex3f(l->start.x, l->start.y, l->start.z);
				glVertex3f(l->end.x, l->end.y, l->end.z);
			glEnd();
			if (!freezeShoot){
				if(l->type!=ME){
					l->start.x += l->dir.x*speed1;
					l->start.y += l->dir.y*speed1;
					l->start.z += l->dir.z*speed1;
					l->end=pointDir(l->start, l->dir, l->size);
				}
				else{
					speed2= speed1+(100*g_camera.getSpeed());
					l->start.x += l->dir.x*speed2;
					l->start.y += l->dir.y*speed2;
					l->start.z += l->dir.z*speed2;
					l->end=pointDir(l->start, l->dir, l->size);
				}
			}
			glutPostRedisplay();

		//glPopMatrix();
	

	//glPopAttrib();


	glPopMatrix();
	glPopAttrib();


	glEnable(GL_LIGHTING);

}

// controlla se il laser della nostra navicella colpisce un nemico
int laserHitEnemy(){

	float d;
	for(int i=0; i<NUM_ENEMY; i++)
	{
		for(int j=0;j<MAX_LASER;j++)
		{
			if(laser[j].draw && enemy[i].life>0)
			{
				d=distance(laser[j].end,enemy[i].pos);
				//nel valutare la distanza considero anche il raggio dei nemici
				if(laser[j].type!=ENEMY && d-(radShootEnemy)<=0)
				{	// abbiamo colpito il nemico i-esimo					
					
					//enemy[i].life-=0.2;
					enemy[i].life-=5;
					// ALBERTO forse da spostare in enemy_police
					if(enemy[i].life<=0 && enemy[i].drawExp){
						newParticles(&explosionEnemy[i],enemy[i].pos);
						enemy[i].drawExp=false;
					}
					// se il laser colpisce il nemico non lo si disegna piu'
					laser[j].draw=false;
					return i;
				}
			}
		}
	}
	// nessuna collisione con i nemici
	return -1;
}

// controlla se il laser della nostra navicella colpisce la polizia
int laserHitPolice(){

	float d;
	for(int i=0; i<NUM_POLICE; i++)
	{
		for(int j=0;j<MAX_LASER;j++)
		{
			if(laser[j].draw && police[i].life>0)
			{
				//d=distance(laser[j].end,police[i].pos);
				d=distance(pointDir(laser[j].start,laser[j].dir,laser[j].size/2),police[i].pos);
				if(laser[j].type!=POLICE &&  d-(radShootPolice)<=0) 
				{		//abbiamo colpito la polizia i-esima
					
					police[i].life-=5;
					if(police[i].life<=0 && police[i].drawExp){
						newParticles(&explosionPolice[i],police[i].pos);
						police[i].drawExp=false;
					}
					laser[j].draw=false;
					return i;
				}
			}
		}
	}
	// nessuna collisione la polizia
	return -1;
}


// inizializzazione del super-sparo
void initSuperShoot(){
	if(abs(clock()-superShootClock)>=CLOCKS_PER_SEC*2 && nSuperShoot>0){
	//if(nSuperShoot>0){
		nSuperShoot--;
		superShoot[nSuperShoot]=true;
		//printf("\tsupersparo %i: %s \n", nSuperShoot, superShoot[nSuperShoot]? "true": "false");
		removeMissiles();
		superShootClock=clock();
	}
	
}
// inserimento del super sparo
void insertSuperShoot(){
	//printf("\tsupersparo %i: %s \n", (i), superShoot[i]? "true": "false"); fflush(stdout);
	for(int i=0;i<MAX_SHOOT;i++)
	{
		
		if(superShoot[i])
		{
			//printf("\tsupersparo %i: %s \n", (i), superShoot[i]? "true": "false"); fflush(stdout);			
			drawSuperShoot(i);
		}
		if(ampiezza[i]>65)
		{	
			//printf("\t\tciao\n");	
			superShoot[i]=false;
			ampiezza[i]=0;
		}
	}
	
}
// disegno del super-sparo
void drawSuperShoot(int i){
	
	//printf("\tsupersparo: %i, raggio: %f\n", i,ampiezza[i]); fflush(stdout);
	Point3 pos=centerCamera;

	if(!freezeShoot)
		ampiezza[i] += pow(clock()-superShootClock,2)/100000000000.0f;
		//ampiezza[i] += pow(dtime,2)/1200;

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
			glBindTexture(GL_TEXTURE_2D, g_textureArray[5]);
			gluQuadricTexture( a, GL_TRUE );
			gluSphere(a, ampiezza[i], 100, 100);
		glPopMatrix(); 
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHT2);
		glDisable(GL_LIGHT1);
		glDisable(GL_LIGHT0);   
		glDisable(GL_BLEND);  
	glPopAttrib();

	// se intercettiamo una navicella del nemico/polizia la distruggiamo
	for(int j=0;j<NUM_ENEMY;j++)
	{
		if(distance(pos,enemy[j].pos)<=ampiezza[i] && enemy[j].drawExp)
		{
			enemy[j].life=0;
			newParticles(&explosionEnemy[j],enemy[j].pos);
			enemy[j].drawExp=false;
		}
	}
	for(int j=0;j<NUM_POLICE;j++)
	{
		if(distance(pos,police[j].pos)<=ampiezza[i] && police[j].drawExp)
		{
			police[j].life=0;
			newParticles(&explosionPolice[j],police[j].pos);
			police[j].drawExp=false;
		}
	}
}
