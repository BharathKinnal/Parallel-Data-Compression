//#include<bits/stdc++.h>
#include"readfilep.cpp"
#include"writefilep.cpp"
#include<omp.h>
#define th 4
using namespace std;

string comp(char* rFile,char* wFile)
{
	int minLength = 1;
    string s1,st[th],s2;
    string sti,interStr,zeroes;//,temps[4-1],tempf[4-1];
	string value,flag,length;
	int iStr,iRun,stisz,header,sizeOfValue,sizeOfLength,maxBits,bitsRepresentingLength,numZeroes,ts[4-1],tf[4-1];
    int nothread;
    int beg,count,i,j,lt;
    
    float start,tim;
    
    tim=readfile(rFile,s1);
	   
	//s1 = "10011100001111100000011111110000000011111111100000000001111111111100000000000011111111111110000000000000011111111111111100000000000000001";
	
	cout<<"\nInitial size = "<<s1.size()<<endl;
	start=omp_get_wtime();
	
    long long int sz=s1.size();
    
	//Header of the compressed file, which is 1 byte long, containing the min length of a run that can be compressed
	header = minLength;
	for(int q=128 ; q>0 ; q/=2)
	{
		if(header/q  ==  1)
			s2.push_back('1');
		else
			s2.push_back('0');
		header %= q;
	}

	//Compression of file
	#pragma omp parallel num_threads(th) shared(st) \
	firstprivate(s1,sz,minLength)\
	private(j,nothread,sti,stisz,beg,iStr,iRun,count,lt,\
	value,flag,length,sizeOfValue,sizeOfLength,\
	maxBits,interStr,bitsRepresentingLength,numZeroes,zeroes)
	{		
		nothread=omp_get_thread_num();
		
		st[nothread]="";
		
		beg=(int)((float)sz/th);
		
		lt=(nothread+1)*beg;
		if(nothread==th-1)
			lt=sz;
		
		beg*=nothread;
		
		sti=s1.substr(beg,lt-beg);
		stisz=sti.size();
		
		iStr=0;
		count=1;
		
		for(iRun=0;iRun<stisz;iRun++)
		{
			if(iRun<stisz-1 && sti[iRun+1] == sti[iRun])
				count++;
				
			else
			{
				//if(iStr<(10*8))
		      		//cout<<count<<" -> "<<s1.substr(iStr,count)<<" - ";//							<-
	
				if(count<=minLength){
		       		st[nothread] +=  sti.substr(iStr,count);
				
					//if(iStr<beg+(10*8))
						//cout<<sti.substr(iStr,count)<<endl;//										<-				
				}
				
				else{       	
													
					value=flag=length="";
		    
		       		value = sti.substr(iStr,minLength);
		       		
	
	
		            sizeOfValue = (int) log2( count-(minLength-1) );
 	           
		            for( j=0 ; j<sizeOfValue ; j++)
		            {
		            	flag+=sti.substr(iStr,1);
					}
					
		      		if(flag.at(0)-'0' == 0)
		       			flag += "1";
		       		else
		       			flag += "0";
					
	
					
					
 	     			sizeOfLength = (count-(minLength-1)) - (int)pow(2,sizeOfValue);
 	      			maxBits = sizeOfValue; 	      			
 	      			interStr="";
	
		        	while( sizeOfValue>0 )
 		           	{
 		           	    bitsRepresentingLength = (int)sizeOfLength/pow(2,--sizeOfValue);
 		           	    sizeOfLength %= (int)pow(2,sizeOfValue);
 		           	    if(bitsRepresentingLength == 1)
 		           	        interStr.push_back('1');
 		           	    else
 		           	        interStr.push_back('0');
 		           	}
					
 		       		if(maxBits-interStr.size()>0) {
	 	       			numZeroes = maxBits-interStr.size();
 		       			zeroes = "0";
    	    			while(numZeroes>1){
    	    				zeroes +=  "0";
    	    				numZeroes--;
    	    			}
    	    			length =  zeroes;
    	    		}
    	    		length += interStr;

					
	    			st[nothread] += value+flag+length;
	
					//if(iStr<beg+(10*8))
    	   				//cout<<value<<' '<<flag<<' '<<length<<endl;//									<-
    	    	}

    	    	iStr+=count;
    	    	if(iRun<stisz-1)
					count = 1;
			}
    	}
    }
 
 	for(i=0;i<th;i++)
 		s2+=st[i]; 		
 	
	tim+=omp_get_wtime()-start;
	tim+=writefile(wFile,s2);
    
    cout<<"\nCompressed length = "<<s2.size()<<endl;
    cout<<"Compression rate = "<<((float)sz/s2.size())<<endl;    
    //cout<<s2<<endl;
            
	cout<<"Time taken = "<<tim<<" s\n";
    return s2;	
}

main(int argc,char **argv)
{
	comp(argv[1],argv[2]);
}
