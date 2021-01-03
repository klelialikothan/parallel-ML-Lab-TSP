#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>

#define Nc 10000
#define Nv 2

using std::srand;
using std::rand;
using std::sqrt;
using std::cout;
using std::endl;

// Define arrays as global
float cities[Nc][Nv];
int route[Nc + 1];
bool visited[Nc] = { 0 };
int start_ind;
float total_dist = 0.0f;

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

// Find the best route (Heinritz and Hsiao)
void find_route(){

    start_ind = rand() % Nc;  // randomly pick first city
    route[0] = start_ind;
    visited[start_ind] = true;

    for (int i=0; i<Nc-1; i++){  // for all remaining cities
        float min_dist = 2000000.0f;  // init with max distance
        int next_ind;
        for (int j=1; j<Nc-1; j++){
            if (!visited[j]){
                float temp_dist = calculate_dist(i, j);
                if (temp_dist < min_dist){
                    min_dist = temp_dist;
                    next_ind = j;
                }
            }
        }
        route[i] = next_ind;
        visited[next_ind] = true;
        total_dist += sqrt(min_dist);
    }

    route[Nc] = start_ind;
    total_dist += sqrt(calculate_dist(route[Nc-1], start_ind));

}

// Main function
int main (){

    cout<<"Nc = "<<Nc<<" | Nv = "<<Nv<<endl;

    init_vectors();
    find_route();

    cout<<"Total distance: "<<total_dist<<endl;

    return 0;

}