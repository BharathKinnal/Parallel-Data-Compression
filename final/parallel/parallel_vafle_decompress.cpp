#include"readfilep.cpp"
#include"writefilep.cpp"
#include<omp.h>
using namespace std;
#define th 4

string decomp(char* rFile,char* wFile)//(char* fil)
{
	string s2,s1;
	string str[th];
	int iRun=0,iStr=0,header,k;
	int minLength=0;
	
	float tim=0,start;
	
    //s1="0000000110010110100010011100100011011011000010001111000100001010111100110001100111101001000011101111011100000100001";	
    tim+=readfile(rFile,s1);
    cout<<"\nCompressed size = "<<s1.size()<<endl;
	start=omp_get_wtime();

	//Extracting the header, which indicates the minimum length of a run in the original file that can be compressed	
	for(header=0;header<8;header++)
	{
		minLength*=2;
		if(s1.at(header)=='1')
			minLength+=1;
	}

	//Decompressing the file
	
	//Note : This part cannot be parallelized, as the encoded runs have variable sizes \
	\
			 And since parallelizing the decompression algorithm would require splitting the data which is encoded to chunks,\
			 it would be impossible to decode each chunk, since the data at the ends of the chunks would be corrupted
			 
	int num,tempnum;
    for(iRun=8;iRun<s1.size();)
    {
    	for(iStr=iRun+1;iStr<s1.size() && s1.at(iStr-1)==s1.at(iStr);iStr++);
        
    	//if(iRun<100*8+8)
        	//cout<<iStr<<'-'<<iRun<<'='<<iStr-iRun<<" => ";//																	<-
		if(iStr>=s1.size())
			break;
		
    	if(iStr-iRun<=minLength)
        {
            //if(iRun<100*8+8)
        		//cout<<s1.substr(iRun,iStr-iRun)<<" - ";//																	<-
            s2+=s1.substr(iRun,iStr-iRun);
            //if(iRun<100*8+8)
        		//cout<<s1.substr(iRun,iStr-iRun)<<endl;//																	<-
            iRun=iStr;
        }
        else
        {
            iStr++;
            int offset=iStr-(iRun+minLength+1);
            //if(iRun<100*8+8)
        		//cout<<s1.substr(iRun,minLength)<<' '<<s1.substr(iRun+minLength,iStr-(iRun+minLength))<<' '<<s1.substr(iStr,offset)<<" - ";//		<-
            
            num=(int)pow(2,offset);
            tempnum=0;
            for(k=0;k<offset;k++)
            {
                tempnum*=2;
                if(s1.at(iStr+k)=='1')
                {
                    tempnum+=1;
                }
            }
            num+=tempnum+minLength-1;
            
            char decompBit=s1.at(iRun);
            for(k=0;k<num;k++){
                    s2.push_back(decompBit);
                    //if(iRun<100*8+8)
        				//cout<<decompBit;//																				<-
            }            
            //if(iRun<100*8+8)
        		//cout<<endl;//																						<-
        		        		
            iRun=iStr+offset;
        }
    }
    
        
    tim+=omp_get_wtime()-start;
    
    tim+=writefile(wFile,s2);
    
    cout<<"\nDecompressed length = "<<s2.size()<<endl;
    cout<<"Time taken = "<<tim<<" s\n";
    return s2;
}


main(int argc,char **argv)
{
	decomp(argv[1],argv[2]);
}

