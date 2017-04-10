/*
 * Program to simulate FIFO, LRU, and OPTIMAL page replacement algorithms given list of page accesses
 * and number of physical frames
 * 
 * Sukrut Rao
 * CS15BTECH11036
 * 
 * To compile the program, use
 * g++ <filename> -o <executablename> --std=c++11
 * 
 * To execute the program, use
 * ./<executablename> <replacementalgorithm>
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

// class for functions simulating Page Replacement Algorithms and associated data
class PageReplacement
{
	private:
		int number_of_faults; // total number of page faults
		int number_of_frames; // number of physical frames
		int number_of_frames_used; // number of physical frames used
		vector<int> frames; // to store the pages in each frame
		vector<string> fault_list; // to store whether each access was a fault or not
		const string file_name; // input file name
		const string roll_number; // for naming the output file
		fstream file_object; // for the input file
		fstream output_file; // for the output file
		string replacement_algorithm; // the replacement algorithm being used
		const string fault; // text to display when there is a fault
		const string no_fault; // text to display when there is no fault
		void FIFO_and_LRU(string replacement_algorithm); // to simulate FIFO and LRU algorithms
		void OPTIMAL(); // to simulate OPTIMAL algorithm
		void display(); // to display which page is in each frame after each access
	public:
		PageReplacement(string input_file_name, string input_roll_number); //to initialize values
		void evaluate(string replacement_algorithm); // to simulate the algorithm
};

/*
 * constructor function
 * assigns values to const variables, opens files, and initializes vectors and ints
 */
PageReplacement::PageReplacement(string input_file_name, string input_roll_number) :
	file_name(input_file_name), roll_number(input_roll_number),
	fault("FAULT"), no_fault("NOFAULT")
{
	file_object.open(file_name.c_str(),ios::in); // open input file
	if(!file_object) // if open failed
	{
		cout << "Error : Could not open " << file_name << endl;
		exit(1);
	}
	number_of_faults = 0;
	number_of_frames_used = 0;
	file_object >> number_of_frames; // accept number of frames from file
	if(number_of_frames < 0) // if it is negative
	{
		cout << "Error : Number of frames must be a non negative integer" << endl;
		exit(1);
	}
	frames.resize(number_of_frames,-1); // initialize each frame with -1
}

/*
 * to simulate based on the algorithm specified in replacement_algorithm
 */
void PageReplacement::evaluate(string replacement_algorithm)
{
	if(replacement_algorithm == "FIFO" || replacement_algorithm == "LRU") // call based on algorithm specified
	{
		FIFO_and_LRU(replacement_algorithm);
	}
	else if(replacement_algorithm == "OPTIMAL")
	{
		OPTIMAL();
	}
	else // if invalid input given
	{
		cout << "Error : Unknown replacement algorithm" << endl;
		exit(1);
	}
	output_file.open((roll_number + "_" + replacement_algorithm + ".out").c_str(),ios::out); // open output file
	if(!output_file) // if opening file failed
	{
		cout << "Error : Could not open output file" << endl;
		exit(1);
	}
	output_file << number_of_faults << endl; // write to file
	for(int i = 0; i < fault_list.size(); i++)
	{
		output_file << fault_list[i] << endl;
	}
	output_file.close(); // close output file
	file_object.close(); // close input file
}

/*
 * simulates FIFO or LRU based on the value of replacement_algorithm
 */
void PageReplacement::FIFO_and_LRU(string replacement_algorithm)
{
	int current_page; // the current page being accessed
	vector<int>::iterator it; // to iterate over the frames
	vector<int> ordering; // to keep track of the order of accesses
	ordering.resize(number_of_frames,0); // initialize order to 0
	while(file_object >> current_page) // read next page access
	{
		it = find(frames.begin(),frames.end(),current_page); // check if already in a frame
		if(it != frames.end()) // if already there
		{
			fault_list.push_back(no_fault); // there is no fault
			if(replacement_algorithm == "LRU")
			{
				int index = distance(frames.begin(),it);
				ordering[index] = 0; // if the algorithm is LRU, access resets the order so that it is last in the list
			}
		}
		else // if not there
		{
			if(number_of_frames_used < number_of_frames) // if an empty frame is available
			{
				frames[number_of_frames_used] = current_page; // add frame
				number_of_frames_used++; // indicate that one more frame has been used
			}
			else // if no empty frame is available
			{
				int evict_index = distance(ordering.begin(),max_element(ordering.begin(), ordering.end())); //the one to be evicted is the one whose ordering value is the highest
				if(evict_index < number_of_frames) // to check if index is in range
				{
					frames[evict_index] = current_page; // replace with the current page
					ordering[evict_index] = 0; // reset ordering
				}					
			}
			fault_list.push_back(fault); // indicate fault
			number_of_faults++; // increment number of faults
		}
		for(int i = 0; i < number_of_frames_used; i++)
		{
			ordering[i]++; // add one to ordering value of each valid frame
		}
	}
}

/*
 * simulates the OPTIMAL algorithm
 */
void PageReplacement::OPTIMAL()
{
	int current_page;  // the current page being accessed
	vector<int>::iterator it; // to iterate over the frames
	vector<int> marker; // to mark the index of the next page access of the page at the given frame
	marker.resize(number_of_frames,-1); // initialize marker with -1s
	vector<int> page_accesses; // to store the list of page accesses
	vector<bool> is_set; // to store whether marker needs to be updated or not
	is_set.resize(number_of_frames,false);
	while(file_object >> current_page) // read the list of page accesses
	{
		page_accesses.push_back(current_page); // store
	}
	int page_accesses_size = page_accesses.size();
	for(int i = 0; i < page_accesses.size(); i++)
	{
		current_page = page_accesses[i]; // the current page being accessed
		it = find(frames.begin(),frames.end(),current_page); // check if it is already in a frame
		if(it != frames.end()) // if already there
		{
			fault_list.push_back(no_fault); // indicate that this access has no fault
		}
		else // if not there
		{
			if(number_of_frames_used < number_of_frames) // if there is an empty frame
			{
				frames[number_of_frames_used] = current_page; // store in the next available frame
				number_of_frames_used++; // update the number of frames used
			}
			else // if there is no empty frame
			{
				vector<int>::iterator find_it; // to iterate over page accesses
				for(int j = 0; j < frames.size(); j++)
				{
					if(is_set[j] == false || marker[j] <= i) // if marker for current frame needs to be updated
					{
						find_it = find(page_accesses.begin() + i + 1, page_accesses.end(),frames[j]); // find earliest next access of page in current frame
						marker[j] = distance(page_accesses.begin(),find_it); // store index of access in marker
						is_set[j] = true; // marker does not need to be updated for now
					}					
				}
				int evict_index = distance(marker.begin(),max_element(marker.begin(),marker.end())); // the highest value in marker is the latest accessed page, the one to be evicted
				if(evict_index < number_of_frames)  // to check if index is in range
				{
					frames[evict_index] = current_page; // replace with the current page
					is_set[evict_index] = false; // marker needs to be updated next iteration
				}	
			}
			fault_list.push_back(fault); // indicate that there was a fault
			number_of_faults++; // update number of faults
		}
	}
}

/*
 * displays the page stored in each frame
 */
void PageReplacement::display()
{
	for(int i = 0; i < number_of_frames_used; i++)
	{
		cout << frames[i] << " "; // display the page in the ith frame
	}
	cout << endl;
}

int main(int argc, char *argv[])
{
	string replacement_algorithm;
	const string file_name = "PageAccessSequence.txt"; // input file name
	const string roll_number = "CS15BTECH11036"; // for output file name
	fstream file_object;
	if(argc != 2) // if number of arguments supplied is incorrect
	{
		cout << "Usage : ./<binaryname> <replacement-algorithm>" << endl;
		return 1;
	}
	replacement_algorithm = argv[1];
	PageReplacement pageReplacement(file_name, roll_number); // create and initialize object
	pageReplacement.evaluate(replacement_algorithm); // simulate algorithm
	return 0;
}
