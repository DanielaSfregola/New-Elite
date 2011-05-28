int 	count=0;
GLsizei imageWidth[TOT_TEXTURE], imageHeight[TOT_TEXTURE];

void initTexture(GLuint textureArray[], char  *imageFileName, int textureID);

// CARICAMENTO TEXTURE
void loadTexture(){
	count=0;
	// SFONDO	
	if (not_pressF1) printf("starmap.sgi..."); fflush(stdout);
	initTexture(g_textureSfondo, "texture/starmap.sgi", 0);
	if (not_pressF1) printf("OK\n");
	// PIANETI
	if (not_pressF1) printf("mercury.sgi...");  fflush(stdout);
	initTexture(g_texturePlanet, "texture/mercury.sgi", 0);
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("venus.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/venus.sgi", 1); 
 	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("earth.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/earth.sgi", 2); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("mars.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/mars.sgi", 3); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("jupiter.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/jupiter.sgi", 4); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("saturn.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/saturn.sgi", 5); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("uranus.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/uranus.sgi", 6); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("neptune.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/neptune.sgi", 7); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("pluto.sgi..."); fflush(stdout);
  	initTexture(g_texturePlanet, "texture/pluto.sgi", 8); 
	if (not_pressF1) printf("OK\n");
	// SATELLITI E ANELLI
	if (not_pressF1) printf("moon.sgi..."); fflush(stdout);
  	initTexture(g_textureSatellite, "texture/moon.sgi", 0); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("io.sgi..."); fflush(stdout);
  	initTexture(g_textureSatellite, "texture/ioJupiter.sgi", 1); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("europa.sgi..."); fflush(stdout);
  	initTexture(g_textureSatellite, "texture/europaJupiter.sgi", 2); 
	if (not_pressF1) printf("OK\n");
	if (not_pressF1) printf("saturn_ring.sgi...");
	initTexture(g_textureRing, "texture/saturn_ring.sgi", 0);
	if (not_pressF1) printf("OK\n");
	// COCKPIT
	if (not_pressF1) printf("cockpit.sgi..."); fflush(stdout);
	initTexture(g_textureArray, "texture/cockpit11.sgi", 0);
	if (not_pressF1) printf("OK\n");
	// SFONDO TEXT ELITE
	if (not_pressF1) printf("backgroundTxt.sgi..."); fflush(stdout);
	initTexture(g_textureArray, "texture/backgroundTxt.sgi", 1);
	if (not_pressF1) printf("OK\n"); 
	// SCUDI
	if (not_pressF1) printf("shield.sgi"); fflush(stdout);
	initTexture(g_textureArray, "texture/scudo.sgi", 2);
	if (not_pressF1) printf("...OK\n");
	// TEXTURE GAME OVER
	if (not_pressF1) printf("gameover.sgi"); fflush(stdout);
	initTexture(g_textureArray, "texture/gameover.sgi", 3);
	if (not_pressF1) printf("...OK\n");
	// PARTICLE
	if (not_pressF1) printf("particle.sgi"); fflush(stdout);
	initTexture(g_textureArray, "texture/particle.sgi", 4);
	if (not_pressF1) printf("...OK\n");
	// SUPER SHOOT
	if (not_pressF1) printf("darkPower.sgi"); fflush(stdout);
	initTexture(g_textureArray, "texture/darkPower2.sgi", 5);
	if (not_pressF1) printf("...OK\n");
	// SIGHT
	if (not_pressF1) printf("sight.sgi"); fflush(stdout);
	initTexture(g_textureArray, "texture/sight.sgi", 6);
	if (not_pressF1) printf("...OK\n");	
}

// LETTURA DELLA TEXTURE IN FORMATO SGI
// deve essere di dimensioni potenze di 2, ma non c'e' controllo
void initTexture(GLuint textureArray[], char *imageFileName, int textureID){
	GLsizei components;
	if (not_pressF1)
		img[count] = read_texture(imageFileName, &imageWidth[count], &imageHeight[count], &components);
	
	// registra la texture con il suo ID
	glGenTextures(1, &textureArray[textureID]);
	// bind the texture to the texture arrays index
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth[count], imageHeight[count],0, GL_RGBA, GL_UNSIGNED_BYTE, img[count]);
	
	// Setting the minification and magnification filters 
	// to nearest instead of linear, may run faster on some 
	// platforms, with possibly lower quality 
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// modo modulate (default) oppure decal 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	count++;
}

// LOAD OBJ
// 	funzione che carica i file presenti nella cartella OBJ
GLvoid loadOBJ(){
	
	printf("enemy.obj");
	//enemyModel = glmReadOBJ("obj/sword.obj");
	//enemyModel = glmReadOBJ("obj/enemy15.obj");
	enemyModel = glmReadOBJ("obj/enemy.obj");
	glmUnitize(enemyModel);
	glmFacetNormals(enemyModel);
	glmVertexNormals(enemyModel, 180.0);
	glmScale(enemyModel,4);
	printf("...OK\n");
	
	printf("police.obj");
	policeModel = glmReadOBJ("obj/ASF-SHP.obj");
	glmUnitize(policeModel);
	glmFacetNormals(policeModel);
	glmVertexNormals(policeModel, 180.0);
	glmScale(policeModel,4);
	printf("...OK\n");

}
