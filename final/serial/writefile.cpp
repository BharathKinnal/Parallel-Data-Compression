#include<stdint.h>
#include<omp.h>
#include<bits/stdc++.h>

float writefile(char *fil,string &s)
{
	float start=omp_get_wtime(),tim=0;
	
	long long int sz=s.size();
	int i,j,k;
	char c;
	uint8_t nono;
    ofstream outp(fil,ios::binary);
    if(!outp.is_open())
      	return 0;
    
    
    outp.seekp(0,ios::end);
    int ct=outp.tellp();
    outp.seekp(0,ios::beg);
    if(ct!=0){
    	outp.close();
    	return 0;
	}
		
	string x;
	x.resize( ceil((float)sz/8) );
	long long int wsz=x.size();	

	for(i=0;i<sz;i+=8)
	{
		for(j=0,nono=0;j<8;j++)
		{
			nono*=2;
			if(i+j>=sz)
				continue;
			if(s[i+j]=='1')
				nono++;
			
		}
		//if(i<(1078*8))
			//cout<<(int)nono<<' ';//												<-
		x[i/8]=nono;
	}

	outp<<x;

	tim+=omp_get_wtime()-start;
	cout<<endl<<x.size()<<endl;

	ct=outp.tellp();
	cout<<"\nSize of file = "<<ct<<" B"<<endl;
        
    outp.close();

    return tim;
}
