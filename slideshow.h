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

void orderImages(vector<image> images, vector<vector<image>> &slides);
int evaluateNeighbourSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide);
void evaluateSwap(vector<vector<image>> &slides, int &firstSlide, int &secondSlide, int &firstPhoto, int &secondPhoto, int &swapChoice, int &currentScore, int &newScore);
void swapPhotosOrSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto, int swap_choice);
bool evaluateHC(vector<vector<image>> &slides, int &scoreDiff);
bool swapSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide);
bool swapPhotos(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);
int hillClimbing(vector<vector<image>> &slides, int maxIterations, int initialScore);
bool evaluateSA(vector<vector<image>> &slides, double &temperature, int scoreDiff);
int simulatedAnnealing(vector<vector<image>> &slides, double initialTemperature, int maxIterations, int initialScore);
int valueSlides(vector<image> &slide1, vector<image> &slide2);
int valueSlideshow(vector<vector<image>>& slides);
vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> &slides, vector<int>& generationScores, int populationSize);
bool hasImage(vector<image> &slide, int id);
bool hasImageVector(vector<vector<image>> &individual, int id);
void chooseOffspringCrossover(image currentImage, image &offspringAux, int &offspringIndex, vector<vector<image>> &offspring);
void crossover(vector<vector<vector<image>>>& generation, int firstParent, int secondParent);
void mutation(vector<vector<image>> &slideshow);
double calculateFitnessProbability(int slideshowScore, int totalScore);
vector<int> roulleteSelection(vector<vector<vector<image>>> &generation, vector<int> generationScores);
int geneticAlgorithm(vector<vector<vector<image>>>& generation, vector<int>& generationScores, int maxGenerations);
bool isEqualSlides(vector<vector<image>> slides1, vector<vector<image>> slides2);
bool findInTabuList(vector<vector<image>> slides, queue<vector<vector<image>>> tabuList);
vector<vector<image>> generateNeighbourhood(vector<vector<image>> &slides, queue<vector<vector<image>>> tabuList, int numNeighbours, int &scoreDiff);
int tabuSearch(vector<vector<image>>& slides, int numIterations, int numNeighbours, int initialScore);
