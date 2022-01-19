/*
Angelica Engström
anen1805, TDTEA
DT019G
2021-05-20
Betyg: A
*/

#include "Maze.h"

bool isInteger(const std::string &input);
int mainMenu(const int &choice);
Maze methodMenu(const char &ch, Maze maze);
Maze setSize();
Maze readMaze(FILE *__stream);
void printSolution(Maze maze);

int main(){
    //How to determine if stdin is empty:
    //https://stackoverflow.com/questions/47320496/how-to-determine-if-stdin-is-empty/47320694
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
    {   
        Maze maze = readMaze(stdin);
        printSolution(maze);
        return 0;
    }

    std::cout << "╔════════════════════════════╗" << std::endl;
    std::cout << "║ Welcome to maze generator! ║" << std::endl;
    std::cout << "╚════════════════════════════╝" << std::endl;
    std::cout << "╔═════════════╦═════════╗" << std::endl;
    std::cout << "║ 1. New maze ║ 2. Exit ║" << std::endl;
    std::cout << "╚═════════════╩═════════╝" << std::endl;

    std::string input;

    while(std::cin >> input){
        std::cin.ignore();
        std::cin.clear();
        
        if(isInteger(input)){
            int choice = stoi(input);
            if(mainMenu(choice) == 2){
                return 0;
            }
        }
        else{
            std::cerr << "Type (1) or (2)" << std::endl;
        }
        sleep(2);
        system("clear");

        std::cout << "╔═════════════╦═════════╗" << std::endl;
        std::cout << "║ 1. New maze ║ 2. Exit ║" << std::endl;
        std::cout << "╚═════════════╩═════════╝" << std::endl;
    }
    throw std::out_of_range("Something went wrong in main");
}


/*
Checks if the string <input> is only containing numbers by checking each character of the string.
@param input The input that is going to be checked.
@returns A boolean thats either true or false.
*/
bool isInteger(const std::string &input){
    for(char i : input){
        if(!isdigit(i)){
            return false;
        }
    }
    return true;
}

/*
Does different kinds of tasks depending what value the int <choice> has.
The mainMenu have the options <1> (New maze) and <2> (Exit).
@param choice An int containting the selection from the menu
@returns An int containing the value of <choice>
*/
int mainMenu(const int &choice){
    char ch;
    Maze maze;

    switch(choice){
        case 1:
            system("clear");
            std::cout << "╔═════════════════════════╗" << std::endl;
            std::cout << "║ New maze! Choose method ║" << std::endl;
            std::cout << "╚═════════════════════════╝" << std::endl;
            std::cout << "╔════════╦════════╗" << std::endl;
            std::cout << "║ 1. BFS ║ 2. DFS ║" << std::endl;
            std::cout << "╚════════╩════════╝" << std::endl;
            std::cout << "Press another key to go back to main menu..." << std::endl;

            ch = getchar();
            maze = methodMenu(ch, maze);

            if(ch == '1' || ch == '2'){
                std::cout << "╔═════════════════╦═════════╗" << std::endl;
                std::cout << "║ 1. Get solution ║ 2. Play ║" << std::endl;
                std::cout << "╚═════════════════╩═════════╝" << std::endl;
                std::cout << "Press another key to go back to main menu..." << std::endl;
                
                ch = getchar();

                if(ch == '1'){
                    size_t steps = maze.getSolution();
                    std::cout << "The shortest path is " << steps << " steps!" << std::endl;
                }
                else if(ch == '2'){ 
                    size_t steps = maze.play(); 
                    std::cout << "You moved " << steps << " steps!" << std::endl;
                }
                else{ std::cout << "Heading back to main menu..." << std::endl; }
            }
        break;
        case 2:
            std::cout << "Exit program..." << std::endl;
            sleep(2);
            system("clear");
        break;
        default:
            std::cerr << "Type (1) or (2)" << std::endl;
        break;
    }
    return choice;
}

/*
The methodMenu have the options <1> (BFS-method) and <2> (DFS-method).
Depending on the value of ch the maze will be created with a different algorithm.
@param ch An char containting the selection from the menu
@param maze The maze that is getting generated
@returns An maze that contains different attributes depending on the method
*/
Maze methodMenu(const char &ch, Maze maze){
    if(ch == '1'){
        system("clear");
        std::cout << "╔══════════════════════╗" << std::endl;
        std::cout << "║ Breadth-first search ║" << std::endl;
        std::cout << "╚══════════════════════╝" << std::endl;

        maze = setSize();

        sleep(2);
        maze.bfs();
    }
    else if(ch == '2'){
        system("clear");
        std::cout << "╔════════════════════╗" << std::endl;
        std::cout << "║ Depth-first search ║" << std::endl;
        std::cout << "╚════════════════════╝" << std::endl;

        maze = setSize();

        sleep(2);
        maze.dfs();
    }
    else{
        std::cout << "Heading back to main menu..." << std::endl;
    }
    return maze;
}

/*
Get the rectangular size of the maze to the sides <sizeX> and <sizeY>.
If one side is invalid the size is default.
@returns the setted size of the maze.
*/
Maze setSize(){
    std::string input1, input2;
    size_t sizeX, sizeY;

    std::cout << "Type length of the X axis... ";
    std::cin >> input1;
    std::cin.ignore();

    if(!(isInteger(input1))){
        std::cout << "Generating a default maze..." << std::endl;
        Maze maze;
        return maze;
    }

    std::cout << "Type length of the Y axis... ";
    std::cin >> input2;
    std::cin.ignore();
    std::cin.clear();


    if(isInteger(input1) && isInteger(input2)){
        sizeX = stoi(input1);
        sizeY = stoi(input2);

        if((sizeX % 2 != 0 && sizeY % 2 != 0) && (sizeX > 3 && sizeY > 3)){
            std::cout << "Generating a " << sizeX << " x " << sizeY << " maze..." << std::endl << std::endl;
            Maze maze(sizeX, sizeY);
            return maze;
        }
        else{
            std::cout << "Generating a default maze..." << std::endl;
            Maze maze;
            return maze;
        }
    }
    else{
        std::cout << "Generating a default maze..." << std::endl;
        Maze maze;
        return maze;
    }
}

/*
Reads from a stream and returns a maze if the input is correctly formatted.
@param __stream The stream where we get the input from.
@returns A maze that contains the unsolved mazeformat. 
The maze is empty if the input is poorly formatted.
*/
Maze readMaze(FILE *__stream){
    rewind(__stream);
    Maze maze;
    maze.destroy();
    std::string input;

    int totalStart = 0;
    int totalEnd = 0;
        
    while(std::getline(std::cin, input)){
        try{
            maze.set(input);
            for(char c : input){
                if(c == 'S' || c == 's'){
                    totalStart++;
                }
                if(c == 'E' || c == 'e'){
                    totalEnd++;
                }
            }
        }
        catch(std::out_of_range &e){
            std::cout << "Exception caught: " << e.what() << std::endl;
            exit(1);
        }
    }
    
    if(totalStart != 1 || totalEnd != 1){
        throw std::out_of_range("Startposition or endposition are incorrectly formatted");
    }
    
    fclose(__stream);
    return maze;
}

/*
    A global function that prints only the solution of the Maze.
    Manipulates the cout stream to print only one maze for stdout.
    @param maze The maze thats getting printed.
*/
void printSolution(Maze maze){
    //How to manipulate the cout stream: https://bbs.archlinux.org/viewtopic.php?id=79378
    std::streambuf* cout_sbuf = std::cout.rdbuf();
    std::ofstream fout("/dev/null");
    std::cout.rdbuf(fout.rdbuf());

    size_t steps;
    try{
        steps = maze.getSolution();
    }
    catch(std::out_of_range &e){
        std::cout.rdbuf(cout_sbuf);
        std::cout << "Exception caught: " << e.what() << std::endl;
        exit(1);
    }
    std::cout.rdbuf(cout_sbuf);
    maze.print();
    std::cout << "We solved your maze in " << steps << " steps" << std::endl;
}
