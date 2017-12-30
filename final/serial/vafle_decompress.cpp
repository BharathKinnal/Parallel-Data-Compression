/*
		This program is for decompressing a file storing compressed data using the VaFLe algorithm	
*/

#include"readfile.cpp"
#include"writefile.cpp"
#include<omp.h>
using namespace std;

string decomp(char* rFile,char* wFile)//(char* fil)
{
	float start=omp_get_wtime(),tim=0;
	
	string s2,s1;
	int iStr=0,iRun=0,header,k,num,tempnum,offset;
	int minLength=0;
	char decompBit;
	
	tim+=omp_get_wtime()-start;
    tim+=readfile(rFile,s1);
    //s1="0000000110010110100010011100100011011101100001000111100010000101011110011000011001111010100001110111101110000010000000";
    cout<<"\nCompressed size = "<<s1.size()<<endl;
	start=omp_get_wtime();
	
	for(header=0;header<8;header++)
	{
		minLength*=2;
		if(s1.at(header)=='1')
			minLength+=1;
		cout<<s1.at(header);
	}
	//minLength-=128;
	
	cout<<minLength<<endl;
    for(iStr=8;iStr<s1.size();)
    {
        for(iRun=iStr+1;iRun<s1.size() && s1.at(iRun-1)==s1.at(iRun);iRun++);
        
        //if(iStr<100*8+8)
        	//cout<<iRun<<'-'<<iStr<<'='<<iRun-iStr<<" => ";//																	<-
		if(iRun>=s1.size())
			break;
		
    	if(iRun-iStr<=minLength)
        {
            //if(iStr<100*8+8)
        		//cout<<s1.substr(iStr,iRun-iStr)<<" - ";//																	<-
            s2+=s1.substr(iStr,iRun-iStr);
            //if(iStr<100*8+8)
        		//cout<<s1.substr(iStr,iRun-iStr)<<endl;//																	<-
            iStr=iRun;
        }
        else
        {
            iRun++;
            offset=iRun-(iStr+minLength+1);
            //if(iStr<100*8+8)
        		//cout<<s1.substr(iStr,minLength)<<' '<<s1.substr(iStr+minLength,iRun-(iStr+minLength))<<' '<<s1.substr(iRun,offset)<<" - ";//		<-
            
            num=(int)pow(2,offset);
            tempnum=0;
            for(k=0;k<offset;k++)
            {
                tempnum*=2;
                if(s1.at(iRun+k)=='1')
                {
                    tempnum+=1;
                }
            }
            num+=tempnum+minLength-1;
            
            decompBit=s1.at(iStr);
            for(k=0;k<num;k++){
            	s2.push_back(decompBit);
                    //if(iStr<100*8+8)
        				//cout<<decompBit;//																				<-
            }            
            //if(iStr<100*8+8)
        		//cout<<endl;//																						<-
        		        		
            iStr=iRun+offset;
        }
    }
    tim+=omp_get_wtime()-start;
    
    cout<<"\nDecompressed length = "<<s2.size()<<endl;
    
    tim+=writefile(wFile,s2);
    cout<<"Time taken = "<<tim<<" s\n";
    
    return s2;
}


main(int argc,char **argv)
{
	decomp(argv[1],argv[2]);
}

