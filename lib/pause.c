int selectedP=0;
int menuIndex=0;
GLboolean menuVar=false;

void pauseScene();
void pausekeyboard(GLubyte key, GLint x, GLint y);
void pausespecialkeys( GLint key, GLint u, GLint v);
void renderingMenu();

void pauseScene(){	



	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport( 0, 0, width, height);
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
		glDisable(GL_LIGHTING); // disattiviamo le luci 
		glDisable(GL_DEPTH_TEST); // disabilitiamo z buffering 
		glDisable(GL_CULL_FACE); // disabilitiamo face culling 	
		
		glLoadIdentity();
		gluOrtho2D(0, width, 0, height);
		glMatrixMode(GL_MODELVIEW);
		
		glScalef(1.0,-1.0,1.0);
		glTranslatef(0.0,-height,0.0);
		
		// carico la texture
		glEnable(GL_TEXTURE_2D);   	
			//glBindTexture(GL_TEXTURE_2D, g_textureSfondo[0]);
			glBindTexture(GL_TEXTURE_2D, g_textureArray[1]);		
			glBegin(GL_QUADS);				
				//coordinate del quadrato "schermo"
				glTexCoord2f(0.0f, 1.0f); glVertex3f(0,0,0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(width,0,0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(width,height,0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(0,height,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
		

		
	/***** scritte menu' *****/
	
	//resume
	if(!menuVar){
		if(selectedP==0)
		 	glColor3f(1.0f,1.0f,1.0f);
		else	
			glColor3f(0.2f,1.0f,0.2f);
		glRasterPos2f(590, 280);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Resume");
		
		// menu'
		if(selectedP==1)
		 	glColor3f(1.0f,1.0f,1.0f);
		else	
			glColor3f(0.2f,1.0f,0.2f);
		glRasterPos2f(590, 330);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Controls");
	
		// quit
		if(selectedP==2)
		 	glColor3f(1.0f,1.0f,1.0f);
		else	
			glColor3f(0.2f,1.0f,0.2f);
		glRasterPos2f(590, 380);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, "  Quit");	

	//box elemento selezionato nell'elenco
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(560, 260+50*selectedP);
			glVertex2f(690, 260+50*selectedP);
			glVertex2f(690, 290+50*selectedP);
			glVertex2f(560, 290+50*selectedP);
		glEnd();
	glPopMatrix();

	}
	else{
		renderingMenu();
	}
	
	glTranslatef(0.0,height,0.0);
	glScalef(1.0,-1.0,1.0);
		
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_LIGHTING); /* disattiviamo le luci */
	glEnable(GL_DEPTH_TEST); /* disabilitiamo z buffering */
	glEnable(GL_CULL_FACE); /* disabilitiamo face culling */
		
	glPopMatrix();	
	glutSwapBuffers();

}


void pausekeyboard(GLubyte key, GLint x, GLint y) {

	if(!menuVar){
		switch (key) {

			case 'p':
			case 'P':
			case KEY_ESC:				
				selectedP=0;
				scene=SOLAR;
				freezeAll(false);
				break;
			case KEY_ENTER: 	
				switch(selectedP){
					// resume
						case 0:
						selectedP=0;
						scene=SOLAR;
						freezeAll(false);
					break;
					// menu'
					case 1:
						menuVar=true;
						
					break;				
					// quit
					case 2:	
						fprintf(stdout, "\n");
						free(*img);						
						exit(0);
					break;
			}
		}
	}else{
		switch (key) {	
		case KEY_ESC:	
			menuVar=false;
			menuIndex=0;			
		break;
		}
	}
}

void pausespecialkeys( GLint key, GLint u, GLint v ){
	
	if(!menuVar){
		switch (key) {	
			case GLUT_KEY_DOWN:
				selectedP=(selectedP+1)%3;
				break;
			
			case GLUT_KEY_UP: 
				selectedP=(3+(selectedP-1))%3;
				break;
		}
	}else{
		switch (key) {
		case GLUT_KEY_LEFT:		
			if(menuIndex==0){
				menuVar=false;
				//menuIndex=0;

			}else{ 
				menuIndex -=1%3;	
			
			}
		break;
		case GLUT_KEY_RIGHT:	
			if(menuIndex==2){
				menuVar=false;
				menuIndex=0;
			}else{	
				menuIndex +=1%3;
			}
		break;		
		}
	}
}

void renderingMenu(){

	glColor3f(0.2f,1.0f,0.2f);	
	glRasterPos2f(350, 650);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,"<----" );

	//glColor3f(0.2f,1.0f,0.2f);
	glRasterPos2f(900, 650);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,"---->" );
	
	//glColor3f(0.2f,1.0f,0.2f);
	
	if(menuIndex==0){// PLAY MODE - MOVEMENT
		//glColor3f(0.2f,1.0f,0.2f);
		//glRasterPos2f(400, 70);
		//renderBitmapString(GLUT_BITMAP_HELVETICA_18,"PLAY MODE:" );
		glRasterPos2f(550, 125);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"1. MOVEMENT" );
		glRasterPos2f(425, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"W key");
		glRasterPos2f(750, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- speed up");
		glRasterPos2f(425, 205);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"S key");
		glRasterPos2f(750, 205);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- slow down");	
		glRasterPos2f(425, 245);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"Q key");
		glRasterPos2f(750, 245);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- extra speed up");	
		glRasterPos2f(425, 285);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"E key");
		glRasterPos2f(750, 285);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- extra slow down");	
		glRasterPos2f(425, 325);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"A/D key");
		glRasterPos2f(750, 325);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- ROLL");	
		glRasterPos2f(425, 365);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"UP/DOWN   arrow");
		glRasterPos2f(425, 390);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"UP/DOWN   MOTION MOUSE");
		glRasterPos2f(750, 365);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- PITCH");		
		glRasterPos2f(425, 430);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"LEFT/RIGHT   arrow");
		glRasterPos2f(425, 455);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"LEFT/RIGHT   MOTION MOUSE");
		glRasterPos2f(750, 430);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- YAW");		
		glRasterPos2f(425, 495);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"WHEEL UP");
		glRasterPos2f(425, 520);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"+ key");
		glRasterPos2f(750, 495);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- more velocity motion");		
		glRasterPos2f(425, 560);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"WHEEL DOWN");
		glRasterPos2f(425, 585);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- key");
		glRasterPos2f(750, 560);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- less velocity motion");	
	}
	// PLAY MODE - FIRE
	if(menuIndex==1){
		//glRasterPos2f(400, 70);
		//renderBitmapString(GLUT_BITMAP_HELVETICA_18,"PLAY MODE:" );
		glRasterPos2f(550, 125);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"2. FIRE" );
		glRasterPos2f(425, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"SPACE key");
		glRasterPos2f(425, 190);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"MOUSE LEFT BUTTON");
		glRasterPos2f(750, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- fire");
		glRasterPos2f(425, 230);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"F key");
		glRasterPos2f(425, 255);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"MOUSE RIGHT BUTTON");
		glRasterPos2f(750, 230);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- extra fire");	
		glRasterPos2f(425, 295);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"WHEEL UP");
		glRasterPos2f(425, 320);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"+ key");
		glRasterPos2f(750, 295);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- less precision");	
		glRasterPos2f(425, 360);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"WHEEL DOWN");
		glRasterPos2f(425, 385);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- key");
		glRasterPos2f(750, 360);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- more precision");
		glRasterPos2f(425, 425);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"F1 key");
		glRasterPos2f(750, 425);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- enter/exit fullscreen");
		glRasterPos2f(425, 465);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"F2 key");
		glRasterPos2f(750, 465);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- change sight");	}
	if(menuIndex==2){
		glRasterPos2f(550, 125);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"3. TXTELITE" );
		glRasterPos2f(425, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"B key");
		glRasterPos2f(750, 165);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- buy a selected item");
		glRasterPos2f(425, 205);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"S key");
		glRasterPos2f(750, 205);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- sell a selected item");	
		glRasterPos2f(425, 245);	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"F key");
		glRasterPos2f(750, 245);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- refuel");	
		glRasterPos2f(425, 285);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"E key");
		glRasterPos2f(750, 285);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- buy energy");	
		glRasterPos2f(425, 325);
	}
	glColor3f(1.0f,1.0f,1.0f);
}
