'''
Created on August 5, 2013
@author: Anusha Bilakanti
'''

#include<stdio.h>
#include<stdbool.h>
#include<string.h>

int w1i[400];//intial wt in 1-d converting to 2-d
int w2i[400];
int w1[20][20];//intial wt in 1-d converting to 2-d
int w2[20][20];
int x[20][20];
int sigma1[200];
int sigma2[200];
int out1,out2;
int k,n;

void compute();
void randip();
void encryptfn();
void decryptfn();
void keyconvert();
void permutateinitial(int);
void expansion();
void xorE();
void substitution();
void permutation();
void xorlr();
void inverse();
void xorD();
void permchoicekey();
void rotate();

int main()
{
	int i,j,ub,lb;
	int temp,lim,diff;
        bool q=false;
	int c,it;
	it=0;
	printf("Enter the number of hidden neurons\n");
	scanf("%d",&k);
	printf("Enter the number of neurons for each hidden neuron\n");
	scanf("%d",&n);
	printf("Generate an input vector\n");
	randip();
	printf("\nEnter the weight limit for TPM-1 and TPM-2\n");
	scanf("%d",&lim);
	ub=lim;
	lb=-lim;
	diff=ub-lb;
	printf("\nTPM-1\n");//random weights for TPM-1
	for(i=0;i<k*n;i++)
	{	
		temp=rand()%diff;
		w1i[i]=temp+lb;
		printf("%d\t",w1i[i]);
		
	}
	printf("\nTPM-2\n");//random weights for TPMa-2
	for(i=0;i<k*n;i++)
	{
		temp=rand()%diff;
		w2i[i]=temp+lb;
		printf("%d\t",w2i[i]);
		
	}
	int p=0;//index for 1-d array
	for(i=0;i<k;i++)//converting 1-D wt array of TPM-1 to 2-D(in place of 20-n and n1 in given pgm)
	{
		for(j=0;j<n;j++)
		{
			w1[i][j]=w1i[p++];
		}
	}
	p=0;//index for 1-d array
	for(i=0;i<k;i++)//converting 1-D wt array of TPM-2 to 2-D(in place of 20-n and n1 in given pgm)
	{
		for(j=0;j<n;j++)
		{
			w2[i][j]=w2i[p++];
		}
	}
	//used for recursion when weights are not equal
	compute();
  label1:if(out1==out2)
	{
		c=0;
		for(i=0;i<k;i++)
		{
			for(j=0;j<n;j++)
			{
				if(w1[i][j]!=w2[i][j])
					q=true;
			}
			
		}
		if(q==true)
		{	
		for(i=0;i<k;i++)
		{
			for(j=0;j<n;j++)
			{
	 			if(out1==sigma1[i])
				{
	 				w1[i][j]=(w1[i][j]+x[i][j]);//random walk learning rule
					if(w1[i][j]>ub)
						w1[i][j]=ub;
					else if(w1[i][j]<lb)
						w1[i][j]=lb;
				}
				if(out2==sigma2[i])
				{
					w2[i][j]=(w2[i][j]+x[i][j]);
					if(w2[i][j]>ub)
						w2[i][j]=ub;
					else if(w2[i][j]<lb)
						w2[i][j]=lb;
				}
	 		
			}
		}
		printf("\nUpdated weights\n");
		printf("TPM-1\n");
		for(i=0;i<k;i++)
		{
			for(j=0;j<n;j++)
			{
				printf("%d\t",w1[i][j]);
			}
			printf("\n");
		}
		printf("TPM-2\n");
		for(i=0;i<k;i++)
		{
			for(j=0;j<n;j++)
			{
				printf("%d\t",w2[i][j]);
			}
			printf("\n");
		}
		it++;
		printf("\nIteration%d:\n",it);
		printf("\nNew inputs\n");
		randip();
		compute();
		q=false;
		printf("\ntauA=%d\n",out1);
		printf("tauB=%d\n",out2);
		goto label1;
		}//if q=true
		else//not q==true
		{
			printf("\n2 neural networks are synchronized\n");
			printf("The secret key is\n");
			for(i=0;i<k;i++)
			{
				for(j=0;j<n;j++)
				{
					printf("%d\t",w1[i][j]);
				}
			}
			keyconvert();//converting one weight vector to 1-d for XOR operation
			encryptfn();
			decryptfn();
			return 0;
	
		}
	}//if
	else//out1!=out2
	{
		it++;
		printf("\nIteration%d:\n",it);
		printf("\nNew inputs\n");
		randip();
		compute();
		q=false;
		printf("\ntauA=%d\n",out1);
		printf("\ntauB=%d\n",out2);
		goto label1;
	}
}
void compute()//to compute sigma value
{
	int i,j;
	int temp[20][20];
	//computation for TPM-1
	for(i=0;i<k;i++)
	{
		for(j=0;j<n;j++)
		{
	 		temp[i][j]=(x[i][j]*w1[i][j]); 
		}
		
	}
	for(i=0;i<k;i++)
	{
		sigma1[i]=0;
		for(j=0;j<n;j++)
		{
			sigma1[i]=sigma1[i]+temp[i][j];
		}
		if(sigma1[i]>0)
			sigma1[i]=1;
		else
			sigma1[i]=-1;
		
	}
	//computation for TPM-2
	for(i=0;i<k;i++)
	{
		for(j=0;j<n;j++)
		{
	 		temp[i][j]=(x[i][j]*w2[i][j]); 
		}
		
	}
	for(i=0;i<k;i++)
	{
		sigma2[i]=0;
		for(j=0;j<n;j++)
		{
			sigma2[i]=sigma2[i]+temp[i][j];
		}
		if(sigma2[i]>0)
			sigma2[i]=1;
		else
			sigma2[i]=-1;
		
	}
	for(i=0;i<k;i++)
	{
		out1=out1*sigma1[i];
		out2=out2*sigma2[i];
	}
	if(out1==0)
		out1=-1;
	if(out2==0)
		out2=-1;
}
void randip()	
{
	int i,j;
	for(i=0;i<k;i++)
	{
		for(j=0;j<n;j++)
		{ 
			x[i][j]=rand()*2-1;
			if(x[i][j]<=0)
				x[i][j]=-1;
			else
				x[i][j]=1;
			printf("%d\t",x[i][j]);
		}
	}
}
/**************************************************ENCRYPT**********************************************************************/
int w21[5000];//array for secret key in 1-D-its length is k*n
int l,k1,k2;//length of final encrypted array
int final[5000][64]={0};
unsigned char encrypt[5000],decrypt[5000];
int p,i,j,quotient,y,key1,key2,left[32],right[32];
//p is index of wt array which is in 1-D;cm:index of cipher msg
bool b=false;
bool bfn;//bfn is used to enter each fn
int key1,key2,leftkey[28],rightkey[28],bitkeyfinal[56],bitkey[64],aexpansion[48],xor1[48];
int xor2[32],sub[32],psub[32],inv[8][8],temp[64],round1,rkey[16][48],total[64],ip[64];
int pkey[48];//key obtained after permutation
int rkey[16][48];//key obtained for each of 16 round1s(48 bits) 
int cm,cm1;//used as index for encrypt & decrypt array resp.
void encryptfn()
{
	bfn=false;
	rotate();
	char str[5000];
	int i,j,l,r,m,quotient,binary[5000],binary1[5000];
	printf("\nEnter a plain message to encrypt:\n");
	scanf("%s",str);
	l=strlen(str);
	int a,b;//index for encrypt message
	int s,t;
	s=0;
	cm=0;
	int p=0;//holds the length of bit array
	int q=1;
	for(i=0;i<l;i++)
	{
		quotient=(int)str[i];//ASCII value
		while(quotient!=0)
		{
			binary[p++]=quotient%2;
			quotient=quotient/2;
		}
		if(p%8!=0)
		{
			s=8-p%8;
			for(t=0;t<s;t++)
				bitkey[p++]=0;
		}
	}
	if(p<64)//p=42
	{
		
		for(i=p;i<64;i++)
			binary[i]=0;
		r=64;//index of last bit
		
	}
	else if(p==64)
		r=64;
	else if(p>64)//check bcuz the last block may have less than 64 bits so must append zero
	{
		m=64-(p%64);//'m' number of zeroes must be appended 
		int x=p;
		int c=1;
		do{
			binary[x]=0;
			x++;
			c++;
		}while(c!=m);
		r=p+m;
	}
	int g=0;
	for(j=r-1;j>=0;j--)
		binary1[g++]=binary[j];
	a=-1;
	for(j=0;j<r;j++)//fine-do not change
	{
		if((j)%64==0)//used to go to next block
		{
			a++;
			b=0;
			final[a][b]=binary1[j];
			b++;
			continue;
		}
		final[a][b]=binary1[j];
		b++;
	}//for
	int h;
	for(h=0;h<=a;h++)
	{	
		permutateinitial(h);//initial permutation
		for(round1=1;round1<=16;round1++)
        	{
            		expansion(); //Performing expansion on `right[32]' to get  `expansion[48]'
            		xorE(round1); //Performing XOR operation on expansion[48] & p[48] 
            		substitution();//Perform substitution on xor1[48] to get sub[32]
            		permutation(); //Performing Permutation on sub[32] to get p[32]
            		xorlr();//Performing XOR operation on left[32],p[32] to get xor2[32]			
            		
            		for(i=0;i<32;i++) 
				left[i]=right[i]; //Dumping right[32] into left[32]
            		for(i=0;i<32;i++) 
				right[i]=xor2[i]; //Dumping xor2[32] into right[32]
        	}//for for round1s
        	for(i=0;i<32;i++) 
			temp[i]=right[i]; // Dumping   -->[ swap32bit ]
        	for(;i<64;i++) 
			temp[i]=left[i-32]; //    left[32],right[32] into temp[64]
		inverse(); //Inversing the bits of temp[64] to get inv[8][8]
        /* Obtaining the Cipher-Text into final[1000]*/
        	r=128;
        	int d=0;
        	for(i=0;i<8;i++)
        	{
            		for(j=0;j<8;j++)
            		{
                		d=d+inv[i][j]*r;
                		r=r/2;
                	}
            		encrypt[cm++]=d;
            		r=128;
            		d=0;
            	}
    	} //for(takes each 64-bit of msg each time & computes)
    	encrypt[cm]='\0';
  	printf("\nCIPHER TEXT\n");
  	for(i=0;i<=cm-1;i++)
  		printf("%c",encrypt[i]);
}//encrypt
void permutateinitial(int m)//Initial Permutation:ip is the resultant array after permutation & breaks 64 into 2 32 bits-gettinf Li & Ri
{
	int q;
	q=m;
	int j,i;
	j=58;
	for(i=0;i<64;i++)
		total[i]=final[q][i];
    	for(i=0;i<32;i++)
    	{
        	ip[i]=total[j-1];
        	if(j-8>0)  
			j=j-8;
        	else       
			j=j+58;
    	}
    	j=57;
    	for(i=32;i<64;i++)
    	{
		ip[i]=total[j-1];
        	if(j-8>0)   
			j=j-8;
        	else     
			j=j+58;
    	}
	int k1,k2;
	k1=k2=0;
	for(j=0;j<64;j++)//breaking 64-bit to 2 32-bit of the msg
	{
		if(j<32)
		{
			left[k1]=ip[j];
			k1++;
		}
		else if(j>=32 && j<64)
		{
			right[k2]=ip[j];
			k2++;
			
		}
	}
}
void expansion()//expanding Ri
{
	int exp[8][6],i,j,r;
    	for(i=0;i<8;i++)
    	{
        	for(j=0;j<6;j++)
        	{
            		if((j!=0)||(j!=5))
            		{
                		r=4*i+j;
                		exp[i][j]=right[r-1];
            		}
            		if(j==0)
            		{
                		r=4*i;
               			exp[i][j]=right[r-1];
            		}
            		if(j==5)
            		{
               			 r=4*i+j;
                		 exp[i][j]=right[r-1];
            		}
        	}
    	}
    	exp[0][0]=right[31];
    	exp[7][5]=right[0];

    	r=0;
    	for(i=0;i<8;i++)//converting 2-D to 1-D of 48-bit Ri
    	{
        	for(j=0;j<6;j++)
            		aexpansion[r++]=exp[i][j];
        }
}	
void xorE(int round1) //for Encrypt
{
    	int i;
    	for(i=0;i<48;i++)
     	{
     		if(aexpansion[i]==rkey[16-round1][i])
     			xor1[i]=0;
     		else
     			xor1[i]=1;
     	}
	
}
void substitution()
{
    	int s1[4][16]=
    	{
        	14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        	0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        	4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        	15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    	};
    	int s2[4][16]=
    	{
        	15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        	3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        	0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        	13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    	};
	int s3[4][16]=
    	{
        	10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        	13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        	13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        	1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    	};
	int s4[4][16]=
    	{
        	7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        	13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        	10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        	3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    	};
	int s5[4][16]=
    	{
        	2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        	14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        	4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        	11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
   	 };
	int s6[4][16]=
    	{
        	12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        	10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        	9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        	4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    	};
    	int s7[4][16]=
    	{
        	4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        	13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        	1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        	6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    	};
	int s8[4][16]=
    	{
        	13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        	1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        	7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        	2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    	};
    	int a[8][6],r=0,i,j,p,q,count=0,g=0,v;
	for(i=0;i<8;i++)
    	{
        	for(j=0; j<6; j++)
        	{
            		a[i][j]=xor1[r++];
        	}
    	}
	for(i=0;i<8;i++)
    	{
        	p=1;
        	q=0;
        	r=(a[i][0]*2)+(a[i][5]*1);
        	j=4;
        	while(j>0)
        	{
            		q=q+(a[i][j]*p);
            		p=p*2;
            		j--;
        	}
        	count=i+1;
        	switch(count)//8 cases as there are 8 S-boxes
        	{
        		case 1:
            			v=s1[r][q];
            		break;
        		case 2:
            			v=s2[r][q];
            		break;
        		case 3:
            			v=s3[r][q];
            		break;
        		case 4:
            			v=s4[r][q];
            		break;
        		case 5:
            			v=s5[r][q];
            		break;
        		case 6:
            			v=s6[r][q];
            		break;
        		case 7:
            			v=s7[r][q];
            		break;
        		case 8:
            			v=s8[r][q];
            		break;
        	}
		int d,i=3,a[4];
		int x=0;
        	while(v>0)
        	{
            		d=v%2;
            		a[i--]=d;
            		v=v/2;
        	}
        	while(i>=0)
        	{
            		a[i--]=0;
        	}
		for(i=0;i<4;i++)
			sub[x++]=a[i];
    	}//for
}//substitution()
void permutation()//32 bits of 8 S-boxes are permutated
{
    	psub[0]=sub[15];//psub is permutated array obtained after substitution using S-boxes
    	psub[1]=sub[6];
    	psub[2]=sub[19];
    	psub[3]=sub[20];
    	psub[4]=sub[28];
    	psub[5]=sub[11];
    	psub[6]=sub[27];
    	psub[7]=sub[16];
    	psub[8]=sub[0];
    	psub[9]=sub[14];
    	psub[10]=sub[22];
    	psub[11]=sub[25];
    	psub[12]=sub[4];
    	psub[13]=sub[17];
    	psub[14]=sub[30];
    	psub[15]=sub[9];
    	psub[16]=sub[1];
    	psub[17]=sub[7];
    	psub[18]=sub[23];
    	psub[19]=sub[13];
    	psub[20]=sub[31];
    	psub[21]=sub[26];
    	psub[22]=sub[2];
    	psub[23]=sub[8];
    	psub[24]=sub[18];
    	psub[25]=sub[12];
    	psub[26]=sub[29];
    	psub[27]=sub[5];
    	psub[28]=sub[21];
    	psub[29]=sub[10];
    	psub[30]=sub[3];
    	psub[31]=sub[24];
}
void xorlr()
{
    	int i;
    	for(i=0;i<32;i++)
    	{
        	if(left[i]==psub[i])
        		xor2[i]=0;
        	else
        		xor2[i]=1;
    	}
}
void inverse()
{
    int p=40,q=8,k1,k2,i,j;
    for(i=0; i<8; i++)
    {
        k1=p;
        k2=q;
        for(j=0; j<8; j++)
        {
            if(j%2==0)
            {
                inv[i][j]=temp[k1-1];
                k1=k1+8;
            }
            else if(j%2!=0)
            {
                inv[i][j]=temp[k2-1];
                k2=k2+8;
            }
        }
        p=p-1;
        q=q-1;
    }
}

/**********************************DECRYPT************************************************/
void decryptfn()
{
	bfn=true;
	rotate();
	int i,j,l,r,m,quotient,binary[5000],binary1[5000];
	int a,b;//index for decrypt message
	int s,t;
	s=0;
	a=b=0;
	cm1=0;
	int p=0;//holds the length of bit array
	int q=1;
	for(i=0;i<cm;i++)
	{

		quotient=(int)encrypt[i];//ASCII value
		while(quotient!=0)
		{
			binary[p++]=quotient%2;
			quotient=quotient/2;
		}
		if(p%8!=0)
		{
			s=8-p%8;
			for(t=0;t<s;t++)
				bitkey[p++]=0;
		}
	}
	if(p<64)//p=42
	{
		for(i=p;i<64;i++)
			binary[i]=0;
		r=64;//index of last bit
		
	}
	else if(p==64)
		r=64;
	else if(p>64)//check bcuz the last block may have less than 64 bits so must append zero
	{
		m=64-(p%64);//'m' number of zeroes must be appended 
		int x=p;
		int c=1;
		do{
			binary[x]=0;
			x++;
			c++;
		}while(c!=m);
		r=p+m;
	}
	int g=0;
	for(j=r-1;j>=0;j--)
		binary1[g++]=binary[j];
	a=-1;
	for(j=0;j<r;j++)//fine-do not change
	{
		if((j)%64==0)
		{
			a++;
			b=0;
			final[a][b]=binary1[j];
			b++;
			continue;
		}
		final[a][b]=binary1[j];
		b++;
	}
	int h;
	for(h=0;h<=a;h++)//initial permutation
	{
		permutateinitial(h);
		for(round1=1;round1<=16;round1++)
        	{
            		expansion(); //Performing expansion on `right[32]' to get  `expansion[48]'
            		xorD(round1); //Performing XOR operation on expansion[48] & p[48] 
            		substitution();//Perform substitution on xor1[48] to get sub[32]
            		permutation(); //Performing Permutation on sub[32] to get p[32]
            		xorlr(); //Performing XOR operation on left[32],p[32] to get xor2[32]
            		for(i=0;i<32;i++) //Dumping right[32] into left[32] 
            			left[i]=right[i];
            		for(i=0;i<32;i++) //Dumping xor2[32] into right[32]
            			 right[i]=xor2[i];
        	}//for for round1s
        	for(i=0;i<32;i++) temp[i]=right[i]; // Dumping   -->[ swap32bit ]
        	for(;i<64;i++) temp[i]=left[i-32]; //    left[32],right[32] into temp[64]
        	inverse(); //Inversing the bits of temp[64] to get inv[8][8]
        /* Obtaining the Cipher-Text into final[1000]*/
        	int r=128;
        	int d=0;
        	for(i=0;i<8;i++)
        	{
            		for(j=0;j<8;j++)
            		{
                		d=d+inv[i][j]*r;
                		r=r/2;
            		}
            		decrypt[cm1++]=d;
            		r=128;
            		d=0;
        	}
    	} //for(takes each 64-bit of msg each time 7 computes)
    	decrypt[cm1]='\0';
  	printf("\nORIGINAL TEXT\n");
  	for(i=0;i<=cm-1;i++)
  		printf("%c",decrypt[i]);
	printf("\n");
}//decrypt
void xorD(int round1) //for Decrypt
{
    	int i;
    	for(i=0;i<48;i++)
     	{
     		if(aexpansion[i]==rkey[16-round1][i])
     			xor1[i]=0;
     		else
     			xor1[i]=1;
     	}	
}
/*************************************************KEY COMPUTATION*****************************************************/
//taking all the keys that are positive;take the keys until 56 bits are obtained
	
void keyconvert()//convert weights to 1-D & in terms of bits(must take care of negative sign)
{	l=k1=k2=0;
	int u,v;
	for(i=0;i<k;i++)
	{
		for(j=0;j<n;j++)
		{
			w21[p++]=w1[i][j];
		}
	}
	for(i=0;i<p;i++)
	{
		if(b==true)
			break;
		if(w21[i]<0)//using only those weights whose value is positive 
			quotient=abs(w21[i]);
		else
			quotient=w21[i];//ASCII value
		while(quotient!=0)
		{
			bitkey[l++]=quotient%2;
			quotient=quotient/2;
			if(l==64)
			{
				b=true;
				break;
			}
		}
		if(l%8!=0)
		{
			u=8-l%8;
			for(v=0;v<u;v++)
				bitkey[l++]=0;
			if(l==64)
			{
				b=true;
				break;
			}
		}	
	}	
	if(l<64)
	{
		y=64-l;
		for(i=0;i<l;i++)
		{	bitkey[i+y]=bitkey[i];
		}			
		for(i=0;i<y;i++)
			bitkey[i]=0;
	}//if
/**********************initial permutation on key before breaking******************/
	int r=57,i;
    	for(i=0;i<28;i++)//from 64 bit we are obtaining 56 bit
    	{
        	bitkeyfinal[i]=bitkey[r-1];
        	if(r-8>0)    
			r=r-8;
        	else      
			r=r+57;
    	}
    	r=63;
    	for( i=28;i<52;i++)
    	{
        	bitkeyfinal[i]=bitkey[r-1];
        	if(r-8>0)    
			r=r-8;
        
		else         
			r=r+55;
	}
    	r=28;
    	for(i=52;i<56;i++)
    	{
        	bitkeyfinal[i]=bitkey[r-1];
        	r=r-8;
    	}
	for(i=0;i<56;i++)//breaking 56-bit to 2 28-bit of the msg
	{
		if(i<=27)
		{
			leftkey[k1]=bitkeyfinal[i];
			k1++;
		}
		else if(i>27 && i<=55)
		{
			rightkey[k2]=bitkeyfinal[i];
			k2++;
		}
	}
	
}//convert()	
/**************************************ROTATE***************************************/
void rotate()
{	int noshift=0;
	if(bfn==false)
	{
    		for(round1=1;round1<=16;round1++)
    		{
        		if(round1==1||round1==2||round1==9||round1==16)
            			noshift=1;
        		else
            			noshift=2;
        		while(noshift>0)
        		{
				int t;
            			t=leftkey[0];
            			for(i=0; i<28; i++)
                			leftkey[i]=leftkey[i+1];
            			leftkey[27]=t;
            			t=rightkey[0];
            			for(i=0; i<28; i++)
                			rightkey[i]=rightkey[i+1];
            			rightkey[27]=t;
            			noshift--;
        		}
			permchoicekey();
			for(i=0;i<48;i++)
           			rkey[round1-1][i]=pkey[i];//produced 48-bit aft.permutation(rkey is the subkey)
		}//for
	}//if
	if(bfn==true)
	{
		for(i=0;i<16;i++)
		{
			for(j=0;j<48;j++)
				rkey[i][j]=rkey[15-i][j];
		}
	}//if
}//rotate()
void permchoicekey()//perkey:used for permutation of key in each round1
{
	int perkey[56],i,r;
    	for(i=0;i<28;i++) perkey[i]=leftkey[i];
   	for(r=0,i=28;i<56;i++) perkey[i]=rightkey[r++];

    	pkey[0]=perkey[13];
    	pkey[1]=perkey[16];
    	pkey[2]=perkey[10];
    	pkey[3]=perkey[23];
    	pkey[4]=perkey[0];
    	pkey[5]=perkey[4];
    	pkey[6]=perkey[2];
    	pkey[7]=perkey[27];
    	pkey[8]=perkey[14];
    	pkey[9]=perkey[5];
    	pkey[10]=perkey[20];
    	pkey[11]=perkey[9];
    	pkey[12]=perkey[22];
    	pkey[13]=perkey[18];
    	pkey[14]=perkey[11];
    	pkey[15]=perkey[3];
    	pkey[16]=perkey[25];
    	pkey[17]=perkey[7];
    	pkey[18]=perkey[15];
    	pkey[19]=perkey[6];
    	pkey[20]=perkey[26];
    	pkey[21]=perkey[19];
    	pkey[22]=perkey[12];
    	pkey[23]=perkey[1];
    	pkey[24]=perkey[40];
    	pkey[25]=perkey[51];
    	pkey[26]=perkey[30];
    	pkey[27]=perkey[36];
    	pkey[28]=perkey[46];
    	pkey[29]=perkey[54];
    	pkey[30]=perkey[29];
    	pkey[31]=perkey[39];
    	pkey[32]=perkey[50];
    	pkey[33]=perkey[46];
    	pkey[34]=perkey[32];
    	pkey[35]=perkey[47];
    	pkey[36]=perkey[43];
    	pkey[37]=perkey[48];
    	pkey[38]=perkey[38];
    	pkey[39]=perkey[55];
    	pkey[40]=perkey[33];
    	pkey[41]=perkey[52];
    	pkey[42]=perkey[45];
    	pkey[43]=perkey[41];
    	pkey[44]=perkey[49];
    	pkey[45]=perkey[35];
    	pkey[46]=perkey[28];
    	pkey[47]=perkey[31];
}	
