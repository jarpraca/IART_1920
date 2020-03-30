#include "slideshow.h"
#include <unistd.h>

using namespace std;
string chooseFile()
{
    while (true)
    {
        try
        {
            int option;
            cout << endl
                 << "Escolha um ficheiro de imagens:" << endl;
            cout << "A - Example " << endl;
            cout << "B - Lovely Landscapes" << endl;
            cout << "C - Memorable Moments" << endl;
            cout << "D - Pet Pictures" << endl;
            cout << "E - Shiny Selfies" << endl;
            cout << "F - Example 2" << endl;
            cout << endl;
            cout << "Opcao? ";
            cin >> option;
            option = toupper(option);
            while (!cin || ((option != 1) && (option != 2) && (option != 3) && (option != 4) && (option != 5) && (option != 6)))
            {
                cin.clear();
                cin.ignore(100000, '\n');
                cout << endl
                     << "Invalid Input" << endl;
                cout << "Option? ";
                cin >> option;
            }
            switch (option)
            {
            case 1:
            {
                return "a_example.txt";
            }
            case 2:
            {
                return "b_lovely_landscapes.txt";
            }
            case 3:
            {
                return "c_memorable_moments.txt";
            }
            case 4:
            {
                return "d_pet_pictures.txt";
            }
            case 5:
            {
                return "e_shiny_selfies.txt";
            }
            case 6:
            {
                return "f.txt";
            }
            }
        }
        catch (exception &e)
        {
            e.what();
        }
    }
}

vector<image> readFile(string nome_ficheiro)
{
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

        images.push_back(img);

        id++;
    }
    return images;
}

void chooseAlgorithm()
{
    while (true)
    {
        try
        {
            int option;
            cout << endl
                 << "Escolha um metodo de pesquisa:" << endl;
            cout << "1 - Hill Climbing " << endl;
            cout << "2 - Simulated Annealing " << endl;
            cout << "3 - Tabu Search " << endl;
            cout << "4 - Genetic Algorithm " << endl;
            cout << "0 - Exit" << endl
                 << endl;
            cout << "Opcao? ";
            cin >> option;
            option = toupper(option);
            while (!cin || ((option != 0) && (option != 1) && (option != 2) && (option != 3) && (option != 4)))
            {
                cin.clear();
                cin.ignore(100000, '\n');
                cout << endl
                     << "Invalid Input" << endl;
                cout << "Option? ";
                cin >> option;
            }
            switch (option)
            {
            case 1:
            {
                string nome_ficheiro = chooseFile();
                vector<image> images = readFile(nome_ficheiro);
                vector<vector<image>> slides;
                orderImages(images, &slides);
                int initialScore = valueSlideshow(&slides);
                cout << "initialScore = " << initialScore << endl;
                hillClimbing(slides);
                break;
            }
            case 2:
            {
                string nome_ficheiro = chooseFile();
                vector<image> images = readFile(nome_ficheiro);
                vector<vector<image>> slides;
                orderImages(images, &slides);
                int initialScore = valueSlideshow(&slides);
                cout << "initialScore = " << initialScore << endl;
                simulatedAnnealing(slides);
                break;
            }
            case 3:
            {
                string nome_ficheiro = chooseFile();
                vector<image> images = readFile(nome_ficheiro);
                vector<vector<image>> slides;
                orderImages(images, &slides);
                int initialScore = valueSlideshow(&slides);
                cout << "initialScore = " << initialScore << endl;
                tabuSearch(slides, initialScore);
                break;
            }
            case 4:
            {
                string nome_ficheiro = chooseFile();
                vector<image> images = readFile(nome_ficheiro);
                vector<vector<image>> slides;
                orderImages(images, &slides);
                int initialScore = valueSlideshow(&slides);
                cout << "initialScore = " << initialScore << endl;
                vector<int> generationScores;
                vector<vector<vector<image>>> generation = createInitialGeneration(&slides, &generationScores);
                geneticAlgorithm(&generation, &generationScores);
                break;
            }
            case 0:
            {
                return;
            }
            }
        }
        catch (exception &e)
        {
            e.what();
        }
    }
}

int main()
{

    cout << "===========================" << endl;
    cout << "===   PHOTO SLIDESHOW   ===" << endl;
    cout << "===========================" << endl;

    chooseAlgorithm();
}