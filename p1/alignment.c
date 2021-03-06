#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define MAXLENGTH 10000000

// parameters
int match;
int mismatch;
int h;
int g;


// sequences
char* sequences[2];
char * alignment[2];
int m;
int n;

char line[1024];


char* bars;

loadSeq(char * seqFile)
{
	// malloc sequence;
	int i=0;	
	for(i=0;i<2;i++)
	{
		sequences[i]=(char *)malloc(MAXLENGTH*sizeof(char));
		memset(sequences[i], 0, MAXLENGTH);

	}


	FILE * sfile= fopen(seqFile, "r");
	int snum=-1;

	while(fgets(line, 1024, sfile))
	{
		line[strlen(line)-1]='\0';
		if(line[0]=='\0')
			continue;
		if(line[0]=='>')
		{

			printf("%s\n",line);
			snum++;
			continue;
		}
		strcat(sequences[snum], line);

	}
	
	//////printf("%s\n",sequences[0]);
	//////printf("%d,%d\n",strlen(sequences[0]), strlen(sequences[1]));

	fclose(sfile);	
	
}



typedef struct tab
{
	int S;
	int D;
	int I;

	int T;


	char dir;
	
} TABLE;

TABLE ** table;

int alcount;

int maxLoc[2];
int max=0;

traceback(int x1, int y1, int x2, int y2 ,int type)
{


	int l=x2;
	int k=y2;
	if(x1<0||y1<0||l<0||k<0)
		return;

	if(x1>=x2 ||y1>=y2)
		return;

	
   int i=0;
   int j=0;
	

	int len1=l-x1;
	int len2=l-y1;

	if(len1<=0|| len2<=0)
		return;

	int alength=len1+len2+2;

	for(i=0;i<2;i++)
	{
		alignment[i]=(char *)malloc(alength*sizeof(char));
		memset(alignment[i], 0, alength);

	}

	bars=(char *)malloc(alength*sizeof(char));
	memset(bars, 0, alength);

 	int minlen=((x2-x1)<(y2-y1))?(x2-x1):(y2-y1)+1;

	////printf("%d alength\n",alength);
	////printf("before trace %d,%d, %d, %d\n",x1,y1,l,k);

/*	for(i=0;i<10;i++)
	{
		for(j=0;j<10;j++)
			//printf("%d\t",table[i][j].T);
		//printf("\n");
	}
*/	
	int acount=0;	
	int nmatchs=0;
	int gaps=0;
	int opens=0;

	int endx= l;
	int endy=k;

//	printf("before trace loop %d,%d\n",l,k);  
   while(l>x1 && k>y1)
   {	
	if(table[l][k].dir=='N')
		break;

	////printf("enter traceback\n");
	if(table[l][k].dir=='D')
	{
		

		////printf("enter D\n");
		alignment[0][acount]=sequences[0][l-1];
		alignment[1][acount]='-';
		bars[acount]=' ';
		acount++;
		gaps++;
	

		l--;

		if(table[l][k].dir!='D')
			opens++;
	}

	else if (table[l][k].dir=='I')
	{

		////printf("enter I\n");

		alignment[0][acount]='-';
		alignment[1][acount]=sequences[1][k-1];
		bars[acount]=' ';
		acount++;
		gaps++;
		
	//	traceback(l,k-1);
		k--;
		if(table[l][k].dir!='I')
			opens++;
	}
		
	else
	{


		////printf("S\n");
		//////printf("%c-%c\n",sequences[0][l-1],sequences[1][k-1]);		
		
		alignment[0][acount]=sequences[0][l-1];
		alignment[1][acount]=sequences[1][k-1];

		
		if(sequences[0][l-1]==sequences[1][k-1])
		{
			bars[acount]='|';
			nmatchs++;
		}
		else
			bars[acount]=' ';
		acount++;
		
		
	
		l--;k--;


		//traceback(l-1,k-1);

	}

   }	

	//printf("%s\n",alignment[0]);
	alignment[0][acount]='\0';
	alignment[1][acount]='\0';
	bars[acount]='\0';

//	//printf("end %d, %d\n",l,k);
		

	printf("matchs %d, gaps %d, opengaps %d\n",nmatchs, gaps,opens);
		
}

globalAlignment()
{

	

	//initial table
	m= strlen(sequences[0]);
	n= strlen(sequences[1]);	
	
	table= (TABLE **) malloc((m+1)* sizeof(TABLE *));
	
	int i=0;
	int j=0;

	for(i=0;i<m+1;i++)	//m+1 row
	{
		table[i]=(TABLE *)malloc((n+1)*sizeof(TABLE));   // n_1 col
		memset(table[i], 0, (n+1)*sizeof(TABLE)/sizeof(char));
	}


	// S
	for(i=0;i<m+1;i++)
		table[i][0].S=-10000;	
	

	for(j=0;j<n+1;j++)
		table[0][j].S=-10000;				
	
	

	table[0][0].S=0;


	// D	
	table[1][0].D=h+g;
	for(i=2;i<m+1;i++)
		table[i][0].D=table[i-1][0].D+g;


	for(j=0;j<n+1;j++)
		table[0][j].D=-10000;		



	//I 
	table[0][1].I=h+g;
	for(j=2;j<n+1;j++)
		table[0][j].I=table[0][j-1].I+g;

	for(i=0;i<m+1;i++)
		table[i][0].I=-10000;	


	// T

	table[1][0].T=h+g;
	for(i=2;i<m+1;i++)
		table[i][0].T=table[i-1][0].T+g;


	table[0][1].T=h+g;
	for(j=2;j<n+1;j++)
		table[0][j].T=table[0][j-1].T+g;

	



	// malloc aligment result
	
	

	

	int theta=0;



	for(i =1;i<m+1;i++)
	{

		for(j=1; j<n+1; j++)
		{
			if(sequences[0][i-1]==sequences[1][j-1])
				theta = match;
			else
				theta= mismatch;

			table[i][j].S = table[i-1][j-1].T+theta;

		//	//////printf("%d,%d : s %d\n", i,j,table[i][j].S);	


		
			table[i][j].D = ( (table[i-1][j].D+g) >( table[i-1][j].T+h+g) )? (table[i-1][j].D+g):( table[i-1][j].T+h+g);

		//	//////printf("%d,%d : d %d\n", i,j,table[i][j].D);	
			

		//	//////printf("Di-1 %d\n",table[i-1][j].D);
		//	//////printf("Ti-1 %d\n",table[i-1][j].T);


			table[i][j].I = ( (table[i][j-1].I+g)>( table[i][j-1].T+h+g) )? (table[i][j-1].I+g):( table[i][j-1].T+h+g) ;

			

			table[i][j].T= (table[i][j].S>table[i][j].D )?table[i][j].S:table[i][j].D;


			table[i][j].dir =(table[i][j].S>table[i][j].D )?'S':'D';

			table[i][j].T= (table[i][j].T>table[i][j].I)?table[i][j].T:table[i][j].I;
			table[i][j].dir =(table[i][j].T>table[i][j].I )?table[i][j].dir:'I';

			////printf("%d,%d,%d,%d,%c\n",table[i][j].T,table[i][j].S,table[i][j].D,table[i][j].I, table[i][j].dir);
			//break;

		}

		//break;
	}

	printf("global optimal score %d\n",table[i-1][j-1].T );

/*	for(i =125;i<m+1;i++)
	{
		for(j=135;j<n+1;j++)
			printf("%c\t", table[i][j].dir);
		printf("\n");
	}
*/	//return;

	//////printf("table computed\n");
	////////printf("%c\n", table[i-1][j-1].dir);

	////printf("%d",table[i-1][j-1].T);
	traceback(0,0,m,n,0);
	
	
}


void reverse(char s[])
{
      int length = strlen(s) ;
      int c, i, j;

      for (i = 0, j = length - 1; i < j; i++, j--)
     {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
      }
}


printAlignment()	// and reverse
{	
	//printf("enter printAlignment\n");
	//printf("%s\n",alignment[1]);
	reverse(alignment[0]);
	reverse(alignment[1]);
	reverse(bars);

	int remain=strlen(bars)-1;

	int i=0;
	
	while(remain>0)
	{
		printf("%.60s\n",alignment[0]+i);
		printf("%.60s\n",bars+i);
		printf("%.60s\n",alignment[1]+i);

		remain-=60;
		i+=60;
	}





	printf("\n");



}





localAlignment()
{


	//initial table
	m= strlen(sequences[0]);
	n= strlen(sequences[1]);	
	
	table= (TABLE **) malloc((m+1)* sizeof(TABLE *));
	
	int i=0;
	int j=0;

	for(i=0;i<m+1;i++)	//m+1 row
	{
		table[i]=(TABLE *)malloc((n+1)*sizeof(TABLE));   // n_1 col
		memset(table[i], 0, (n+1)*sizeof(TABLE)/sizeof(char));
	}


	// S
	for(i=0;i<m+1;i++)
		table[i][0].S=-10000;	
	

	for(j=0;j<n+1;j++)
		table[0][j].S=-10000;				
	
	

	table[0][0].S=0;


	// D	
	table[1][0].D=h+g;
	for(i=2;i<m+1;i++)
		table[i][0].D=table[i-1][0].D+g;


	for(j=0;j<n+1;j++)
		table[0][j].D=-10000;		



	//I 
	table[0][1].I=h+g;
	for(j=2;j<n+1;j++)
		table[0][j].I=table[0][j-1].I+g;

	for(i=0;i<m+1;i++)
		table[i][0].I=-10000;	


/*	// T

	table[1][0].T=h+g;
	for(i=2;i<m+1;i++)
		table[i][0].T=table[i-1][0].T+g;


	table[0][1].T=h+g;
	for(j=2;j<n+1;j++)
		table[0][j].T=table[0][j-1].T+g;

*/	
	

	// malloc aligment result


	int theta=0;

	
	for(i =1;i<m+1;i++)
	{

		for(j=1; j<n+1; j++)
		{
			if(sequences[0][i-1]==sequences[1][j-1])
				theta = match;
			else
				theta= mismatch;

			table[i][j].S = table[i-1][j-1].T+theta;

		//	//////printf("%d,%d : s %d\n", i,j,table[i][j].S);	


		
			table[i][j].D = ( (table[i-1][j].D+g) >( table[i-1][j].T+h+g) )? (table[i-1][j].D+g):( table[i-1][j].T+h+g);

		//	//////printf("%d,%d : d %d\n", i,j,table[i][j].D);	
			

		//	//////printf("Di-1 %d\n",table[i-1][j].D);
		//	//////printf("Ti-1 %d\n",table[i-1][j].T);


			table[i][j].I = ( (table[i][j-1].I+g)>( table[i][j-1].T+h+g) )? (table[i][j-1].I+g):( table[i][j-1].T+h+g) ;

			

			table[i][j].T= (table[i][j].S>table[i][j].D )?table[i][j].S:table[i][j].D;


			table[i][j].dir =(table[i][j].S>table[i][j].D )?'S':'D';

			table[i][j].T= (table[i][j].T>table[i][j].I)?table[i][j].T:table[i][j].I;
			table[i][j].dir =(table[i][j].T>table[i][j].I )?table[i][j].dir:'I';

		

			table[i][j].T= (table[i][j].T>0)?table[i][j].T:0;
			table[i][j].dir =(table[i][j].T>0 )?table[i][j].dir:'N';

			if(max<table[i][j].T)
			{
				maxLoc[0]=i;
				maxLoc[1]=j;
				max=table[i][j].T;

			}
			////printf("%d,%d,%d,%d,%c\n",table[i][j].T,table[i][j].S,table[i][j].D,table[i][j].I, table[i][j].dir);
			//break;

		}

		//break;
	}

printf("local optimal score %d\n",max);

	for(i=125;i<m+1;i++)
	{
	
		for(j=135;j<n+1;j++)
		{
			//printf("%d\t",table[i][j].T);
		}
		//printf("\n");
	}

//	traceback(i-1,j-1);
	//printf("max %d, %d\n",maxLoc[0],maxLoc[1]);
	traceback(0,0,m,n,1);
}

readCof(char * conFile)
{

	//printf("%s\n", conFile);
	FILE* cf=fopen(conFile, "r");

	while(fgets(line, 1024, cf))
	{
		if(strncmp(line, "match", 5)==0)
		{
			strtok(line, " \t\n");
			char* p=strtok(NULL, " \t\n");
		
			match= atoi(p);
			//printf("%d\n", match);

			continue;
		}

		if(strncmp(line, "mismatch", 8)==0)
		{
			strtok(line, "\t");
			char* p=strtok(NULL, " \t\n");
			
			mismatch= atoi(p);
			continue;
		}

		if(strncmp(line, "h", 1)==0)
		{
			strtok(line, "\t");
			char* p=strtok(NULL, " \t\n");
			h= atoi(p);

			continue;
		}


		if(strncmp(line, "g", 1)==0)
		{
			strtok(line, "\t");
			char* p=strtok(NULL, " \t\n");
			g= atoi(p);
			continue;
		}

	
	} 


	//printf("Paras %d, %d,%d,%d\n",match, mismatch,h,g);
	fclose(cf);
}


// $ <executable name> <input file containing both s1 and s2> <0: global, 1: local> <optional: path to parameters config file>
int main(int argc, char* argv[])
{
	if(argc<4)
		readCof("parameters.config");	
	else	
		readCof(argv[3]);


	loadSeq(argv[1]);

	

	if(atoi(argv[2])==0)
	{
		globalAlignment();


	}
	else	
		localAlignment();

}
