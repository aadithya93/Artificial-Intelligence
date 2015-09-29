#define MAX 100

class queue
{
	int a[MAX];
	int front,rear;
	public:
           queue()
           {
                  front =0;
                  rear = -1;
           }
            int isempty()
            {
            	if(rear==front-1)return 1;
            	return 0;
            }
            
            int isfull()
            {
            	if(rear==MAX-1)return 1;
            	return 0;
            }
            
            void enqueue(int value)
            {
            	if(!isfull())
            	{
            		rear++;
            		a[rear] = value;
            	}
            }
            
            int dequeue()
            {
            	if(!isempty())
            	{
            		front++;
			return a[front-1];
            	}
            }
};

