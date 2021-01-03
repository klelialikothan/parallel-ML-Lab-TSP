#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <omp.h>

#define Nc 10000
#define Nv 2
#define NUM_THREADS 4

using std::srand;
using std::rand;
using std::sqrt;
using std::cout;
using std::endl;

// Define arrays as global
float cities[Nc][Nv];
int routes[NUM_THREADS][Nc + 1];
int final_route;
bool visited[NUM_THREADS][Nc] = { 0 };
int start_ind;
float total_dists[NUM_THREADS] = { 0 };
float final_dist = 0.0f;

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

// Random selection by "dice roll"
bool select_branch(){

    return (rand() % 6) < 2;

}

// Find the best route (Heinritz and Hsiao)
void find_route(){

    start_ind = rand() % Nc;  // randomly pick first city

    for (int i=0; i<NUM_THREADS; i++){
        routes[i][0] = start_ind;
        visited[i][start_ind] = true;
    }

    #pragma omp parallel for
    for (int k=0; k<NUM_THREADS; k++){
        for (int i=0; i<Nc-1; i++){  // for all remaining cities
            float min_dist_1 = 2000000;  // init with max distance
            float min_dist_2 = 2000000;  // init with max distance
            int next_ind_1, next_ind_2;
            #pragma omp parallel for shared(min_dist_1, min_dist_2, next_ind_1, next_ind_2)
            for (int j=1; j<Nc-1; j++){
                if (!visited[k][j]){
                    float temp_dist = calculate_dist(i, j);
                    #pragma omp critical
                    {
                        if (temp_dist < min_dist_1){
                            min_dist_1 = temp_dist;
                            next_ind_1 = j;
                        }
                        else if (temp_dist < min_dist_2){
                            min_dist_2 = temp_dist;
                            next_ind_2 = j;
                        }
                    }
                }
            }
            if (select_branch()){
                routes[k][i] = next_ind_1;
                visited[k][next_ind_1] = true;
                total_dists[k] += sqrt(min_dist_1);
            }
            else {
                routes[k][i] = next_ind_2;
                visited[k][next_ind_2] = true;
                total_dists[k] += sqrt(min_dist_2);
            }
        }
        routes[k][Nc] = start_ind;
        total_dists[k] += sqrt(calculate_dist(routes[k][Nc-1], start_ind));
    }

    final_dist = 2000000;
    for (int i=0; i<NUM_THREADS; i++){
        if (total_dists[i] < final_dist){
            final_route = i;
            final_dist = total_dists[final_route];
        }
    }

}

// Main function
int main (){

    cout<<"Nc = "<<Nc<<" | Nv = "<<Nv<<endl;
    omp_set_num_threads(NUM_THREADS);
    init_vectors();
    find_route();

    // for (int j=0; j<NUM_THREADS; j++){
    //     cout<<"Route #"<<j<<" total distance: "<<total_dists[j]<<endl;
    // }

    cout<<"Final distance: "<<final_dist<<endl;

    return 0;

}