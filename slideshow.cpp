#include "slideshow.h"

/**
 * Joins all slideshow's vertical images in pairs to form slides.
 */
void orderImages(vector<image> images, vector<vector<image>> &slideshow)
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

					slideshow.push_back(slide);
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
			slideshow.push_back(slide);
			i++;
		}
		else
		{
			vector<image> slide;
			slide.push_back(images.at(i));
			slideshow.push_back(slide);
		}
	}
}

/**
 * Hill Climbing algorithm stopping when a specific number of iterations without the slideshow being changed is reached.
 */
int hillClimbing(vector<vector<image>> &slideshow, int maxIterations, int initialScore)
{
	int numIterations = 0;

	int scoreDiff;
	while (numIterations < maxIterations)
	{
		if (evaluateHC(slideshow, scoreDiff))
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
 * Auxiliar function of the Hill Climing algorithm that changes the slideshow if the score of the random neighbour solution obtained is better than the older one.
 */
bool evaluateHC(vector<vector<image>> &slideshow, int &scoreDiff)
{
	int newScore, currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;

	evaluateSwap(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

	if (newScore <= currentScore)
	{
		swapPhotosOrSlides(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
		return false;
	}
	else
	{
		scoreDiff = newScore - currentScore;
		return true;
	}
}

/**
 * Generates two different random indexes used to swap slides and generates two different random indexes used to swap two photos if needed.
 * Generates a random number to determine which swap will be done and gets the score before and after the swap is done.
 */
void evaluateSwap(vector<vector<image>> &slideshow, int &firstSlide, int &secondSlide, int &firstPhoto, int &secondPhoto, int &swapChoice, int &currentScore, int &newScore)
{
	// Gets two random slides, with the first slide being the smaller index
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

	currentScore = evaluateNeighbourSlides(slideshow, firstSlide, secondSlide);

	swapChoice = rand() % slideshow.size();
	firstPhoto = rand() % 2;
	secondPhoto = rand() % 2;

	swapPhotosOrSlides(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);

	newScore = evaluateNeighbourSlides(slideshow, firstSlide, secondSlide);
}

/**
 * Evaluates the score of two slides taking into account the left and right slides of each one.
 */
int evaluateNeighbourSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide)
{
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2;

	if (firstSlide > 0)
	{
		scoreLeftNeighbour1 = valueSlides(slideshow.at(firstSlide - 1), slideshow.at(firstSlide));
	}
	else
	{
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slideshow.size() - 1)
	{
		scoreRightNeighbour2 = valueSlides(slideshow.at(secondSlide + 1), slideshow.at(secondSlide));
	}
	else
	{
		scoreRightNeighbour2 = 0;
	}

	scoreRightNeighbour1 = valueSlides(slideshow.at(firstSlide + 1), slideshow.at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(slideshow.at(secondSlide - 1), slideshow.at(secondSlide));

	return scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;
}

/**
 * Swaps two slides in the slideshow or two photos between two slides according to a random number received (swapChoice).
 */
void swapPhotosOrSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto, int swapChoice)
{
	if (slideshow.at(firstSlide).size() == 2 && slideshow.at(secondSlide).size() == 2)
	{
		if (swapChoice / 2 == 0)
		{
			swapSlides(slideshow, firstSlide, secondSlide);
		}
		else
		{
			swapPhotos(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swapSlides(slideshow, firstSlide, secondSlide);
	}
}

/**
 * Swaps two slideshow's slides.
 */
bool swapSlides(vector<vector<image>> &slideshow, int firstSlide, int secondSlide)
{
	swap(slideshow.at(firstSlide), slideshow.at(secondSlide));
	return true;
}

/**
 * Swaps two vertical photos of two different slides.
 */
bool swapPhotos(vector<vector<image>> &slideshow, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
{
	image img = slideshow.at(firstSlide).at(firstPhoto);
	slideshow.at(firstSlide).at(firstPhoto) = slideshow.at(secondSlide).at(secondPhoto);
	slideshow.at(secondSlide).at(secondPhoto) = img;
	return true;
}

/**
 * Simulated Annealing algorithm stopping when the temperature is less than 0,01 such that for each temperature a determined number of iterations is done.
 */
int simulatedAnnealing(vector<vector<image>> &slideshow, double initialTemperature, int maxIterations, int initialScore)
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
			evaluateSA(slideshow, temperature, scoreDiff);
			numIterations++;
		}
		numIterations = 0;
	}

	return initialScore;
}

/**
 * Auxiliar function of the Simulated Annealing algorithm that changes the slideshow if the score of the random neighbour solution obatined is better than the older one
 * or if the probability to choose a worst neighbour solution, which is controlled by the temperature, demands so.
 */
bool evaluateSA(vector<vector<image>> &slideshow, double &temperature, int scoreDiff)
{
	int newScore, currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;
	evaluateSwap(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

	if (newScore < currentScore)
	{
		double delta = newScore - currentScore;
		double randE = (double)(rand() % 1000) / 1000;

		if (randE >= exp(delta / temperature))
		{
			swapPhotosOrSlides(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
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
 * Tabu search algorithm stopping after a maximum number of iterations is reached or if the score doesn't change for a long time.
 */
int tabuSearch(vector<vector<image>> &slideshow, int maxIterations, int numNeighbours, int initialScore)
{
	vector<vector<image>> bestNeighbourWithoutTabu;
	vector<vector<image>> bestSlideshow = slideshow;
	int numIterations = 0;
	queue<vector<vector<image>>> tabuList;
	int scoreDiff;
	int repeatedScores = 0;

	do
	{
		bestNeighbourWithoutTabu = generateNeighbourhood(bestSlideshow, tabuList, numNeighbours, scoreDiff);
		tabuList.push(bestSlideshow);
		if (tabuList.size() > sqrt(maxIterations))
		{
			tabuList.pop();
		}

		bestSlideshow = bestNeighbourWithoutTabu;
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

/**
 * Generates a determined number of neighbour solutions of the one received and chooses the one with the best score.
 */
vector<vector<image>> generateNeighbourhood(vector<vector<image>> &slideshow, queue<vector<vector<image>>> tabuList, int numNeighbours, int &scoreDiff)
{
	vector<vector<image>> bestNeighbour = slideshow;
	int initialScore, newScore;
	scoreDiff = 0;

	for (int i = 0; i < numNeighbours; i++)
	{
		int currentScore, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice;

		evaluateSwap(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice, currentScore, newScore);

		if (i == 0){
			initialScore = currentScore;
		}

		if (newScore > currentScore && !findInTabuList(slideshow, tabuList))
		{
			currentScore = newScore;
			scoreDiff = newScore - initialScore;
			copy(slideshow.begin(), slideshow.end(), bestNeighbour.begin());
		}

		swapPhotosOrSlides(slideshow, firstSlide, secondSlide, firstPhoto, secondPhoto, swapChoice);
	}


	return bestNeighbour;
}

/**
 * Checks if a slideshow is in the tabu list.
 */
bool findInTabuList(vector<vector<image>> slideshow, queue<vector<vector<image>>> tabuList)
{
	while (!tabuList.empty())
	{
		vector<vector<image>> element = tabuList.front();
		tabuList.pop();
		if (areSlideshowsEqual(element, slideshow))
			return true;
	}
	return false;
}

/**
 * Checks if two slideshows contain the same images in the same order.
 */
bool areSlideshowsEqual(vector<vector<image>> slideshow1, vector<vector<image>> slideshow2)
{
	for (int i = 0; i < slideshow1.size(); i++)
	{
		if (slideshow1.at(i).size() != slideshow2.at(i).size())
		{

			return false;
		}

		for (int j = 0; j < slideshow1.at(i).size(); j++)
		{
			if (slideshow1.at(i).at(j).id != slideshow2.at(i).at(j).id)
			{
				return false;
			}
		}
	}
	return true;
}

/**
 * Creates the initial generation by adding random neighbour solutions of the given one.
 */
vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> &slideshow, vector<int> &generationScores, int populationSize)
{
	vector<vector<vector<image>>> generation;

	generation.push_back(slideshow);
	generationScores.push_back(valueSlideshow(slideshow));

	for (auto i = 0; i < populationSize - 1; i++)
	{
		int rand1 = rand() % slideshow.size();
		int rand2 = rand() % slideshow.size();
		vector<vector<image>> newSlideshow = slideshow;

		swapSlides(newSlideshow, rand1, rand2);

		generation.push_back(newSlideshow);
		generationScores.push_back(valueSlideshow(newSlideshow));
	}

	return generation;
}

/**
 * Genetic algorithm stopping when a maximum of generations are created. Each generation is obtained after aplying crossover and mutation operations until the 
 * offspring score becomes better than the score of the parent replaced.
 */
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

/**
 * Applies a roullete wheel selection that assigns a bigger probability of being chosen to slideshows with better fitness. Two of the slideshows are chosen randomly.
 */
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

/**
 * Calculates the fitness probability of a slideshow.
 */
double calculateFitnessProbability(int slideshowScore, int totalScore)
{
	return (double)slideshowScore / (double)totalScore;
}

/**
 * Apllies a linear order crossover (LOX) separating the first parent into two offsprings, at a random point, and adding each second parent's image to the offspring
 * that doesn't have it yet.
 */
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

/**
 * Adds to the offspring a slide with an horizontal image or a slide with two vertical images. A vertical image is saved if it has no other vertical image to pair with.
 */
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

/**
 * Checks if the slideshow contains the image.
 */
bool hasImageVector(vector<vector<image>> &slideshow, int id)
{
	for (int i = 0; i < slideshow.size(); i++)
	{
		if (hasImage(slideshow.at(i), id))
			return true;
	}
	return false;
}

/**
 * Checks if the slide contains the image.
 */
bool hasImage(vector<image> &slide, int id)
{
	for (int i = 0; i < slide.size(); i++)
	{
		if (slide.at(i).id == id)
			return true;
	}

	return false;
}

/**
 * Mutates a offspring by swapping two random slides or swapping two random vertical photos of two different slides.
 */
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

/**
 * Caculates the score between two adjacent slides, which is the minimum between the number of their common tags, the number of tags in the first slide
 * but not in the second and the number of tags in the second slide but not in the first.
 */
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

/**
 * Evaluation function that calculates the score of the slideshow.
 */
int valueSlideshow(vector<vector<image>> &slideshow)
{
	int score = 0;

	for (size_t i = 0; i < slideshow.size() - 1; i++)
	{
		score += valueSlides(slideshow.at(i), slideshow.at(i + 1));
	}

	return score;
}
