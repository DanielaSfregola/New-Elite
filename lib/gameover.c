void gameover();
void gameoverkeyboard(GLubyte key, GLint x, GLint y);

void gameover() {	

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
			glBindTexture(GL_TEXTURE_2D, g_textureArray[3]);   	
			glBegin(GL_QUADS);				
				//coordinate del quadrato "schermo"
				glTexCoord2f(0.0f, 1.0f); glVertex3f(0,0,0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(width,0,0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(width,height,0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(0,height,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
		
	// rendering scritta
	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2f(550, 650);
	if(glutGet(GLUT_ELAPSED_TIME)%1500>750)	
		renderBitmapString(GLUT_BITMAP_HELVETICA_18,"Press Esc to exit" );

		glTranslatef(0.0,height,0.0);
		glScalef(1.0,-1.0,1.0);
		
		glMatrixMode(GL_PROJECTION);
		glEnable(GL_LIGHTING); /* disattiviamo le luci */
		glEnable(GL_DEPTH_TEST); /* disabilitiamo z buffering */
		glEnable(GL_CULL_FACE); /* disabilitiamo face culling */
		//glColor3f(0.0f,0.0f,1.0f); //luce ambientale blu per una migliore visualizzazione
	glPopMatrix();	
	glutSwapBuffers();


}


void gameoverkeyboard(GLubyte key, GLint x, GLint y) {
	switch (key) {
		case KEY_ESC:
			printf("\n________________________________\n\n	GAME OVER \n________________________________\n\n");
			free(*img);
			exit(0);
		break;
	}
}

