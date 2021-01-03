#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <omp.h>

#define Nc 10000
#define Nv 2
#define NUM_STEPS 100000

using std::srand;
using std::rand;
using std::sqrt;
using std::cout;
using std::endl;

// Define arrays as global
float cities[Nc][Nv];
int route[Nc + 1];
int start_ind;
float total_dist = 0.0f;
float total_sq_dist = 0.0f;

// Vector initialisation
void init_vectors(){

    // use current time as seed for random number generator
    srand((unsigned) time(0));

    for (int i=0; i<Nc; i++){
        for (int j=0; j<Nv; j++){
            // generate number in range [0, 1000]
            cities[i][j] = static_cast<float> (rand()) / (static_cast<float> (RAND_MAX/1000));
        }
    }

}

// Calculate the travel distance between a pair of cities
float calculate_dist(int city_a, int city_b){

    float distance_0 = cities[city_a][0] - cities[city_b][0];
    distance_0 *= distance_0;
    float distance_1 = cities[city_a][1] - cities[city_b][1];
    distance_1 *= distance_1;

    return distance_0 + distance_1;

}

// Pick initial route
void init_route(){

    int i = 1;
    bool flag = false;
    start_ind = rand() % Nc;  // randomly pick first city
    route[0] = start_ind;

    while (i < Nc){
        route[i] = rand() % Nc;  // randomly pick one city
        for (int j=0; j<i; j++){  // check similarity with all other cities
            if (route[i] == route[j]){  // if not unique, stop checking
                flag = true;
                break;
            }
        }
        if(!flag){  // if unique, continue to next index
            i++;
        }
        flag = false; // else, try again
    }

    route[Nc] = start_ind;  // the first city is also the last city

    // calculate initial total distance
    #pragma omp parallel for
    for (int j=0; j<Nc; j++){
        float temp_sq_dist = calculate_dist(route[j], route[j + 1]);
        #pragma omp atomic
        total_sq_dist += temp_sq_dist;
        #pragma omp atomic
        total_dist += sqrt(temp_sq_dist);
    }

}

// Examine a new possible route
void new_route(){

    float temp_dist = total_sq_dist;
    // randomly pick two cities a, b from current route
    // (excluding start/stop city)
    int city_a = (rand() % (Nc - 1)) + 1;
    int city_b = (rand() % (Nc - 1)) + 1;


    #pragma omp parallel sections reduction(+:temp_dist)
    {
        // subtract distances: prev_a -> city_a -> next_a,
        // prev_b -> city_b -> next_b from current total distance
        #pragma omp section
        {
            temp_dist += -calculate_dist(route[city_a - 1], route[city_a]);
        }
        #pragma omp section
        {
            temp_dist += -calculate_dist(route[city_a], route[city_a + 1]);
        }
        #pragma omp section
        {
            temp_dist += -calculate_dist(route[city_b - 1], route[city_b]);
        }
        #pragma omp section
        {
            temp_dist += -calculate_dist(route[city_b], route[city_b + 1]);
        }

        // add distances: prev_a -> city_b -> next_a,
        // prev_b -> city_a -> next_b to temporary total distance
        #pragma omp section
        {
            temp_dist += calculate_dist(route[city_a - 1], route[city_b]);
        }
        #pragma omp section
        {
            temp_dist += calculate_dist(route[city_b], route[city_a + 1]);
        }
        #pragma omp section
        {
            temp_dist += calculate_dist(route[city_b - 1], route[city_a]);
        }
        #pragma omp section
        {
            temp_dist += calculate_dist(route[city_a], route[city_b + 1]);
        }
    }

    // compare the two distance sums and choose the min
    if (temp_dist < total_sq_dist){
        total_sq_dist = temp_dist;
        // update route
        int temp_idx = route[city_a];
        route[city_a] = route[city_b];
        route[city_b] = temp_idx;
    }

}

// Main function
int main (){

    cout<<"Nc = "<<Nc<<" | Nv = "<<Nv<<" | Iterations = "<<NUM_STEPS<<endl;

    init_vectors();
    init_route();

    cout<<endl;
    cout<<"Initial distance: "<<total_dist<<endl;

    for (int i=0; i<NUM_STEPS; i++){
        new_route();
    }

    // calculate final total distance
    total_dist = 0.0f;
    #pragma omp parallel for reduction(+:total_dist)
    for (int i=0; i<Nc; i++){
        total_dist += sqrt(calculate_dist(route[i], route[i + 1]));
    }

    cout<<"Final distance: "<<total_dist<<endl;

    return 0;

}
