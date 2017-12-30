/*
		This program is for compressing the file using the RLE algorithm	
*/

#include<bits/stdc++.h>
#include"readfilep.cpp"
#include"writefilep.cpp"
#include<omp.h>

using namespace std;
#define th 4

main(int argc, char** argv)
{
	float start,tim=0;
	
    string s1,s2,st[th];

    tim+=readfile(argv[1],s1);
    //s1="100111000011111000000111111100000000111111111000000000011111111111";
    cout<<"\nInitial size = "<<s1.size()<<endl;
	start=omp_get_wtime();

    int count=1,index=0,i=0,j=0,max_val=0;
    vector<int> freq[th];

    //To set frequency counter
	#pragma omp parallel firstprivate(count) private(i) reduction(max:max_val)
	{
		int nothread=omp_get_thread_num();
		
		int beg=s1.size()/th,end;
		if(nothread==th-1)
			end=s1.size();
		else
			end=beg*(nothread+1);
		beg*=nothread;
		
    	for(i=beg;i<end;i++)
    	{
    		if(i<end-1 && s1.at(i)==s1.at(i+1))
    	    	count++;
    	    else
    	    {
    	    	freq[nothread].push_back(count);
    	        if(max_val < count)
    	    		max_val = count;
    	        count = 1 ;
    	    }
    	}
    }

        /*
        for(i=0;i<freq.size();i++)
                cout<<freq[i];
        cout<<endl;
        */

    int sz=log2(max_val-1)+1,temp=0;
    
    tim+=omp_get_wtime()-start;
    cout<<"Size of length = "<<sz<<endl;
    start=omp_get_wtime();

	//Adding the header, which consists of 1 argument which is the size required to store the length of a run
	temp=sz;
	for(j=128;j>0;j/=2)
	{
		if(temp/j==1)
			s2.push_back('1');
		else
			s2.push_back('0');
		temp%=j;
	}
	
	//Compressing the file
	#pragma omp parallel private(i,temp)
	{
		int nothread=omp_get_thread_num();
		
		st[nothread]="";
		
		int beg=s1.size()/th,end,k,twemp,length;
		if(nothread==th-1)
			end=s1.size();
		else
			end=beg*(nothread+1);
		beg*=nothread;
		
    	for(i=0,temp=beg;i<freq[nothread].size() && temp<end;i++)
    	{
    		//cout<<s1.substr(temp,freq[i])<<" - ";//												<-
    	    st[nothread]+=s1.substr(temp,1);
    	    //cout<<s1.substr(temp,1);//															<-
	
    	    string intermediate;
    	    length=freq[nothread][i]-1;
    	    for( k=sz-1, twemp=pow(2,sz-1) ; k>=0 ; k--, twemp/=2 )
    	    {    	    	
    	        if(length/twemp==1)
    	        	intermediate.push_back('1');
    	        else
    	            intermediate.push_back('0');
    	        length%=twemp;
		    }
    	    //cout<<' '<<intermediate<<endl;//														<-
    	    st[nothread]+=intermediate;
    	    temp+=freq[nothread][i];
    	}
    }
    
    for(i=0;i<th;i++)
    	s2+=st[i];
    
    tim+=omp_get_wtime()-start;
    cout<<"\nFinal length = "<<s2.size()<<endl;
    cout<<"\nCompression rate = "<<(float)s1.size()/s2.size()<<endl;

    tim+=writefile(argv[2],s2);
    cout<<"Time taken = "<<tim<<" s\n";

    return 0;
}
