#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

struct image
{
	int id;
	char orientation;
	int nr_tags;
	vector<string> tags;
};

//Main function with very basic game play engine
int main()
{
	string nome_ficheiro = "a_example.txt";
	ifstream ficheiro;
	ficheiro.open(nome_ficheiro);
	if (ficheiro.fail())
	{
		cerr << "Input file opening failed." << endl;
		exit(1);
	}

	string aux;
	string nr_images_aux;
	int nr_images;
	getline(ficheiro, nr_images_aux);
	nr_images = stoi(nr_images_aux);

	vector<image> slides;

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

		cout << img.id << "   " << img.orientation << "   " << img.nr_tags << "   " << img.tags.size() << endl;
		slides.push_back(img);

		id++;
	}

	
}
