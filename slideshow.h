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

void orderImages(vector<image> images, vector<vector<image>> &slideshow);
int hillClimbing(vector<vector<image>> &slideshow, int maxIterations, int initialScore);
bool evaluateHC(vector<vector<image>> &slideshow, int &scoreDiff);
void evaluateSwap(vector<vector<image>> &slideshow, int &firstSlide, int &secondSlide, int &firstPhoto, int &secondPhoto, int &swapChoice, int &currentScore, int &newScore);
int evaluateNeighbourSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide);
void swapPhotosOrSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto, int swapChoice);
bool swapSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide);
bool swapPhotos(vector<vector<image>> &slideshow, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);
int simulatedAnnealing(vector<vector<image>> &slideshow, double initialTemperature, int maxIterations, int initialScore);
bool evaluateSA(vector<vector<image>> &slideshow, double &temperature, int scoreDiff);
int tabuSearch(vector<vector<image>> &slideshow, int maxIterations, int numNeighbours, int initialScore);
vector<vector<image>> generateNeighbourhood(vector<vector<image>> &slideshow, queue<vector<vector<image>>> tabuList, int numNeighbours, int &scoreDiff);
bool findInTabuList(vector<vector<image>> slideshow, queue<vector<vector<image>>> tabuList);
bool areSlideshowsEqual(vector<vector<image>> slideshow1, vector<vector<image>> slideshow2);
vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> &slideshow, vector<int> &generationScores, int populationSize);
int geneticAlgorithm(vector<vector<vector<image>>> &generation, vector<int> &generationScores, int maxGenerations);
vector<int> roulleteSelection(vector<vector<vector<image>>> &generation, vector<int> generationScores);
double calculateFitnessProbability(int slideshowScore, int totalScore);
int crossover(vector<vector<image>> &firstParent, vector<vector<image>> &secondParent, vector<vector<image>> &leastFitParent);
void chooseOffspringCrossover(image currentImage, image &offspringAux, int &offspringIndex, vector<vector<image>> &offspring);
bool hasImageVector(vector<vector<image>> &slideshow, int id);
bool hasImage(vector<image> &slide, int id);
void mutation(vector<vector<image>> &slideshow);
int valueSlides(vector<image> &slide1, vector<image> &slide2);
int valueSlideshow(vector<vector<image>> &slideshow);

