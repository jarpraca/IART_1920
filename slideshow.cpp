#include "slideshow.h"

void orderImages(vector<image> images, vector<vector<image>> *slides)
{
	for (int i = 0; i < images.size(); i++)
	{
		if (images[i].orientation == 'V' && images[i + 1].orientation == 'H')
		{
			for (int j = i + 2; j < images.size(); j++)
			{
				if ((images[j].orientation == 'V' && j + 1 < images.size() && images[j + 1].orientation == 'H') || (images[j].orientation == 'V' && j == images.size() - 1))
				{
					vector<image> slide;
					swap(images[i + 1], images[j]);
					slide.push_back(images[i]);
					slide.push_back(images[i + 1]);

					slides->push_back(slide);
					i++;
					break;
				}
			}
		}
		else if (images[i].orientation == 'V' && images[i + 1].orientation == 'V')
		{
			vector<image> slide;
			slide.push_back(images[i]);
			slide.push_back(images[i + 1]);
			slides->push_back(slide);
			i++;
		}
		else
		{
			vector<image> slide;
			slide.push_back(images[i]);
			slides->push_back(slide);
		}
	}
}

bool evaluate_swap_hc(vector<vector<image>> *slides)
{

	int firstSlide;
	int secondSlide;
	firstSlide = rand() % slides->size();
	do
	{
		secondSlide = rand() % slides->size();
	} while (secondSlide == firstSlide);

	if (secondSlide < firstSlide)
	{
		int first_temp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = first_temp;
	}

	vector<image> leftNeighbourSlide1, rightNeighbourSlide1, leftNeighbourSlide2, rightNeighbourSlide2;
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2, currentScore;

	if (firstSlide > 0)
	{
		leftNeighbourSlide1 = slides->at(firstSlide - 1);
		scoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		rightNeighbourSlide2 = slides->at(secondSlide + 1);
		scoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		scoreRightNeighbour2 = 0;
	}

	rightNeighbourSlide1 = slides->at(firstSlide + 1);
	leftNeighbourSlide2 = slides->at(secondSlide - 1);

	scoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;

	int swap_choice, firstPhoto, secondPhoto;
	if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
	{
		swap_choice = rand() % slides->size();
		if (swap_choice / 2 == 0)
			swapSlides(slides, firstSlide, secondSlide);
		else
		{
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swapPhotos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swapSlides(slides, firstSlide, secondSlide);
	}

	int newScoreLeftNeighbour1, newScoreRightNeighbour1, newScoreLeftNeighbour2, newScoreRightNeighbour2, newScore;

	if (firstSlide > 0)
	{
		newScoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		newScoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		newScoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		newScoreRightNeighbour2 = 0;
	}

	newScoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;

	if (newScore <= currentScore)
	{
		if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
		{
			if (swap_choice / 2 == 0)
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
		return false;
	}
	else
	{
		return true;
	}
}

bool evaluate_swap_sa(vector<vector<image>> *slides, double &temperature)
{
	int firstSlide;
	int secondSlide;
	firstSlide = rand() % slides->size();
	do
	{
		secondSlide = rand() % slides->size();
	} while (secondSlide == firstSlide);

	if (secondSlide < firstSlide)
	{
		int first_temp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = first_temp;
	}

	vector<image> leftNeighbourSlide1, rightNeighbourSlide1, leftNeighbourSlide2, rightNeighbourSlide2;
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2, currentScore;

	if (firstSlide > 0)
	{
		leftNeighbourSlide1 = slides->at(firstSlide - 1);
		scoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		rightNeighbourSlide2 = slides->at(secondSlide + 1);
		scoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		scoreRightNeighbour2 = 0;
	}

	rightNeighbourSlide1 = slides->at(firstSlide + 1);
	leftNeighbourSlide2 = slides->at(secondSlide - 1);

	scoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;

	int swap_choice, firstPhoto, secondPhoto;
	if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
	{
		swap_choice = rand() % slides->size();
		if (swap_choice / 2 == 0)
			swapSlides(slides, firstSlide, secondSlide);
		else
		{
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swapPhotos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swapSlides(slides, firstSlide, secondSlide);
	}

	int newScoreLeftNeighbour1, newScoreRightNeighbour1, newScoreLeftNeighbour2, newScoreRightNeighbour2, newScore;

	if (firstSlide > 0)
	{
		newScoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		newScoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		newScoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		newScoreRightNeighbour2 = 0;
	}

	newScoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;

	if (newScore < currentScore)
	{
		double delta = newScore - currentScore;
		double randE = (double)(rand() % 1000) / 1000;
		if (randE >= exp(delta / temperature))
		{
			if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
			{
				if (swap_choice / 2 == 0)
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

			return false;
		}
		else
		{
			double alpha = 0.9;
			temperature *= alpha;
			return true;
		}
	}
	else
	{
		return true;
	}
}

bool swapSlides(vector<vector<image>> *slides, int firstSlide, int secondSlide)
{
	swap(slides->at(firstSlide), slides->at(secondSlide));
	return true;
}

bool swapPhotos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
{
	image img = slides->at(firstSlide).at(firstPhoto);
	slides->at(firstSlide).at(firstPhoto) = slides->at(secondSlide).at(secondPhoto);
	slides->at(secondSlide).at(secondPhoto) = img;
	return true;
}

int hillClimbing(vector<vector<image>> slides)
{
	int initialScore = 0;
	int finalScore = 0;
	int maxTries = 0;

	int currentScore, newScore;

	while (maxTries < slides.size())
	{
		if (evaluate_swap_hc(&slides))
		{
			maxTries = 0;
		}
		else
		{
			maxTries++;
		}
	}
	cout << endl;

	finalScore = valueSlideshow(&slides);

	cout << "final " << finalScore << endl;

	return 0;
}

int simulatedAnnealing(vector<vector<image>> slides)
{
	int initialScore = 0;
	int finalScore = 0;
	int maxTries = 0;

	double temperature = 1;

	while (temperature > pow(10, -2))
	{
		while (maxTries < slides.size())
		{
			evaluate_swap_sa(&slides, temperature);
			maxTries++;
		}
		maxTries = 0;
	}

	finalScore = valueSlideshow(&slides);

	cout << "final " << finalScore << endl;

	return 0;
}

int valueSlides(vector<image> slide1, vector<image> slide2)
{
	vector<string> tags1;

	for (size_t i = 0; i < slide1.size(); i++)
	{
		tags1.insert(tags1.end(), slide1[i].tags.begin(), slide1[i].tags.end());
	}

	sort(tags1.begin(), tags1.end());
	unique(tags1.begin(), tags1.end());

	vector<string> tags2;

	for (size_t i = 0; i < slide2.size(); i++)
	{
		tags2.insert(tags2.end(), slide2[i].tags.begin(), slide2[i].tags.end());
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

int valueSlideshow(vector<vector<image>> *slides)
{
	int score = 0;

	for (size_t i = 0; i < slides->size() - 1; i++)
	{
		score += valueSlides(slides->at(i), slides->at(i + 1));
	}

	return score;
}

vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> *slides, vector<int> *generationScores)
{
	vector<vector<vector<image>>> generation;

	generation.push_back(*slides);
	generationScores->push_back(valueSlideshow(slides));

	for (auto i = 0; i < 29; i++)
	{
		int rand1 = rand() % slides->size();
		int rand2 = rand() % slides->size();
		vector<vector<image>> newSlides = *slides;

		swapSlides(&newSlides, rand1, rand2);

		generation.push_back(newSlides);
		generationScores->push_back(valueSlideshow(&newSlides));
	}

	return generation;
}

bool hasImage(vector<image> &slide, int id)
{
	for (int i = 0; i < slide.size(); i++)
	{
		if (slide[i].id == id)
			return true;
	}

	return false;
}

bool hasImageVector(vector<vector<image>> &individual, int id)
{
	for (int i = 0; i < individual.size(); i++)
	{
		if (hasImage(individual[i], id))
			return true;
	}
	return false;
}

void crossover(vector<vector<image>> &firstParent, vector<vector<image>> &secondParent)
{
	vector<vector<image>> child1(firstParent.size());
	vector<vector<image>> child2(firstParent.size());

	image child1Aux;
	image child2Aux;

	child1Aux.id = -1;
	child2Aux.id = -1;

	int crossoverPoint = (rand() % firstParent.size()) + 1;

	copy(firstParent.begin(), firstParent.begin() + crossoverPoint, child1.begin());
	copy(firstParent.begin() + crossoverPoint, firstParent.end(), child2.begin() + crossoverPoint);

	int child1Index = crossoverPoint;
	int child2Index = 0;

	for (int i = 0; i < secondParent.size(); i++)
	{
		for (int j = 0; j < secondParent[i].size(); j++)
		{
			image currentImage = secondParent[i][j];
			if (hasImageVector(child1, currentImage.id))
			{
				if (currentImage.orientation == 'V')
				{
					if (child2Aux.id == -1)
					{
						child2Aux = currentImage;
					}
					else
					{
						vector<image> slide;
						slide.push_back(currentImage);
						slide.push_back(child2Aux);
						child2Aux.id = -1;
						child2[child2Index] = slide;
						child2Index++;
					}
				}
				else
				{
					vector<image> slide;
					slide.push_back(currentImage);
					child2[child2Index] = slide;
					child2Index++;
				}
			}
			else
			{
				if (currentImage.orientation == 'V')
				{
					if (child1Aux.id == -1)
					{
						child1Aux = currentImage;
					}
					else
					{
						vector<image> slide;
						slide.push_back(currentImage);
						slide.push_back(child1Aux);
						child1Aux.id = -1;
						child1[child1Index] = slide;
						child1Index++;
					}
				}
				else
				{
					vector<image> slide;
					slide.push_back(currentImage);
					child1[child1Index] = slide;
					child1Index++;
				}
			}
		}
	}

	int scoreChild1 = valueSlideshow(&child1);
	int scoreChild2 = valueSlideshow(&child2);

	if (scoreChild1 > scoreChild2)
	{
		secondParent = child1;
	}
	else
	{
		secondParent = child2;
	}
}

void mutation(vector<vector<image>> &individual)
{
	int firstSlide;
	int secondSlide;
	firstSlide = rand() % individual.size();
	do
	{
		secondSlide = rand() % individual.size();
	} while (secondSlide == firstSlide);

	if (secondSlide < firstSlide)
	{
		int first_temp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = first_temp;
	}

	if (individual[firstSlide].size() == 2 && individual[secondSlide].size() == 2)
	{
		int swap_choice = rand() % individual.size();
		if (swap_choice / 2 == 0)
			swapSlides(&individual, firstSlide, secondSlide);
		else
		{
			int firstPhoto = rand() % 2;
			int secondPhoto = rand() % 2;
			swapPhotos(&individual, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swapSlides(&individual, firstSlide, secondSlide);
	}
}

void geneticAlgorithm(vector<vector<vector<image>>> *generation, vector<int> *generationScores)
{
	int numGenerations = 0;
	int newScore, currentScore;
	vector<int>::iterator it;

	while (numGenerations < generation->at(0).size())
	{
		do
		{
			it = max_element(generationScores->begin(), generationScores->end());
			int firstParent = it - generationScores->begin();
			int secondParent;
			do
			{
				secondParent = rand() % generation->size();
			} while (secondParent == firstParent);

			currentScore = valueSlideshow(&generation->at(secondParent));

			crossover(generation->at(firstParent), generation->at(secondParent));
			mutation(generation->at(secondParent));

			newScore = valueSlideshow(&generation->at(secondParent));
			generationScores->at(secondParent) = newScore;
		} while (newScore <= currentScore);
		numGenerations++;
	}

	it = max_element(generationScores->begin(), generationScores->end());
	int index = it - generationScores->begin();

	cout << "\nFinal score  " << generationScores->at(index) << endl;
}

bool isEqualSlides(vector<vector<image>> slides1, vector<vector<image>> slides2)
{
	for (int i = 0; i < slides1.size(); i++)
	{
		if (slides1[i].size() != slides2[i].size())
		{

			return false;
		}

		for (int j = 0; j < slides1[i].size(); j++)
		{
			if (slides1[i][j].id != slides2[i][j].id)
			{
				return false;
			}
		}
	}
	return true;
}

bool findInTabuList(vector<vector<image>> slides, queue<vector<vector<image>>> tabu_list)
{
	while (!tabu_list.empty())
	{
		vector<vector<image>> element = tabu_list.front();
		tabu_list.pop();
		if (isEqualSlides(element, slides))
			return true;
	}
	return false;
}

vector<vector<image>> generateNeighbourhood(vector<vector<image>> slides, int currentScore, queue<vector<vector<image>>> tabu_list)
{
	vector<vector<image>> bestNeighbour;

	// for (int i = 0; i < slides.size() - 1; i++)
	// {
	// 	for (int j = i + 1; j < slides.size(); j++)
	// 	{
	// 		swapSlides(&slides, i, j);
	// 		int newScore = valueSlideshow(&slides);
	// 		if (newScore > currentScore && !findInTabuList(slides, tabu_list))
	// 		{
	// 			currentScore = newScore;
	// 			bestNeighbour = slides;
	// 		}
	// 		swapSlides(&slides, i, j);
	// 	}
	// }

	for (int i = 0; i < (int)sqrt(slides.size()); i++)
	{
		int rand1 = rand() % slides.size();
		int rand2;
		do
		{
			rand2 = rand() % slides.size();
		} while (rand1 == rand2);

		swapSlides(&slides, rand1, rand2);
		int newScore = valueSlideshow(&slides);
		if (newScore > currentScore && !findInTabuList(slides, tabu_list))
		{
			currentScore = newScore;
			bestNeighbour = slides;
		}
		swapSlides(&slides, rand1, rand2);
	}
	return bestNeighbour;
}

vector<vector<image>> tabuSearch(vector<vector<image>> &slides, int initialScore)
{
	vector<vector<image>> bestNeighbourhoodWithoutTabu;
	vector<vector<image>> bestSlides = slides;
	int maxTries = 0;
	queue<vector<vector<image>>> tabu_list;

	do
	{
		bestNeighbourhoodWithoutTabu = generateNeighbourhood(bestSlides, initialScore, tabu_list);
		tabu_list.push(bestSlides);
		if (tabu_list.size() > sqrt(slides.size()))
		{
			tabu_list.pop();
		}

		bestSlides = bestNeighbourhoodWithoutTabu;
		maxTries++;
	} while (maxTries < slides.size() / 2);

	int newScore = valueSlideshow(&bestSlides);
	cout << "\nNewScore = " << newScore << endl;

	return bestSlides;
}