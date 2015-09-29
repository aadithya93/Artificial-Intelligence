#define MAX 100

class stack
{
	char a[MAX];
	int top;
	public: 
            stack()
            {
                   top=-1;
            }           
            int isempty()
            {
            	if(top==-1)return 1;
            	return 0;
            }
            
            int isfull()
            {
            	if(top==MAX)return 1;
            	return 0;
            }
            
            void push(int value)
            {
            	if(!isfull())
            	{
            		top++;
	                a[top] = value;
            	}
            }
            
            int pop()
            {
            	if(!isempty())
            	{
            		top--;
			return a[top+1];
            	}
            }
};
