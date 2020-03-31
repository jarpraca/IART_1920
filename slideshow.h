#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip> 
#include <queue>
#include <numeric>

using namespace std;

struct image
{
	int id;
	char orientation;
	int nrTags;
	vector<string> tags;
};

int valueSlides(vector<image> &slide1, vector<image> &slide2);
int valueSlideshow(vector<vector<image>>& slides);
int hillClimbing(vector<vector<image>> &slides, int maxIterations, int initialScore);
int simulatedAnnealing(vector<vector<image>> &slides, double initialTemperature, int maxIterations, int initialScore);
void orderImages(vector<image> images, vector<vector<image>> &slides);
bool swapSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide);
bool swapPhotos(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);
void swapPhotosOrSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto, int swap_choice);
int valueSlidesSet(vector<image> slide1, vector<image> slide2);
vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> &slides, vector<int>& generationScores, int populationSize);
void chooseChildCrossover(image currentImage, image &child1Aux, image &child2Aux, image &child1Index, image &child2Index);
void crossover(vector<vector<vector<image>>>& generation, int firstParent, int secondParent);
void mutation(vector<vector<image>>& individual);
int geneticAlgorithm(vector<vector<vector<image>>>& generation, vector<int>& generationScores, int maxGenerations);
vector<vector<image>> generateNeighbourhood(vector<vector<image>> &slides, queue<vector<vector<image>>> tabuList, int numNeighbours);
vector<vector<image>> tabuSearch(vector<vector<image>>& slides, int numIterations, int numNeighbours);