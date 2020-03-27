#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iomanip> 

using namespace std;

struct image
{
	int id;
	char orientation;
	int nr_tags;
	vector<string> tags;
};

int simulatedAnnealing(vector<vector<image>> slides);
int valueSlides(vector<image> slide1, vector<image> slide2);
int valueSlideshow(vector<vector<image>>* slides);
int hillClimbing(vector<vector<image>> slides);
void orderImages(vector<image> images, vector<vector<image>> *slides);
bool swap_slides(vector<vector<image>> *slides, int firstSlide, int secondSlide);
bool swap_photos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);

int main()
{
	// string nome_ficheiro = "f.txt";
	//string nome_ficheiro = "b_lovely_landscapes.txt";
	// string nome_ficheiro = "c_memorable_moments.txt";
	// string nome_ficheiro = "d_pet_pictures.txt";
	string nome_ficheiro = "e_shiny_selfies.txt";
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

	orderImages(images, &slides);

	// for (int i = 0; i < slides.size(); i++)
	// {
	// 	cout << "[";
	// 	cout << slides[i][0].id;
	// 	if (slides[i].size() == 2)
	// 		cout << " " << slides[i][1].id;
	// 	cout << "] ";
	// }
	cout << endl;

	//hillClimbing(slides);
	simulatedAnnealing(slides);
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
	// cout << "1" << endl;

	vector<image> leftNeighbourSlide1, rightNeighbourSlide1, leftNeighbourSlide2, rightNeighbourSlide2;
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2, currentScore;

	// cout << "2" << endl;
	if (firstSlide > 0)
	{
		// cout << "3" << endl;
		leftNeighbourSlide1 = slides->at(firstSlide - 1);
		scoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		// cout << "4" << endl;
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		// cout << "5" << endl;
		rightNeighbourSlide2 = slides->at(secondSlide + 1);
		scoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		// cout << "6" << endl;
		scoreRightNeighbour2 = 0;
	}

	// cout << "7" << endl;
	rightNeighbourSlide1 = slides->at(firstSlide + 1);
	leftNeighbourSlide2 = slides->at(secondSlide - 1);

	// cout << "8" << endl;
	scoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;

	// cout << "9" << endl;
	int swap_choice, firstPhoto, secondPhoto;
	if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
	{
		// cout << "10" << endl;
		swap_choice = rand() % slides->size();
		if (swap_choice / 2 == 0)
			swap_slides(slides, firstSlide, secondSlide);
		else
		{
			// cout << "11" << endl;
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
			// cout << "12" << endl;
		}
	}
	else
	{
		// cout << "13" << endl;
		swap_slides(slides, firstSlide, secondSlide);
	}

	int newScoreLeftNeighbour1, newScoreRightNeighbour1, newScoreLeftNeighbour2, newScoreRightNeighbour2, newScore;

	if (firstSlide > 0)
	{
		// cout << "14" << endl;
		newScoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		// cout << "15" << endl;
		newScoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		// cout << "16" << endl;
		newScoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		// cout << "17" << endl;
		newScoreRightNeighbour2 = 0;
	}
	// cout << "18" << endl;

	newScoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	//cout << "19" << endl;
	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;
	// //cout << "first  " << firstSlide << "  second  " << secondSlide;
	// //cout << "current  " << currentScore << "  new  " << newScore << endl;
	if (newScore <= currentScore)
	{
		//cout << "20" << endl;
		if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
		{
			//cout << "21" << endl;
			if (swap_choice / 2 == 0)
			{
				swap_slides(slides, firstSlide, secondSlide);
				//cout << "22" << endl;
			}
			else
			{
				swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
				//cout << "23" << endl;
			}
			//cout << "24" << endl;
		}
		else
		{
			//cout << "25" << endl;
			swap_slides(slides, firstSlide, secondSlide);
			//cout << "26" << endl;
		}
		//cout << "27" <<endl;
		return false;
	}
	else
	{
		//cout << "28" <<endl;
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
	// cout << "1" << endl;

	vector<image> leftNeighbourSlide1, rightNeighbourSlide1, leftNeighbourSlide2, rightNeighbourSlide2;
	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2, currentScore;

	// cout << "2" << endl;
	if (firstSlide > 0)
	{
		// cout << "3" << endl;
		leftNeighbourSlide1 = slides->at(firstSlide - 1);
		scoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		// cout << "4" << endl;
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		// cout << "5" << endl;
		rightNeighbourSlide2 = slides->at(secondSlide + 1);
		scoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		// cout << "6" << endl;
		scoreRightNeighbour2 = 0;
	}

	// cout << "7" << endl;
	rightNeighbourSlide1 = slides->at(firstSlide + 1);
	leftNeighbourSlide2 = slides->at(secondSlide - 1);

	// cout << "8" << endl;
	scoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;

	
	// cout << "9" << endl;
	int swap_choice, firstPhoto, secondPhoto;
	if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
	{
		// cout << "10" << endl;
		swap_choice = rand() % slides->size();
		if (swap_choice / 2 == 0)
			swap_slides(slides, firstSlide, secondSlide);
		else
		{
			// cout << "11" << endl;
			firstPhoto = rand() % 2;
			secondPhoto = rand() % 2;
			swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
			// cout << "12" << endl;
		}
	}
	else
	{
		// cout << "13" << endl;
		swap_slides(slides, firstSlide, secondSlide);
	}

	int newScoreLeftNeighbour1, newScoreRightNeighbour1, newScoreLeftNeighbour2, newScoreRightNeighbour2, newScore;

	if (firstSlide > 0)
	{
		// cout << "14" << endl;
		newScoreLeftNeighbour1 = valueSlides(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		// cout << "15" << endl;
		newScoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		// cout << "16" << endl;
		newScoreRightNeighbour2 = valueSlides(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		// cout << "17" << endl;
		newScoreRightNeighbour2 = 0;
	}
	// cout << "18" << endl;

	newScoreRightNeighbour1 = valueSlides(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlides(leftNeighbourSlide2, slides->at(secondSlide));

	//cout << "19" << endl;
	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;
	// //cout << "first  " << firstSlide << "  second  " << secondSlide;
	// //cout << "current  " << currentScore << "  new  " << newScore << endl;

	if (newScore < currentScore){
		double delta = newScore - currentScore;
		double randE = (double)(rand() % 1000) / 1000;
		// cout << "rand " << std::setprecision(5) << randE << "  exp  " << exp(delta/temperature) << endl;
		if (randE >= exp(delta / temperature))
		{
			if (slides->at(firstSlide).size() == 2 && slides->at(secondSlide).size() == 2)
			{
				//cout << "21" << endl;
				if (swap_choice / 2 == 0)
				{
					swap_slides(slides, firstSlide, secondSlide);
					//cout << "22" << endl;
				}
				else
				{
					swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
					//cout << "23" << endl;
				}
				//cout << "24" << endl;
			}
			else
			{
				//cout << "25" << endl;
				swap_slides(slides, firstSlide, secondSlide);
				//cout << "26" << endl;
			}
			//cout << "27" <<endl;

			return false;
		}else{
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
	// cout << ("slides") << endl;
	swap(slides->at(firstSlide), slides->at(secondSlide));

	return true;
}

bool swap_photos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
{
	// cout << ("photos") << endl;
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

	for (size_t i = 0; i < slides.size() - 1; i++)
	{
		initialScore += valueSlides(slides[i], slides[i + 1]);
	}

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

	for (size_t i = 0; i < slides.size() - 1; i++)
	{
		finalScore += valueSlides(slides[i], slides[i + 1]);
	}

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
			// cout << temperature << endl;
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

int valueSlideshow(vector<vector<image>>* slides)
{
	int score = 0;

	for (size_t i = 0; i < slides->size() - 1; i++)
	{
		score += valueSlides(slides->at(i), slides->at(i + 1));
	}

	return score;
}