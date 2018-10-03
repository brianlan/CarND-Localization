#include <iostream>
#include <algorithm>
#include <vector>

#include "helpers.h"
using namespace std;

std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev);

float motion_model(float pseudo_position, float movement, std::vector<float> priors,
                   int map_size, int control_stdev);

//function to get pseudo ranges
std::vector<float> pseudo_range_estimator(std::vector<float> landmark_positions, 
                                          float pseudo_position);

//observation model: calculates likelihood prob term based on landmark proximity
float observation_model(std::vector<float> landmark_positions, std::vector<float> observations, 
                        std::vector<float> pseudo_ranges, float distance_max, 
                        float observation_stdev);

int main() {
    
    //set standard deviation of control: 
    float control_stdev = 1.0f;
    
    //set standard deviation of position:
    float position_stdev = 1.0f;

    //meters vehicle moves per time step
    float movement_per_timestep = 1.0f;

    //number of x positions on map
    unsigned int map_size = 25;

    //initialize landmarks
    std::vector<float> landmark_positions {5, 10, 20};
    
    // initialize priors
    std::vector<float> priors = initialize_priors(map_size, landmark_positions,
                                                  position_stdev);
    
    //step through each pseudo position x (i)    
    for (unsigned int i = 0; i < map_size; ++i) {
        float pseudo_position = float(i);

        //get the motion model probability for each x position
        float motion_prob = motion_model(pseudo_position, movement_per_timestep,
                            priors, map_size, control_stdev);
        
        //print to stdout
        std::cout << pseudo_position << "\t" << motion_prob << endl;
    }    

    return 0;
};

//TODO, implement the motion model: calculates prob of being at an estimated position at time t
float motion_model(float pseudo_position, float movement, std::vector<float> priors,
                   int map_size, int control_stdev) {

    //initialize probability
    float position_prob = 0.0f;
    
    //YOUR CODE HERE
    for ( int i = 0 ; i < map_size ; ++i ) {
		position_prob += Helpers::normpdf(pseudo_position - i, movement, control_stdev) * priors[i];
	}
    
    return position_prob;
}

//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev
std::vector<float> initialize_priors(int map_size, std::vector<float> landmark_positions,
                                     float position_stdev) {
//initialize priors assumimg vehicle at landmark +/- 1.0 meters position stdev

    //set all priors to 0.0
    std::vector<float> priors(map_size, 0.0);


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

    //set each landmark positon +/1 to 1.0/9.0 (9 possible postions)
    /* float normalization_term = landmark_positions.size() * (position_stdev * 2 + 1); */
    /* for (unsigned int i = 0; i < landmark_positions.size(); i++){ */
    /*     int landmark_center = landmark_positions[i]; */
    /*     priors[landmark_center] = 1.0f/normalization_term; */
    /*     priors[landmark_center - 1] = 1.0f/normalization_term; */
    /*     priors[landmark_center + 1] = 1.0f/normalization_term; */
    /* } */
    return priors;
}

std::vector<float> pseudo_range_estimator(std::vector<float> landmark_positions, float pseudo_position) {
    
    //define pseudo observation vector:
    std::vector<float> pseudo_ranges;
            
    //loop over number of landmarks and estimate pseudo ranges:
    //YOUR CODE HERE
    for ( size_t i = 0 ; i < landmark_positions.size() ; ++i ) {
        if ( landmark_positions[i] - pseudo_position > 0 ) {
            pseudo_ranges.push_back(landmark_positions[i] - pseudo_position);
        }
    }

    //sort pseudo range vector:
    //YOUR CODE HERE
    // bubble sort
    for ( size_t i = 0 ; i < pseudo_ranges.size() ; ++i ) {
        for ( size_t j = i ; j < pseudo_ranges.size() - 1 ; ++j ) {
            if ( pseudo_ranges[j] > pseudo_ranges[j + 1] ) {
                float tmp = pseudo_ranges[j + 1];
                pseudo_ranges[j + 1] = pseudo_ranges[j];
                pseudo_ranges[j] = tmp;
            }
        }
    }
    
    return pseudo_ranges;
}

//TODO Complete the observation model function
//calculates likelihood prob term based on landmark proximity
float observation_model(std::vector<float> landmark_positions, std::vector<float> observations, 
                        std::vector<float> pseudo_ranges, float distance_max,
                        float observation_stdev) {

    float distance_prob = 1.0f;
    //YOUR CODE HERE
    if ( pseudo_ranges.size() == 0) {
        return Helpers::normpdf(distance_max, 0, observation_stdev);
    }

    for ( size_t i = 0 ; i < observations.size() ; ++i ) {
        float p;
        if ( pseudo_ranges.size() > 0) {
            p = Helpers::normpdf(observations[i], pseudo_ranges[0], observation_stdev);
            //cout << "observ " << observations[i] << ", pseudo range " << pseudo_ranges[0] << ", prob = " << p << endl;
            pseudo_ranges.erase(pseudo_ranges.begin());
        }
        else {
            p = Helpers::normpdf(distance_max, 0, observation_stdev);
            //cout << "observ " << observations[i] << ", no matching pseudo range, prob = " << p << endl;
        }
        distance_prob *= p;
    }
    
    return distance_prob;
}
