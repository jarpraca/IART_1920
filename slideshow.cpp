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
void orderImages(vector<image> images, vector<vector<image>>* slides);

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

	hillClimbing(slides);
}

void orderImages(vector<image> images, vector<vector<image>>* slides)
{
	for (int i = 0; i < images.size() - 1; i++)
	{
		if (images[i].orientation == 'V' && images[i+1].orientation == 'H')
		{
			for (int j = i + 2; j < images.size(); j++)
			{
				if ((images[j].orientation == 'V' && j + 1 < images.size() && images[j + 1].orientation == 'H') || (images[j].orientation == 'V' && j == images.size() - 1))
				{
					vector<image> slide;
					slide.push_back(images[i]);
					slide.push_back(images[j]);
					
					slides->push_back(slide);
					i++;
					break;
				}
			}
		}
		else if(images[i].orientation == 'V' && images[i+1].orientation == 'V'){
			vector<image> slide;
			slide.push_back(images[i]);
			slide.push_back(images[i+1]);

			slides->push_back(slide);
			i++;
		}
		else {
			vector<image> slide;
			slide.push_back(images[i]);
			slides->push_back(slide);
		}
	}
}

bool swap_slides(vector<vector<image>>* slides, int firstSlide, int secondSlide) {

	vector<image> leftNeighbourSlide1, rightNeighbourSlide1, leftNeighbourSlide2, rightNeighbourSlide2;
	
	copy( slides->at(firstSlide-1).begin(), slides->at(firstSlide-1).end(), leftNeighbourSlide1.begin() );
	copy( slides->at(firstSlide+1).begin(), slides->at(firstSlide+1).end(), rightNeighbourSlide1.begin() );
	copy( slides->at(secondSlide-1).begin(), slides->at(secondSlide-1).end(), leftNeighbourSlide2.begin() );
	copy( slides->at(secondSlide+1).begin(), slides->at(secondSlide+1).end(), rightNeighbourSlide2.begin() );

	int scoreLeftNeighbour1, scoreRightNeighbour1, scoreLeftNeighbour2, scoreRightNeighbour2, currentScore;

	scoreLeftNeighbour1 = valueSlideshow(leftNeighbourSlide1, slides->at(firstSlide));
	scoreRightNeighbour1 = valueSlideshow(rightNeighbourSlide1, slides->at(firstSlide));
	scoreLeftNeighbour2 = valueSlideshow(leftNeighbourSlide2, slides->at(secondSlide));
	scoreRightNeighbour2 = valueSlideshow(rightNeighbourSlide1, slides->at(secondSlide));
	currentScore = scoreLeftNeighbour1 + scoreLeftNeighbour2 + scoreRightNeighbour1 + scoreRightNeighbour2;
	
	swap(slides->at(firstSlide), slides->at(secondSlide));

	int newScoreLeftNeighbour1, newScoreRightNeighbour1, newScoreLeftNeighbour2, newScoreRightNeighbour2, newScore;
	newScoreLeftNeighbour1 = valueSlideshow(leftNeighbourSlide1, slides->at(firstSlide));
	newScoreRightNeighbour1 = valueSlideshow(rightNeighbourSlide1, slides->at(firstSlide));
	newScoreLeftNeighbour2 = valueSlideshow(leftNeighbourSlide2, slides->at(secondSlide));
	newScoreRightNeighbour2 = valueSlideshow(rightNeighbourSlide1, slides->at(secondSlide));
	newScore = newScoreLeftNeighbour1 + newScoreLeftNeighbour2 + newScoreRightNeighbour1 + newScoreRightNeighbour2;

	if(newScore < currentScore){
		swap(slides->at(firstSlide), slides->at(secondSlide));
		return false;
	}
	else
		return true;
	
}

bool swap_photos(vector<vector<image>>* slides, int firstSlide, int secondSlide){
	int firstPhoto = rand() % 2;
	int secondPhoto = rand() % 2;

	image img = slides->at(firstSlide).at(firstPhoto);
	slides->at(firstSlide).at(firstPhoto) = slides->at(secondSlide).at(secondPhoto);
	slides->at(secondSlide).at(secondPhoto) = img;
}

int hillClimbing(vector<vector<image>> slides)
{	
	int finalScore = 0;

	int firstSlide = rand() % slides.size();
	int secondSlide = rand() % slides.size();
	cout << "olaaaaaaaaaa" << endl;

	// if (slides[firstSlide].size() == 2 && slides[secondSlide].size() == 2){
	// 	int swap_choice = rand() % slides.size();
		// if(swap_choice/2 == 0)
			// swap_slides(&slides, firstSlide, secondSlide);
		// else
		// 	swap_photos(slides, firstSlide, secondSlide);
	// }else{
		swap_slides(&slides, firstSlide, secondSlide);
	// }
	for (int i = 0; i < slides.size(); i++)
	{
		cout << "[";
		cout << slides[i][0].id << " ";
		if(slides[i].size() == 2)
			cout << slides[i][1].id;
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

<<<<<<< HEAD
	cout << endl << finalScore << endl;*/
=====
>>>>>>> ddf7eeb39b2161645f517360132ebc5910ad3574

	return 0;
}

int valueSlideshow(vector<image> slide1, vector<image> slide2)
{
	vector<string> tags1;

	for (size_t i = 0; i < slide1.size(); i++)
	{
		tags1.insert(tags1.end(), slide1[i].tags.begin(), slide1[i].tags.end());
	}

	if(slide1.size() == 2)
		unique(tags1.begin(), tags1.end());

	vector<string> tags2;

	for (size_t i = 0; i < slide2.size(); i++)
	vector<string> tags1NotTags2(max(tags1.size(), tags2.size()));

	return min(tagsCommon.size(), min(tags1NotTags2.size(), tags2NotTags1.size()));
