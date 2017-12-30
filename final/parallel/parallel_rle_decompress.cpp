/*
		This program is for decompressing a file storing compressed data using the RLE algorithm	
*/

#include<bits/stdc++.h>
#include"readfilep.cpp"
#include<omp.h>
#include"writefilep.cpp"

using namespace std;

main(int argc, char** argv)
{
		float start,tim=0;
        string s1,s2;

        tim+=readfile(argv[1],s1);
        //s1="100000000001100010000011100100000101100110000111101000001001101010010000";
        cout<<"\nInitial size = "<<s1.size()<<endl;
		start=omp_get_wtime();
		
		//Extracting the header, which indicates the size used to store the length of the original run
        int sz=0,i,j;
        for(i=0;i<8;i++)
        {
        	sz*=2;
        	if(s1.at(i)=='1')
        		sz++;
        }
        
        //Decompressing the file
        for(i=8;i<s1.size();)
        {
                //cout<<s1.at(i)<<' '<<s1.substr(i+1,sz)<<" - ";//											<-
                char c=s1.at(i);
                int num=0;
                
                if(i+sz<s1.size())
                {
    	            for(j=1;j<=sz;j++)
    	            {
    	                    num*=2;
    	                    if(s1.at(i+j)=='1')
    	                            num+=1;
	
    	            }
    	            num+=1;
	                for(j=0;j<num;j++){
                    	    s2.push_back(c);
                	}
                }
                //cout<<endl;//																				<-
                i+=1+sz;
        }
        
        tim+=omp_get_wtime()-start;
        cout<<"\nFinal length = "<<s2.size()<<endl;
        tim+=writefile(argv[2],s2);
        cout<<"Time taken = "<<tim<<" s\n";
        return 0;
}
