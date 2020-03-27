#include "slideshow.h"

int main()
{
	// string nome_ficheiro = "a_example.txt";
	// string nome_ficheiro = "b_lovely_landscapes.txt";
	// string nome_ficheiro = "c_memorable_moments.txt";
	// string nome_ficheiro = "d_pet_pictures.txt";
	// string nome_ficheiro = "e_shiny_selfies.txt";
	string nome_ficheiro = "f.txt";

	ifstream ficheiro;
	ficheiro.open(nome_ficheiro);
	if (ficheiro.fail())
	{
		cerr << "Input file opening failed." << endl;
		exit(1);
	}

	srand(time(NULL));

	string aux;
	string nr_images_aux;
	int nr_images;
	getline(ficheiro, nr_images_aux);
	nr_images = stoi(nr_images_aux);

	vector<vector<image>> slides;
	vector<image> images;

	int id = 0;
	while (getline(ficheiro, aux))
	{
		struct image img;
		string orientation, tag;
		vector<string> tags;
		orientation = aux.substr(0, aux.find(" "));
		aux = aux.substr(aux.find(" ") + 1);
		int nr_tags = stoi(aux.substr(0, aux.find(" ")));
		aux = aux.substr(aux.find(" ") + 1);
		int n = 0;
		while (n < nr_tags)
		{
			tag = aux.substr(0, aux.find(" "));
			tags.push_back(tag);
			aux = aux.substr(aux.find(" ") + 1);
			n++;
		}

		img.id = id;
		img.orientation = orientation[0];
		img.nr_tags = nr_tags;
		img.tags = tags;

		images.push_back(img);

		id++;
	}

	cout << "read" << endl;

	orderImages(images, &slides);

	cout << "ordered" << endl;

	// hillClimbing(slides);
	// simulatedAnnealing(slides);

	createInitialGeneration(&slides);
}

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
			swap_slides(slides, firstSlide, secondSlide);
		else
		{
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swap_slides(slides, firstSlide, secondSlide);
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
				swap_slides(slides, firstSlide, secondSlide);
			}
			else
			{
				swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
			}
		}
		else
		{
			swap_slides(slides, firstSlide, secondSlide);
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
			swap_slides(slides, firstSlide, secondSlide);
		else
		{
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
		}
	}
	else
	{
		swap_slides(slides, firstSlide, secondSlide);
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
					swap_slides(slides, firstSlide, secondSlide);
				}
				else
				{
					swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
				}
			}
			else
			{
				swap_slides(slides, firstSlide, secondSlide);
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

bool swap_slides(vector<vector<image>> *slides, int firstSlide, int secondSlide)
{
	swap(slides->at(firstSlide), slides->at(secondSlide));
	return true;
}

bool swap_photos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
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

	initialScore = valueSlideshow(&slides);

	cout << "initial " << initialScore << endl;

	int currentScore, newScore;

	while (maxTries < sqrt(slides.size()))
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

	initialScore = valueSlideshow(&slides);

	cout << "initial " << initialScore << endl;

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
	cout << endl;

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

vector<vector<vector<image>>> createInitialGeneration(vector<vector<image>> *slides)
{
	vector<vector<vector<image>>> generation;

	generation.push_back(*slides);

	for (auto i = 0; i < 29; i++)
	{
		int rand1 = rand() % slides->size();
		int rand2 = rand() % slides->size();
		vector<vector<image>> newSlides = *slides;

		swap_slides(&newSlides, rand1, rand2);

		generation.push_back(newSlides);
	}

	for (auto j = 0; j < generation.size(); j++)
	{
		for (int i = 0; i < generation[j].size(); i++)
		{
			cout << "[";
			cout << generation[j][i][0].id;
			if (generation[j][i].size() == 2)
				cout << " " << generation[j][i][1].id;
			cout << "] ";
		}
		cout << endl;
	}

	return generation;
}