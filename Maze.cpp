/*
Angelica Engström
anen1805, TDTEA
DT019G
2021-05-20
Betyg: A
*/
#include "Maze.h"

/*
    Constructor for the struct node. 
    In the creation of a node the up, down, left and right pointers will be nullpointers
    @param visited A boolean that holds the information if the node is visited or not for generating
    walls and passages
    @param path A boolean that holds the information if the node is a part of the solution path
*/
Maze::node::node(bool visited, bool path){
    this->visited = visited;
    this->path = path;
    up = nullptr;
    down = nullptr;
    left = nullptr;
    right = nullptr;
}

/*
    Constructor for the class Maze. In the declaration of a Maze its head, start and end
    pointers will be nullpointers. Makes the shell of the maze with the default sizes 11 x 11.
*/
Maze::Maze(){
    head = nullptr;
    start = nullptr;
    end = nullptr;

    srand(time(0));
    create(11, 11);
}

/*
    A parameterized constructor for the class Maze. 
    In the declaration of a Maze its head, start and end pointers will be nullpointers. 
    Creates the shell of the maze with the parameters sizeX and sizeY.
    @param sizeX An size_t containing the width of the maze.
    @param sizeY An size_t containing the height of the maze.
*/
Maze::Maze(const size_t sizeX, const size_t sizeY){
    head = nullptr;
    start = nullptr;
    end = nullptr;

    srand(time(0));
    create(sizeX, sizeY);
}

/*
    Deconstructor for the class Maze. Upon destruction of a Maze all the values will be destroyed.
*/
Maze::~Maze(){
    if (head != nullptr){
        destroy();
    }
}

/*
    Copy constructor for the class Maze. 
    A deep copy is performed by operator= of the Maze <src> to the
    reference of the calling object.
    @param src The Maze that will be copied
*/
Maze::Maze(const Maze &src){
    head = nullptr;
    start = nullptr;
    end = nullptr;

    *this = src;
}

/*
    Overloads the operator = to assign a Maze to another Maze.
    Sets all the attributes of the assignment of a Maze.
    @param rhs The Maze on the right side that the calling object will copy.
    @returns A reference of the calling object.
*/
Maze& Maze::operator=(const Maze &rhs){
    if(head != nullptr){
        destroy();
    }

    std::ofstream outfile;
    std::ifstream infile;
    outfile.open("temp.txt");
    infile.open("temp.txt");

    for(node* d = rhs.head; d != nullptr; d = d->down){
        for(node* r = d; r != nullptr; r = r->right){
            if(r->visited){
                if(r == rhs.start){
                    outfile << "S";
                }
                else if(r == rhs.end){
                    outfile << "E";
                }
                else{
                    outfile << " ";
                }
            }
            else{
                outfile << "X";
            }
        }
        outfile << std::endl;
    }
    
    std::string input;
    while(std::getline(infile, input)){
        (*this).set(input);
    }
    outfile.close();
    infile.close();
    remove("temp.txt");

    return *this;
}

/*
Makes the size of the maze to correct attributes.
Sets start- and endposition
@param sizeX An size_t containing the width of the maze.
@param sizeY An size_t containing the height of the maze.
*/
void Maze::create(size_t sizeX, size_t sizeY){
    node* temp = new node(false, false);
    node* n;
    head = temp;
    for(size_t i = 0; i < sizeX - 1; i++){
        node* r = new node(false, false);
        temp->right = r;
        r->left = temp;
        temp = r;
    }
    for(temp = head, n = head; temp != nullptr; temp = temp->right){
        for(size_t i = 0; i < sizeY - 1; i++){
            node* d = new node(false, false);
            n->down = d;
            d->up = n;
            n = d;
        }
        n = temp->right;
    }
    for(node* r = head; r->right != nullptr; r = r->right){
        for(node* d = r; d->down != nullptr; d = d->down){
            d->down->right = d->right->down;
            d->right->down->left = d->down;
        }
    }

    size_t circumference = 2 * sizeX + 2 * sizeY;
    size_t pos = 1 + rand() % circumference;
    createStart(pos);
    createEnd(pos);
}

/*
Generates walls and passages with the depth-first search algorithm.
*/
void Maze::dfs(){
    node* passage = startPassage();
    std::vector<node*> passages; 
    passages.push_back(passage);

    size_t direction;

    while(!passages.empty()){
        direction = rand() % 4;
        if(hasNoNeighbours(passage)){
            passages.pop_back();
            passage = passages.back();
        }
        else{
            if(direction == Right){ goRight(passage, passages); }
            if(direction == Left){ goLeft(passage, passages); }
            if(direction == Up){ goUp(passage, passages); } 
            if(direction == Down){ goDown(passage, passages); }
            passage = passages.back();
        }
    }
}

/*
Generates walls and passages with the breadth-first search algorithm.
*/
void Maze::bfs(){
    node* passage = startPassage();
    std::vector<node*> passages;
    passages.push_back(passage);

    size_t direction;

    while(!passages.empty()){
        if(hasNoNeighbours(passage)){
            passage = passages.front();
            if(hasNoNeighbours(passage)){
                passages.erase(passages.begin());
            }
        }
        else{
            direction = rand() % 4;
            if(direction == Right){ goRight(passage, passages); }
            if(direction == Left){ goLeft(passage, passages); }
            if(direction == Up){ goUp(passage, passages); } 
            if(direction == Down){ goDown(passage, passages); }
            passage = passages.back();
        }
    }
}

/*
Determines where the startpassage will be determined on the startposition of the maze.
@param returns A node containing the adress of the startpassage.
*/
Maze::node* Maze::startPassage(){
    node* p;
    if(start->left == nullptr){ p = start->right; }
    else if(start->right == nullptr){ p = start->left; }
    else if(start->up == nullptr){ p = start->down; }
    else if(start->down == nullptr){ p = start->up; }
    
    p->visited = true;
    return p;
}
/*
Makes the passage going right if its possible.
If possible the new adress is saved inside a vector and prints the maze with a new passage.
@param p A node that holds the current passage adress.
@param passages The vector that holds all the passages.
*/
void Maze::goRight(node* &p, std::vector<node*> &passages){
    if((p->right->right != nullptr) && (p->right->right->visited == 0)){
        p->right->right->visited = true;
        p->right->visited = true;
        passages.push_back(p->right->right);
        system("clear");
        print();
        usleep(10000);
    }
}
/*
Makes the passage going left if its possible.
If possible the new adress is saved inside a vector and prints the maze with a new passage.
@param p A node that holds the current passage adress.
@param passages The vector that holds all the passages.
*/
void Maze::goLeft(node* &p, std::vector<node*> &passages){
    if((p->left->left != nullptr) && (p->left->left->visited == 0)){
        p->left->left->visited = true;
        p->left->visited = true;
        passages.push_back(p->left->left);
        system("clear");
        print();
        usleep(10000);
    }
}

/*
Makes the passage going up if its possible.
If possible the new adress is saved inside a vector and prints the maze with a new passage.
@param p A node that holds the current passage adress.
@param passages The vector that holds all the passages.
*/
void Maze::goUp(node* &p, std::vector<node*> &passages){
    if((p->up->up != nullptr) && (p->up->up->visited == 0)){
        p->up->up->visited = true;
        p->up->visited = true;
        passages.push_back(p->up->up);
        system("clear");
        print();
        usleep(10000);
    }
}

/*
Makes the passage going down if its possible.
If possible the new adress is saved inside a vector and prints the maze with a new passage.
@param p A node that holds the current passage adress.
@param passages The vector that holds all the passages.
*/
void Maze::goDown(node* &p, std::vector<node*> &passages){
    if((p->down->down != nullptr) && (p->down->down->visited == 0)){
        p->down->down->visited = true;
        p->down->visited = true;
        passages.push_back(p->down->down);
        system("clear");
        print();
        usleep(10000);
    }
}

/*
Checks if the current passage have no neighbours to visit.
If possible returns false.
@param p A node that holds the current passage adress.
*/
bool Maze::hasNoNeighbours(const node* p) const{
    if(p->right->right == nullptr){
        if(p->up->up == nullptr){
            if(p->left->left->visited == 1 &&
            p->down->down->visited == 1){
                return true;
            }
        }
        else if(p->down->down == nullptr){
            if(p->left->left->visited == 1 &&
            p->up->up->visited == 1){
                return true;
            }
        }
        else{
            if(p->left->left->visited == 1 &&
            p->up->up->visited == 1 &&
            p->down->down->visited == 1){
                return true;
            }
        }
    }
    else if(p->down->down == nullptr){
        if(p->left->left == nullptr){
            if(p->right->right->visited == 1 &&
            p->up->up->visited == 1){
                return true;
            }
        }
        else{
            if(p->right->right->visited == 1 &&
            p->up->up->visited == 1 &&
            p->left->left->visited == 1){
                return true;
            }
        }
    }
    else if(p->up->up == nullptr){
        if(p->left->left == nullptr){
            if(p->right->right->visited == 1 &&
            p->down->down->visited == 1){
                return true;
            }
        }
        else{
            if(p->right->right->visited == 1 &&
            p->down->down->visited == 1 &&
            p->left->left->visited == 1){
                return true;
            }
        }
    }
    else if(p->left->left == nullptr){
        if(p->right->right->visited == 1 &&
            p->up->up->visited == 1 &&
            p->down->down->visited == 1){
                return true;
            }
    }
    else{
        if(p->left->left->visited == 1 &&
        p->right->right->visited == 1 &&
        p->up->up->visited == 1 &&
        p->down->down->visited == 1){
            return true;
        }
    }
    return false;
}

/*
Generates a starting position depending on the determined position.
@param pos An size_t which holds where the start will be
@returns the position start ended up with.
*/
size_t Maze::createStart(size_t pos){
    node* temp = head->down;

    for(int counter = 0; counter != pos; counter++){
        if(temp->left == nullptr){
            if(temp->down->down == nullptr){
                temp = temp->down->right;
            }
            else{
                temp = temp->down->down;
            }
        }
        else if(temp->down == nullptr){
            if(temp->right->right == nullptr){
                temp = temp->right->up;
            }
            else{
                temp = temp->right->right;
            }
        }
        else if(temp->right == nullptr){
            if(temp->up->up == nullptr){
                temp = temp->up->left;
            }
            else{
                temp = temp->up->up;
            }
        }
        else if(temp->up == nullptr){
            if(temp->left->left == nullptr){
                temp = temp->left->down;
            }
            else{
                temp = temp->left->left;
            }
        }
    }

    start = temp;
    start->visited = true;
    start->path = false;

    if(start == end){
        pos = rand() % pos;
        return createStart(pos);
    }

    return pos;
}

/*
Generates an ending position depending on the determined position.
@param pos An size_t which holds where the end will be
@returns the position end ended up with.
*/
size_t Maze::createEnd(size_t pos){
    node* temp = head->down;

    for(int counter = 0; counter != pos; counter++){
        if(temp->left == nullptr){
            if(temp->down->down == nullptr){
                temp = temp->down->right;
            }
            else{
                temp = temp->down->down;
            }
        }
        else if(temp->down == nullptr){
            if(temp->right->right == nullptr){
                temp = temp->right->up;
            }
            else{
                temp = temp->right->right;
            }
        }
        else if(temp->right == nullptr){
            if(temp->up->up == nullptr){
                temp = temp->up->left;
            }
            else{
                temp = temp->up->up;
            }
        }
        else if(temp->up == nullptr){
            if(temp->left->left == nullptr){
                temp = temp->left->down;
            }
            else{
                temp = temp->left->left;
            }
        }
    }
    
    end = temp;
    end->visited = true;
    end->path = false;

    if(end == start){
        pos = rand() % pos;
        return createEnd(pos);
    }

    return pos;
}

/*
Sets all the attributes of the maze depending on the input.
@param input A string which is being read to set attributes of the maze.
*/
void Maze::set(std::string input){
    static char wall;
    static size_t width;

    node* temp;
    node* n = nullptr;

    if(head == nullptr){
        wall = input[0];
        width = input.size();
    }
    else{
        for(temp = head; temp->down != nullptr; temp = temp->down){}
    }

    if(input.size() < 5 || input.size() % 2 == 0){
        throw std::out_of_range("The maze is too small or do not have odd lengths");
    }
    if(width != input.size()){
        throw std::out_of_range("The maze does not have rectangular shape");
    }

    for(char c : input){
        if(c != ' '){
            if(c == wall){
                if(n != nullptr){
                    node* r = new node(false, false);
                    n->right = r;
                    r->left = n;
                    n = r;
                }
                else{
                    if(head == nullptr){
                        head = new node(false, false);
                        n = head;
                        temp = head;
                    }
                    else{
                        n = new node(false, false);
                        temp->down = n;
                        n->up = temp;
                    }
                }
            }
            else if(c == 'S' || c == 's'){
                if(n != nullptr){
                    start = new node(true, false);
                    n->right = start;
                    start->left = n;
                    n = start;
                }
                else{
                    n = new node(true, false);
                    start = n;
                    temp->down = n;
                    n->up = temp;
                }
            }
            else if(c == 'E' || c == 'e'){
                if(n != nullptr){
                    end = new node(true, false);
                    n->right = end;
                    end->left = n;
                    n = end;
                }
                else{
                    n = new node(true, false);
                    end = n;
                    temp->down = n;
                    n->up = temp;
                }
            }
            else{
                throw std::out_of_range("Could not read walls");
            }
        }   
        else{
            if(n != nullptr){
                node* r = new node(true, false);
                n->right = r;
                r->left = n;
                n = r;
            }
            else{
                throw std::out_of_range("Could not read walls");
            }
        }
    }

    for(node* d = head; d->down != nullptr; d = d->down){
        for(node* r = d; r->right != nullptr; r = r->right){
            r->right->down = r->down->right;
            r->down->right->up = r->right;
        }
    }
}

/*
Gets the amount of steps needed for the solution.
Prints the solving progress on the screen for design.
@returns A size_t that holds the amount of steps it took to solve the maze.
*/
size_t Maze::getSolution(){
    node* temp = start;
    temp->path = true;
    
    std::vector<node*> solution, passages;
    solution.push_back(temp);
    passages.push_back(temp);

    size_t direction;
    srand(time(0));
    
    while(temp != end){
        
        direction = rand() % 4;
        if(temp != start && (isOldDirection(temp->right, passages) || temp->right->visited == 0)
        && (isOldDirection(temp->left, passages) || temp->left->visited == 0)
        && (isOldDirection(temp->up, passages) || temp->up->visited == 0)
        && (isOldDirection(temp->down, passages) || temp->down->visited == 0)){
            solution.pop_back();
            temp->path = false;
            temp = solution.back();

            system("clear");
            print();
            usleep(10000);

            if(temp == start){
                throw std::out_of_range("The maze could not be solved");
            }
        }
        else{
            switch(direction){
                case Right:
                    if(temp->right != nullptr && temp->right->visited == 1){
                        if(isOldDirection(temp->right, passages) == false){
                            temp = temp->right;
                            temp->path = true;

                            solution.push_back(temp);
                            passages.push_back(temp);

                            system("clear");
                            print();
                            usleep(10000);
                        }
                    }
                break;
                case Down:
                    if(temp->down != nullptr && temp->down->visited == 1){
                        if(isOldDirection(temp->down, passages) == false){
                            temp = temp->down;
                            temp->path = true;

                            solution.push_back(temp);
                            passages.push_back(temp);

                            system("clear");
                            print();
                            usleep(10000);
                        }
                    }
                break;
                case Up:
                    if(temp->up != nullptr && temp->up->visited == 1){
                        if(isOldDirection(temp->up, passages) == false){
                            temp = temp->up;
                            temp->path = true;

                            solution.push_back(temp);
                            passages.push_back(temp);

                            system("clear");
                            print();
                            usleep(10000);
                        }
                    }
                break;
                case Left:
                    if(temp->left != nullptr && temp->left->visited == 1){
                        if(isOldDirection(temp->left, passages) == false){
                            temp = temp->left;
                            temp->path = true;

                            solution.push_back(temp);
                            passages.push_back(temp);

                            system("clear");
                            print();
                            usleep(10000);
                        }
                    }
                break;
            }
        }
    }
    return solution.size() - 1;
}

/*
Checks if the current passage is a passage thats already been visited.
@param passage A node that holds the current passage adress.
@returns A boolean that returns false if the passage holds a new adress.
*/
bool Maze::isOldDirection(const node* passage, const std::vector<node*> oldpassages) const{
    for(node* p : oldpassages){
        if(p == passage){
            return true;
        }
    }
    return false;
}

/*
Starts a minigame for the user to solve the maze by pressing characters on the keyboard.
Prints the solving progress on the screen for design.
@returns A size_t that holds the amount of steps it took to solve the maze.
*/
size_t Maze::play(){
    char input;
    size_t steps = 0;
    
    node* player = startPassage();
    player->path = true;
    
    while(input != 'q'){
        system("clear");
        print();

        std::cout << "Use 'w' 'a' 's' 'd' to move your character" << std::endl;
        std::cout << "Press 'q' to quit" << std::endl;

        system("stty raw");
        input = getchar();
        std::cout << "[" << input << "]";
        system("stty cooked");

        if(input == 'w' || input == 'W'){
            if(player->up != nullptr && player->up->visited == true){
                player = player->up;
                player->path = true;
                player->down->path = false;
            }
        }
        else if(input == 'a' || input == 'A'){
            if(player->left != nullptr && player->left->visited == true){
                player = player->left;
                player->path = true;
                player->right->path = false;
            }
        }
        else if(input == 's' || input == 'S'){
            if(player->down != nullptr && player->down->visited == true){
                player = player->down;
                player->path = true;
                player->up->path = false;
            }
        }
        else if(input == 'd' || input == 'D'){
            if(player->right != nullptr && player->right->visited == true){
                player = player->right;
                player->path = true;
                player->left->path = false;
            }
        }
        if(player == end){
            system("clear");
            print();
            std::cout << "Winner winner chicken dinner!" << std::endl;
            break;
        }
        steps++;
    }
    return steps;
}

/*
    Destroys the maze by cleaning up all the nodes to prevent memory leak and dangling pointers
*/
void Maze::destroy(){
    if(head != nullptr){
        node* d;
        for(node* r = head; r->right != nullptr; r = r->right){
            head = r->right;
            for(d = r; d->down != nullptr;){
                node* temp = d;
                d = d->down;
                d->up = nullptr;
                d->right->left = nullptr;

                delete temp;
                temp = nullptr;
            }
            if(d != nullptr){
                d->right->left = nullptr;
                delete d;
                d = nullptr;
            }
        }
        if(head != nullptr){
            for(d = head; d->down != nullptr;){
                node* temp = d;
                d = d->down;
                d->up = nullptr;

                delete temp;
                temp = nullptr;
            }
            if(d != nullptr){
                delete d;
                d = nullptr;
            }
        }

        head = nullptr;
        start = nullptr;
        end = nullptr;
    }
}

/*
    Prints all the different kinds of attributes (walls, passages, solution etc.)
    that are hold in the maze.
*/
void Maze::print() const{
    std::cout << std::endl;
    if(head != nullptr){
        for(node* temp = head; temp != nullptr; temp = temp->down){
            for(node* r = temp; r != nullptr; r = r->right){
                if(!(r->visited)){
                    std::cout << "██" << std::flush;
                }
                else{
                    if(r == start){
                        std::cout << "S " << std::flush;
                    }
                    else if(r == end){
                        std::cout << "E " << std::flush;
                    }
                    else{
                        if(r->path){
                            std::cout << ". " << std::flush;
                        }
                        else{
                            std::cout << "  " << std::flush;
                        }
                    }
                }
            }
            std::cout << std::endl;
        }
    }
}