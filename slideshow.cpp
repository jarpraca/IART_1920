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
int hillClimbing(vector<image> images);

int main()
{
	string nome_ficheiro = "f.txt";
	// string nome_ficheiro = "b_lovely_landscapes.txt";
	// string nome_ficheiro = "c_memorable_moments.txt";
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

		// cout << img.id << "   " << img.orientation << "   " << img.nr_tags << "   " << img.tags.size() << endl;
		images.push_back(img);

		id++;
	}

	hillClimbing(images);
}

int hillClimbing(vector<image> images)
{
	vector<vector<image>> slides;
	int firstSlide = rand() % images.size();
	int finalScore = 0;

	vector<image> v;
	v.push_back(images[firstSlide]);
	slides.push_back(v);

	images.erase(images.begin() + firstSlide);
	vector<image>::iterator it;

	while(images.size() != 0)
	{
		vector<int> scores;
		// cout << "1" << endl;
		for (int i = 0; i < images.size(); i++)
		{
			vector<image> v2;
			// cout << "2" << endl;
			v2.push_back(images[i]);

			// cout << "3" << endl;
			scores.push_back(valueSlideshow(slides[slides.size() - 1], v2));
		}
		// cout << "4" << endl;
		int maxIndex = max_element(scores.begin(), scores.end()) - scores.begin();
		vector<image> v1;
		// cout << "5" << endl;
		v1.push_back(images[maxIndex]);
		// cout << "5.5" << endl;
		slides.push_back(v1);
		// cout << "6" << endl;
		finalScore += scores[maxIndex];
		// cout << "7" << endl;
		images.erase(images.begin() + maxIndex);
		// cout << "8" << endl;
		// cout << endl;
	}

	for (int i = 0; i < slides.size(); i++)
	{
		cout << slides[i][0].id << " ";
	}

	cout << endl
		 << "finalScore: " << finalScore << endl;

	return 0;
}

int valueSlideshow(vector<image> slide1, vector<image> slide2)
{
	vector<string> tags1;

	for (size_t i = 0; i < slide1.size(); i++)
	{
		tags1.insert(tags1.end(), slide1[i].tags.begin(), slide1[i].tags.end());
	}

	// cout << "9" << endl;
	sort(tags1.begin(), tags1.end());
	// unique(tags1.begin(), tags1.end());
	// cout << "10" << endl;

	vector<string> tags2;

	// cout << "11" << endl;
	for (size_t i = 0; i < slide2.size(); i++)
	{
		tags2.insert(tags2.end(), slide2[i].tags.begin(), slide2[i].tags.end());
	}

	// cout << "12" << endl;
	sort(tags2.begin(), tags2.end());
	// unique(tags2.begin(), tags2.end());

	vector<string> tagsCommon(min(tags1.size(), tags2.size()));
	vector<string> tags1NotTags2(max(tags1.size(), tags2.size()));
	vector<string> tags2NotTags1(max(tags1.size(), tags2.size()));
	vector<string>::iterator it;

		// cout << "13" << endl;
	it = set_intersection(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tagsCommon.begin());
	tagsCommon.resize(it - tagsCommon.begin());

		// cout << "14" << endl;
	it = set_difference(tags1.begin(), tags1.end(), tags2.begin(), tags2.end(), tags1NotTags2.begin());
	tags1NotTags2.resize(it - tags1NotTags2.begin());

		// cout << "15" << endl;
	it = set_difference(tags2.begin(), tags2.end(), tags1.begin(), tags1.end(), tags2NotTags1.begin());
	tags2NotTags1.resize(it - tags2NotTags1.begin());

		// cout << "16" << endl;
	return min(tagsCommon.size(), min(tags1NotTags2.size(), tags2NotTags1.size()));
}