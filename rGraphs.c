#include<stdio.h>
#include<string.h>
#include<omp.h>
#include<stdlib.h>
char delim[]= " 	!@#$%^&*()_-+={}[]|:;,<.>?", *token,commentDescriptions[500];
int op,choice;
struct node
{
	int data;
	struct node *link;
};

int GenerateDenseMatrix(const char[],int);
int GenerateSparseMatrix(const char[],int);
int DenseOutDegree(const char[],int);
int GenerateSparsetoDenseMatrix(const char[],int);
int GenerateSparsetoSparseMatrix(const char[],int);
int SparseOutDegree(const char[],int);

int main(int argc,char **argv)
{
	int n = 0;
	
	if (argc != 2)	{
		fprintf(stderr,"\nError!\n");
		exit(1);
	}
	else {
		fprintf(stderr,"1.Graph data is in sparse form\n");
		fprintf(stderr,"2.Graph data is in dense form\n");
		fprintf(stderr,"\nEnter choice: ");
		scanf("%d",&choice);
		if(choice==2)
		{
			fprintf(stderr,"\nEnter the number of comment lines : ");
			scanf("%d",&n);
			GenerateDenseMatrix(argv[1],n);
			GenerateSparseMatrix(argv[1],n);
			DenseOutDegree(argv[1],n);
		}
		if(choice==1)
		{
			fprintf(stderr,"\nEnter the number of comment lines : ");
			scanf("%d",&n);
			GenerateSparsetoDenseMatrix(argv[1],n);
			GenerateSparsetoSparseMatrix(argv[1],n);
			SparseOutDegree(argv[1],n);
		}
	}
}

int GenerateDenseMatrix(const char FileName[],int n)
{
	int i,r,c,j;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	r=0,c=0;
	while ( fgets(ch,sizeof(ch),fp) )
	{
			token = strtok(ch,delim);
			while(token!=NULL)
			{
				c++;
				token= strtok(NULL,delim);
			}
			//Computing the number of rows
			r++;
	}
	c/=r;	//computes the columns
	

	printf("%d %d\n",r,c);

			
	int **denseMatrix = (int **) malloc(r * sizeof(int));

	omp_set_num_threads(r);
	#pragma omp parallel for
	for(i = 0; i < r; i++)
		denseMatrix[i] = (int *) malloc(c * sizeof(int));	
		//Creates a 2-D array of size depending on the graph elements in the file		
				
		
	rewind(fp);	
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);	
	
	i=0,j=0;
	while( fgets(ch,sizeof(ch),fp))
	{
			token = strtok(ch,delim);			
			while(token != NULL)
			{
				sscanf(token,"%d",&op);
				denseMatrix[i][j++] = op ; // filling the adjacency matrix with elements
				token = strtok(NULL,delim);
			}
			i++;
			j=0;
	}
	printf("\nDENSE MATRIX FORM\n");
	for(i=0;i<c;i++)
	{
		printf("%d\t",i);
	}
	printf("\n");
	for(i=0;i<r;i++)
	{
		
		for(j=0;j<c;j++)
			printf("%d\t",denseMatrix[i][j]);
		
		printf("\n");
	}
	return 0;
}


//*************************************************************************************************************************
int GenerateSparseMatrix(const char FileName[],int n)
{
	
	int i,r,c,j,count=0,m;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	r=0,c=0;
	while ( fgets(ch,sizeof(ch),fp) )
	{
			token = strtok(ch,delim);
			while(token!= NULL)
			{
				sscanf(token,"%d",&op);
				if( op == 1)
					count++; //counting only non-zero elements
				c++;
				token= strtok(NULL,delim);
			}
			//Computing the number of rows
			r++;
	}
	c/=r;	//computes the columns		
	int **sparseMatrix = (int **) malloc((count) * sizeof(int *));

	omp_set_num_threads(count);

	#pragma omp parallel for
	for(i = 0; i <count; i++)
		sparseMatrix[i] = (int *) malloc(1* sizeof(int));
		
	
	rewind(fp);
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	i=0,j=0,m=0;
	while( fgets(ch,sizeof(ch),fp))
	{
			token = strtok(ch,delim);			
			while(token != NULL)
			{
				sscanf(token,"%d",&op);
				if(op == 1)
				{
					sparseMatrix[m][0] = (i+1); 
					sparseMatrix[m++][1] = (j+1); // filling the adjacency matrix with elements
				}
				token = strtok(NULL,delim);
				j++;
			}
			i++;
			j=0;
	}
	printf("\nSPARSE MATRIX FORM\n");
	
	for(i=0;i<count;i++)
		printf("%d\t%d\n",sparseMatrix[i][0],sparseMatrix[i][1]);
		
	return 0;
}

//*******************************************************************************************************************************

int DenseOutDegree(const char FileName[],int n)
{
	int i,r,c,j,count=0;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	r=0,c=0;
	while ( fgets(ch,sizeof(ch),fp) )
	{
			token = strtok(ch,delim);
			while(token!=NULL)
			{
				c++;
				token= strtok(NULL,delim);
			}
			//Computing the number of rows
			r++;
	}
	c/=r;	//computes the columns
			
	int **denseMatrix = (int **) malloc(r * sizeof(int));
	omp_set_num_threads(r);

	#pragma omp parallel for
	for(i = 0; i < r; i++)
		denseMatrix[i] = (int *) malloc(c * sizeof(int));	
		//Creates a 2-D array of size depending on the graph elements in the file		
				
		
	rewind(fp);	
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);	
	
	i=0,j=0;
	while( fgets(ch,sizeof(ch),fp))
	{
			token = strtok(ch,delim);			
			while(token != NULL)
			{
				sscanf(token,"%d",&op);
				denseMatrix[i][j++] = op ; // filling the adjacency matrix with elements
				token = strtok(NULL,delim);
			}
			i++;
			j=0;
	}
	printf("\nOUT DEGREES\n");
	for(i=0;i<r;i++)
	{
		count=0;
		for(j=0;j<c;j++)
		{
			if(denseMatrix[i][j] == 1)
				count++;
		}
				
		printf("\nOut degree of node %d is : %d",i,count);
	}
	printf("\n");
	return 0;
}
void insert(int m,int n,struct node **first)
{
	struct node *p,*temp=first[m];
	p=malloc(sizeof(struct node));
	p->data=n;
	p->link=NULL;
	if(first[m]->link==NULL)
	{
		first[m]->link=p;
	}
	else
	{
		temp=first[m];
		while(temp->link!=NULL)
		{
			temp=temp->link;
		}
		temp->link=p;
	}
}
void printalist(struct node **first,int n)
{
	int i;
	struct node *temp;
	for(i=0;i<n;i++)
	{
			temp=first[i];
			while(temp->link)
			{
				printf("%d-->",temp->data);
				temp=temp->link;
			}
			printf("%d-->NULL\n",temp->data);
	}
}

//*************************************************************************************************************************

int GenerateSparsetoDenseMatrix(const char FileName[],int n)
{
	int i,r,c,j,count=1,k,num,copy=0;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);

	while ( fgets(ch,sizeof(ch),fp) )
	{
		token = strtok(ch,delim);
		while(token != NULL)
		{
			sscanf(token,"%d",&op);
			if(op>copy)
				copy=op;
			else
			{}
			token= strtok(NULL,delim);
		}
	}	
	op = copy;
	op++;

	int **denseMatrix = (int **) malloc((op) * sizeof(int *));

	struct node *first[op],*p;

	for(i = 0; i <op; i++)
	{
		first[i]=malloc(sizeof(struct node));
		first[i]->data=i;
		first[i]->link=NULL;
		denseMatrix[i] = (int *) malloc(op * sizeof(int));

	}

	for(i=0;i<op;i++)
	{
		for(j=0;j<op;j++)
		{
			denseMatrix[i][j]=0;
		}
	}
	rewind(fp);
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	while ( fgets(ch,sizeof(ch),fp) )
	{
		token = strtok(ch,delim);
		while(token!= NULL)
		{
			sscanf(token,"%d",&num);
			if(count == 1)
			{
				k=num;
				count=2;
			}
			token=strtok(NULL,delim);
		}

		insert(k,num,first);
		denseMatrix[k][num]=1;
		count=1;
	}
	printf("ADJACENCY LIST FORM\n");
	printf("\n");
	printf("\n");
	printalist(first,op);
	printf("\nDENSE MATRIX FORM\n");
	printf("\n");
	printf("\n");	
	for(i=0;i<op;i++)
	{
		for(j=0;j<op;j++)
		{
			printf("%d  ",denseMatrix[i][j]);
		}
		printf("\n");
	}
	//printalist(first,op);
}

//********************************************************************************************************************
int GenerateSparsetoSparseMatrix(const char FileName[],int n)
{

	int i,r,c,j,count=0,k,num,c1=1,m,copy=0;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	while ( fgets(ch,sizeof(ch),fp) )
	{
		
		token = strtok(ch,delim);
			sscanf(token,"%d",&op);
			//printf("%d\n",op);
			token= strtok(NULL,delim);
		count++;
	}
	//fprintf(stderr,"%d %d\n",count,op);
	//fprintf(stderr,"%d %d\n",count,op);

	int **sparseMatrix = (int **) malloc((count) * sizeof(int *));
	for(i = 0; i <count; i++)
		sparseMatrix[i] = (int *) malloc(1* sizeof(int));

	rewind(fp);
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);

	i=0,j=0,m=0;
	while( fgets(ch,sizeof(ch),fp))
	{
			token = strtok(ch,delim);
			while(token != NULL)
			{
				sscanf(token,"%d",&op);
				//printf("%d\n",op);
				if(c1 == 1)
				{
					sparseMatrix[m][0] = (op);
					c1=2;
					//sparseMatrix[m++][1] = (j+1); // filling the adjacency matrix with elements
				}
				else if(c1 == 2)
				{
					sparseMatrix[m++][1] = (op);
					c1=1;
				}
				token = strtok(NULL,delim);
				j++;
			}
			i++;
			j=0;
	}
	printf("\nSPARSE MATRIX FORM\n");


	for(i=0;i<count;i++)
		printf("%d\t%d\n",sparseMatrix[i][0],sparseMatrix[i][1]);

	return 0;
}
//**********************************************************************************************************************

int SparseOutDegree(const char FileName[],int n)
{
	int i,r,c,j,count=1,k,num,copy=0;
	char ch[20];
	FILE *fp=fopen(FileName, "r");
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);

	while ( fgets(ch,sizeof(ch),fp) )
	{
		token = strtok(ch,delim);
		while(token != NULL)
		{
			sscanf(token,"%d",&op);
			if(op>copy)
				copy=op;
			else
			{}
			token= strtok(NULL,delim);
		}
	}	
	op = copy;
	//printf("op = %d\n",op);
	op++;

	int **denseMatrix = (int **) malloc((op) * sizeof(int *));
	for(i = 0; i <op; i++)
		denseMatrix[i] = (int *) malloc(op * sizeof(int));

	for(i=0;i<op;i++)
	{
		for(j=0;j<op;j++)
		{
			denseMatrix[i][j]=0;
		}
	}
	rewind(fp);
	for(i=0;i<n;i++)
		fgets(commentDescriptions,sizeof(commentDescriptions),fp);
	
	while ( fgets(ch,sizeof(ch),fp) )
	{
		token = strtok(ch,delim);
		while(token!= NULL)
		{
			sscanf(token,"%d",&num);
			if(count == 1)
			{
				k=num;
				count=2;
			}
			token= strtok(NULL,delim);
		}
		denseMatrix[k][num]=1;
		count=1;
	}
	fprintf(stderr,"\nOUT DEGREES\n");	
	for(i=0;i<op;i++)
	{
		copy = 0;
		for(j=0;j<op;j++)
		{
			
			if(denseMatrix[i][j] == 1)
				copy++;
		}
		fprintf(stderr,"\nOut degree of node %d is : %d ",i,copy);
	}
	printf("\n");
	return 0;
}
