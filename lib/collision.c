/*
*	funzione distanza della posizione della telecamera ad un obbiettivo passato come parametro. Tolleranza del 20% rispetto al raggio
*/
int checkDistPlanet(Point3 me);
bool checkCollision();

// Poin3 me:		il punto su cui controllare se siamo abbastanza vicini ad un pianeta
// return:		restituisce l'indice dell'oggetto vicino a me, -1 se non vi e' nulla vicinp
// la tolleranza è espressa in funzione del raggio di ciascun pianeta (raggio + 20%)
int checkDistPlanet(Point3 me){

	//Point3 me;
	float d, tol;
	//posizione della telecamera
	//me = g_camera.getPos();

	for(int i=0; i<9; i++){
		d=distance(me,posPlanet[i]);
		//tolleranza
		tol=radius[i]+radius[i]/5;
		//nel valutare la distanza considero anche il raggio del pianeta
		if(d-radius[i]<tol && d-radius[i]>0) 
			return i;
	}
	// nessun pianeta abb vicino
	return -1;

}


// Poin3 me:		il punto su cui controllare se siamo abbastanza vicini ad un satellite
// return:		restituisce l'indice dell'oggetto vicino a me, -1 se non vi e' nulla vicino
// la tolleranza è espressa in funzione del raggio di ciascun satellite (raggio + 20%)
int checkDistSatellite(Point3 me){
	//Point3 me;
	float d, tol;
	//posizione della telecamera
	//me = g_camera.getPos();
	for(int i=0; i<NUM_SAT; i++){
		d=distance(me,satellite[i].pos);
		//tolleranza
		tol=satellite[i].rad+satellite[i].rad/5;
		// nel valutare la distanza considero anche il raggio del satellite
		if(d-satellite[i].rad<tol && d-satellite[i].rad>0)
			return i;
	}
	// nessun satellite abb vicino
	return -1;

}
// funzione che rileva la collisione con un pianeta, praticamente uguale a prima ma con tolleranza al 0%
//restituisce 1 se rivela una collisione, 0 altrimenti 
bool checkCollision(){

	float d;
	// pianeti
	for(int i=0; i<9; i++){
		d=distance(g_camera.getPos(),posPlanet[i]);
		//nel valutare la distanza considero anche il raggio del pianeta
		if(d-radius[i]<=0) 	return true;
	}
	// satelliti
	for(int i=0; i<NUM_SAT; i++){
		d=distance(g_camera.getPos(),satellite[i].pos);
		//nel valutare la distanza considero anche il raggio del pianeta
		if(d-satellite[i].rad<=0) return true; 
	}
	// nessuna collisione
	return false;

}

int checkCollisionEnemy(){
	float d;
	for(int i=0; i<NUM_ENEMY; i++){
		if(enemy[i].life>0){
			d=distance(g_camera.getPos(),enemy[i].pos);
			//nel valutare la distanza considero anche il raggio del nemico
			if(d-enemy[i].rad<=0.2) 
				return i;
		}
	}
	// nessuna collisione con il nemico
	return -1;
}

int checkCollisionPolice(){
	float d;
	for(int i=0; i<NUM_POLICE; i++){
		if(police[i].life>0){
			d=distance(g_camera.getPos(),police[i].pos);
			//nel valutare la distanza considero anche il raggio della polizia
			if(d-(police[i].rad+radiusCamera)<=0.2) 
				return i;
		}
	}
	// nessuna collisione con la polizia
	return -1;
}
