/*
* File : circularBuffer.cpp
*/


#include<iostream>
using namespace std;

 const int MAX = 100;
 const int minimumUpperLimit = 10; /*10 adalah space minimum yang akan mengaktifkan XOFF*/
 const int maximumLowerLimit = 20;  /*20 adalah space minimum untuk mengaktifkan XON*/

class circularBuffer{
    int a[MAX];
    int head,tail;

    public:
        circularBuffer()
		   {
			 head,tail=-1;
		   }
		   void addElmt(int );
		   int delElmt();
		   void display();
		   bool isOverFlow();
    };

void circularBuffer::addElmt(int elmt){
    if((head==0 && tail==MAX-1) || (head+1==tail))
			  cout<<" Circular Queue is Full";
		 else
		 {
		   if(head==MAX-1)
			  tail=0;
		   else
			 tail++;
		   a[tail]=elmt;
		 }
		 if(head==-1)
		   head=0;
}


int circularBuffer::delElmt(){
    int k;
		 if(head==-1)
			cout<<"Circular Queue is Empty";
		 else
		 {
			k=a[head];
			if(head==tail)
			   head=tail=-1;
			else
			{
			   if(head==MAX-1)
				  head=0;
			   else
				  head++;
			}
		 }
		 return k;

}


void circularBuffer::display(){
    int i;
		  if(head==-1)
			 cout<<"Circular Queue is Empty";
		  else
		  {
			 if(tail < head)
			 {
				for(i=head;i<=MAX-1;i++)
				   cout<<a[i]<<"   ";
				for(i=0;i<=head;i++)
				   cout<<a[i]<<"   ";
			 }
			 else
			 {
				for(i=head;i<=tail;i++)
				   cout<<a[i]<<"   ";
				cout<<endl;
			 }
		  }
}


bool circularBuffer::isOverFlow(){
    if (head>tail){
        if ((MAX-head+tail) <=minimumUpperLimit){
            return true;
        }
        else if ((MAX-head+tail) >=maximumLowerLimit){
            return false;
        }
    }
    else{
        if (head-tail <= minimumUpperLimit){
            return true;
        }
        else if (head-tail >= maximumLowerLimit){
                return false;
            }
        }
}


