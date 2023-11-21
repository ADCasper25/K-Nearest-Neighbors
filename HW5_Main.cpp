#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include "attribute.h"
#include "instance.h"

// parses a line of data
void Parse_Data(string s, const vector<Attribute>&  attribute_list, vector<Instance>& example_list){
  Instance cur_instance;
  cur_instance.Set_Attributes(attribute_list);

  stringstream ss;
  ss.str(s);
  int cur_attribute = 0;
  string cur_val;
  while(ss >> cur_val){
    if(cur_val[cur_val.size()-1] == ',')
      cur_val = cur_val.substr(0, cur_val.size()-1); // trim off comma
    cur_instance.Add_Value(cur_attribute, cur_val);
    cur_attribute++;
  }
  if(cur_attribute != attribute_list.size()){
    cout << "ERROR: Wrong number of attributes on line: " << s << endl;
    exit(1);
  }
  example_list.push_back(cur_instance);
  
}

// gets a new attribute from an @attribute line
void New_Attribute(string s, vector<Attribute>& attribute_list){
  Attribute cur_attribute;
  stringstream ss;
  ss.str(s);
  string junk;
  ss >> junk; // remove "@attribute"
  string name;
  ss >> name;
  cur_attribute.Set_Name(name);
  string next; // either "{" or "numeric"
  ss >> next;
  if(next == "numeric")
    cur_attribute.Set_Numeric(true);
  else{
    cur_attribute.Set_Numeric(false);
    string temp;
    while(temp != "}"){
      ss >> temp;
      if(temp != "}"){
	if(temp[temp.size()-1] == ',')
	  temp = temp.substr(0, temp.size()-1); // trim off comma
	cur_attribute.Add_Category(temp);
      }
    }
  }
  attribute_list.push_back(cur_attribute);
}

void Print_Data(vector<Attribute>& attribute_list, vector<Instance>& examples){
  char choice = 'y';
  while(choice == 'y'){
    cout << "We have " << examples.size() << " examples.  Which would you like? ";
    int cur_example;
    cin >> cur_example;

    cout << "Type 0 to see the all attributes" << endl;
    for(int i = 0; i < attribute_list.size(); i++){
      cout << "Type " << i+1 << " to see value of attribute " << attribute_list[i].Get_Name() << endl;
    }
    int which_attribute;
    cin >> which_attribute;
    for(int i = 0; i < attribute_list.size(); i++){
      if(which_attribute == 0 || which_attribute == i+1){
	if(attribute_list[i].Numeric())
	  cout << attribute_list[i].Get_Name() << ": "
	       << examples[cur_example].Get_Numeric_Value(i) << endl;
	else
	  cout << attribute_list[i].Get_Name() << ": "
	       << examples[cur_example].Get_Nominal_Value(i) << endl;
      }
    }
    cout << "Another? (y/n) ";
    cin >> choice;
  }
}

//I just took the code to parse the data and made it a function so I could call it twice (once for each file)
void ParseDataset(ifstream &fin, vector<Attribute> &attribute_list, vector<Instance> &examples) {
    bool data_mode = false;
    string s;
    int num_read = 0;

    while(getline(fin, s)) {
        if(s[s.size()-1] == '\r') // grrr
            s = s.substr(0, s.size()-1);
        
        if(s.size() > 0 && s[0] != '%') { // ignore comments
            if(data_mode) { // it's a line of data
                Parse_Data(s, attribute_list, examples);
            } else {
                // then it had better start with an '@'
                if(s[0] != '@') {
                    cout << "ERROR: ILLEGAL LINE: " << s << endl;
                    exit(1);
                }
                // is it @attribute?
                else if(s.substr(0,10) == "@attribute")
                    New_Attribute(s, attribute_list);
                //is it @data?
                else if(s.substr(0,5) == "@data")
                    data_mode = true;
                else {
                    cout << "ERROR: ILLEGAL LINE: " << s << endl;
                    exit(1);
                }
            }
        }
        num_read++;
        //cout << num_read << endl;
    }
}

double Distance(const Instance& first_instance, const Instance& second_instance, const vector<Attribute>& attribute_list){
	//create our distance variable that we will return
	double distance = 0.0;
	
	//now loop through every attribute except the classification attribute
	for(int i = 0; i < attribute_list.size() - 1; i++){
		//if the attribute we're looking at is numeric
		if(first_instance.Is_Numeric_Attribute(i)){
			distance += pow((first_instance.Get_Numeric_Value(i) - second_instance.Get_Numeric_Value(i)), 2);
		}
		
		//else we're looking at a nominal attribute
		else{
			//if the first and second instance attribute is the same
			if(first_instance.Get_Nominal_Value(i) != second_instance.Get_Nominal_Value(i)){
				//then yi - xi = 1 so...
				distance += 1.0;
			}
			//else they're the same, so yi - xi = 0 so do nothing
		}
	}
	//once we leave the for loop we should have the sum of everything under the square root so return the square root of distance
	return sqrt(distance);
}


vector<int> Find_K_Nearest_Neighbors(const Instance& new_instance, const vector<Instance>& training_examples, const vector<Attribute>& attribute_list, int K){
	//initalize our two vectors that we'll be keeping track of the be the size of how many neighbors we need (K)
	vector<int> neighbor_indicies(K, -1);
	vector<double> neighbor_distances(K, 100000000000000);
	
	//begin by loooping throuhg all instances
	for(int i = 0; i < training_examples.size(); i++){
		//compute the the distance between the new instance and each training data instance
		double cur_distance = Distance(new_instance, training_examples[i], attribute_list);
		
		//check to see if this current distance is smaller than anything currently in our vector...
		int j = 0;
		//increment j while we're still checking in the vector and while the current distance is NOT smaller than what is currently in the vector
		while(j < K && cur_distance > neighbor_distances[j]){
			j++;
		}
		//if we jump out of the while loop then we either...
		//found a closer neighbor...
		if(j < K){
			//so shift everything to the right of our new distance down one
			for(int h = K - 1; h > j; h--){
				neighbor_distances[h] = neighbor_distances[h - 1];
				neighbor_indicies[h] = neighbor_indicies[h - 1];
			}
			
			//now that everything is shifted
			//insert our newest neighbor
			neighbor_distances[j] = cur_distance;
			neighbor_indicies[j] = i;
		}
		//...or the current distance was not smaller than anything currently in the vector so move onto next instance in training_examples
	}
	
	//we've now looped through all instances and should have our array of nearest neighbors 
	//so return
	return neighbor_indicies;
}


void Find_Max_Min(const vector<Instance> training_examples, vector<double>& maxs, vector<double>& mins){
	int num_attributes = training_examples[0].Get_Num_Attributes();
	
	//resize vectors to be the amount of attributes we have (minus our classification)
	mins.resize(num_attributes - 1);
	maxs.resize(num_attributes - 1);
	
	//lets initalize the values of both our vectors so that we have something to compare to
	//in our next step. We'll just start with min and max value being whatever is in our
	//first instance
	for (int i = 0; i < num_attributes - 1; i++) {
		//check if it is numeric
		if(training_examples[0].Is_Numeric_Attribute(i)){
			mins[i] = training_examples[0].Get_Numeric_Value(i);
			maxs[i] = training_examples[0].Get_Numeric_Value(i);
		}
		//if not then set to 0
		else{
			maxs[i] = 0.0;
			mins[i] = 0.0;
		}
    }	

	//for each instance...
	for(int i = 1; i < training_examples.size(); i++){
		//and for every attribute (minus our classification attribute)
		for(int j = 0; j < num_attributes - 1; j++){
			//check to see if we're looking at a numeric value...
			if(training_examples[i].Is_Numeric_Attribute(j)){
				//look at the current attribute value
				double value = training_examples[i].Get_Numeric_Value(j);
				//...and now check if it is smaller than what is currently in our mins vector...
				if(value < mins[j]){
					mins[j] = value;
				}
			
				//...or if it is bigger than what is in our max vector
				if(value > maxs[j]){
					maxs[j] = value;
				}
			}
			//...or else we're looking at a nominal attribute
			else{
				//so set 0 to the min and max vectors
				maxs[j] = 0.0;
				mins[j] = 0.0;
			}
		}	
	}
}

void Scale(Instance& example, const vector<double> maxs, const vector<double> mins){
	int num_attributes = example.Get_Num_Attributes();

	//loop through every attribute that we need to scale (not classification)
	for(int i = 0; i < num_attributes - 1; i++){
		//only scale numeric attributes
		if(example.Is_Numeric_Attribute(i)){
			//and if they are not the same value
			if(maxs[i] != mins[i]){
				
			//find the value that we want to scale
			double value = example.Get_Numeric_Value(i);
			
			//scale that value
			double scaled_value = (value - mins[i]) / (maxs[i] - mins[i]);
			if(scaled_value < 0){
				//cout << " Attribute: " << i << ". Now actual scaled value: " << scaled_value << " and now value: " << value << endl;
			}
			//add the scaled value to our instance
			example.Add_Value(i, to_string(scaled_value));
			}
		}
	}
}

int main(){
  vector<Attribute> attribute_list_training;
  vector<Attribute> attribute_list_test;
  vector<Instance> training_examples;
  vector<Instance> test_examples;
  int incorrect = 0;
  vector<double> maxs;
  vector<double> mins;
  
  //first read the training data file
  string training_filename;
  cout << "Enter the training filename: ";
  cin >> training_filename;

  ifstream training_fin;
  training_fin.open(training_filename.data());
  while(!training_fin){
    cout << "File not found" << endl;
    cout << "Enter the training filename: ";
    cin >> training_filename;
    training_fin.clear();
    training_fin.open(training_filename.data());
  }
  
  //training file is open so parse it
  ParseDataset(training_fin, attribute_list_training, training_examples);
  
  //find the min and max values after reading in the training data
  Find_Max_Min(training_examples, maxs, mins);
  
  //then scale the training data
  for(int i = 0; i < training_examples.size(); i++){
	Scale(training_examples[i], maxs, mins);
  }
  
  //now read test data file
  string test_filename;
  cout << "Enter the test filename: ";
  cin >> test_filename;
  
  ifstream test_fin;
  test_fin.open(test_filename.data());
  while(!test_fin) {
    cout << "File not found" << endl;
    cout << "Enter the test filename: ";
    cin >> test_filename;
    test_fin.clear();
    test_fin.open(test_filename.data());
  }
  
  //test file is now open so parse it
  ParseDataset(test_fin, attribute_list_test, test_examples);
  
  //now scale the test data with the maxs and mins from the training data
  for(int i = 0; i < test_examples.size(); i++){
	  Scale(test_examples[i], maxs, mins);
  }
  
  //get K from user
  cout << "Insert the value K of many neighbors we should look at: " << endl;
  int K;
  cin >> K;
  
  //now that all our files are open and scaled we need to do something with them
  //loop through all instances in test data
  for(int i = 0; i < test_examples.size(); i++){
	  //find the nearest neighbor(s) for every test example
	  vector<int> nearest_neighbors = Find_K_Nearest_Neighbors(test_examples[i], training_examples, attribute_list_training, K);
	  
	  //create our vector that will hold our nearest neighbor's classifications
	  vector<string> neighbors_classifications;
	  
	  //we need to find what the nearest neighbor's indecie's classifications are...
	  for(int j = 0; j < nearest_neighbors.size(); j++){
		  //...so find out and then...
		  string cur_classification = training_examples[nearest_neighbors[j]].Get_Nominal_Value(attribute_list_training.size() - 1);
		  //...add it to our vector
		  neighbors_classifications.push_back(cur_classification);
	  }
	  
	 //now find the string in neighbors_classifications that is the most common (that will be our prediction)
	 string prediction = "";
	 int maxCount = 0;
	 
	 //loop through our neighbors_classifications vector
	 for(int j = 0; j < neighbors_classifications.size(); j++){
		 int count = 1;
		 //then another for loop so we can compare what we're currently looking at to everything else in the vector
		 for(int h = j + 1; h < neighbors_classifications.size(); h++){
			 //if they're equal...
			 if(neighbors_classifications[j] == neighbors_classifications[h]){
				 //...increment
				 count++;
			 }
		 }
		 //once out of the first for loop we need to check to see if our count of the 
		 //string we just counted is bigger than our max count
		 if(count > maxCount){
			 //if it is... update
			 maxCount = count;
			 prediction = neighbors_classifications[j];
		 }
	 }
	 
	  //once out of both above for loops we should have our prediction so...
	  //check to see if we were right
	  if(prediction != test_examples[i].Get_Nominal_Value(attribute_list_test.size() - 1)){
		  //if we weren't increment by one
		  incorrect++;
		  //and display what was wrong
		  cout << "Instance " << i << " in the test data was misclassified as a " << prediction << ". It was really a " << 
		  test_examples[i].Get_Nominal_Value(attribute_list_test.size() - 1) << endl;
	  }
  }
  
  //once out of the big loop we have how many time we were incorrect so calculate error rate
  double error_rate = static_cast<double>(incorrect) / test_examples.size();
  
  //and display it 
  cout << "Error Rate : " << error_rate << " (" << error_rate * 100 << "%)";
  
  return 0;
}