#include<iostream>
#include<random>
using namespace std;

int main()
{
	int number_of_page_accesses;
	cin>>number_of_page_accesses;
	int number_of_frames;
	cin>>number_of_frames;
	int maxval;
	cin>>maxval;
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> dist(0,maxval);	
	cout<<number_of_frames<<endl;
	for(int i=0;i<10;i++)
	{
		int min = dist(rng);
		int max = dist(rng);
		if(min>max)
		{
			int temp = min;
			min = max;
			max = temp;
		}
		for(int i = 0; i < number_of_page_accesses/10; i++)
		{
			uniform_int_distribution<int> rand(min,max);
			cout<<rand(rng)<<endl;
		}		
	}
	return 0;
}
