#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>

using namespace std;

//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev
std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev);

int main() {

    //set standard deviation of position:
    float position_stdev = 1.0f;

    //set map horizon distance in meters 
    int map_size = 25;

    //initialize landmarks
    std::vector<float> landmark_positions {5, 10, 20};

    // initialize priors
    std::vector<float> priors = initialize_priors(map_size, landmark_positions,
                                                  position_stdev);
    
    //print values to stdout 
    for (unsigned int p = 0; p < priors.size(); p++) {
        std::cout << priors[p] << endl;
    }
        
    return 0;

};

//TODO: Complete the initialize_priors function
std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev) {

//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev

    //set all priors to 0.0
    std::vector<float> priors(map_size, 0.0);
    
    //YOUR CODE HERE
    float sum_value = 0.0;
    for ( size_t i = 0 ; i < landmark_positions.size() ; ++i ) {
        int pos = round(landmark_positions[i]);
        if ( pos >= 0 && pos < map_size ) {
            priors[pos] = 1;
            sum_value += 1;
        }
        else {
            printf("Invalid landmark_position %d", pos);
        }
        for ( int j = round(position_stdev) ; j > 0 ; --j ) {
            if ( pos - j >= 0 ) {
                priors[pos - j] = 1;
                sum_value += 1;
            }
            if ( pos + j < map_size ) {
                priors[pos + j] = 1;
                sum_value += 1;
            }
        }
    }

    for ( size_t i = 0 ; i < priors.size() ; ++i ) {
        priors[i] /= sum_value;
    }

    return priors;
}

