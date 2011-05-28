/* txtelite.c  1.4 */
/* Textual version of Elite trading (C implementation) */
/* Converted by Ian Bell from 6502 Elite sources.
   Original 6502 Elite by Ian Bell & David Braben. */


/* ----------------------------------------------------------------------
  The nature of basic mechanisms used to generate the Elite socio-economic
universe are now widely known. A competant games programmer should be able to
produce equivalent functionality. A competant hacker should be able to lift 
the exact system from the object code base of official conversions.

  This file may be regarded as defining the Classic Elite universe.

  It contains a C implementation of the precise 6502 algorithms used in the
 original BBC Micro version of Acornsoft Elite together with a parsed textual
 command testbed.

  Note that this is not the universe of David Braben's 'Frontier' series.


ICGB 13/10/99
iancgbell@email.com
www.ibell.co.uk
  ---------------------------------------------------------------------- */



/* Note that this program is "quick-hack" text parser-driven version
of Elite with no combat or missions.
*/

#include <string.h>
#include <time.h>
#include <malloc.h>

#define true (-1)
#define false (0)
#define tonnes (0)#define maxlen (20) /* Length of strings */

#define MAXROCKET 3


typedef int boolean;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef signed short int16;
typedef signed long int32;

typedef int planetnum;


typedef struct
{ uint8 a,b,c,d;
} fastseedtype;  /* four byte random number used for planet description */


typedef struct
{ uint16 w0;
  uint16 w1;
  uint16 w2;
} seedtype;  /* six byte random number used as seed for planets */

typedef struct
{	 uint x;
   uint y;       /* One byte unsigned */
   uint economy; /* These two are actually only 0-7  */
   uint govtype;   
   uint techlev; /* 0-16 i think */
   uint population;   /* One byte */
   uint productivity; /* Two byte */
   uint radius; /* Two byte (not used by game at all) */
	fastseedtype	goatsoupseed;
   char name[12];
} plansys ;

#define galsize (256)
#define AlienItems (16)
#define lasttrade AlienItems

#define numforLave 7       /* Lave is 7th generated planet in galaxy one */
#define numforZaonce 129
#define numforDiso 147
#define numforRied 46

plansys galaxy[galsize]; /* Need 0 to galsize-1 inclusive */

seedtype seed;

fastseedtype rnd_seed;

boolean nativerand;

typedef struct
{                         /* In 6502 version these were: */
   uint baseprice;        /* one byte */
   int16 gradient;   /* five bits plus sign */
   uint basequant;        /* one byte */
   uint maskbyte;         /* one byte */
   uint units;            /* two bits */
   char   name[20];         /* longest="Radioactives" */
  } tradegood ;


typedef struct
{	uint quantity[lasttrade+1];
  uint price[lasttrade+1];
} markettype ;


/*DANY Variabili Globali*/

// si imposta la scena a SOLAR cosi' alla prossima pressione di T si torna al gioco grafico
int rocketNo=3;
int message=-1;
int selected=0;		//selected tradegood 
int genlocalmarket=1; 	//1 se bisogna generare il localmarket. Si genera all'inizio della textscene una sola volta, e viene 
			//settato a 0 quando si esce premendo M
char gift[1024];	//stringa dove viene memorizzato il premio
char gift2[1024];	//stringa dove viene memorizzato il premio
char temp2[1024];	//varie ed eventuali
/* Player workspace */
uint     shipshold[lasttrade+1];  /* Contents of cargo bay */
planetnum  currentplanet;           /* Current planet */
uint     galaxynum;               /* Galaxy number (1-8) */
GLfloat    cash=1000;

markettype localmarket;
uint     holdspace;
GLboolean generate[9]; //mi serve per generare per ogni pianeta 1 solo mercato e un solo premio
time_t last=0.0;
int lastPlanet=-1;
//energy
GLfloat energycost=30;
GLfloat maxenergy=100;

//fuel
GLfloat fuelcost =20; /* 0.2 CR/Light year */
GLfloat maxfuel =1; /* 7.0 LY tank */

const uint16 base0=0x5A4A;
const uint16 base1=0x0248;
const uint16 base2=0xB753;  /* Base seed for galaxy 1 */


//static const char *digrams=
//							 "ABOUSEITILETSTONLONUTHNO"
//							 "ALLEXEGEZACEBISO"
//							 "USESARMAINDIREA?"
//							 "ERATENBERALAVETI"
//							 "EDORQUANTEISRION";

char pairs0[]="ABOUSEITILETSTONLONUTHNO";
/* must continue into .. */
char pairs[] = "..LEXEGEZACEBISO"
               "USESARMAINDIREA."
               "ERATENBERALAVETI"
               "EDORQUANTEISRION"; /* Dots should be nullprint characters */

char govnames[][maxlen]={"Anarchy","Feudal","Multi-gov","Dictatorship",
                    "Communist","Confederacy","Democracy","Corporate State"};

char econnames[][maxlen]={"Rich Ind","Average Ind","Poor Ind","Mainly Ind",
                      "Mainly Agri","Rich Agri","Average Agri","Poor Agri"};


char unitnames[][5] ={"t","kg","g"};

/* Data for DB's price/availability generation system */
/*                   Base  Grad Base Mask Un   Name
                     price ient quant     it              */ 

#define POLITICALLY_CORRECT	0
/* Set to 1 for NES-sanitised trade goods */

tradegood commodities[]=
                   {
                    {0x13,-0x02,0x06,0x01,0,"Food        "},
                    {0x14,-0x01,0x0A,0x03,0,"Textiles    "},
                    {0x41,-0x03,0x02,0x07,0,"Radioactives"},
#if POLITICALLY_CORRECT //DANY da togliere?
                    {0x28,-0x05,0xE2,0x1F,0,"Robot Slaves"},
                    {0x53,-0x05,0xFB,0x0F,0,"Beverages   "},
#else
                    {0x28,-0x05,0xE2,0x1F,0,"Slaves      "},
                    {0x53,-0x05,0xFB,0x0F,0,"Liquor/Wines"},
#endif 
                    {0xC4,+0x08,0x36,0x03,0,"Luxuries    "},
#if POLITICALLY_CORRECT
                    {0xEB,+0x1D,0x08,0x78,0,"Rare Species"},
#else
                    {0xEB,+0x1D,0x08,0x78,0,"Narcotics   "},
#endif 
                    {0x9A,+0x0E,0x38,0x03,0,"Computers   "},
                    {0x75,+0x06,0x28,0x07,0,"Machinery   "},
                    {0x4E,+0x01,0x11,0x1F,0,"Alloys      "},
                    {0x7C,+0x0d,0x1D,0x07,0,"Dark power  "},
                    {0xB0,-0x09,0xDC,0x3F,0,"Furs        "},
                    {0x20,-0x01,0x35,0x03,0,"Minerals    "},
                    {0x61,-0x01,0x42,0x07,1,"Gold        "},
                    {0xAB,-0x02,0x37,0x1F,1,"Platinum    "},
                    {0x2D,-0x01,0xFA,0x0F,2,"Gem-Strones "},
                    {0x35,+0x0F,0xC0,0x07,0,"Alien Items "},
                   };

/**-Required data for text interface **/
char tradnames[lasttrade][maxlen]; /* Tradegood names used in text commands
                                      Set using commodities array */
//DANY elenco dei nomi in transizione
char namesTransition[][maxlen]={"food","textles","radioactives","slaves","liquor/wines","luxuries","narcotics","computers","machinery","alloys","dark power","furs","minerals","gold","platinum","gem-strones","alien items"};
#define nocomms (14)

boolean dobuy(char *);
boolean dosell(char *);
boolean dofuel(char *);
boolean dojump(char *);
boolean docash(char *);
boolean domkt(char *);
boolean dohelp(char *);
boolean dohold(char *);
boolean dosneak(char *);
boolean dolocal(char *);
boolean doinfo(char *);
boolean dogalhyp(char *);
boolean doquit(char *);
boolean dotweakrand(char *);

void goat_soup(const char *source,plansys * psy);
void freezeAll(GLboolean b);

// gestione random dei premi
void randomGift(time_t last, int lastPlanet, int planet);
//aggiungi missili
void addMissiles();
//tolgo i missili
void removeMissiles();
//rivelo il nome del pianeta obbiettivo
char* getPlanetName(int planet);
//rivela commercio illegale
GLboolean isIllegal();


char commands[nocomms][maxlen]=
  {"buy",        "sell",     "fuel",     "jump",
   "cash",       "mkt",      "help",     "hold",
   "sneak",      "local",    "info",     "galhyp",
	 "quit",       "rand"	
  };

boolean (*comfuncs[nocomms])(char *)=
   {dobuy,         dosell,       dofuel,    dojump,
    docash,        domkt,        dohelp,    dohold,
    dosneak,       dolocal,      doinfo,    dogalhyp,
		doquit,				 dotweakrand
  };  

/* DANY: Prototipi di funzione */
void cameraLook(void);
void glmyprint(int x,int y,char *s);
void initTexture(GLuint textureArray[], char  *imageFileName, int textureID);
int txtelite_start();

/**- General functions **/


void port_srand(unsigned int);
int port_rand(void);

static unsigned int lastrand = 0;

void mysrand(unsigned int seed)
{	srand(seed);
	lastrand = seed - 1;
}

int myrand(void)
{	int r;
	if(nativerand) r=rand();
	else
	{	// As supplied by D McDonnell	from SAS Insititute C
		r = (((((((((((lastrand << 3) - lastrand) << 3)
        + lastrand) << 1) + lastrand) << 4)
        - lastrand) << 1) - lastrand) + 0xe60)
        & 0x7fffffff;
    lastrand = r - 1;	
	}
	return(r);
}

char randbyte(void)	{ return (char)(myrand()&0xFF);}

uint mymin(uint a,uint b) { if(a<b) return(a);	else return(b);}

void stop(char * string)
{ printf("\n%s",string);
  exit(1);
}

 /**+  ftoi **/
signed int ftoi(double value)
{ return ((signed int)floor(value+0.5));
}

 /**+  ftoi2 **/
signed int ftoi2(double value)
{ return ((signed int)floor(value));
}

void tweakseed(seedtype *s)
{ uint16 temp;
  temp = ((*s).w0)+((*s).w1)+((*s).w2); /* 2 byte aritmetic */
  (*s).w0 = (*s).w1;
  (*s).w1 = (*s).w2;
  (*s).w2 = temp;
}

/**-String functions for text interface **/

void stripout(char *s,const char c) /* Remove all c's from string s */
{   size_t i=0,j=0;
    while(i<strlen(s))
    { if(s[i]!=c) { s[j]=s[i]; j++;}
      i++;
    }
    s[j]=0;
}

int toupper(char c)
{	if((c>='a')&&(c<='z')) return(c+'A'-'a');
	return((int)c);
}

int tolower(char c)
{	if((c>='A')&&(c<='Z')) return(c+'a'-'A');
	return((int)c);
}


int stringbeg(char *s,char *t)
/* Return nonzero iff string t begins with non-empty string s */
{ size_t i=0;
  size_t l=strlen(s);
  if(l>0)
  { while((i<l)&(toupper(s[i])==toupper(t[i])))	i++;
  	if(i==l) return true;
  }
  return false;
}

uint stringmatch(char *s,char a[][20],uint n)
/* Check string s against n options in string array a
   If matches ith element return i+1 else return 0 */
{	uint i=0;    
  while(i<n)
  { if(stringbeg(s,a[i])) return i+1;
    i++;
  }
return 0;
}

void spacesplit(char *s,char *t)
/* Split string s at first space, returning first 'word' in t & shortening s
*/
{   size_t i=0,j=0;
    size_t l=strlen(s);
    while((i<l)&(s[i]==' ')) i++;; /* Strip leading spaces */
    if(i==l) {s[0]=0; t[0]=0; return;};
    while((i<l)&(s[i]!=' ')) t[j++]=s[i++];
    t[j]=0;	i++; j=0;
    while(i<l) s[j++]=s[i++];
    s[j]=0;
}

/**-Functions for stock market **/

uint gamebuy(uint i, uint a)
 /* Try to buy ammount a  of good i  Return ammount bought */
 /* Cannot buy more than is availble, can afford, or will fit in hold */
{   
	if(cash>=(localmarket.price[i]/10.0) && localmarket.quantity[i]>=a){
		cash-=a*(localmarket.price[i]/10.0);
		localmarket.quantity[i]-=a;
		shipshold[i]+=a;
		if(i==10) nSuperShoot+=a;
		return 1;
	}
	return 0;

/*uint t;
    if(cash<0) t=0;
    else
    {	t=mymin(localmarket.quantity[i],a);
    	if ((commodities[i].units)==tonnes) {t = mymin(holdspace,t);}
    	t = mymin(t, (uint)floor((double)cash/(localmarket.price[i])));
    }
		shipshold[i]+=t;
    localmarket.quantity[i]-=t;
    cash-=t*(localmarket.price[i]);
    if ((commodities[i].units)==tonnes) {holdspace-=t;}
		return t;*/
}

uint gamesell(uint i,uint a) /* As gamebuy but selling */
{   
	if(shipshold[i]>=a){
		cash+=a*(localmarket.price[i]/10.0);
		localmarket.quantity[i]+=a;
		shipshold[i]-=a;		
		if(i==10) nSuperShoot-=a;
		return 1;
	}
	return 0;
/*uint t=mymin(shipshold[i],a);
    shipshold[i]-=t;
    localmarket.quantity[i]+=t;
    if ((commodities[i].units)==tonnes) {holdspace+=t;}
    cash+=t*(localmarket.price[i]);
    return t;*/
}

markettype genmarket(uint fluct, plansys p)
/* Prices and availabilities are influenced by the planet's economy type
   (0-7) and a random "fluctuation" byte that was kept within the saved
   commander position to keep the market prices constant over gamesaves.
   Availabilities must be saved with the game since the player alters them
   by buying (and selling(?))

   Almost all operations are one byte only and overflow "errors" are
   extremely frequent and exploited.

   Trade Item prices are held internally in a single byte=true value/4.
   The decimal point in prices is introduced only when printing them.
   Internally, all prices are integers.
   The player's cash is held in four bytes. 
 */

{	markettype market;
  unsigned short i;
  for(i=0;i<=lasttrade;i++)
  {	signed int q; 
    signed int product = (p.economy)*(commodities[i].gradient);
    signed int changing = fluct & (commodities[i].maskbyte);
		q =  (commodities[i].basequant) + changing - product;	
    q = q&0xFF;
    if(q&0x80) {q=0;};                       /* Clip to positive 8-bit */

    market.quantity[i] = (uint16)(q & 0x3F); /* Mask to 6 bits */

    q =  (commodities[i].baseprice) + changing + product;
    q = q & 0xFF;
    market.price[i] = (uint16) (q*4);
  }
	market.quantity[AlienItems] = 0; /* Override to force nonavailability */	
	return market;
}

void displaymarket(markettype m)
{	unsigned short i;
 	for(i=0;i<=lasttrade;i++)
 	{ printf("\n");
   printf(commodities[i].name);
   printf("   %.1f",((float)(m.price[i])/10));
   printf("   %u",m.quantity[i]);
   printf(unitnames[commodities[i].units]);
   printf("   %u",shipshold[i]);
 }
}	

/**-Generate system info from seed **/

plansys makesystem(seedtype *s)
{	plansys thissys; 
  uint pair1,pair2,pair3,pair4;
  uint16 longnameflag=((*s).w0)&64;
 
  thissys.x=(((*s).w1)>>8);
  thissys.y=(((*s).w0)>>8);

  thissys.govtype =((((*s).w1)>>3)&7); /* bits 3,4 &5 of w1 */

  thissys.economy =((((*s).w0)>>8)&7); /* bits 8,9 &A of w0 */
  if (thissys.govtype <=1)
  { thissys.economy = ((thissys.economy)|2);
  } 

  thissys.techlev =((((*s).w1)>>8)&3)+((thissys.economy)^7);
  thissys.techlev +=((thissys.govtype)>>1);
  if (((thissys.govtype)&1)==1)	thissys.techlev+=1;
   /* C simulation of 6502's LSR then ADC */
 
  thissys.population = 4*(thissys.techlev) + (thissys.economy);
  thissys.population +=  (thissys.govtype) + 1;

  thissys.productivity = (((thissys.economy)^7)+3)*((thissys.govtype)+4);
  thissys.productivity *= (thissys.population)*8;

  thissys.radius = 256*(((((*s).w2)>>8)&15)+11) + thissys.x;  

	thissys.goatsoupseed.a = (*s).w1 & 0xFF;;
	thissys.goatsoupseed.b = (*s).w1 >>8;
	thissys.goatsoupseed.c = (*s).w2 & 0xFF;
	thissys.goatsoupseed.d = (*s).w2 >> 8;

  pair1=2*((((*s).w2)>>8)&31);  tweakseed(s);
  pair2=2*((((*s).w2)>>8)&31);  tweakseed(s);
  pair3=2*((((*s).w2)>>8)&31);  tweakseed(s);
  pair4=2*((((*s).w2)>>8)&31);	tweakseed(s);
   /* Always four iterations of random number */

  (thissys.name)[0]=pairs[pair1];
  (thissys.name)[1]=pairs[pair1+1];
  (thissys.name)[2]=pairs[pair2];
  (thissys.name)[3]=pairs[pair2+1];
  (thissys.name)[4]=pairs[pair3];
  (thissys.name)[5]=pairs[pair3+1];

  if(longnameflag) /* bit 6 of ORIGINAL w0 flags a four-pair name */
  {
  (thissys.name)[6]=pairs[pair4];
  (thissys.name)[7]=pairs[pair4+1];
  (thissys.name)[8]=0;
  }
  else (thissys.name)[6]=0;
  stripout(thissys.name,'.');



return thissys;
}  


/**+Generate galaxy **/


/* Functions for galactic hyperspace */

uint16 rotatel(uint16 x) /* rotate 8 bit number leftwards */
  /* (tried to use chars but too much effort persuading this braindead
     language to do bit operations on bytes!) */
{ uint16 temp = x&128;
	return (2*(x&127))+(temp>>7);
} 

uint16 twist(uint16 x)
{ return (uint16)((256*rotatel(x>>8))+rotatel(x&255));
} 

void nextgalaxy(seedtype *s) /* Apply to base seed; once for galaxy 2  */
{ (*s).w0 = twist((*s).w0);  /* twice for galaxy 3, etc. */
  (*s).w1 = twist((*s).w1);  /* Eighth application gives galaxy 1 again*/
  (*s).w2 = twist((*s).w2);
}

/* Original game generated from scratch each time info needed */
void buildgalaxy(uint galaxynum)
{	uint syscount,galcount;
	seed.w0=base0; seed.w1=base1; seed.w2=base2; /* Initialise seed for galaxy 1 */
	for(galcount=1;galcount<galaxynum;++galcount) nextgalaxy(&seed);
	/* Put galaxy data into array of structures */  
  for(syscount=0;syscount<galsize;++syscount) galaxy[syscount]=makesystem(&seed);
}

/**-Functions for navigation **/

void gamejump(planetnum i) /* Move to system i */
{ currentplanet=i;
  localmarket = genmarket(randbyte(),galaxy[i]);
}

uint distance(plansys a,plansys b)
/* Seperation between two planets (4*sqrt(X*X+Y*Y/4)) */
{	return (uint)ftoi(4*sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)/4));
}


planetnum matchsys(char *s)
/* Return id of the planet whose name matches passed strinmg
   closest to currentplanet - if none return currentplanet */
{	 planetnum syscount;
   planetnum p=currentplanet;
   uint d=9999;
	 for(syscount=0;syscount<galsize;++syscount)
   { if (stringbeg(s,galaxy[syscount].name))
     {	if (distance(galaxy[syscount],galaxy[currentplanet])<d)
      	{ d=distance(galaxy[syscount],galaxy[currentplanet]);
        	p=syscount;
      	}
    	}
   }
	return p;
}


/**-Print data for given system **/
void prisys(plansys plsy,boolean compressed)
{	if (compressed)
	{	//	uint i; DANY variabile nn usata
   //	  printf("\n ");
	  printf("%10s",plsy.name);
  	printf(" TL: %2i ",(plsy.techlev)+1);
  	printf("%12s",econnames[plsy.economy]);
  	printf(" %15s",govnames[plsy.govtype]);
	}
	else
	{	printf("\n\nSystem:  ");
  	printf(plsy.name);
  	printf("\nPosition (%i,",plsy.x);
  	printf("%i)",plsy.y);
  	printf("\nEconomy: (%i) ",plsy.economy);
  	printf(econnames[plsy.economy]);
  	printf("\nGovernment: (%i) ",plsy.govtype);
  	printf(govnames[plsy.govtype]);
  	printf("\nTech Level: %2i",(plsy.techlev)+1);
  	printf("\nTurnover: %u",(plsy.productivity));
  	printf("\nRadius: %u",plsy.radius);
  	printf("\nPopulation: %u Billion",(plsy.population)>>3);
	
		rnd_seed = plsy.goatsoupseed;
		printf("\n");goat_soup("\x8F is \x97.",&plsy);
	}
}

/**-Various command functions **/

boolean dotweakrand(char *s)
{	 nativerand ^=1;
   return true;
}

boolean dolocal(char *s)
{	 planetnum syscount;
   uint d;
//   atoi(s); 
   printf("Galaxy number %i",galaxynum);
   for(syscount=0;syscount<galsize;++syscount)
   { 	d=distance(galaxy[syscount],galaxy[currentplanet]);
   		if(d<=maxfuel)
    	{ 	if(d<=fuel)	printf("\n * "); else printf("\n - ");
    		prisys(galaxy[syscount],true);
      		printf(" (%.1f LY)",(float)d/10);
    	}
   }
   return true;
}


boolean dojump(char *s) /* Jump to planet name s */
{ uint d;
  planetnum dest=matchsys(s);
  if(dest==currentplanet) { printf("\nBad jump"); return false; }
  d=distance(galaxy[dest],galaxy[currentplanet]);
  if (d>fuel) { printf("\nJump to far"); return false; }
  fuel-=d;
  gamejump(dest);
  prisys(galaxy[currentplanet],false);
  return true;
}

boolean dosneak(char *s) /* As dojump but no fuel cost */
{	uint fuelkeep=fuel;
  boolean b;
  fuel=666;
  b=dojump(s);
  fuel=fuelkeep;
  return b;
}

boolean dogalhyp(char *s) /* Jump to next galaxy */
                          /* Preserve planetnum (eg. if leave 7th planet
                             arrive at 7th planet) */
{	(void)(&s);     /* Discard s */
  galaxynum++;
  if(galaxynum==9) {galaxynum=1;}
  buildgalaxy(galaxynum);
  return true;
}

boolean doinfo(char *s) /* Info on planet */
{	planetnum dest=matchsys(s);
  prisys(galaxy[dest],false);
	return true;
}


boolean dohold(char *s)
{	uint a=(uint)atoi(s),t=0,i;
  for(i=0;i<=lasttrade;++i)
  {	if ((commodities[i].units)==tonnes) t+=shipshold[i];
  }
  if(t>a) {printf("\nHold too full"); return false;}
  holdspace=a-t;
  return true;
}

boolean dosell(char *s) /* Sell ammount S(2) of good S(1) */
{	uint i,a,t;
  char s2[maxlen];
  spacesplit(s,s2);
  a=(uint)atoi(s);
  if (a==0) {a=1;}
  i=stringmatch(s2,tradnames,lasttrade+1);
  if(i==0) { printf("\nUnknown trade good"); return false; } 
  i-=1;
 
  t=gamesell(i,a);

  if(t==0) { printf("Cannot sell any "); }
  else
  {	printf("\nSelling %i",t);
    printf(unitnames[commodities[i].units]);
    printf(" of ");
  }
    printf(tradnames[i]);

    return true;

}

   
boolean dobuy(char *s) /* Buy ammount S(2) of good S(1) */
{	uint i,a,t;
  char s2[maxlen];
  spacesplit(s,s2);
	a=(uint)atoi(s);
  if (a==0) a=1;
	i=stringmatch(s2,tradnames,lasttrade+1);
  if(i==0) { printf("\nUnknown trade good"); return false; } 
  i-=1;

  t=gamebuy(i,a);
	if(t==0) printf("Cannot buy any ");
  else
  { printf("\nBuying %i",t);
    printf(unitnames[commodities[i].units]);
    printf(" of ");
  }
  printf(tradnames[i]);
  return true;
}

uint gamefuel(GLfloat f) /* Attempt to buy f tonnes of fuel */
{
	if(f+fuel>maxfuel)  f=maxfuel-fuel;
	if(fuelcost<=cash){
 		fuel+=f;
  		cash-=fuelcost;
		return 1;
	}
  	return 0;
}

// funzione che mi ricarica di energia
uint gamenergy(GLfloat e) /* Attempt to buy f tonnes of fuel */
{
	if(e+energy>maxenergy)  e=maxenergy-energy;
	if(energycost<=cash){
 		energy+=e;
  		cash-=energycost;
		return 1;
	}
  	return 0;
}

boolean dofuel(char *s)
/* Buy ammount S of fuel */
{	uint f=gamefuel((uint)floor(10*atof(s)));
  if(f==0) { printf("\nCan't buy any fuel");}
  printf("\nBuying %.1fLY fuel",(float)f);
  return true;
}

boolean docash(char *s) /* Cheat alter cash by S */
{	int a=(int)(10*atof(s));
  cash+=(long)a;
  if(a!=0) return true;
  printf("Number not understood");
  return false;
}

boolean domkt(char *s) /* Show stock market */
{ // atoi(s); nn ha alcun effetto
  displaymarket(localmarket);
  printf("\nFuel :%.1f",(float)fuel); // tolto il : 10
  printf("      Holdspace :%it",holdspace);
  return true;
}

boolean parser(char *s) /* Obey command s */
{  uint i;
   char c[maxlen];
   spacesplit(s,c);
   i=stringmatch(c,commands,nocomms);
   if(i)return (*comfuncs[i-1])(s) ;
   printf("\n Bad command (");
   printf(c);
   printf(")");
   return false;
}


boolean doquit(char *s)
{	(void)(&s);
	exit(0);
	return(0);
}

boolean dohelp(char *s)
{
   (void)(&s);
   printf("\nCommands are:");
   printf("\nBuy   tradegood ammount");
   printf("\nSell  tradegood ammount");
   printf("\nFuel  ammount    (buy ammount LY of fuel)");
   printf("\nJump  planetname (limited by fuel)");
   printf("\nSneak planetname (any distance - no fuel cost)");
   printf("\nGalhyp           (jumps to next galaxy)");
   printf("\nInfo  planetname (prints info on system");
   printf("\nMkt              (shows market prices)");
   printf("\nLocal            (lists systems within 7 light years)");
   printf("\nCash number      (alters cash - cheating!)");
   printf("\nHold number      (change cargo bay)");
   printf("\nQuit or ^C       (exit)");
   printf("\nHelp             (display this text)");
 	 printf("\nRand             (toggle RNG)");
   printf("\n\nAbbreviations allowed eg. b fo 5 = Buy Food 5, m= Mkt");
return true;
}
			 
/**+main di txtelite **/
int txtelite_start()
{	 uint i;
   char getcommand[maxlen];
   nativerand=1;
   for(i=0;i<=lasttrade;i++) strcpy(tradnames[i],commodities[i].name);

   mysrand(12345);/* Ensure repeatability */

   galaxynum=1;	buildgalaxy(galaxynum);

   currentplanet=numforLave;                        /* Don't use jump */
   localmarket = genmarket(0x00,galaxy[numforLave]);/* Since want seed=0 */

   fuel=maxfuel;

   
#define PARSER(S) { char buf[0x10];strcpy(buf,S);parser(buf);}   
   
   PARSER("hold 20");         /* Small cargo bay */
   PARSER("cash +100");       /* 100 CR */
   PARSER("help");

#undef PARSER

   for(;;)
   { printf("\n\nCash :%.1f>",((float)cash)); // tolto il : 10
     fgets(getcommand,maxlen,stdin);
     i=0;
     while((getcommand[i]!='\n') && (i<maxlen)) i++;
     getcommand[i]=0;
     parser(getcommand);
   } 

 
   /* 6502 Elite fires up at Lave with fluctuation=00
      and these prices tally with the NES ones.
      However, the availabilities reside in the saved game data.
      Availabilities are calculated (and fluctuation randomised)
      on hyperspacing
      I have checked with this code for Zaonce with fluctaution &AB 
      against the SuperVision 6502 code and both prices and availabilities tally.
   */
return(0);
}

/* "Goat Soup" planetary description string code - adapted from Christian Pinder's
  reverse engineered sources. */

struct desc_choice {	const char *option[5];};

static struct desc_choice desc_list[] =
{
/* 81 */	{"fabled", "notable", "well known", "famous", "noted"},
/* 82 */	{"very", "mildly", "most", "reasonably", ""},
/* 83 */	{"ancient", "\x95", "great", "vast", "pink"},
/* 84 */	{"\x9E \x9D plantations", "mountains", "\x9C", "\x94 forests", "oceans"},
/* 85 */	{"shyness", "silliness", "mating traditions", "loathing of \x86", "love for \x86"},
/* 86 */	{"food blenders", "tourists", "poetry", "discos", "\x8E"},
/* 87 */	{"talking tree", "crab", "bat", "lobst", "\xB2"},
/* 88 */	{"beset", "plagued", "ravaged", "cursed", "scourged"},
/* 89 */	{"\x96 civil war", "\x9B \x98 \x99s", "a \x9B disease", "\x96 earthquakes", "\x96 solar activity"},
/* 8A */	{"its \x83 \x84", "the \xB1 \x98 \x99","its inhabitants' \x9A \x85", "\xA1", "its \x8D \x8E"},
/* 8B */	{"juice", "brandy", "water", "brew", "gargle blasters"},
/* 8C */	{"\xB2", "\xB1 \x99", "\xB1 \xB2", "\xB1 \x9B", "\x9B \xB2"},
/* 8D */	{"fabulous", "exotic", "hoopy", "unusual", "exciting"},
/* 8E */	{"cuisine", "night life", "casinos", "sit coms", " \xA1 "},
/* 8F */	{"\xB0", "The planet \xB0", "The world \xB0", "This planet", "This world"},
/* 90 */	{"n unremarkable", " boring", " dull", " tedious", " revolting"},
/* 91 */	{"planet", "world", "place", "little planet", "dump"},
/* 92 */	{"wasp", "moth", "grub", "ant", "\xB2"},
/* 93 */	{"poet", "arts graduate", "yak", "snail", "slug"},
/* 94 */	{"tropical", "dense", "rain", "impenetrable", "exuberant"},
/* 95 */	{"funny", "wierd", "unusual", "strange", "peculiar"},
/* 96 */	{"frequent", "occasional", "unpredictable", "dreadful", "deadly"},
/* 97 */	{"\x82 \x81 for \x8A", "\x82 \x81 for \x8A and \x8A", "\x88 by \x89", "\x82 \x81 for \x8A but \x88 by \x89","a\x90 \x91"},
/* 98 */	{"\x9B", "mountain", "edible", "tree", "spotted"},
/* 99 */	{"\x9F", "\xA0", "\x87oid", "\x93", "\x92"},
/* 9A */	{"ancient", "exceptional", "eccentric", "ingrained", "\x95"},
/* 9B */	{"killer", "deadly", "evil", "lethal", "vicious"},
/* 9C */	{"parking meters", "dust clouds", "ice bergs", "rock formations", "volcanoes"},
/* 9D */	{"plant", "tulip", "banana", "corn", "\xB2weed"},
/* 9E */	{"\xB2", "\xB1 \xB2", "\xB1 \x9B", "inhabitant", "\xB1 \xB2"},
/* 9F */	{"shrew", "beast", "bison", "snake", "wolf"},
/* A0 */	{"leopard", "cat", "monkey", "goat", "fish"},
/* A1 */	{"\x8C \x8B", "\xB1 \x9F \xA2","its \x8D \xA0 \xA2", "\xA3 \xA4", "\x8C \x8B"},
/* A2 */	{"meat", "cutlet", "steak", "burgers", "soup"},
/* A3 */	{"ice", "mud", "Zero-G", "vacuum", "\xB1 ultra"},
/* A4 */	{"hockey", "cricket", "karate", "polo", "tennis"}
};

/* B0 = <planet name>
	 B1 = <planet name>ian
	 B2 = <random name>
*/

int gen_rnd_number (void)
{	int a,x;
	x = (rnd_seed.a * 2) & 0xFF;
	a = x + rnd_seed.c;
	if (rnd_seed.a > 127)	a++;
	rnd_seed.a = a & 0xFF;
	rnd_seed.c = x;

	a = a / 256;	/* a = any carry left from above */
	x = rnd_seed.b;
	a = (a + x + rnd_seed.d) & 0xFF;
	rnd_seed.b = a;
	rnd_seed.d = x;
	return a;
}


void goat_soup(const char *source,plansys * psy)
{	for(;;)
	{	uint8 c=*(source++);
		if(c=='\0')	break;
		if(c<0x80) printf("%c",c);
		else
		{	if (c <=0xA4)
			{	int rnd = gen_rnd_number();
				goat_soup(desc_list[c-0x81].option[(rnd >= 0x33)+(rnd >= 0x66)+(rnd >= 0x99)+(rnd >= 0xCC)],psy);
			}
			else switch(c)
			{ case 0xB0: /* planet name */
		 		{ int i=1;
					printf("%c",psy->name[0]);
					while(psy->name[i]!='\0') printf("%c",tolower(psy->name[i++]));
				}	break;
				case 0xB1: /* <planet name>ian */
				{ int i=1;
					printf("%c",psy->name[0]);
					while(psy->name[i]!='\0')
					{	if((psy->name[i+1]!='\0') || ((psy->name[i]!='E')	&& (psy->name[i]!='I')))
						printf("%c",tolower(psy->name[i]));
						i++;
					}
					printf("ian");
				}	break;
				case 0xB2: /* random name */
				{	int i;
					int len = gen_rnd_number() & 3;
					for(i=0;i<=len;i++)
					{	int x = gen_rnd_number() & 0x3e;
						if(pairs0[x]!='.') printf("%c",pairs0[x]);
						if(i && (pairs0[x+1]!='.')) printf("%c",pairs0[x+1]);
					}
				}	break;
				default: printf("<bad char in data [%X]>",c); return;
			}	/* endswitch */
		}	/* endelse */
	}	/* endwhile */
}	/* endfunc */

/**+end **/

//*****************************************************************************************************************************************************


//void textInit(void) { // DANY funzione nn utilizzata!
/* Setta il clear color a nero */
	/*DANY*/
/*	scene=SOLAR;
	message=-1;
	
	int i=0;
	localmarket = genmarket(0x00,galaxy[numforLave]);*//* Since want seed=0 *//*
	glClearColor(0.0, 0.0, 0.0, 1.0);
	*//* Setta la scena */
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLdouble) width / (GLdouble) height, 0.1f, 5.0f);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	
	//pulisco la stringa utilizzata per stampare a video
	for(i=0; i<1024;i++) temp[i]=' ';
	txtelite_start();
	shipshold[10]=rocketNo;
}

*/
void textScene(int planet) {	
	if(genlocalmarket){
		if(!generate[planet]){
			localmarket = genmarket(randbyte(),galaxy[planet]);
			generate[planet]=true;	
			// evito che un pianeta nn abbia degli elementi a priori
			for(int i=0; i<17; i++)
				if(localmarket.quantity[i]==0)
					localmarket.quantity[i]=(rand() % 8 + 1);
		}	
	// evito che un pianeta nn abbia degli elementi a priori
	for(int i=0; i<17; i++)
		if(localmarket.quantity[i]==0)
			localmarket.quantity[i]=(rand() % 8 + 1);	

	genlocalmarket=0;
	message=-1;		
	randomGift(last, lastPlanet, planet);
	time(&last);
	lastPlanet=planet;
	}



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
			glBindTexture(GL_TEXTURE_2D, g_textureArray[1]);   	
			glBegin(GL_QUADS);				//coordinate del quadrato "schermo"
				glTexCoord2f(0.0f, 1.0f); glVertex3f(0,0,0);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(width,0,0);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(width,height,0);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(0,height,0);
			glEnd();
		glDisable(GL_TEXTURE_2D);
		
		
	unsigned short i;


	glColor3f(0.0f,0.5f,1.0f);	
	glRasterPos2f(80, 60);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, getPlanetName(planet));

	glRasterPos2f(580, 40);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, temp);
	
	//intestazione tabella
	glColor3f(0.2f,1.0f,0.2f);
	glRasterPos2f(370, 90);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Items");
	glRasterPos2f(590, 90);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Price");
	glRasterPos2f(700, 90);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Quantity");
	glRasterPos2f(820, 90);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18, "Owned");
	
	glPushMatrix();
		glLineWidth( 2.0 );
		glBegin( GL_LINES);
			glVertex2f(320, 100);
			glVertex2f(920, 100);
		glEnd();
	glPopMatrix();
	
	for(i=0;i<=lasttrade;i++){
		if(i==selected) glColor3f(1.0f,1.0f,1.0f);
		else glColor3f(0.2f,1.0f,0.2f);
		glRasterPos2f(370, 140+30*i);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, commodities[i].name);
		
		sprintf(temp,"%.1f $",((float)(localmarket.price[i])/10));
		glRasterPos2f(590, 140+30*i);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, temp);
		
		if( i==3 || i==7 || i==10 || i==11)
			sprintf(temp,"%u",localmarket.quantity[i]);
		else
			sprintf(temp,"%u%s",localmarket.quantity[i],unitnames[commodities[i].units]);
		glRasterPos2f(713, 140+30*i);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, temp);
		

		if(i==10)
			sprintf(temp,"%u/%d",shipshold[i],MAXROCKET);
		else if( i==3 || i==7 || i==11)
			sprintf(temp,"%u",shipshold[i]);
		else
			sprintf(temp,"%u%s",shipshold[i],unitnames[commodities[i].units]);
		glRasterPos2f(820, 140+30*i);
		renderBitmapString(GLUT_BITMAP_HELVETICA_18, temp);
	}
	
	//box elemento selezionato nell'elenco
	glPushMatrix();
		glColor3f(1.0f,1.0f,1.0f);
		glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(350, 120+30*selected);
			glVertex2f(870, 120+30*selected);
			glVertex2f(870, 145+30*selected);
			glVertex2f(350, 145+30*selected);
		glEnd();
	glPopMatrix();

	//box istruzioni
	glColor3f(1.0f,0.0f,0.0f);
	glPushMatrix();
		glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(1010, 60);
			glVertex2f(1260, 60);
			glVertex2f(1260, 365);
			glVertex2f(1010, 365);		
		glEnd();
	glPopMatrix();	

	//istruzioni
	glRasterPos2f(1060, 90);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,"INSTRUCTIONS:" );
	glRasterPos2f(1033, 125);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,"- Press B to buy a unit" );
	glRasterPos2f(1043, 150);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18," of the selected item" );
	glRasterPos2f(1033, 190);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18," - Press S to sell a unit" );
	glRasterPos2f(1043, 215);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18," of the selected item" );
	
	glRasterPos2f(1033, 255);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18," - Press F to refuel" );
	
	sprintf(temp,"(+20%% for %g $)",(float)fuelcost); 
	glRasterPos2f(1053, 280);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);	

	glRasterPos2f(1033, 320);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18," - Press E to buy energy" );
	
	sprintf(temp,"(+20%% for %d $)",(int)energycost); 
	glRasterPos2f(1053, 345);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);

	
	//missione
	glColor3f(1.0f,0.0f,0.0f);
	if(!treasure){
	sprintf(temp, "MISSION:");
	sprintf(temp2, "			- find the Treasure");
	}
	else{
	sprintf(temp, "MISSION COMPLETED:");
	sprintf(temp2,"				well done! =)");
	}
	glRasterPos2f(25, 275);		
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);
	glRasterPos2f(25, 300);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp2);	
	//genero la stringa del premio
	glColor3f(0.0f,0.5f,1.0f);	
	glRasterPos2f(50, 350);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,gift);	
	glRasterPos2f(50, 375);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,gift2);	

	 //box messaggi
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(290, 630);
			glVertex2f(950, 630);
			glVertex2f(950, 705);
			glVertex2f(290, 705);
		glEnd();
	glPopMatrix();

 	//box principale
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
	glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(258, 15);
			glVertex2f(991, 15);
			glVertex2f(991, 725);
			glVertex2f(258, 725);
		glEnd();
	glPopMatrix();	

	switch (message){
		//buy
		case 0: glColor3f(1.0f,0.0f,0.0f); sprintf(temp,"You cannot buy %s",namesTransition[selected]);  break;
		case 1:	glColor3f(0.0f,1.0f,0.0f); sprintf(temp,"You have bought %s",namesTransition[selected]); break;
		//sell
		case 2: glColor3f(1.0f,0.0f,0.0f); sprintf(temp,"No %s to sell",namesTransition[selected]); break;
		case 3: glColor3f(0.0f,1.0f,0.0f); sprintf(temp,"You have sell %s",namesTransition[selected]); break;
		//fuel
		case 4: glColor3f(1.0f,0.0f,0.0f); sprintf(temp,"You cannot refuel"); break;
		case 5: glColor3f(0.0f,1.0f,0.0f); sprintf(temp,"You have refuel (+20%)"); break;
		//energy
		case 6: glColor3f(1.0f,0.0f,0.0f); sprintf(temp,"You cannot buy energy"); break;
		case 7: glColor3f(0.0f,1.0f,0.0f); sprintf(temp,"You have bought energy (+20%)"); break;
		//firearms
		case 8: glColor3f(1.0f,0.0f,0.0f); sprintf(temp,"You cannot buy more than 3 dark power."); break;
		default: sprintf(temp," "); break;
	}
	glRasterPos2f(475, 670);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);

	//box di gioco
	glColor3f(1.0f,1.0f,1.0f);
	glPushMatrix();
		glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(30, 100);
			glVertex2f(220, 100);
			glVertex2f(220, 220);
			glVertex2f(30, 220);		
		glEnd();
	glPopMatrix();	

	//energy	
	sprintf(temp, "Energy	%d%%",(int)(energy));
	glRasterPos2f(63, 130);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);	
	
	//fuel
	sprintf(temp, "Fuel	%d%%",(int)(fuel*100));
	glRasterPos2f(63, 165);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);

	//cash
	sprintf(temp, "Cash	%g $",(float)cash);
	glRasterPos2f(63, 200);
	renderBitmapString(GLUT_BITMAP_HELVETICA_18,temp);
		glColor3f(1.0f,1.0f,1.0f);
		
		glTranslatef(0.0,height,0.0);
		glScalef(1.0,-1.0,1.0);
		
		glMatrixMode(GL_PROJECTION);
		glEnable(GL_LIGHTING); /* disattiviamo le luci */
		glEnable(GL_DEPTH_TEST); /* disabilitiamo z buffering */
		glEnable(GL_CULL_FACE); /* disabilitiamo face culling */
		//glColor3f(0.0f,0.0f,1.0f); //luce ambientale blu per una migliore visualizzazione
	glPopMatrix();	
	glutSwapBuffers();


	 //box premi
	
	/*glPushMatrix();
	glLineWidth( 0.4 );
		glBegin( GL_LINE_LOOP);
			glVertex2f(30, 250);
			glVertex2f(220, 250);
			glVertex2f(220, 450);
			glVertex2f(30, 450);
		glEnd();
	glPopMatrix();*/
}

void glmyprint(int x,int y,char *s,int size){
		
						
}

void textkeyboard(GLubyte key, GLint x, GLint y) {
	switch (key) {
		case 'b':
		case 'B':
			if(selected==10){//ho selezionato i missili
				if(shipshold[10]<MAXROCKET)
					message=gamebuy(10,1);
				else
					message=8; //impossibile comprare altri missili
			}else{
				message=gamebuy(selected,1);
			}
			break;
		case 's':
		case 'S':
			/*
			if(selected==10){//ho selezionato i missili
				((shipshold[10]>0)?message=gamesell(10,1)+2:message=2);
				//if(message=3) rocketNo--;
			}else{*/
				message=gamesell(selected,1)+2;
			//}
			break;
		case 'f':
		case 'F':
			message=gamefuel(0.2)+4;
			break;
		case 'e':
		case 'E':
			message=gamenergy(20)+6;
			break;	
		case 't':
		case 'T':
		case KEY_ESC:				
			selected=0;
			genlocalmarket=1;
			scene=SOLAR;
			freezeAll(false);
			break;
		
			/* Exit when the Escape key is pressed */
//			fprintf(stdout, "\n");
//		exit(0);
	}
}

GLvoid textspecialkeys( GLint key, GLint u, GLint v ){

	switch (key) {	
		case GLUT_KEY_DOWN:
			selected=(selected+1)%17;
			message=-1;
			break;
		
		case GLUT_KEY_UP: 
			selected=(17+(selected-1))%17;
			message=-1;
			break;
	}

}

uint* getRocketNo(){
	return &shipshold[10];
}

/********************************************************************/
// Gestione random dei premi

void randomGift(time_t last, int lastPlanet, int planet){
	
	time_t now; //tempo attuale
	double diff; //differenza di tempo
	int i;	
	srand(time(NULL)); 
	i = rand() % 8 + 1;

	time(&now);
	diff=difftime(now,last);
	//printf("time %d....    ", (int )diff);	
	
	//ANTIFURBETTO: se nn sono passati 2 minuti dall'ultimo accesso allo stesso pianeta forzo i
	if(diff<120 && lastPlanet==planet) i=0;

	//printf("numero generato %d \n", i);
	switch (i) {
	case 1:
		if(!treasure){
			sprintf(gift, "Mission Completed:");
			sprintf(gift2, "Treasure found! =)");
			sprintf(temp,"");
			treasure=true;
		}
		break;

	case 2:
		fuel+=0.2;
		if(fuel>1) fuel=1;
		sprintf(gift, "Fuel:");
		sprintf(gift2, " + 20%%");
		sprintf(temp,"");
		break;

	
	case 3:
		fuel+=0.5;		
		if(fuel>1) fuel=1;
		sprintf(gift, "Fuel:");
		sprintf(gift2, " + 50%%");
		sprintf(temp,"");
		break;

	
	case 4:
		cash+=100;
		sprintf(gift, "Cash: ");
		sprintf(gift2, " + 100 $");
		sprintf(temp,"");		
		break;


	case 5:
		cash+=300;
		sprintf(gift, "Cash:");
		sprintf(gift2, " + 300 $");
		sprintf(temp,"");
		break;
	default:
		sprintf(gift, "");
		sprintf(gift2, "");	
		sprintf(temp,"");
		break;
	}


}

void addMissiles(){
	if(shipshold[10]<3)
		shipshold[10]++;
}

void removeMissiles(){
	if(shipshold[10]>0)
		shipshold[10]--;
}

char* getPlanetName(int planet){
	//nome pianeta	
	char namePlanet[10]="";
	//printf(" pianeta %d", planet);
	switch (planet){
		case 0: 
			return "MERCURY";
		case 1: 
			return "VENUS";	
		case 2: 
			return "EARTH";
		case 3: 
			return "MARS";
		case 4: 
			return "JUPITER";
		case 5: 
			return "SATURN";
		case 6: 
			return "URANUS";
		case 7: 
			return "NEPTUNE";
		case 8: 
			return "PLUTO";
		default:
			return "";
		}
}

//caso di commercio illegale: ritorna true se si posseggono quantità di radioactives (2), slave(3), liquor/wine(4), narcotics(6) ), false altrimenti. 
GLboolean isIllegal(){
	int sum = shipshold[2]+shipshold[3]+shipshold[4]+shipshold[6];
	//printf("%d", sum>0);
	return sum>0;
}
