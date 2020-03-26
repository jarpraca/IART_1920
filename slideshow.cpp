#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>

using namespace std;

struct image
{
	int id;
	char orientation;
	int nr_tags;
	vector<string> tags;
};

int valueSlideshow(vector<image> slide1, vector<image> slide2);
int hillClimbing(vector<vector<image>> slides);
void orderImages(vector<image> images, vector<vector<image>> *slides);
bool swap_slides(vector<vector<image>> *slides, int firstSlide, int secondSlide);
bool swap_photos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto);

int main()
{
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

	orderImages(images, &slides);

	for (int i = 0; i < slides.size(); i++)
	{
		cout << "[";
		cout << slides[i][0].id;
		if (slides[i].size() == 2)
			cout << " " << slides[i][1].id;
		cout << "] ";
	}
	cout << endl;

	hillClimbing(slides);
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

bool evaluate_swap(vector<vector<image>> *slides)
{
	int firstSlide = rand() % slides->size();
	int secondSlide = rand() % slides->size();

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
		scoreLeftNeighbour1 = valueSlideshow(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		scoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		rightNeighbourSlide2 = slides->at(secondSlide + 1);
		scoreRightNeighbour2 = valueSlideshow(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		scoreRightNeighbour2 = 0;
	}

	rightNeighbourSlide1 = slides->at(firstSlide + 1);
	leftNeighbourSlide2 = slides->at(secondSlide - 1);

	scoreRightNeighbour1 = valueSlideshow(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlideshow(leftNeighbourSlide2, slides->at(secondSlide));

	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;
	
	int swap_choice, firstPhoto, secondPhoto;
	if (slides[firstSlide].size() == 2 && slides[secondSlide].size() == 2)
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
		newScoreLeftNeighbour1 = valueSlideshow(leftNeighbourSlide1, slides->at(firstSlide));
	}
	else
	{
		newScoreLeftNeighbour1 = 0;
	}

	if (secondSlide < slides->size() - 1)
	{
		newScoreRightNeighbour2 = valueSlideshow(rightNeighbourSlide2, slides->at(secondSlide));
	}
	else
	{
		newScoreRightNeighbour2 = 0;
	}

	newScoreRightNeighbour1 = valueSlideshow(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlideshow(leftNeighbourSlide2, slides->at(secondSlide));

	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;
	cout << "first  " << firstSlide << "  second  " << secondSlide; 
	cout << "current  " << currentScore << "  new  " << newScore << endl;
	if (newScore < currentScore)
	{
		if (slides[firstSlide].size() == 2 && slides[secondSlide].size() == 2)
		{
			if (swap_choice / 2 == 0)
				swap_slides(slides, firstSlide, secondSlide);
			else
			{
				swap_photos(slides, firstSlide, secondSlide, firstPhoto, secondPhoto);
			}
		}
		else
			swap_slides(slides, firstSlide, secondSlide);
		return false;
	}
	else
		return true;
}

bool swap_slides(vector<vector<image>> *slides, int firstSlide, int secondSlide)
{
	cout <<("slides") << endl;
	swap(slides->at(firstSlide), slides->at(secondSlide));
}

bool swap_photos(vector<vector<image>> *slides, int firstSlide, int secondSlide, int firstPhoto, int secondPhoto)
{
		cout <<("photos") << endl;
	image img = slides->at(firstSlide).at(firstPhoto);
	slides->at(firstSlide).at(firstPhoto) = slides->at(secondSlide).at(secondPhoto);
	slides->at(secondSlide).at(secondPhoto) = img;
}

int hillClimbing(vector<vector<image>> slides)
{
	int finalScore = 0;

	int firstSlide = rand() % slides.size();
	int secondSlide = rand() % slides.size();

	if (secondSlide < firstSlide)
	{
		int first_temp = firstSlide;
		firstSlide = secondSlide;
		secondSlide = first_temp;
	}
	// if (slides[firstSlide].size() == 2 && slides[secondSlide].size() == 2)
	// {
	// 	int swap_choice = rand() % slides.size();
	// 	// if (swap_choice / 2 == 0)
	// 	// 	swap_slides(&slides, firstSlide, secondSlide);
	// 	// else
	// 	swap_photos(&slides, firstSlide, secondSlide);
	// }
	// else
	// {
	// 	swap_slides(&slides, firstSlide, secondSlide);
	// }

	evaluate_swap(&slides);
	
	for (int i = 0; i < slides.size(); i++)
	{
		cout << "[";
		cout << slides[i][0].id;
		if (slides[i].size() == 2)
			cout << " " << slides[i][1].id;
		cout << "] ";
	}

	cout << endl;

	/*vector<image> v;
	v.push_back(images[firstSlide]);
	slides.push_back(v);

	images.erase(images.begin() + firstSlide);
	vector<image>::iterator it;

	while (images.size() != 0)
	{
		vector<int> scores;
		
	}

	for (int i = 0; i < slides.size(); i++)
	{
		cout << slides[i][0].id << " ";
	}

	cout << endl << finalScore << endl;*/

	return 0;
}

int valueSlideshow(vector<image> slide1, vector<image> slide2)
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

	vector<string> tagsCommon(10000000);
	vector<string> tags1NotTags2(10000000);
	vector<string> tags2NotTags1(10000000);
	vector<string>::iterator it;

	it = set_intersection(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tagsCommon.begin());
	tagsCommon.resize(it - tagsCommon.begin());

	it = set_difference(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tags1NotTags2.begin());
	tags1NotTags2.resize(it - tags1NotTags2.begin());

	it = set_difference(tags2.begin(), tags2.end(), tags1.begin(), tags1.end(), tags2NotTags1.begin());
	tags2NotTags1.resize(it - tags2NotTags1.begin());

	return min(tagsCommon.size(), min(tags1NotTags2.size(), tags2NotTags1.size()));
}