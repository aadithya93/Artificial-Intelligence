#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<math.h>
#include<string.h>
using namespace std;

class city
{
	int index;
	string name;
	double latitude;
	double longitude;
	public:
		void setIndex(int i)
		{
			this->index = i;
		}
		int getIndex()
		{
			return index;
		}
		void setName(string name)
		{
			this->name = name;
		}
		string getName()
		{
			return name;
		}
		void setLatitude(double lat)
		{
			this->latitude = lat;
		}
		double getLatitude()
		{
			return latitude;
		}
		void setLongitude(double lon)
		{
			this->longitude = lon;
		}
		double getLongitude()
		{
			return longitude;
		}
		void printCity()
		{
			cout<<index<<": "<<name<<" ("<<latitude<<", "<<longitude<<")\n";
		}
};

struct closed_list
{
	int city_index;
	double fValue;
};

struct open_path
{
	int city_path[120];
	int pathlen;
	double fValue;
};

int fCompare(const void* lhs, const void* rhs)
{
	const open_path* a = (open_path*)lhs;
	const open_path* b = (open_path*)rhs;	
		
	if(a->fValue < b->fValue) return -1;
	else if(a->fValue > b->fValue) return 1;
	else return 0;
}
		
class Map
{
	city cities[112];
	int numCities;
	int map_matrix[112][112];
	public:
		Map()
		{
			numCities=0;
			for(int i=0;i<112;i++)
				for(int j=0;j<112;j++)
					map_matrix[i][j] = 0;
		}
		void readCityList()
		{
				string name="",lat="",lon="";
				ifstream input;
				input.open("cities.txt",ios::in);
				while(!input.eof())
				{
					 input >> name;
					 input >> lat;
					 input >> lon;
					 name = name.substr(5,name.length()-6);
					 lat = lat.substr(0,lat.length()-1);
					 lon = lon.substr(0,lon.length()-2);
					 cities[numCities].setIndex(numCities);
					 cities[numCities].setName(name);
					 cities[numCities].setLatitude(atof(lat.c_str()));
					 cities[numCities].setLongitude(atof(lon.c_str()));
					 numCities++;
				}
				input.close();
		}
		int getIndexByName(string name)
		{
			for(int i=0;i<numCities;i++)
				if(name==cities[i].getName())return cities[i].getIndex();
		}
		string getNameByIndex(int index)
		{
				return cities[index].getName();
		}
		double computeHsld(int src,int dest)
		{
			double Hsld;
			double pi = 3.141593;
			Hsld = sqrt(pow((69.5 * (cities[src].getLatitude() - cities[dest].getLatitude())), 2) + pow((69.5 * cos((cities[src].getLatitude() + cities[dest].getLatitude())/360.0 * pi) * (cities[src].getLongitude() - cities[dest].getLongitude())),2));
			return Hsld;
		}
		double computeDist(int city_path[], int pathlen)
		{
			double sum =0;
			for(int i=1;i<pathlen;i++)
				sum += map_matrix[city_path[i]][city_path[i-1]];
			return sum;
		}
		void buildMap()
		{
			string src="",dest="",dist="";
			ifstream input;
			int count =0;
			input.open("roads.txt",ios::in);
			while(!input.eof())
			{
				input >> src;
				input >> dest;
				input >> dist;
				src = src.substr(5,src.length()-6);
				dest = dest.substr(0,dest.length()-1);
				dist = dist.substr(0,dist.length()-2);	
				map_matrix[getIndexByName(src)][getIndexByName(dest)] = atoi(dist.c_str());
				map_matrix[getIndexByName(dest)][getIndexByName(src)] = atoi(dist.c_str());
			}
			input.close();
		}
		void displayCities()
		{
			for(int i=0;i<numCities;i++)
				cities[i].printCity();
		}
		void displayMap()
		{
			int count=0;
			for(int i=0;i<numCities;i++)
			{
				for(int j=0;j<numCities;j++)
					if(map_matrix[i][j]>0)
						count++;	
			}
			cout<<count<<endl;
		}
		int in(int place, closed_list explored[], int numExplored)
		{
			for(int i=0;i<numExplored;i++)
				if(place == explored[i].city_index) return 1;
			return 0;
		}
		void astar(string src, string dest)
		{
			closed_list explored[112];
			open_path frontier[100];
			open_path solution;
			int numPaths = 0, numExplored = 0;
			frontier[numPaths].city_path[0] = getIndexByName(src);
			frontier[numPaths].pathlen = 1;
			frontier[numPaths].fValue = computeHsld(getIndexByName(src),getIndexByName(dest));
			numPaths++;
			while(numPaths>0 && !in(getIndexByName(dest),explored,numExplored))
			{
				if(in(frontier[0].city_path[frontier[0].pathlen-1],explored,numExplored))
				{
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;
				}		
				else
				{
					for(int i=0;i<numCities;i++)
					{
						if(in(i,explored,numExplored)==0)
						{
							if(map_matrix[i][frontier[0].city_path[frontier[0].pathlen-1]] && getIndexByName(dest)!=frontier[0].city_path[frontier[0].pathlen-1])
							{
								for(int j=0;j<frontier[0].pathlen;j++)
									frontier[numPaths].city_path[j] = frontier[0].city_path[j];
								frontier[numPaths].city_path[frontier[0].pathlen] = i;
								frontier[numPaths].pathlen = frontier[0].pathlen+1;
								frontier[numPaths].fValue = computeDist(frontier[0].city_path,frontier[0].pathlen) + map_matrix[i][frontier[0].city_path[frontier[0].pathlen-1]] + computeHsld(i,getIndexByName(dest));
								numPaths++;
							}
						}
					}
					explored[numExplored].city_index = frontier[0].city_path[frontier[0].pathlen-1];
					explored[numExplored].fValue = frontier[0].fValue;
					numExplored++;//add explored
					solution = frontier[0];
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;//Remove first				
					qsort(frontier,numPaths,sizeof(open_path),fCompare);//sort
				}				
			}
			cout<<"Explored\n";
			for(int i=0;i<numExplored;i++)
			{
				cout<<getNameByIndex(explored[i].city_index);
				if(i<(numExplored-1))cout<<", ";
			}
			cout<<"\nNumber of nodes explored: "<<numExplored<<"\nSolution path\n";
			for(int i=0;i<solution.pathlen;i++)
			{
				cout<<getNameByIndex(solution.city_path[i]);
				if(i<(solution.pathlen-1))cout<<", ";
			}
			cout<<endl<<"Path length = "<<solution.pathlen<<endl<<"Path distance = "<<solution.fValue<<endl;
		}
		void greedy(string src, string dest)
		{
			closed_list explored[112];
			open_path frontier[100];
			open_path solution;
			int numPaths = 0, numExplored = 0;
			frontier[numPaths].city_path[0] = getIndexByName(src);
			frontier[numPaths].pathlen = 1;
			frontier[numPaths].fValue = computeHsld(getIndexByName(src),getIndexByName(dest));
			numPaths++;
			while(numPaths>0 && !in(getIndexByName(dest),explored,numExplored))
			{
				if(in(frontier[0].city_path[frontier[0].pathlen-1],explored,numExplored))
				{
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;
				}		
				else
				{
					for(int i=0;i<numCities;i++)
					{
						if(in(i,explored,numExplored)==0)
						{
							if(map_matrix[i][frontier[0].city_path[frontier[0].pathlen-1]] && getIndexByName(dest)!=frontier[0].city_path[frontier[0].pathlen-1])
							{
								for(int j=0;j<frontier[0].pathlen;j++)
									frontier[numPaths].city_path[j] = frontier[0].city_path[j];
								frontier[numPaths].city_path[frontier[0].pathlen] = i;
								frontier[numPaths].pathlen = frontier[0].pathlen+1;
								frontier[numPaths].fValue = computeHsld(i,getIndexByName(dest));
								numPaths++;
							}
						}
					}
					explored[numExplored].city_index = frontier[0].city_path[frontier[0].pathlen-1];
					explored[numExplored].fValue = frontier[0].fValue;
					numExplored++;//add explored
					solution = frontier[0];
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;//Remove first				
					qsort(frontier,numPaths,sizeof(open_path),fCompare);//sort
				}				
			}
			cout<<"Explored\n";
			for(int i=0;i<numExplored;i++)
			{
				cout<<getNameByIndex(explored[i].city_index);
				if(i<(numExplored-1))cout<<", ";
			}
			cout<<"\nNumber of nodes explored: "<<numExplored<<"\nSolution path\n";
			for(int i=0;i<solution.pathlen;i++)
			{
				cout<<getNameByIndex(solution.city_path[i]);
				if(i<(solution.pathlen-1))cout<<", ";
			}
			cout<<endl<<"Path length = "<<solution.pathlen<<endl<<"Path distance = "<<computeDist(solution.city_path,solution.pathlen)<<endl;
		}
		void uniform(string src, string dest)
		{
			closed_list explored[112];
			open_path frontier[100];
			open_path solution;
			int numPaths = 0, numExplored = 0;
			frontier[numPaths].city_path[0] = getIndexByName(src);
			frontier[numPaths].pathlen = 1;
			frontier[numPaths].fValue = 0;
			numPaths++;
			while(numPaths>0 && !in(getIndexByName(dest),explored,numExplored))
			{
				if(in(frontier[0].city_path[frontier[0].pathlen-1],explored,numExplored))
				{
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;
				}		
				else
				{
					for(int i=0;i<numCities;i++)
					{
						if(in(i,explored,numExplored)==0)
						{
							if(map_matrix[i][frontier[0].city_path[frontier[0].pathlen-1]] && getIndexByName(dest)!=frontier[0].city_path[frontier[0].pathlen-1])
							{
								for(int j=0;j<frontier[0].pathlen;j++)
									frontier[numPaths].city_path[j] = frontier[0].city_path[j];
								frontier[numPaths].city_path[frontier[0].pathlen] = i;
								frontier[numPaths].pathlen = frontier[0].pathlen+1;
								frontier[numPaths].fValue = computeDist(frontier[0].city_path,frontier[0].pathlen) + map_matrix[i][frontier[0].city_path[frontier[0].pathlen-1]];
								numPaths++;
							}
						}
					}
					explored[numExplored].city_index = frontier[0].city_path[frontier[0].pathlen-1];
					explored[numExplored].fValue = frontier[0].fValue;
					numExplored++;//add explored
					solution = frontier[0];
					for(int i=1;i<numPaths;i++)
						frontier[i-1]=frontier[i];
					numPaths--;//Remove first				
					qsort(frontier,numPaths,sizeof(open_path),fCompare);//sort
				}				
			}
			cout<<"Explored\n";
			for(int i=0;i<numExplored;i++)
			{
				cout<<getNameByIndex(explored[i].city_index);
				if(i<(numExplored-1))cout<<", ";
			}
			cout<<"\nNumber of nodes explored: "<<numExplored<<"\nSolution path\n";
			for(int i=0;i<solution.pathlen;i++)
			{
				cout<<getNameByIndex(solution.city_path[i]);
				if(i<(solution.pathlen-1))cout<<", ";
			}
			cout<<endl<<"Path length = "<<solution.pathlen<<endl<<"Path distance = "<<solution.fValue<<endl;
		}
};

int main(int argc, char** argv)
{
	if(argc==4)
	{
		Map Usa;
		Usa.readCityList();
		Usa.buildMap();
		if (strcmp(argv[1],"astar")==0)
		{
			cout<<"A-Star"<<endl;
			Usa.astar(argv[2],argv[3]);
		}
		else if(strcmp(argv[1],"greedy")==0)
		{
			cout<<"Greedy"<<endl;
			Usa.greedy(argv[2],argv[3]);
		}
		else if(strcmp(argv[1],"uniform")==0)
		{
			cout<<"Uniform"<<endl;
			Usa.uniform(argv[2],argv[3]);
		}
	}
	return 0;
}
	
		
