#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;

auto start = chrono::steady_clock::now();

bool inSet(int currFeature, vector<int> set) {

    for(unsigned int i = 0; i < set.size(); ++i) {
        if(currFeature == set.at(i)) {
            return true;
        }
    }

    return false;

}

double SumDistance(vector<double> object, vector<double> row) {

    double sum = 0;

    for(unsigned int i = 0; i < row.size(); ++i) {
        sum += pow(object.at(i) - row.at(i), 2);
    }

    sum = sqrt(sum); 

    //cout << sum << endl;

    return sum;

}

double stub() {                                                 //test function

    double random = 0;

    random = rand() % 100 + 1;

    return random;
}

void PrintSet(vector<int> set) {


    for(unsigned int i = 0; i < set.size(); ++i) {
        cout << set.at(i) << ","; 
    }

}

double accuracy(vector<vector<double>> _data, vector<int> current_set, int feature_to_add) {        //calculate leave_out_cross_validation

    vector<double> object_to_classify;
    vector<double> temp;
    double label_object_to_classify = 0;
    double nearest_neighbor_distance = 0;
    double nearest_neighbor_location = 0;
    double distance = 0;
    double nearest_neighbor_label = 0;
    double number_correctly_classified = 0;
    double accuracy = 0;

    current_set.push_back(feature_to_add);

    if(feature_to_add != -1) { 
        for(unsigned int i = 1; i < _data[0].size(); ++i) {             //Zero out all features that are not in the current set
            if(!inSet(i, current_set)) {
                for(unsigned int j = 0; j < _data.size(); ++j) {
                    _data[j][i] = 0;
                }
            }
        }
    }
        
    for(unsigned int i = 0; i < _data.size(); ++i) {                //Main nested for loop that calculates features nearest neighbor/feature
        object_to_classify = {_data[i].begin() + 1, _data[i].end()};
        label_object_to_classify = _data[i][0];

        nearest_neighbor_distance = INFINITY;
        nearest_neighbor_location = INFINITY;
        for(unsigned int j = 0; j < _data.size(); ++j) {
            if(j != i) {
                temp = {_data[j].begin() + 1, _data[j].end()};
                distance = SumDistance(object_to_classify, temp);
                if(distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = _data[nearest_neighbor_location][0];
                }
            }
        }
        // cout << "Object " << i + 1 << " is class " << label_object_to_classify << endl;
        // cout << "Its nearest neighbor is " << nearest_neighbor_location + 1 << " which is in class " << nearest_neighbor_label << endl;
        if(label_object_to_classify == nearest_neighbor_label) {
            ++number_correctly_classified;
        }
    }

    accuracy = number_correctly_classified / _data.size();

    return accuracy;
}

double accuracy_BackWards(vector<vector<double>> _data, vector<int> current_set, int feature_to_remove) {       //Calculates Accuracy for backwards elimination

    vector<double> object_to_classify;
    vector<double> temp;
    double label_object_to_classify = 0;
    double nearest_neighbor_distance = 0;
    double nearest_neighbor_location = 0;
    double distance = 0;
    double nearest_neighbor_label = 0;
    double number_correctly_classified = 0;
    double accuracy = 0;

    for(unsigned int i = 0; i < current_set.size(); ++i) {              //Erases feature to remove in current set
        if(current_set.at(i) == feature_to_remove) {
            current_set.erase(current_set.begin() + i);
        }
    }

    for(unsigned int i = 1; i < _data[0].size(); ++i) {
        if(!inSet(i, current_set)) {
            for(unsigned int j = 0; j < _data.size(); ++j) {
                _data[j][i] = 0;
            }
        }
    }
    
    for(unsigned int i = 0; i < _data.size(); ++i) {
        object_to_classify = {_data[i].begin() + 1, _data[i].end()};
        label_object_to_classify = _data[i][0];

        nearest_neighbor_distance = INFINITY;
        nearest_neighbor_location = INFINITY;
        for(unsigned int j = 0; j < _data.size(); ++j) {
            if(j != i) {
                temp = {_data[j].begin() + 1, _data[j].end()};
                distance = SumDistance(object_to_classify, temp);
                if(distance < nearest_neighbor_distance) {
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = _data[nearest_neighbor_location][0];
                }
            }
        }
        // cout << "Object " << i + 1 << " is class " << label_object_to_classify << endl;
        // cout << "Its nearest neighbor is " << nearest_neighbor_location + 1 << " which is in class " << nearest_neighbor_label << endl;
        if(label_object_to_classify == nearest_neighbor_label) {
            ++number_correctly_classified;
        }
    }

    accuracy = number_correctly_classified / _data.size();
    

   // cout << endl;

    // for(unsigned int i = 0; i < object_to_classify.size(); ++i) {
    //     cout << object_to_classify.at(i) << " ";
    // } 

    return accuracy;
}


void search(vector<vector<double>> dat) {           //Search algorithm for searching through dataset

    vector<int> currSet;
    vector<int> set;
    double max = 0;
    int feature_to_add;
    double _accuracy = 0;
    double best = 0;

    for(int i = 1; i < dat[0].size(); ++i) {                                //Main nested for loop. Outside for loop searches through the level of the tree
        //cout << "On the " << i  << "'th level of the search tree" << endl;
        best = 0;
        feature_to_add  = 0;
        for(int j = 1; j < dat[0].size(); ++j) {                            //Nested for loop will consider adding features
            if (inSet(j, currSet) != true) {
                cout << "   Using features(s)  " << "{" << j << ",";
                PrintSet(currSet);
                cout << "}";
                _accuracy = accuracy(dat, currSet, j);                      //Calculates leave_out_cross_validation or accuracy when adding feature
                cout << " accuracy is " << _accuracy*100 << "%" << endl;
                if(_accuracy > best) {                                      //Store best accuracy
                    best = _accuracy;
                    feature_to_add = j;
                }
            }
        }
        currSet.push_back(feature_to_add);
        if(best > max) {                                                    //Stores total max accuracy calculated and stores that accuracy's set
            max = best;
            set = currSet;
        }
        //cout << "On level " << i << " I added feature " << feature_to_add << " to current set" << endl;
        cout << "Feature set {";
        PrintSet(currSet);
        cout << "}" << " was best, accuracy is " << best*100 << "%" << endl;
    }

    cout << endl;
   
    cout << "Finished search!! The best feature subset is {";
    
    for(unsigned int i = 0; i < set.size(); ++i) {
        cout << set.at(i) << ",";
    }

    cout << "}, " << "which has an accuracy of " << max*100 << "%";
    

}

void searchBackWards(vector<vector<double>> dat) {

    vector<int> current_set_of_features;
    int feature_to_remove = 0;
    double best_so_far_accuracy = 0;
    double _accuracy = 0;
    double max = 0;
    vector<int> set;

    for(unsigned int i = 1; i < dat[1].size(); ++i) {
        current_set_of_features.push_back(i); 
    }

    for(unsigned int i = 1; i < dat[0].size(); ++i) {
        //cout << "On the " << i  << "'th level of the search tree" << endl;
        best_so_far_accuracy = 0;
        feature_to_remove = 1;
        for(unsigned int j = 1; j < dat[0].size(); ++j) {
            if (inSet(j, current_set_of_features) == true) {
                cout << "--Considering removing the " << j << " feature" << endl;
                _accuracy = accuracy_BackWards(dat, current_set_of_features, j);
                if(_accuracy > best_so_far_accuracy) {
                    best_so_far_accuracy = _accuracy;
                    feature_to_remove = j;
                }
            }
        }
        for(unsigned int i = 0; i < current_set_of_features.size(); ++i) {
            if(current_set_of_features.at(i) == feature_to_remove) {
                current_set_of_features.erase(current_set_of_features.begin() + i);
            }
        }
        if(max < best_so_far_accuracy) {
            max = best_so_far_accuracy;
            set = current_set_of_features;
        }
        cout << "On level " << i << " i removed feature " << feature_to_remove << " to current set" << endl;
        cout << "Best accuracy at this level: "  << best_so_far_accuracy << endl << endl;
    }

    cout << endl;
    cout << "Best Accuracy: " << max << endl;
    cout << "Best Set of Features: ";
    for(unsigned int i = 0; i < set.size(); ++i) {
        cout << set.at(i) << " ";
    }
    
}


int main() {

    string fileName;
    ifstream myFile;
    string line;
    double entry;

    srand(time(NULL));

    cout << "Enter file name: ";

    cin >> fileName;
    cout << endl;

    myFile.open(fileName);

    vector<vector<double>> data;
    vector<int> test;

    if(myFile.fail()) {
        cout << "File failed to open." << endl;
        return 0;
    }

    while(getline(myFile, line)) {
        data.push_back(vector<double>());
        istringstream ss(line);
        while(ss >> entry) {
            data.back().push_back(entry);
        }
    }

    myFile.close();

    //int temp = 0;

   // cout << accuracy(data, test, temp);

    int userInput = 0;

    cout << "Forward Selection (1) ";
    cout << "Backward Elimination (2) ";

    cin >> userInput;

    int count = 0;

    for(unsigned int i = 0; i < data[0].size(); ++i) {
        ++count;
    }

    count = count - 1;

    cout << "This dataset has " << count << " features (not including the class attribute), with " << data.size() << " instances." << endl; 

    vector<int> temp;

    for(unsigned int i = 1; i < data[0].size(); ++i) {
        temp.push_back(i);
    }


    cout << "Running nearest neighbor with all " << count << " features, using 'leaving-one-out' evaluation, I get an accuracy of ";
    cout << accuracy(data, temp, -1) * 100 << "%" << endl;
    cout << "Beginning Search." << endl;

    if(userInput == 1) {
        search(data);
    }
    else if(userInput == 2) {
        searchBackWards(data);
    }

    cout << endl;

    auto end = chrono::steady_clock::now();

    auto diff = end - start;

    cout << chrono::duration<double, milli>(diff).count() << " ms" << endl;

    return 0;

}