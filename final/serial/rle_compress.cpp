/*
		This program is for compressing the file using the RLE algorithm	
*/

#include<bits/stdc++.h>
#include"readfile.cpp"
#include"writefile.cpp"

using namespace std;

main(int argc, char** argv)
{
	float start,tim=0;
    string s1,s2;
    
    tim+=readfile(argv[1],s1);
    //s1="100111000011111000000111111100000000111111111000000000011111111111";
    cout<<"\nInitial size = "<<s1.size()<<endl;
	start=omp_get_wtime();
	
    int count=1,i=0,j=0,max_val=0,sz,temp,k,length;
    vector<int> freq;

    //To set frequency counter
    for(i=0;i<s1.size();i++)
    {
    	if(i<s1.size()-1 && s1.at(i)==s1.at(i+1))
        	count++;
        else
        {
        	freq.push_back(count);
            if(max_val < count)
        	max_val = count;
            count = 1 ;
        }
    }

        /*
        for(i=0;i<freq.size();i++)
                cout<<freq[i];
        cout<<endl;
        */

    
    tim+=omp_get_wtime()-start;
    cout<<"Size of length = "<<sz<<endl;
    //s2.push_back((char)sz);
    start=omp_get_wtime();


	//Adding the header, which consists of 1 argument which is the size required to store the length of a run
	sz=log2(max_val-1)+1;
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
	temp=0;
	
    for(i=0;i<freq.size() && temp<s1.size();i++)
    {
    	//cout<<s1.substr(temp,freq[i])<<" - ";//												<-
        
        s2+=s1.substr(temp,1);
        
        //cout<<s1.substr(temp,1);//															<-

        string intermediate;
        
        length=freq[i]-1;
        for(k=sz-1;k>=0;k--)
        {
        	int twemp=pow(2,k);
            if(length/twemp==1)
            	intermediate.push_back('1');
            else
                intermediate.push_back('0');
            length%=twemp;
	    }
 
        //cout<<' '<<intermediate<<endl;//														<-
 
        s2+=intermediate;
        temp+=freq[i];
    }
    tim+=omp_get_wtime()-start;
    
    cout<<"\nFinal length = "<<s2.size()<<endl;
    cout<<"\nCompression rate = "<<(float)s1.size()/s2.size()<<endl;
    
    tim+=writefile(argv[2],s2);
    cout<<"Time taken = "<<tim<<" s\n";
    return 0;
}
