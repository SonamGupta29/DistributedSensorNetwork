#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <math.h>
#include <time.h>
#include <math.h>
char DAT_FILE[100];   /* locations of sensor nodes in the targetfield to be stored in a data file */
int KEYRING_SIZE;     /* key ring size m for each sensor node */
int KEYPOOL_SIZE;     /* key pool size M */
int n;                /* total number of sensor nodes to be deployed */
int d;                /* average number of neighbor nodes for each sensor node */
double RANGE = 27;         /* communication range of sensor node */
double p;             /* network connectivity during direct key establishment phase */
double pi;            /* network connectivity during path key establishment phase using i hops in the path */


/* a sensor node representation */
typedef struct {
int x; /* x-coordinate of sensor node location in target field */
int y; /* y-coordinate of sensor node location in target field */
int fhc;
int shc;
int *keyring; /* key ring */
int phynbrsize; /* number of physical neighbors of a sensor node */
int keynbrsize; /* number of key neighbors of a sensor node */
int nkeynbrsize;
int *phynbr; /* list of physical neighbors of a sensor node */
int *keynbr; /* list of key neighbors of a sensor node */
int *nkeynbr;
} sensor;

int points[505][505]={{0}};

void compute_theoretical()
{
        double t1=0,t2=0,t3=0,t4=0,t5=0;
	long m = KEYRING_SIZE;
        long M = KEYPOOL_SIZE;
        long i=0;
	double result =0;
	double ans=1,den=0,num=0;
	while(i<m)
	{
	        t2=M-i;
		den=t2;
		t1=M-m-i;
		num=t1;
		
		t3=num;
		result = t3/den;
		ans*=result;
		i++;
	}
	t5=ans;
	t4=1-t5;
	result=t4;
	printf("Theoretical connectivity =  %lf\n",result);
}
double compute_theoritical_network_connectivity(double res,double nbr)
{
        double p = 1 - ((1 - res)* pow((1 - (res * res)), nbr));
       
        return p;
}


int main(int argc,char *argv[])
{       
        
        double numerator=0,nbrHoodSize=0;
        double denominator=0;
        double result=0;
        long int total_points,count;
        strcpy(DAT_FILE,argv[1]);
        KEYRING_SIZE = atoi(argv[2]);
        KEYPOOL_SIZE = atoi(argv[3]);
        
        //scanf("%ld",&total_points);
        total_points = 10000;
        sensor *sen;
        sen = NULL;
        sen = (sensor *)malloc(sizeof(sensor)*total_points);
        count = 0;
        FILE *f = fopen(DAT_FILE,"w");
        char filename[100] = "DATA_FILE1"; 
        FILE *fp = fopen(filename,"w");
        FILE *fplot = fopen("wsn.gp", "w");
        long int i,x_cordinate,y_cordinate,j;
        x_cordinate =0;
        y_cordinate =0;
        fprintf(f,"10000");
        fprintf(f,"\n");
        for(i = 1;count<total_points;i++)
        {
             //printf("hello");
             x_cordinate = rand() % 500;
             y_cordinate = rand() % 500;
             if(points[x_cordinate][y_cordinate]==0)
		{
				
				points[x_cordinate][y_cordinate]=1;
				fprintf(f,"%ld %ld",x_cordinate,y_cordinate);
				fprintf(fp,"%ld %ld",x_cordinate,y_cordinate);
				fprintf(f,"\n");
				fprintf(fp,"\n");
				count = count + 1;
					
		}
		
		
	}
	printf("\nReading data file...\n\n");
	fprintf(fplot, "plot '%s'", filename);
	
	fclose(f);
	fclose(fp);
        fclose(fplot);
        system("gnuplot -p 'wsn.gp'");
	//part1 complete
	printf("Scaling communication range...\n");
	
	//printf("part1 done!!!\n");
        sen = (sensor *)malloc(sizeof(sensor)*count);
        int t=0,n=0;
	long int num=0;
        long keycount = 0,k=0,c = 0;
        int arr[40010] = {0};
        //while(i< total_points)
        for(i=0;i<500;i++)
	{
	       
	        for(j=0;j<500;j++)
	        {
	                      
	                      if(points[i][j]==1)
	                        {
	                               // printf("%ld \n",i);
	                                keycount = 0;
	                                sen[num].x = i;
	                                sen[num].y = j;
	                                sen[num].phynbr=(int *)malloc(sizeof(int)*200);
			                sen[num].keynbr=(int *)malloc(sizeof(int)*200);
	                                sen[num].keyring=(int *)malloc(sizeof(int)*KEYRING_SIZE);
			              //  k=0;
			                for(k=0 ; keycount < KEYRING_SIZE;k++)
			                {
			                       
			                        t = rand() % KEYPOOL_SIZE;
			                        if(arr[t] == 0)
			                        {
			                                arr[t] = 1;
			                                (sen[num].keyring)[keycount] = t;
			                                keycount = keycount + 1;
		                                }
	                                }
	                                n = 0;
	                                while(n < 40000)
	                                {
	                                        arr[n] = 0;
	                                        n++;
                                        }
                                       
                                     // printf("hereeeeee\n");
                                        num  = num + 1;
	                         }
	                        
                              
                }              
               
        } 	
        // calculating keyneighbours and physicalneighbours
        long int p = 0,x1,y1,phynode,keynode,x2,y2,d,d1,d2,check;
        double distance,sum_distance = 0,avg_distance;
        phynode = 0;
        keynode = 0;
        check = 0;
              
        while( p <  num)
        {
                x1 = sen[p].x;
                y1 = sen[p].y;
                phynode = 0;
                keynode = 0;
                long int i = 0;
                while(i<num)
                {
                        if(p != i)
                        {
                              y2 = sen[i].y;
                              x2 = sen[i].x;
                              d2 = y2 - y1;
                              d1 = x2 - x1;
                              d = (d2*d2) + (d1*d1);
                              distance = sqrt(d);
                              sum_distance = sum_distance + distance;
                              c = c + 1;
                              if(distance <= RANGE)
                              {
                                  
                                  check = 0;
                                  (sen[p].phynbrsize)++;
                                  (sen[p].phynbr)[phynode++] = i;
                                  int r = 0;
                                  while(r< KEYRING_SIZE)
                                  {
                                        t = (sen[p].keyring)[r];
                                        int l = 0;
                                        while(l< KEYRING_SIZE)
                                        {
                                                if(t == ((sen[i].keyring)[l]))
                                                {
                                                        check =1;
                                                        break;
                                                }
                                                l++;
                                        }
                                        if(check == 1)
                                                break;
                                        r++;
                                }
                                if(check == 1)
                                {
                                        ((sen[p]).keynbrsize)++;
					((sen[p]).keynbr)[keynode++]=i;
				}
			}
		}
		i++;
	}
	
	numerator+=(sen[p]).keynbrsize;
	denominator+=(sen[p]).phynbrsize;
	
	p++;
	
}
	
	avg_distance = sum_distance / c;
	
	printf("Average distance  = %lf m\n",avg_distance);
	printf("Communication range of sensor nodes = %lf m\n ",avg_distance/10);
	printf("\n\n");
	printf("Computing physical neighbors...\n");
	nbrHoodSize = denominator/10000;
        printf("Average neighbourhood size = %d\n",(int)ceil(nbrHoodSize));
        printf("\n\n");
        printf("EG scheme\n");
        printf("Distributing keys...\n");
        printf("\n");
        printf("Computing key neighborhood in direct key establishment phase...\n");
	result = numerator/denominator;
	printf("Simulated average network connectivity  = %lf\n",result);
	
        compute_theoretical();
        // part2 complete
        
        //part3 start
        
        // Get the non key neighbours of all the sensor nodes
	printf("\n\n");
	printf("Computing key neighborhood in path key establishment phase...\n");
	i=0;
	while(i< total_points)
	{
		int nKeyNbr[40000] = {0};
		sen[i].fhc = sen[i].shc = 0;
		sen[i].nkeynbr = (int*)malloc(sizeof(int) * (200));
		int counter = 0;
		for(j = 0; j < sen[i].phynbrsize; j++)
		{
		        nKeyNbr[sen[i].phynbr[j]] = 1;
			for(k = 0; k < sen[i].keynbrsize; k++)
				if(sen[i].phynbr[j] == sen[i].keynbr[k])
					nKeyNbr[sen[i].keynbr[k]] = 0;
		}
		for(j = 0; j < 40000; j++)
			if(nKeyNbr[j] == 1)
				sen[i].nkeynbr[counter++] = j;
		sen[i].nkeynbrsize = counter;
		i++;
	}
        
        i = 0;
        int l,currentnode;
        while(i<total_points)
        {
                j=0;
                while(j< sen[i].nkeynbrsize)
                {
                        k = 0;
                        check = 0;

                        while(k < sen[i].keynbrsize)
                        {
                                currentnode = sen[i].keynbr[k];
                                l = 0;
                                while(l < sen[currentnode].keynbrsize)
                                {
                                        if(sen[currentnode].keynbr[l] == sen[i].nkeynbr[j])
                                        {
                                                sen[i].fhc++;
                                                check = 1;
                                                break;
                                        }
                                        l++;
                                }
                                if(check == 1)
                                        break;
                                k++;
                        }
                        j++;
                }
                i++;
        }
        long int sum_fhc = 0;
        double simulated_network_conne,val;
        for(i=0;i<total_points;i++)
        {
                sum_fhc = sum_fhc + sen[i].fhc;
        }
        simulated_network_conne = (sum_fhc + numerator) / denominator;
        printf("Simulated network connectivity for 1 hop = %lf\n",simulated_network_conne);
        val = compute_theoritical_network_connectivity(result,nbrHoodSize);
        printf("Theoretical connectivity for 1 hop = %lf\n",val);
        // for 2 hops
        i = 0;
        int m;
        while(i<total_points)
        {
                j=0;
                while(j< sen[i].nkeynbrsize)
                {
                        k = 0;
                        check = 0;

                        while(k < sen[i].keynbrsize)
                        {
                                currentnode = sen[i].keynbr[k];
                                l = 0;
                                while(l < sen[currentnode].keynbrsize)
                                {
                                        m=  0;
                                        while(m < sen[sen[currentnode].keynbr[l]].keynbrsize)
                                        {
                                                
                                                if(sen[sen[currentnode].keynbr[l]].keynbr[m] == sen[i].nkeynbr[j])
                                                {
                                                        sen[i].shc++;
                                                        check = 1;
                                                        break;
                                                }
                                                m++;
                                        }
                                        l++;
                                        if(check == 1)
                                        break;
                                      
                                }
                                if(check == 1)
                                        break;
                                k++;
                        }
                        j++;
                }
                i++;
        }
        long int sum_shc = 0;
        double simulated_network_connectivity;
        for(i=0;i<total_points;i++)
        {
                sum_shc = sum_shc + sen[i].shc;
        }
        simulated_network_connectivity = (sum_shc + numerator) / denominator;
        printf("\n");
        printf("Simulated network connectivity for 2 hops = %lf\n",simulated_network_connectivity);
        
	double res1 = 1 - ((1 - val) * (pow((1 - (result * val)),nbrHoodSize)));
	printf("Theoretical connectivity for 2 hops = %lf\n",res1);
	printf("\n");
		
													
return 0;		
}
                                  
                                  
                                  
                             
                              
                                
                
                
