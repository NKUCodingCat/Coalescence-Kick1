// Version 2.0     07/10/09
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>
#define PI 3.141593

struct Atom {
	char symbol[3];				// maximum 3-1=2 letters
	double coord[3];
	float radius;
	int part;
};
typedef struct Atom atom;

struct List {
	char symbol[20];			// maximum 20-1 letters in symbol
	int  coeff;
};
typedef struct List list;

struct Input
{

	char title[40];
	int restart;            // seems unused; by NKUCodingCat
	int atoms;
	float box[3];
	list lst[100];				//maximum 100 types
	int maxList;
	int population;
	int memory;
	int processors;
	int headers;
	int charge[10];
	int mult[10];
	float gap;
	char header[10][128];
	char tail[65536];
};
typedef struct Input input;

FILE *instructions;
int readInstructions(input *inp, atom *molecule);		//returns number of errors
void genStructure(input *inp, atom* molecule);
int command(char *filename);
void rotate(double coord[3], double ph, double th, double ps);
void makeFile(int filenumber, char *instruction);
int submit(int jobnumber);
int takeHeader(char *instruction, input *inp, FILE *ins);
int parse(char *instruction,input *inp);
int fillMolecule(char *instruction, input *inp, FILE *ins, atom *molecule);
float dist(atom *molecule, int i, int j);
int checkParts(atom *mol, input *inp, int **parts);
void shrink(atom *molecule, input *inp, int **parts);

extern float atomicRadii(char* name, int type);

int main(int argc, char** argvar)
{
	int i=0, j=0;
	int file=0;
	int **parts;					// now number of atoms is unlimited
	int trial=0;
	int instruct=0;
	atom molecule[1000];
	input *inp=malloc(sizeof(input));
	inp->gap = 1.2;
	if (readInstructions(inp, molecule)!=0)
	{
		printf("Bad INS file\n");
		return 1;

	}
	// return 0;
	parts=malloc(inp->atoms*(sizeof(int*)));
	for (i=0; i < inp->atoms; i++)
		parts[i]=malloc(inp->atoms*(sizeof(int)));

	for (trial=0; trial < inp->maxList; trial++)
	{
		printf("\n%s %d\n",inp->lst[trial].symbol, inp->lst[trial].coeff);
	}
	for (file=0; file < inp->population; file++)
	{
		genStructure(inp, molecule);

		while(checkParts(molecule, inp, parts)>1)
		{
			for (i=0; i < inp->atoms; i++)
			{
				for (j=0; j < inp->atoms; j++)
				{
					printf("%d ",parts[i][j]);
				}
				printf("\n");
			}
			printf("Number of parts=%d\n",checkParts(molecule, inp, parts));
			shrink(molecule, inp, parts);
		}
		for (i=0; i < inp->atoms; i++)
		{
			for (j=0; j < inp->atoms; j++)
			{
				printf("%d ",parts[i][j]);
			}
			printf("\n");
		}

		printf("\n");
		char fileName[80];		
		sprintf(fileName,"%s%d.com",inp->title,file);
		FILE *com=fopen(fileName,"w");
		fprintf(com,"%%mem=%dMb\n%%Nprocshared=%d\n%s\nCoal_kick_v2_Trial # %d\n\n%d %d\n", inp->memory, inp->processors, inp->header[0], file, inp->charge[0], inp->mult[0]);
		for (i=0; i < inp->atoms; i++)
		{
			fprintf(com,"%s ",molecule[i].symbol);
			fprintf(com," %10f ",molecule[i].coord[0]);
			fprintf(com," %10f ",molecule[i].coord[1]);
			fprintf(com," %10f ",molecule[i].coord[2]);
			fprintf(com," \n");
			//		printf(" %10f ",molecule[i].radius);
			//		printf(" %d\n",molecule[i].part);
		}
		fprintf(com," \n");
		if(strlen(inp->tail))fprintf(com,inp->tail); 
		fclose(com);
	}
	for (i=0; i < inp->atoms; i++)
		free(parts[i]);
	free(parts);


}


int readInstructions(input* inp, atom *molecule)
{
	char *instruction=malloc(128*sizeof(char));
	char dump[40];
	FILE *ins;
	int error=0;
	if (ins=fopen("INS","r"))
	{
		while(fgets(instruction,128,ins))
		{
			switch (instruction[0])
			{
				case ('t'):
					sscanf(instruction,"%s %s",dump,inp->title);  break;
				case ('r'):
					sscanf(instruction,"%s %d",dump,
							&(inp->restart)); break;
				case ('g'):
					sscanf(instruction,"%s %f",dump,
							&(inp->gap)); break;
				case ('b'):
					sscanf(instruction,"%s %f %f %f",dump,
							&(inp->box[0]),&(inp->box[1]),&(inp->box[2])); break;
				case ('p'):
					sscanf(instruction,"%s %d",dump,
							&(inp->population)); break;
				case ('m'):
					sscanf(instruction,"%s %d %d",dump,
							&(inp->memory), &(inp->processors)); break;
				case ('a'):
					inp->maxList=parse(instruction,inp); break;
				case ('!'): case ('\n'): case(' '): break;
				case ('h'): inp->headers=takeHeader(instruction, inp, ins);  
					    inp->atoms=fillMolecule(instruction, inp, ins, molecule); 
					    if (inp->atoms==0) error=1; break;	
				case ('>'): fseek(ins, -(1+strlen(instruction)) ,SEEK_CUR); if ( 1 == getTail(instruction, inp, ins)) error=1; break;
				default:  printf("unknown instruction: %s\n",instruction);
			}
		}
		fclose(ins);
		free(instruction);
		return error;
	}
	else
		return 1;
}

int parse(char *instruction, input *inp)
{
	char name[8];
	int coeff=0;
	int i=0;
	instruction=strchr(instruction,' ');
	while (instruction[0]==' ') 
		instruction++;
	while(sscanf(instruction,"%s %d",name, &coeff)==2)
	{
		strcpy(inp->lst[i].symbol,name);
		inp->lst[i].coeff=coeff;
		i++;
		instruction=strchr(instruction,' ');
		while (instruction[0]==' ') 
			instruction++;
		instruction=strchr(instruction,' ');
		if (instruction!=0)
		{
			while (instruction[0]==' ') 
				instruction++;
		}
		else break;
	}
	return i;
}

int takeHeader(char *instruction, input *inp, FILE *ins)
{
	int i=0;
	fgets(instruction,128,ins);
	while (instruction[0]=='#')
	{
		strcpy(inp->header[i],instruction);
		fgets(instruction,128,ins);
		sscanf(instruction,"%d %d",&(inp->charge[i]),&(inp->mult[i]));
		fgets(instruction,128,ins);
		i++;	
	}
	return i;		
}

bool startsWith(const char *pre, const char *str)
{
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

int getTail(char * instruction, input *inp, FILE *ins)
{
	char *  line = NULL;
    size_t  len = 128;
	ssize_t read;

	read = getline(&line, &len, ins);
	if(startsWith(">TAIL", line))
	{
		while((read = getline(&line, &len, ins)) != -1)
		{
			// printf("%s", line);
			if(startsWith(">ENDTAIL", line)) break; 
			strcat(inp->tail, line);
		}
	}
	else return 1;
	printf("************TAIL*************\n%s\n**********TAIL END***********", inp->tail);
	if (line) free(line); return 0;
}


int fillMolecule(char *instruction, input *inp, FILE *ins, atom *molecule)
{
	int fragment=0, atom=0, coeff=1, part=0;
	int f=0,        a=0,    c=0;
	int type=1;
	char cluster;			// part of cluster is marked with  *
	char nameFrag[20];
	double coord[3];
	char symbol[3];
	while(isalpha(instruction[0])!=0)
	{
		sscanf(instruction,"%s %c", nameFrag, &cluster);
		if (cluster=='*') type=1;
		else     	  type=0;
		for (fragment=0; fragment < inp->maxList; fragment++)
		{
			if((strcmp(nameFrag,inp->lst[fragment].symbol))==0)
			{
				atom=0;
				coeff=inp->lst[fragment].coeff;
				printf(" coeff %d", coeff);
				fgets(instruction,128,ins);
				while(sscanf(instruction,"%s %lf %lf %lf", molecule[atom+f].symbol, 
							&molecule[atom+f].coord[0], &molecule[atom+f].coord[1],
							&molecule[atom+f].coord[2])==4)
				{
					molecule[atom+f].radius=atomicRadii(molecule[atom+f].symbol,type);
					molecule[atom+f].part=part;
					atom++;
					if(fgets(instruction,128,ins)==0) 
					{
						instruction[0]='\0';
					}
				}
				for (c=1; c < coeff; c++)
					for (a=0; a < atom; a++ )
					{
						strcpy(molecule[a+c*atom+f].symbol,molecule[a+f].symbol);
						molecule[a+c*atom+f].coord[0]=molecule[a+f].coord[0];
						molecule[a+c*atom+f].coord[1]=molecule[a+f].coord[1];
						molecule[a+c*atom+f].coord[2]=molecule[a+f].coord[2];
						molecule[a+c*atom+f].radius=molecule[a+f].radius;
						molecule[a+c*atom+f].part=part+c;
					}
				break;
			}
		}
		f+=atom*coeff;
		part+=c;
		if (fragment==inp->maxList)
		{
			printf("\nFragment %s not found in the atom list\n",nameFrag);
			return 0;
		}
	}
	for (atom=0; atom < inp->maxList; atom++)
	{
		if (atomicRadii(inp->lst[atom].symbol,1)!=0)
		{
			strcpy(molecule[f].symbol,inp->lst[atom].symbol);
			molecule[f].part=-1;
			molecule[f].radius=atomicRadii(inp->lst[atom].symbol,1);			

			for (c=0; c < inp->lst[atom].coeff; c++)
			{
				strcpy(molecule[f+c].symbol,molecule[f].symbol);
				molecule[f+c].part=-1;
				molecule[f+c].radius=molecule[f].radius;			
			}
			f+=c;
		}
	}
	printf("\n %d\n",f);
	return f;
}


void genStructure(input *inp, atom *molecule)
{
	int i=0;
	int j=0;
	double PH, TH, PS, shift[3];
	while (i < inp->atoms)
	{
		if (molecule[i].part==-1)
		{
			molecule[i].coord[0]=inp->box[0]*((double)rand()/RAND_MAX-0.5);
			molecule[i].coord[1]=inp->box[1]*((double)rand()/RAND_MAX-0.5);
			molecule[i].coord[2]=inp->box[2]*((double)rand()/RAND_MAX-0.5);
			i++;
		}
		else
		{
			j=molecule[i].part;
			shift[0]=inp->box[0]*((double)rand()/RAND_MAX-0.5);
			shift[1]=inp->box[1]*((double)rand()/RAND_MAX-0.5);
			shift[2]=inp->box[2]*((double)rand()/RAND_MAX-0.5);
			PH=2*PI*(double)rand()/RAND_MAX;
			TH=PI*(double)rand()/RAND_MAX;
			PS=2*PI*(double)rand()/RAND_MAX;
			while (molecule[i].part==j)
			{
				rotate(molecule[i].coord,PH,TH,PS);
				molecule[i].coord[0]+=shift[0];
				molecule[i].coord[1]+=shift[1];
				molecule[i].coord[2]+=shift[2];
				i++;
			}
		}
	}
}


void rotate(double coord[3], double ph, double th, double ps)
{
	double old[3];
	old[0]=coord[0];
	old[1]=coord[1];
	old[2]=coord[2];
	coord[0]=old[0]*(cos(ps)*cos(ph)-cos(th)*sin(ph)*sin(ps))
		+ old[1]*(cos(ps)*sin(ph)+cos(th)*cos(ph)*sin(ps))
		+ old[2]*sin(ps)*sin(th);
	coord[1]=old[0]*(-sin(ps)*cos(ph)-cos(th)*sin(ph)*cos(ps))
		+ old[1]*(-sin(ps)*sin(ph)+cos(th)*cos(ph)*cos(ps))
		+ old[2]*cos(ps)*sin(th);
	coord[2]=old[0]*sin(th)*sin(ph)
		-old[1]*sin(th)*cos(ph)
		+old[2]*cos(th);
}


float dist(atom *molecule,int i, int j)
{
	float x=molecule[i].coord[0]-molecule[j].coord[0];
	float y=molecule[i].coord[1]-molecule[j].coord[1];
	float z=molecule[i].coord[2]-molecule[j].coord[2];
	return sqrt(x*x+y*y+z*z);
}



int checkParts(atom *mol, input *inp, int **parts)
{
	int numberParts=inp->atoms;
	int i=0, j=0, k=0, found=0;
	float distance=0;
	for (i=0; i < inp->atoms; i++)
		for (j=i+1; j < inp->atoms; j++)
		{
			distance=dist(mol,i,j)/(mol[i].radius+mol[j].radius);
			if (distance < inp->gap)
			{
				parts[i][j]=1;
				parts[j][i]=1;
			}
			else
			{
				parts[i][j]=0;
				parts[j][i]=0;
			}
		}
	for (i=0; i < inp->atoms; i++)
		parts[i][i]=1;
	for (i=0; i < inp->atoms; i++)
		if(parts[i][i]==1)
		{
			do
			{
				found=0;
				for (j=i+1; j < inp->atoms; j++)
				{
					if (parts[i][j]==1)
					{
						parts[i][j]=2;
						for (k=i+1; k < inp->atoms; k++)
							if ((parts[j][k]==1)&&(parts[i][k]==0))
							{
								parts[i][k]=1;
							}
						found=1;
						parts[i][i]++;
						parts[j][j]=0;
						numberParts--;
					}
				}
			}
			while (found);
		}
	return numberParts;
}


void shrink(atom *mol, input *inp, int **parts)
{
	int numberAtoms = inp->atoms;
	int i=0, j=0;
	float coeff=1;
	float r=1;
	float center[3]={0,0,0};
	float center1[3]={0,0,0};
	for (i=0; i<numberAtoms; i++)
	{
		center[0]+=mol[i].coord[0];
		center[1]+=mol[i].coord[1];
		center[2]+=mol[i].coord[2];
	}
	center[0]/=numberAtoms;
	center[1]/=numberAtoms;
	center[2]/=numberAtoms;
	for (i=0; i<numberAtoms; i++)
	{
		if (parts[i][i]==1)
		{
			r=sqrt(pow(mol[i].coord[0]-center[0],2)+pow(mol[i].coord[1]-center[1],2)+pow(mol[i].coord[2]-center[2],2));
			if (r > 0.2)
			{
				coeff=r/(r-0.2);
				mol[i].coord[0]=(mol[i].coord[0]-center[0])/coeff+center[0];
				mol[i].coord[1]=(mol[i].coord[1]-center[1])/coeff+center[1];
				mol[i].coord[2]=(mol[i].coord[2]-center[2])/coeff+center[2];
			}
		}
		if (parts[i][i]>1)
		{
			center1[0]=0;
			center1[1]=0;
			center1[2]=0;
			for (j=i; j<numberAtoms; j++)
				if (parts[i][j]!=0)
				{
					center1[0]+=mol[j].coord[0];
					center1[1]+=mol[j].coord[1];
					center1[2]+=mol[j].coord[2];
				}
			center1[0]/=parts[i][i];
			center1[1]/=parts[i][i];
			center1[2]/=parts[i][i];
			r=sqrt(pow(center1[0]-center[0],2)+pow(center1[1]-center[1],2)+pow(center1[2]-center[2],2));
			if (r > 0.2)
			{
				coeff=r/(r-0.2);

				for (j=i; j<numberAtoms; j++)
					if (parts[i][j]!=0)
					{
						mol[j].coord[0]+=(-center1[0]+(center1[0]-center[0])/coeff+center[0]);
						mol[j].coord[1]+=(-center1[1]+(center1[1]-center[1])/coeff+center[1]);
						mol[j].coord[2]+=(-center1[2]+(center1[2]-center[2])/coeff+center[2]);
					}
			}
		}
	}
}







int submit(int i)
{
	return 1;
}
