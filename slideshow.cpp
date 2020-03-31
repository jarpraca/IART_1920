#include "slideshow.h"

/**
 * Joins all slideshow's vertical slides in pairs.
 */
void orderImages(vector<image> images, vector<vector<image>> &slides)
{
	for (int i = 0; i < images.size(); i++)
	{
		if (images.at(i).orientation == 'V' && images.at(i + 1).orientation == 'H')
		{
			for (int j = i + 2; j < images.size(); j++)
			{
				if ((images.at(j).orientation == 'V' && j + 1 < images.size() && images.at(j + 1).orientation == 'H') || (images.at(j).orientation == 'V' && j == images.size() - 1))
				{
					vector<image> slide;
					swap(images.at(i + 1), images.at(j));
					slide.push_back(images.at(i));
					slide.push_back(images.at(i + 1));

					slides.push_back(slide);
					i++;
					break;
				}
			}
		}
		else if (images.at(i).orientation == 'V' && images.at(i + 1).orientation == 'V')
		{
			vector<image> slide;
			slide.push_back(images.at(i));
			slide.push_back(images.at(i + 1));
			slides.push_back(slide);
			i++;
		}
		else
		{
			vector<image> slide;
			slide.push_back(images.at(i));
			slides.push_back(slide);
		}
	}
}

/**
 * Evaluates the score of two slides taking into account the left and right slides of each one needed to calculate the score
 */
int evaluateNeighbourSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide)
{
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2;

	if (firstSlide > 0)
	{
		scoreLeftNeighbour1 = valueSlides(slides.at(firstSlide - 1), slides.at(firstSlide));
	}
	else
	{
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides.size() - 1)
	{
		scoreRightNeighbour2 = valueSlides(slides.at(secondSlide + 1), slides.at(secondSlide));
	}
	else
	{
		scoreRightNeighbour2 = 0;
	}

	scoreRightNeighbour1 = valueSlides(slides.at(firstSlide + 1), slides.at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(slides.at(secondSlide - 1), slides.at(secondSlide));

	return scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;
}

/**
 * Generates two different random indexes used to swap slides and generates two different random indexes used to swap photos if needed.
 * Generates a random number to determine which swap will be done and gets the score before and after the swap is done.
 */
void evaluateSwap(vector<vector<image>> &slides, int &firstSlide, int &secondSlide, int &firstPhoto, int &secondPhoto, int &swapChoice, int &currentScore, int &newScore)
{
	// Gets two random slides, with the first slide being the smaller index
	firstSlide = rand() % slides[0].size();
	do
	{
		secondSlide = rand() % slides.size();
	} while (secondSlide == firstSlide);

	if (secondSlide < firstSlide)
	{
		int firstTemp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = firstTemp;
	}

	currentScore = evaluateNeighbourSlides(slides, firstSlide, secondSlide);

	swapChoice = rand() % slides.size();
	firstPhoto = rand() % 2;
	secondPhoto = rand() % 2;

	swapPhotosOrSlides(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);

	newScore = evaluateNeighbourSlides(slides, firstSlide, secondSlide);
}

/**
 * Swaps two slides in the slideshow or two photos between two slides according to a random number received (swapChoice).
 */
void swapPhotosOrSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto, int swapChoice)
{
	if (slides.at(firstSlide).size() == 2 && slides.at(secondSlide).size() == 2)
	{
		if (swapChoice / 2 == 0)
		{
			swapSlides(slides, firstSlide, secondSlide);
		}
		else
		{
			swapPhotos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swapSlides(slides, firstSlide, secondSlide);
	}
}

/**
 * Evaluation function of the Hill Climing algorithm that changes the slideshow if the score of the random neighbour obtained is better than the older one.
 */
bool evaluateHC(vector<vector<image>> &slides, int &scoreDiff)
{
	int newScore, currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;

	evaluateSwap(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

	if (newScore <= currentScore)
	{
		swapPhotosOrSlides(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
		return false;
	}
	else
	{
		scoreDiff = newScore - currentScore;
		return true;
	}
}

/**
 * Swaps two slideshow's slides.
 */
bool swapSlides(vector<vector<image>> &slides, int firstSlide, int secondSlide)
{
	swap(slides.at(firstSlide), slides.at(secondSlide));
	return true;
}

/**
 * Swaps two vertical photos in two different slides.
 */
bool swapPhotos(vector<vector<image>> &slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
{
	image img = slides.at(firstSlide).at(firstPhoto);
	slides.at(firstSlide).at(firstPhoto) = slides.at(secondSlide).at(secondPhoto);
	slides.at(secondSlide).at(secondPhoto) = img;
	return true;
}

/**
 * Hill Climbing algorithm stopping when a specific number of iterations is reached without the slideshow being changed.
 */
int hillClimbing(vector<vector<image>> &slides, int maxIterations, int initialScore)
{
	int numIterations = 0;

	int scoreDiff;
	while (numIterations < maxIterations)
	{
		if (evaluateHC(slides, scoreDiff))
		{
			initialScore += scoreDiff;
			cout << "\nBest score so far: " << initialScore << endl;
			numIterations = 0;
		}
		else
		{
			cout << ".";
			numIterations++;
		}
	}

	return initialScore;
}

/**
 * Evaluation function of the Simulated Annealing algorithm that changes the slideshow if the score of the random neighbour obatined is better than the older one
 * or if the probability to choose a worst neighbour, which is controlled by the temperature, demands so.
 */
bool evaluateSA(vector<vector<image>> &slides, double &temperature, int scoreDiff)
{
	int newScore, currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;
	evaluateSwap(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

	if (newScore < currentScore)
	{
		double delta = newScore - currentScore;
		double randE = (double)(rand() % 1000) / 1000;

		if (randE >= exp(delta / temperature))
		{
			swapPhotosOrSlides(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
			return false;
		}
		else
		{
			double alpha = 0.9;
			temperature *= alpha;
			scoreDiff = newScore - currentScore;
			return true;
		}
	}
	else
	{
		scoreDiff = newScore - currentScore;
		return true;
	}
}

/**
 * 
 */
int simulatedAnnealing(vector<vector<image>> &slides, double initialTemperature, int maxIterations, int initialScore)
{
	int scoreDiff = 0;
	int finalScore = 0;
	int numIterations = 0;

	double temperature = initialTemperature;

	while (temperature > pow(10, -2))
	{
		initialScore += scoreDiff;
		cout << "Current Temperature: " << temperature << endl;
		cout << "Current Score: " << initialScore << endl;

		while (numIterations < maxIterations)
		{
			evaluateSA(slides, temperature, scoreDiff);
			numIterations++;
		}
		numIterations = 0;
	}

	return initialScore;
}

int valueSlides(vector<image> &slide1, vector<image> &slide2)
{
	vector<string> tags1;

	for (size_t i = 0; i < slide1.size(); i++)
	{
		tags1.insert(tags1.end(), slide1.at(i).tags.begin(), slide1.at(i).tags.end());
	}

	sort(tags1.begin(), tags1.end());
	unique(tags1.begin(), tags1.end());

	vector<string> tags2;

	for (size_t i = 0; i < slide2.size(); i++)
	{
		tags2.insert(tags2.end(), slide2.at(i).tags.begin(), slide2.at(i).tags.end());
	}

	sort(tags2.begin(), tags2.end());
	unique(tags2.begin(), tags2.end());

	vector<string> tagsCommon(min(tags1.size(), tags2.size()));
	vector<string> tags1NotTags2(tags1.size());
	vector<string> tags2NotTags1(tags2.size());
	vector<string>::iterator it;

	it = set_intersection(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tagsCommon.begin());
	tagsCommon.resize(it - tagsCommon.begin());

	it = set_difference(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tags1NotTags2.begin());
	tags1NotTags2.resize(it - tags1NotTags2.begin());

	it = set_difference(tags2.begin(), tags2.end(), tags1.begin(), tags1.end(), tags2NotTags1.begin());
	tags2NotTags1.resize(it - tags2NotTags1.begin());

	return min(tagsCommon.size(), min(tags1NotTags2.size(), tags2NotTags1.size()));
}

int valueSlideshow(vector<vector<image>> &slides)
{
	int score = 0;

	for (size_t i = 0; i < slides.size() - 1; i++)
	{
		score += valueSlides(slides.at(i), slides.at(i + 1));
	}

	return score;
}

vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> &slides, vector<int> &generationScores, int populationSize)
{
	vector<vector<vector<image>>> generation;

	generation.push_back(slides);
	generationScores.push_back(valueSlideshow(slides));

	for (auto i = 0; i < populationSize - 1; i++)
	{
		int rand1 = rand() % slides.size();
		int rand2 = rand() % slides.size();
		vector<vector<image>> newSlides = slides;

		swapSlides(newSlides, rand1, rand2);

		generation.push_back(newSlides);
		generationScores.push_back(valueSlideshow(newSlides));
	}

	return generation;
}

bool hasImage(vector<image> &slide, int id)
{
	for (int i = 0; i < slide.size(); i++)
	{
		if (slide.at(i).id == id)
			return true;
	}

	return false;
}

bool hasImageVector(vector<vector<image>> &individual, int id)
{
	for (int i = 0; i < individual.size(); i++)
	{
		if (hasImage(individual.at(i), id))
			return true;
	}
	return false;
}

void chooseOffspringCrossover(image currentImage, image &offspringAux, int &offspringIndex, vector<vector<image>> &offspring)
{
	if (currentImage.orientation == 'V')
	{
		if (offspringAux.id == -1)
		{
			offspringAux = currentImage;
		}
		else
		{
			vector<image> slide;
			slide.push_back(currentImage);
			slide.push_back(offspringAux);
			offspringAux.id = -1;
			offspring.at(offspringIndex) = slide;
			offspringIndex++;
		}
	}
	else
	{
		vector<image> slide;
		slide.push_back(currentImage);
		offspring.at(offspringIndex) = slide;
		offspringIndex++;
	}
}

int crossover(vector<vector<image>> &firstParent, vector<vector<image>> &secondParent, vector<vector<image>> &leastFitParent)
{
	vector<vector<image>> offspring1(firstParent.size());
	vector<vector<image>> offspring2(firstParent.size());

	image offspring1Aux;
	image offspring2Aux;

	offspring1Aux.id = -1;
	offspring2Aux.id = -1;
	int crossoverPoint = (rand() % firstParent.size()) + 1;

	copy(firstParent.begin(), firstParent.begin() + crossoverPoint, offspring1.begin());
	copy(firstParent.begin() + crossoverPoint, firstParent.end(), offspring2.begin() + crossoverPoint);

	int offspring1Index = crossoverPoint;
	int offspring2Index = 0;

	for (int i = 0; i < secondParent.size(); i++)
	{
		for (int j = 0; j < secondParent.at(i).size(); j++)
		{
			image currentImage = secondParent.at(i).at(j);
			if (hasImageVector(offspring1, currentImage.id))
			{
				chooseOffspringCrossover(currentImage, offspring2Aux, offspring2Index, offspring2);
			}
			else
			{
				chooseOffspringCrossover(currentImage, offspring1Aux, offspring1Index, offspring1);
			}
		}
	}

	int scoreFirstOffspring = valueSlideshow(offspring1);
	int scoreSecondOffspring = valueSlideshow(offspring2);
	
	if(scoreFirstOffspring > scoreSecondOffspring){
		leastFitParent = offspring1;
		return scoreFirstOffspring;
	}
	else {
		leastFitParent = offspring2;
		return scoreSecondOffspring;
	}
}

void mutation(vector<vector<image>> &slideshow)
{
	// Gets two random slides, with the first slide being the smaller index
	int firstSlide;
	int secondSlide;
	firstSlide = rand() % slideshow.size();
	do
	{
		secondSlide = rand() % slideshow.size();
	} while (secondSlide == firstSlide);

	if (secondSlide < firstSlide)
	{
		int firstTemp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = firstTemp;
	}

	int swapChoice = rand() % slideshow.size();
	int firstPhoto = rand() % 2;
	int secondPhoto = rand() % 2;

	swapPhotosOrSlides(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
}

double calculateFitnessProbability(int slideshowScore, int totalScore)
{
	return (double)slideshowScore / (double)totalScore;
}

vector<int> roulleteSelection(vector<vector<vector<image>>> &generation, vector<int> generationScores)
{
	vector<double> fitnessProbability;
	int totalScore = accumulate(generationScores.begin(), generationScores.end(), 0);
	double fitnessProb;
	double currentSlot = 0.0;

	for (int i = 0; i < generation.size(); i++)
	{
		fitnessProb = calculateFitnessProbability(generationScores.at(i), totalScore);
		currentSlot += fitnessProb;
		fitnessProbability.push_back(currentSlot);
	}

	fitnessProbability.at(fitnessProbability.size() - 1) = 1.0;

	int firstParent;
	int secondParent = -1;

	double firstParentSelect = (double)(rand() % 101) / (double)100;

	for (int i = 0; i < fitnessProbability.size(); i++)
	{
		if (firstParentSelect <= fitnessProbability.at(i))
		{
			firstParent = i;
			break;
		}
	}

	do
	{
		double secondParentSelect = (double)(rand() % 101) / (double)100;
		secondParent = -1;
		for (int i = 0; i < fitnessProbability.size(); i++)
		{
			if (secondParentSelect <= fitnessProbability.at(i) && i != firstParent)
			{
				secondParent = i;
				break;
			}
		}
	} while (secondParent == firstParent || secondParent == -1);

	vector<int> parents;
	parents.push_back(firstParent);
	parents.push_back(secondParent);

	return parents;
}

int geneticAlgorithm(vector<vector<vector<image>>> &generation, vector<int> &generationScores, int maxGenerations)
{
	int numGenerations = 0;
	int maxImproveTries = 100;
	int improveTries = 0;
	int currentScore, newScore, scoreFirstParent, scoreSecondParent, scoreFirstOffspring, scoreSecondOffspring;
	vector<int>::iterator it;

	while (numGenerations < maxGenerations)
	{
		vector<int> parents = roulleteSelection(generation, generationScores);
		int leastFit;
		int firstParent = parents.at(0);
		int secondParent = parents.at(1);
		scoreFirstParent = generationScores.at(firstParent);
		scoreSecondParent = generationScores.at(secondParent);

		do
		{
			if (scoreFirstParent < scoreSecondParent){
				leastFit = firstParent;
				currentScore = scoreFirstParent;
			}else{
				leastFit = secondParent;
				currentScore = scoreSecondParent;
			}

			newScore = crossover(generation.at(firstParent), generation.at(secondParent), generation.at(leastFit));
			
			double mutationRand = (double)(rand() % 1001) / (double)1000;

			if(mutationRand < 0.01){
				mutation(generation.at(leastFit));
				newScore = valueSlideshow(generation.at(leastFit));
			}
			generationScores.at(leastFit) = newScore;

			cout << "." ;
			improveTries++;
		} while (newScore <= currentScore && improveTries < maxImproveTries);

		numGenerations++;
		int max = generationScores.at(max_element(generationScores.begin(), generationScores.end()) - generationScores.begin());
		cout << endl << endl << "Current Best Score = " << max << endl; 
		cout << "Current Generation = " << numGenerations << endl;
		
		improveTries = 0;
	
	}

	it = max_element(generationScores.begin(), generationScores.end());
	int index = it - generationScores.begin();

	return generationScores.at(index);
}

bool isEqualSlides(vector<vector<image>> slides1, vector<vector<image>> slides2)
{
	for (int i = 0; i < slides1.size(); i++)
	{
		if (slides1.at(i).size() != slides2.at(i).size())
		{

			return false;
		}

		for (int j = 0; j < slides1.at(i).size(); j++)
		{
			if (slides1.at(i).at(j).id != slides2.at(i).at(j).id)
			{
				return false;
			}
		}
	}
	return true;
}

bool findInTabuList(vector<vector<image>> slides, queue<vector<vector<image>>> tabuList)
{
	while (!tabuList.empty())
	{
		vector<vector<image>> element = tabuList.front();
		tabuList.pop();
		if (isEqualSlides(element, slides))
			return true;
	}
	return false;
}

vector<vector<image>> generateNeighbourhood(vector<vector<image>> &slides, queue<vector<vector<image>>> tabuList, int numNeighbours, int &scoreDiff)
{
	vector<vector<image>> bestNeighbour = slides;
	int initialScore, newScore;
	scoreDiff = 0;

	for (int i = 0; i < numNeighbours; i++)
	{
		int currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;

		evaluateSwap(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

		if (i == 0){
			initialScore = currentScore;
		}

		if (newScore > currentScore && !findInTabuList(slides, tabuList))
		{
			currentScore = newScore;
			scoreDiff = newScore - initialScore;
			copy(slides.begin(), slides.end(), bestNeighbour.begin());
		}

		swapPhotosOrSlides(slides, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
	}


	return bestNeighbour;
}

int tabuSearch(vector<vector<image>> &slides, int maxIterations, int numNeighbours, int initialScore)
{
	vector<vector<image>> bestNeighbourWithoutTabu;
	vector<vector<image>> bestSlides = slides;
	int numIterations = 0;
	queue<vector<vector<image>>> tabuList;
	int scoreDiff;
	int repeatedScores = 0;

	do
	{
		bestNeighbourWithoutTabu = generateNeighbourhood(bestSlides, tabuList, numNeighbours, scoreDiff);
		tabuList.push(bestSlides);
		if (tabuList.size() > sqrt(maxIterations))
		{
			tabuList.pop();
		}

		bestSlides = bestNeighbourWithoutTabu;
		numIterations++;

		initialScore += scoreDiff;
		if(scoreDiff == 0)
			repeatedScores++;
		else
			repeatedScores = 0;
		
		
		cout << endl << "Current Score = " << initialScore << endl;

	} while (numIterations < maxIterations && repeatedScores < max(sqrt(maxIterations), 100.0));

	return initialScore;
}