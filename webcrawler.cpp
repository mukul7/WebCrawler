#include<fstream>
#include<iostream>
#include<string.h>
#include<windows.h>

using namespace std;

class index_page
{
	string homepage,data_file,sublink_file;
	ifstream infile;
	ofstream outfile;
	char* fname;
	public:
		static int link_count;										//Static Variable Declaration
		index_page(string home,string file="Data.txt")				//Parameterised Constructor
		{															//Dynamic Initialisation of object;
			homepage=home;											//With Default Value
			infile.open(homepage);
			data_file=file;
			outfile.open(data_file,ios_base::app);
		}
		
		void display_link_count()									//Inline Function
		{
			cout<<link_count<<" links saved\n";
		}
		
		int isworking()
		{
			if(infile.is_open() && outfile.is_open() )
				return 1;
			else
				return 0;
		}
		
		friend void fetchlinks(index_page &);								//Friend Function
		
		void fetchsublinks(string);											//Polymorphism
		void fetchsublinks(string,string);
		void getfilename()
		{
			cin>>sublink_file;
			sublink_file=sublink_file+".txt";
			fetchsublinks(data_file,sublink_file);
			
		}
		~index_page()														//Destructor
		{
			infile.close();
			outfile.close();
		}
		char* getfname_sublink()
		{
			int len=sublink_file.length();
			fname = new char [len];
			strcpy(fname, sublink_file.c_str());
			return fname;
		}
};

void fetchlinks(index_page &b)
{
	string line;
	while(getline(b.infile,line))												//Reference Variable;
	{
		int line_length=line.length(),start=0,stop=0;
		for(int i=0;i<line_length;i++)
		{
			if(!start)
			{
				if(line[i]=='h' && line[i+1]=='r' && line[i+2]=='e' && line[i+3]=='f' && line[i+4]=='=')
				{
					i+=5;
					start=i;
				}
			}
			else if(start && line[i]=='"')
			{
				stop=i;
				break;
			}
		}
		if(start && stop)
		{
			for(int i=start+1;i<stop;i++)
				b.outfile<<line[i];
			b.outfile<<endl;
			index_page::link_count++;
		}
	}
	b.outfile.close();
}

void index_page::fetchsublinks(string indexes_file)
{
	string link;
	int lines;
	ifstream count;
	count.open(indexes_file);
	while(getline(count,link))
		lines++;
	count.close();
	ifstream indexes;
	indexes.open(indexes_file);
	while(getline(indexes,link) && lines)
	{
		link="html/"+link;
		index_page sublin(link,data_file);
		if(!(sublin.isworking()))
		{
			cout<<link<<" Not Found!"<<endl;
		}
		fetchlinks(sublin);
		lines--;
	}
	indexes.close();			
}

void index_page::fetchsublinks(string indexes_file,string sublink_file)
{
	string link;
	int lines;
	ifstream count;
	count.open(indexes_file);
	while(getline(count,link))
		lines++;
	count.close();
	ifstream indexes;
	indexes.open(indexes_file);
	while(getline(indexes,link) && lines)
	{
		link="html/"+link;
		index_page sublin(link,sublink_file);
		if(!(sublin.isworking()))
		{
			cout<<link<<" Not Found!"<<endl;
		}
		fetchlinks(sublin);
		lines--;
	}
	indexes.close();			
}

int index_page::link_count=0;

class choice
{
	char c;
	public:
	int get()
	{
		cout<<"(y/n)";
		cin>>c;
		if(c=='y' || c=='Y')
			return 1;
		else
			return 0;
			
	}
};

int main()
{
	string index_webpage="html/index.html",data_file="links.txt";
	index_page ind(index_webpage,data_file);
	choice Q;
	if(ind.isworking())
	{
			fetchlinks(ind);
			cout<<"Do you want to save sublinks in same file?";
			if(Q.get())
				ind.fetchsublinks(data_file);								
			else
			{	
				cout<<"Enter File Name -> ";									//Calling from object
				ind.getfilename();
			}
			ind.display_link_count();
	}
	else
	{
		cout<<"Index File Not Found";
	}
	
	cout<<"Do you want to view created files?";
	
	if(Q.get())
	{
		system("links.txt");
		system(ind.getfname_sublink());
		
	}
	cout<<"Do you want to delete created files?";
	if(Q.get())
	{
		if(remove("links.txt") && remove(ind.getfname_sublink()))
			perror( "Error Deleting Files" );
		else
			cout<<"Files Deleted Successfully!";
	}
	
}
