/*
		This program is for compressing the file using the VaFLe algorithm	
*/


#include"readfile.cpp"
#include"writefile.cpp"
using namespace std;

string comp(char* rFile,char* wFile)
{
	float start=omp_get_wtime(),tim=0;
	
	int minLength = 1;
    string s1,s2;
	string value,flag,length,interStr,zeroes;
	int iStr,iRun,header,sizeOfValue,sizeOfLength,maxBits,bitsRepresentingLength,numZeroes;
    int count,i,j;
    
    tim+=omp_get_wtime()-start;
    tim+=readfile(rFile,s1);
    
	
    //s1 = "1001110000111110000001111111000000001111111110000000000111111111110000000000001111111111111000000000000001111111111111110000000000000000";
    cout<<"\nInitial size = "<<s1.size()<<endl;
	start=omp_get_wtime();
	
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
	for(iStr=iRun=0,count=1;iRun<s1.size();iRun++)
	{
		if(iRun<s1.size()-1 && s1.at(iRun+1) == s1.at(iRun))
			count++;
			
		else
		{
			//if(iStr<(10*8))
	      		//cout<<count<<" -> "<<s1.substr(iStr,count)<<" - ";//							<-

	       	if(count<=minLength){
	       		s2 += (s1.substr(iStr,count));

				//if(iStr<(10*8))
					//cout<<s1.substr(iStr,count)<<endl;//										<-
    
				iStr = iStr+count;   
	       	}
	       	else{       	
	       	
				value=flag=length="";
	       		value += (s1.substr(iStr,minLength));


	            sizeOfValue = (int)log2( count - (minLength-1) );
            
	            for(j = 0;j<sizeOfValue;j++)
	            {
	            	flag += (s1.substr(iStr,1));
				}
			
	      		if(flag[0]-'0' == 0)
	       			flag += "1";
	       		else
	       			flag += "0";



      			sizeOfLength = (count-(minLength-1)) - (int)pow(2,sizeOfValue);
       			maxBits = sizeOfValue;
       			interStr="";

	        	while(sizeOfValue>0)
            	{
            	    bitsRepresentingLength = (int)sizeOfLength/pow(2,--sizeOfValue);
            	    sizeOfLength %= (int)pow(2,sizeOfValue);
            	    if(bitsRepresentingLength == 1)
            	        interStr += "1";
            	    else
            	        interStr += "0";
            	}

        		if(maxBits-interStr.size()>0) {
        			numZeroes = maxBits-interStr.size();
        			zeroes = "0";
        			while(numZeroes>1){
        				zeroes +=  "0";
        				numZeroes--;
        			}
        			length +=  zeroes;
        		}
        		length +=  interStr;



        		s2 +=  (value+flag+length);

				//if(iStr<(10*8))
       				//cout<<value<<' '<<flag<<' '<<length<<endl;//									<-

        		iStr = iStr+count;        		
        	}
			count = 1;
		}
		
    }
	tim+=omp_get_wtime()-start;
	
	cout<<"\nCompressed length = "<<s2.size()<<endl;
    cout<<"Compression rate = "<<((float)s1.size()/s2.size())<<endl;    
    
    tim+=writefile(wFile,s2);
    cout<<"Time taken = "<<tim<<" s\n";
        
    return s2;	
}

main(int argc,char **argv)
{
	comp(argv[1],argv[2]);
}
