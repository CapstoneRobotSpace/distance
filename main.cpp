#include "serial.cpp"
#include <iostream>
#include <cstring>

using namespace std;
int main(int argc,char **argv){
	Serial LRR("/dev/ttyUSB0",115200);
	if(!LRR.good){
		cout<<"Sensor ERROR"<<endl;
		return 0;
	}
	char buf[9] {};
	int buf_index = 0;
	memset(buf,'\0',9);
	unsigned int dist = 0;
	while(1){
		if(buf_index < 2){
			LRR.sread(buf+buf_index,1);
			if(*(buf+buf_index) == 0x59)
				buf_index ++;
			else
				buf_index = 0;
		}
		else{
			LRR.sread(buf+buf_index,1);
			buf_index++;
			if(buf_index == 9){
				char check = 0;
				for(int i=0;i<8;i++)
					check += buf[i];
				if(check == buf[8]){
					unsigned int temp = (0xffff & buf[3])<<8;
					temp += buf[2];
					if(temp < 180000)
						dist = temp;
					cout<<dist<<endl;
				}
				buf_index = 0;
			}
		}
	}
}