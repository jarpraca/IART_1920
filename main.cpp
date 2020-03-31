#include "slideshow.h"
#include <chrono>

using namespace std;
string chooseFile()
{

    try
    {
        int option;
        cout << endl
             << "Escolha um ficheiro de imagens:" << endl;
        cout << "1 - Ficheiro A: Example " << endl;
        cout << "2 - Ficheiro B: Lovely Landscapes" << endl;
        cout << "3 - Ficheiro C: Memorable Moments" << endl;
        cout << "4 - Ficheiro D: Pet Pictures" << endl;
        cout << "5 - Ficheiro E: Shiny Selfies" << endl;
        cout << "6 - Ficheiro F: Another Example" << endl;
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
    return "";
}

vector<image> readFile(string nomeFicheiro)
{
    ifstream ficheiro;
    ficheiro.open(nomeFicheiro);

    if (ficheiro.fail())
    {
        cerr << "Input file opening failed." << endl;
        exit(1);
    }

    srand(time(NULL));

    string aux;
    string nrImagesAux;
    int nrImages;
    getline(ficheiro, nrImagesAux);
    nrImages = stoi(nrImagesAux);

    vector<image> images;

    int id = 0;
    while (getline(ficheiro, aux))
    {
        struct image img;
        string orientation, tag;
        vector<string> tags;
        orientation = aux.substr(0, aux.find(" "));
        aux = aux.substr(aux.find(" ") + 1);
        int nrTags = stoi(aux.substr(0, aux.find(" ")));
        aux = aux.substr(aux.find(" ") + 1);
        int n = 0;
        while (n < nrTags)
        {
            tag = aux.substr(0, aux.find(" "));
            tags.push_back(tag);
            aux = aux.substr(aux.find(" ") + 1);
            n++;
        }

        img.id = id;
        img.orientation = orientation[0];
        img.nrTags = nrTags;
        img.tags = tags;

        images.push_back(img);

        id++;
    }
    return images;
}

void chooseNumIterations(int &numIterations)
{

    try
    {
        string option;
        cout << endl
             << "Choose number of iterations:" << endl;
        cin >> option;
        numIterations = stoi(option);
    }
    catch (exception &e)
    {
        e.what();
    }
}

void chooseTemperature(double &temperature)
{
    try
    {
        string option;
        cout << endl
             << "Choose initial temperature:" << endl;
        cin >> option;
        temperature = stod(option);
    }
    catch (exception &e)
    {
        e.what();
    }
}

void chooseNeighbourhoodSize(int &numNeighbours)
{
    try
    {
        string option;
        cout << endl
             << "Choose number of neighbours:" << endl;
        cin >> option;
        numNeighbours = stoi(option);
    }
    catch (exception &e)
    {
        e.what();
    }
}

void choosePopulationSize(int &populationSize, int &numGenerations)
{
    try
    {
        string option;
        cout << endl
             << "Choose population size:" << endl;
        cin >> option;
        populationSize = stoi(option);
        option = "";
        cout << endl
             << "Choose number of generations:" << endl;
        cin >> option;
        numGenerations = stoi(option);
    }
    catch (exception &e)
    {
        e.what();
    }
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
                string nomeFicheiro = chooseFile();
                vector<image> images = readFile(nomeFicheiro);
                int numIterations;
                chooseNumIterations(numIterations);
                vector<vector<image>> slideshow;
                orderImages(images, slideshow);
                int initialScore = valueSlideshow(slideshow);
                cout << "\nInitial Score = " << initialScore << endl;
                auto ti = std::chrono::high_resolution_clock::now();
                int finalScore = hillClimbing(slideshow, numIterations, initialScore);
                auto tf = std::chrono::high_resolution_clock::now();
                cout << "\n\nInitial Score = " << initialScore << endl;
                cout << "\nFinal Score = " << finalScore << endl;
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(tf - ti).count();
                cout << "Time Execution = " << duration << " seconds" << endl;
                break;
            }
            case 2:
            {
                string nomeFicheiro = chooseFile();
                vector<image> images = readFile(nomeFicheiro);
                vector<vector<image>> slideshow;
                int numIterations;
                double temperature;
                chooseNumIterations(numIterations);
                chooseTemperature(temperature);
                orderImages(images, slideshow);
                int initialScore = valueSlideshow(slideshow);
                cout << "\nInitial Score = " << initialScore << endl;
                auto ti = std::chrono::high_resolution_clock::now();
                int finalScore = simulatedAnnealing(slideshow, temperature, numIterations, initialScore);
                auto tf = std::chrono::high_resolution_clock::now();
                cout << "\n\nInitial Score = " << initialScore << endl;
                cout << "\nFinal Score = " << finalScore << endl;
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(tf - ti).count();
                cout << "Time Execution = " << duration << " seconds" << endl;
                break;
            }
            case 3:
            {
                string nomeFicheiro = chooseFile();
                vector<image> images = readFile(nomeFicheiro);
                vector<vector<image>> slideshow;
                int numIterations, numNeighbours;
                chooseNumIterations(numIterations);
                chooseNeighbourhoodSize(numNeighbours);
                orderImages(images, slideshow);
                int initialScore = valueSlideshow(slideshow);
                cout << "\nInitial Score = " << initialScore << endl;
                auto ti = std::chrono::high_resolution_clock::now();
                int finalScore = tabuSearch(slideshow, numIterations, numNeighbours, initialScore);
                auto tf = std::chrono::high_resolution_clock::now();
                cout << "\n\nInitial Score = " << initialScore << endl;
                cout << "\nFinal Score = " << finalScore << endl;
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(tf - ti).count();
                cout << "Time Execution = " << duration << " seconds" << endl;
                break;
            }
            case 4:
            {
                string nomeFicheiro = chooseFile();
                vector<image> images = readFile(nomeFicheiro);
                vector<vector<image>> slideshow;
                int numGenerations, populationSize;
                choosePopulationSize(populationSize, numGenerations);
                orderImages(images, slideshow);
                int initialScore = valueSlideshow(slideshow);
                cout << "\nInitial Score = " << initialScore << endl;
                auto ti = std::chrono::high_resolution_clock::now();
                vector<int> generationScores;
                vector<vector<vector<image>>> generation = createInitialGeneration(slideshow, generationScores, populationSize);
                int finalScore = geneticAlgorithm(generation, generationScores, numGenerations);
                auto tf = std::chrono::high_resolution_clock::now();
                cout << "\n\nInitial Score = " << initialScore << endl;
                cout << "\nFinal Score = " << finalScore << endl;
                auto duration = std::chrono::duration_cast<std::chrono::seconds>(tf - ti).count();
                cout << "Time Execution = " << duration << " seconds" << endl;
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
