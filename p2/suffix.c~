#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <sys/types.h>
#include <unistd.h>

#define MLENGTH 2000000

char sequence[MLENGTH];
int seqlength;


char alpha[30];
int nalpha;

char line[1024];
int loadseq(char* filename)
{
	FILE *fin= fopen(filename, "r");

	while(fgets(line, 1024, fin))
	{
		if(line[0]=='>')
			continue;
		line[strlen(line)-1]='\0';
		strcat(sequence, line);
	}

	
	sequence[strlen(sequence)]='$';
	
	fclose(fin);

	return strlen(sequence);
}

int loadalpha(char *filename)
{
	FILE* fin= fopen(filename, "r");
	int i=0;
	while(fgets(line, 1024, fin))
	{
		char* p=strtok(line, " \t\n");
		
		
		while(p!=NULL)
		{
			alpha[i]=*p;
			i++;
			p=strtok(NULL, " \n");
		}
	}

	alpha[i]='$';

	fclose(fin);
	return i+1;
}

int findid(int k)
{

	char c= sequence[k];

	int i=0;
	while(alpha[i])
	{
		if(alpha[i]==c)
			return i;
		i++;
	}

}


int leafID;	// <seqlength
int internalID;	// initial with seqlength, from root

typedef struct node
{
	int id;
	struct node* sl;
	struct node* parent;
	struct node** children;  //nalpha
	int start; int end;	//parent edge;
	int depth;		//length from root
}NODE;




NODE *makenode(int start, int end, int id, int depth)
{
	NODE* p=(NODE * )malloc(sizeof(NODE));
	p->id= id;
	p->sl=0;
	p->parent=0;

	p->children= malloc(nalpha*sizeof(NODE*));  
	memset(p->children, 0, nalpha*sizeof(NODE*));

	p->start=start; 
	p->end=end;

	p->depth=depth;

	return p;
}

NODE* breaknode(NODE *p, int j, int i, int s)
{

	//printf("break node\n");
	int id = findid(p->start);
			
	NODE * q= makenode(p->start,j-1, internalID,p->depth- (p->end-j+1) );
	internalID++;
	//printf("make internal %d, %d, %d, %d\n",  q->id,q->start, q->end, q->depth);

	

	q->parent= p->parent;
	p->parent->children[id] = q;
						
				
	// update p
	p->start= j;
	p->parent=q;
				
	id= findid(j);					
	q->children[id]=p;							
			
	NODE * leaf=makenode( i, seqlength-1, s, seqlength-i+q->depth);
	//printf("makeleaf %d, %d, %d, %d\n",  leaf->id,leaf->start, leaf->end, leaf->depth);						
	leaf->parent= q;					
	id= findid(i);
	q->children[id]=leaf;
					
	return leaf;

				
}
 
NODE * findPath(int i, NODE *p, int s)
{

	//printf("findpath \n");
	while(i<seqlength)
	{
		int id =findid(i);
				
		if(p->children[id]==0)
		{
			//printf("IA %d\n", s);
			
			NODE* q= makenode(i,seqlength-1, s, seqlength-i+p->depth); //start end id depht

			//printf("make leaf for %d,%d, %d, %d\n", q->id, q->start,q->end, q->depth);
			p->children[id]=q;
			q->parent=p;
					

			return q;
			
		}	
		else
		{

			//printf("IB %d\n", s);
			p= p->children[id];


			int j=p->start;

			while(j<=p->end && i<seqlength)
			{
				if(sequence[i]!=sequence[j])
					break;
				i++;
				j++;
			}

			if(j<=p->end)		// not match
				return breaknode(p, j, i,s ); // old node, j for update, i for leaf, s for suffix id
			
		}
	}
}
			
	

NODE* nodhop(int * k, int b,NODE *v)
{

	int i = *k;
	////////printf("nodhop %d, %d, %d\n", v->id, i, b);
	while(b)
	{
		int id= findid(i);
	
		v = v->children[id] ;

		////////printf("loop %d, %d\n", i, v->id);
		
		int length= v->end-v->start+1;
		////////printf("length %d\n", length);

		if(length ==b)
		{
			////////printf("length == b\n");
			b-=length;
			i+=length;
			break;
		}

		else if(length>b)
		{

			////////printf("length >= b\n");

			
			NODE * q= makenode(v->start,v->start+b-1, internalID,v->depth -(length-b));
			////////printf("makeinternal node %d,%d,%d,%d\n",q->id, q->start, q->end, q->depth );
			internalID++;
				
			q->parent= v->parent;

			int id =findid(v->start);
			v->parent->children[id] = q;

			id= findid(v->start+b);
			q->children[id]=v;
	
								
			// update v
			v->start= v->start+b;
			v->parent=q;
				
								
			i+=b;			
			b-=b;			
			v=q;
			break;
		}
	
		////////printf("length< b\n");
		b-=length;
		i+= length;
		
		//id=findid(i);

		//////////printf("i , char %d, %c\n", i, sequence[i]);
		//v=v->children[id];
		//////////printf();

	}

	*k=i;
	return v;

}
NODE* root;
char topcommand[100];
buildTree()
{

	internalID = seqlength;
	

	clock_t Start_t= clock(); //record time that task 1 begins


	root = makenode(-1,-1,internalID,0);

	root->parent=root;
	root->sl=root;

	
	internalID++;
	//////////printf("internal ID %d\n", internalID);

	NODE * p=root;
	//printf("%d\n", p);
	
	int s =0;
	
	
	for (s =0; s<l;s++)
	{

		//printf("++++++++++++++++++++++++++\n");
		//printf("iter %d, %d, %d, %d, %d\n", s, p->id, p->start, p->end, p->depth);
		//printf("++++++++++++++++++++++++++\n");

	//	printTree(root);
		
		if(p->parent->sl)
		{	
			p=p->parent->sl;
				
			int i=s+p->depth;
			
			p= findPath(i,p,s);
			continue;
		}
		else
		{

			NODE * u= p->parent;
			NODE *v= u->parent->sl;

			int b= 0;
			int i=s;

			if(u->parent==root)
			{
				////printf("II B \n");
				b= u->end-u->start;
			}
			else
			{
				////printf("II A \n");
				b = u->end - u->start +1;
				i+=v->depth;
			}
		

			p= nodhop(&i, b,v);
			////////printf("return of nodehop %d, %d\n", i, v->id);

			u->sl=p;
			p=findPath(i, p, s);
		}	
	}	
	
	clock_t End_t = clock();    
	printf("Build Tree Time %ld clock with rate %d (cps)\n ", End_t-Start_t, CLOCKS_PER_SEC);
	printf("#internal %d, #leaves %d, #total %d, size %d\n", internalID -seqlength, seqlength,internalID, internalID*sizeof(NODE));

	
}

//print some basic statistics about the tree (#internal nodes, #leaves, #total number of nodes, size of the tree (in bytes), 
//average string-depth of an internal node, string-depth of the deepest internal node)

int maxdepth;
int sumdepth;
NODE * maxnodes[10];
int *potdepths;
int *dfsdepths;
int d;
printTree(NODE *p)
{	
	if(p->id>=seqlength)	// internal node
	{


		int i =0;
	
	
		for (i =0; i<nalpha; i++)
		{
			if(p->children[i]==0)		
				continue;
	
			printTree(p->children[i]);
		}


		sumdepth+=p->depth;
		if(p->depth>maxdepth)	
		{
			maxdepth=p->depth;
		//	maxnode=p;
		}
	}


	/*	if(p->depth==0)
	{
		printf("root\t%d\t%d\n", p->id, p->depth);
		return;
	}
	for(i =p->start; i<= p->end;i++)
		printf("%c", sequence[i]);
	printf("\t%d\t%d\n", p->id, p->depth);
*/
	
	potdepths[d]=p->depth;
	d++;

}

int maxn;
DFS(NODE* p)
{

	dfsdepths[d]=p->depth;
	d++;
	int i =0;
	
	if(p->id>=seqlength)
	{
		for (i =0; i<nalpha; i++)
		{
			if(!p->children[i])
				continue;
			DFS(p->children[i]);
		}

		if(p->depth==maxdepth)
		{
		//	printf("Max ID %d ", p->id);
			maxnodes[maxn]=p;
			maxn++	;
		}
	}


}
printDepth(int * depths)
{
	int i=0;
	int j=0;
	while(i<internalID)
	{
		
		for(j=i; j<i+10 && j<internalID; j++ )
		{
			printf("%d ",depths[j]);
		}
		
		printf("\n");
		i+=10;
	}

}

char* longest;

catparent(char * s, NODE* p)
{
	if(p==root)
		return;
	catparent(s, p->parent);
	strncat(s, sequence+p->start, p->end-p->start+1);

}

printLongest(NODE * p)
{
	longest =(char *)malloc (p->depth+2);
	memset(longest, 0, p->depth+2);
	catparent(longest, p->parent);
	strncat(longest, sequence+p->start, p->end-p->start+1);

	char* s= longest;

	while(*s)
	{
		printf("%.60s\n",s )	;	
		s+=60;
	}
}

printChildren(NODE *p)
{
	int i=0;
	NODE *c;

	if(p->id<seqlength)	// leaf
	{
		printf("%d\t",p->id );
		return;
	}

	for(i=0;i<nalpha;i++)
	{
		if(!p->children[i])
			continue;
		printChildren(p->children[i]);
		
	}
	printf("\n");

}


//$ <test executable> <input file containing sequence s> <input alphabet file> 
int main(int argc, char* argv[])
{
	seqlength=loadseq(argv[1]);
	printf("seqlength %d \n", seqlength);


	nalpha=loadalpha(argv[2]);			
	printf("nalpha %d \n",nalpha);

	

	
	buildTree();
	
	
	//// top results
	pid_t pid=getpid();
	printf("pid %d\n", pid);
	
	sprintf(topcommand, "top -b -n 1 -p %d >%s.top ", pid , argv[1]);

	system(topcommand);

	
	/////post -order print
	potdepths=malloc(internalID*sizeof(int));
	memset(potdepths, 0, internalID*sizeof(int));

	printf("\n\n++++++++++++++++\n");
	printTree(root);		// post-order, find max depth here

	printf("post-order traversal depths\n");
	printDepth(potdepths);

	
	
	/////// DFS print	
	d=0;
	dfsdepths=malloc(internalID*sizeof(int));
	memset(dfsdepths, 0, internalID*sizeof(int));

	printf("\n\n++++++++++++++++\n");
	DFS(root);	// post-order

	
	printf("DFS depths\n");
	printDepth(dfsdepths);

	
	// longest exact matching
	printf("\n\n++++++++++++++++\n");

	printf("avg internal depth: %d, max internal depth: %d\n", sumdepth/(internalID-seqlength),maxdepth );	
	int i =0;
	while(maxnodes[i])
	{
		printf("longest exact matching sequence is at node %d :\n", maxnodes[i]->id);
		printLongest(maxnodes[i]);

		printf("for suffix :\n");
		printChildren(maxnodes[i]);	
		i++;
		printf("\n");
	}

}
