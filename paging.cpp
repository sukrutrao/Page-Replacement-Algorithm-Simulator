#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

class PageReplacement
{
	private:
		int number_of_faults;
		int number_of_frames;
		int number_of_frames_used;
		vector<int> frames;
		vector<string> fault_list;
		const string file_name;
		const string roll_number;
		fstream file_object;
		fstream output_file;
		string replacement_algorithm;
		const string fault;
		const string no_fault;
		void FIFO_and_LRU(string replacement_algorithm);
		void OPTIMAL();
		void display();
	public:
		PageReplacement(string input_file_name, string input_roll_number);
		void evaluate(string replacement_algorithm);		
};

PageReplacement::PageReplacement(string input_file_name, string input_roll_number) :
	file_name(input_file_name), roll_number(input_roll_number),
	fault("FAULT"), no_fault("NO FAULT")
{
	file_object.open(file_name.c_str(),ios::in);
	if(!file_object)
	{
		cout << "Error : Could not open " << file_name << endl;
		exit(1);
	}
	number_of_faults = 0;
	number_of_frames_used = 0;
	file_object >> number_of_frames;
	frames.resize(number_of_frames,-1);
}

void PageReplacement::evaluate(string replacement_algorithm)
{
	if(replacement_algorithm == "FIFO" || replacement_algorithm == "LRU")
	{
		FIFO_and_LRU(replacement_algorithm);
	}
	else if(replacement_algorithm == "OPTIMAL")
	{
		OPTIMAL();
	}
	else
	{
		cout << "Error : Unknown replacement algorithm" << endl;
		exit(1);
	}
	output_file.open((roll_number + "_" + replacement_algorithm + ".out").c_str(),ios::out);
	if(!output_file)
	{
		cout << "Error : Could not open output file" << endl;
		exit(1);
	}
	output_file << number_of_faults << endl;
	for(int i = 0; i < fault_list.size(); i++)
	{
		output_file << fault_list[i] << endl;
	}
	output_file.close();
	file_object.close();
}

void PageReplacement::FIFO_and_LRU(string replacement_algorithm)
{
	int current_page;
	vector<int>::iterator it;
	vector<int> ordering;
	ordering.resize(number_of_frames,0);
	while(file_object >> current_page)
	{
		it = find(frames.begin(),frames.end(),current_page);
		if(it != frames.end())
		{
			fault_list.push_back(no_fault);
			if(replacement_algorithm == "LRU")
			{
				int index = distance(frames.begin(),it);
				ordering[index] = 0;
			}
		}
		else
		{
			if(number_of_frames_used < number_of_frames)
			{
				frames[number_of_frames_used] = current_page;
				number_of_frames_used++;
			}
			else
			{
				int evict_index = distance(ordering.begin(),max_element(ordering.begin(), ordering.end()));
				frames[evict_index] = current_page;
				ordering[evict_index] = 0;
			}
			fault_list.push_back(fault);
			number_of_faults++;
		}
		for(int i = 0; i < number_of_frames_used; i++)
		{
			ordering[i]++;
		}
	}
}

void PageReplacement::OPTIMAL()
{
	int current_page;
	vector<int>::iterator it;
	vector<int> marker;
	marker.resize(number_of_frames,-1);
	vector<int> page_accesses;
	int number_marked = 0;
	while(file_object >> current_page)
	{
		page_accesses.push_back(current_page);
	}
	for(int i = 0; i < page_accesses.size(); i++)
	{
		current_page = page_accesses[i];
		it = find(frames.begin(),frames.end(),current_page);
		if(it != frames.end())
		{
			fault_list.push_back(no_fault);
		}
		else
		{
			if(number_of_frames_used < number_of_frames)
			{
				frames[number_of_frames_used] = current_page;
				number_of_frames_used++;
			}
			else
			{
				vector<int>::iterator find_it;
				for(int j = 0; j < frames.size(); j++)
				{
					find_it = find(page_accesses.begin() + i + 1, page_accesses.end(),frames[j]); //check for seg fault
					marker[j] = distance(page_accesses.begin(),find_it);
				}
				int evict_index = distance(marker.begin(),max_element(marker.begin(),marker.end()));
				frames[evict_index] = current_page;
			}
			fault_list.push_back(fault);
			number_of_faults++;
		}
	}
}


void PageReplacement::display()
{
	for(int i = 0; i < number_of_frames_used; i++)
	{
		cout << frames[i] << " ";
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	string replacement_algorithm;
	const string file_name = "PageAccessSequence.txt";
	const string roll_number = "CS15BTECH11036";
	fstream file_object;
	if(argc != 2)
	{
		cout << "Usage : ./<binaryname> <replacement-algorithm>" << endl;
		return 1;
	}
	replacement_algorithm = argv[1];
	PageReplacement pageReplacement(file_name, roll_number);
	pageReplacement.evaluate(replacement_algorithm);
	return 0;
}
