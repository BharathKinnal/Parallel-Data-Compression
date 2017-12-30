#include<stdint.h>
#include <streambuf>
#include<omp.h>
#include<bits/stdc++.h>
using namespace std;

float readfile(char *fil, string &s)
{
	float start=omp_get_wtime(),tim=0;
	
	uint8_t c;
	long long int i,j,k;
    ifstream inp(fil,ios::binary);
    if(!inp.is_open())
      	return 0;
	
    inp.seekg(0,ios::end);  
    int count=inp.tellg();
    inp.seekg(0,ios::beg);
    
    tim+=omp_get_wtime()-start;   
    cout<<"Initial size = "<<count<<" B"<<endl;
    start=omp_get_wtime();

    string u;
    stringstream x;

    s="";
    s.resize(8*count);
	x<<inp.rdbuf();
	u=x.str();
	
	for(int i=0;i<count;i++)
    {
		
		c=u[i];
		
		//if(i<100)
			//cout<<(int)u<<' ';//											<-
		
       	for(j=128,k=0;j>0;j/=2,k++)
       	{
       		if(c/j==1)
       			s[8*i+k]='1';
       		else
       			s[8*i+k]='0';
       		c%=j;
       	}
       	
    }
    
    inp.close();
    tim+=omp_get_wtime()-start;   
    
    return tim;
}
