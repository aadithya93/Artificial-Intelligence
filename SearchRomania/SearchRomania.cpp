#include<iostream>
#include<string.h>
#include "stack.h"
#include "queue.h"
using namespace std;


class Map
{
	string cities[20];
	int adj_matrix[20][20];	
	public:
		void load_cities(string names[20])
		{
			for(int i=0;i<20;i++)
				cities[i] = names[i];
		}

		int get_city_index(string name)
		{
			for(int i=0;i<20;i++)
				if(name == cities[i]) return i;
		}

		void build_map()
		{
			for(int i=0;i<20;i++)
				for(int j=0;j<20;j++)
					adj_matrix[i][j] = 0;
			adj_matrix[get_city_index("oradea")][get_city_index("zerind")] = 71;  
			adj_matrix[get_city_index("zerind")][get_city_index("arad")] = 75;
			adj_matrix[get_city_index("arad")][get_city_index("timisoara")] = 118;
			adj_matrix[get_city_index("timisoara")][get_city_index("lugoj")] = 111;
			adj_matrix[get_city_index("lugoj")][get_city_index("mehadia")] = 70;
			adj_matrix[get_city_index("dobreta")][get_city_index("mehadia")] = 75;
			adj_matrix[get_city_index("oradea")][get_city_index("sibiu")] = 151;
			adj_matrix[get_city_index("arad")][get_city_index("sibiu")] = 140;
			adj_matrix[get_city_index("dobreta")][get_city_index("craiova")] = 120;
			adj_matrix[get_city_index("sibiu")][get_city_index("rimnicu_vilcea")] = 80;
			adj_matrix[get_city_index("sibiu")][get_city_index("fagaras")] = 99;
			adj_matrix[get_city_index("rimnicu_vilcea")][get_city_index("craiova")] = 146;
			adj_matrix[get_city_index("pitesti")][get_city_index("craiova")] = 138;
			adj_matrix[get_city_index("rimnicu_vilcea")][get_city_index("pitesti")] = 97;
			adj_matrix[get_city_index("bucharest")][get_city_index("pitesti")] = 101;
			adj_matrix[get_city_index("bucharest")][get_city_index("fagaras")] = 211;
			adj_matrix[get_city_index("bucharest")][get_city_index("giurgiu")] = 90;
			adj_matrix[get_city_index("bucharest")][get_city_index("urziceni")] = 85;
			adj_matrix[get_city_index("vaslui")][get_city_index("urziceni")] = 142;
			adj_matrix[get_city_index("hirsova")][get_city_index("urziceni")] = 98;
			adj_matrix[get_city_index("hirsova")][get_city_index("eforie")] = 86;
			adj_matrix[get_city_index("vaslui")][get_city_index("iasi")] = 92;
			adj_matrix[get_city_index("neamt")][get_city_index("iasi")] = 87;
			for(int i=0;i<20;i++)
				for(int j=0;j<20;j++)
					if(adj_matrix[i][j])
						adj_matrix[j][i] = adj_matrix[i][j];

/*			for(int i=0;i<20;i++)
			{
				for(int j=0;j<20;j++)
					cout<<adj_matrix[i][j]<<"  "; 
				cout<<endl;
			}*/
		}

		string dfs(int start,int dest)
		{
			stack st;
    			int visited[20];
			string path = "";
			for(int i=0;i<20;i++)
				visited[i]=0;	
			st.push(start);

    				while(!st.isempty())
				{
					int src;
					src = st.pop();
					if(visited[src] == 1)continue;
					visited[src] = 1;
					path = path + cities[src] + ","; 
					if (src == dest)break;
					for(int i=19;i>=0;i--)
						if(adj_matrix[src][i] && visited[i] ==0)
						{
							st.push(i);
						}
					
				}
			return path;
		}
		
		string bfs(int start, int dest)
		{
			queue q;
    			int visited[20];
			string path = "";
			for(int i=0;i<20;i++)
				visited[i]=0;	
			visited[start] = 1;
			q.enqueue(start);
			visited[start] =1;
			while(!q.isempty())
			{
				int src;
				src = q.dequeue();
				path = path + cities[src] + ",";
				if(src == dest) break;
				for(int i=0;i<20;i++)
					if(adj_matrix[src][i] && visited[i] ==0)
					{
						q.enqueue(i);
						visited[i] =1;
					}
			}
			return path;
		}
};

int main(int argc, char *argv[])
{
	string names[20]={"arad","bucharest","craiova","dobreta","eforie","fagaras","giurgiu",
"hirsova","iasi","lugoj","mehadia","neamt","oradea","pitesti","rimnicu_vilcea","sibiu","timisoara",
"urziceni","vaslui","zerind"};

	string path;
	Map romania;
	romania.load_cities(names);
	romania.build_map();
	if(strcmp(argv[1],"BFS")==0)
		path = romania.bfs(romania.get_city_index(argv[2]),romania.get_city_index(argv[3]));
	else if(strcmp(argv[1],"DFS")==0)
		path = romania.dfs(romania.get_city_index(argv[2]),romania.get_city_index(argv[3]));
	cout<<path.substr(0,path.size()-1)<<endl;
	return 0;
}
