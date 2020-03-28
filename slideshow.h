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

using namespace std;

struct image
{
	int id;
	char orientation;
	int nr_tags;
	vector<string> tags;
};

int valueSlides(vector<image> slide1, vector<image> slide2);
int valueSlideshow(vector<vector<image>>* slides);
int hillClimbing(vector<vector<image>> slides);
int simulatedAnnealing(vector<vector<image>> slides);
void orderImages(vector<image> images, vector<vector<image>> *slides);
bool swapSlides(vector<vector<image>> *slides, int firstSlide, int secondSlide);
bool swapPhotos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);
int valueSlidesSet(vector<image> slide1, vector<image> slide2);
vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> *slides, vector<int>* generationScores);
void crossover(vector<vector<vector<image>>>* generation, int firstParent, int secondParent);
void mutation(vector<vector<image>>& individual);
void geneticAlgorithm(vector<vector<vector<image>>>* generation, vector<int>* generationScores);
vector<vector<image>> tabuSearch(vector<vector<image>>& slides, int initialScore);