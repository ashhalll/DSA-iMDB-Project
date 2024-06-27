#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <queue>
#include <conio.h>
#include <windows.h>
using namespace std;

class Node
{


public:
    string userid;
    string name;
    string id;
    string rating;
    string genre;
    string year;
    string dur;
    string imdb;
    string d_name;
    string description;
    int height;
    Node *left;
    Node *right;

    Node()
    {
        userid = "";
        name = "";
        id = "";
        rating = "";
        genre = "";
        year = "";
        dur = "";
        imdb = "";
        d_name = "";
        description = "";
        height = 0;
        left = NULL;
        right = NULL;
    }

    Node(string a, string b, string c, string d, string e, string f, string g, string h, string i, string j)
    {
        userid = a;
        name = b;
        id = c;
        rating = d;
        genre = e;
        year = f;
        dur = g;
        imdb = h;
        d_name = i;
        description = j;
        left = right = 0;
    }
};

void display(Node *temp);

int stringtoint(string data)
{
    int value = 0;
    int length = data.length();
    for (int i = 0; i < length; i++)
    {
        value = value * 10 + (data[i] - '0');
    }
    return value;
}

int hashfunc(string data, int size)
{
    int n = stringtoint(data);
    return n % size;
}
template <typename T>
class BSTNode
{
public:
    T data;
    BSTNode<T> *right;
    BSTNode<T> *left;

    BSTNode(T value) : data(value), left(nullptr), right(nullptr) {}
};

class User;

template <typename T>
class List
{
public:
    BSTNode<T> *head;
    BSTNode<T> *tail; // For convenience in some operations

    List() : head(nullptr), tail(nullptr) {}

    // Function to add a node to the end of the list
    void pushBack(T value)
    {
        BSTNode<T> *newNode = new BSTNode<T>(value);
        if (!head)
        {
            // List is empty
            head = tail = newNode;
        }
        else
        {
            // List is not empty
            newNode->left = tail;
            tail->right = newNode;
            tail = newNode;
        }
    }

    // Function to print the elements of the list
    void printList() const
    {
        if (!head)
        {
            cout << "Your list is Empty. Please add movies to the list\n";
            cout << "press any key to continue\n";
            getch();
            system("CLS");
        }

        BSTNode<T> *current = head;
        while (current)
        {
            system("CLS");
            display(current->data);
            _getch();
            current = current->right;
        }
        cout << endl;
    }

    void printList(vector<pair<string, string>> &v, bool &signedIn) const
    {
        BSTNode<Node *> *current = head;
        while (current)
        {
            system("CLS");
            display(current->data);
            if (signedIn)
            {
                int index = stringtoint(current->data->id);
                cout << "\t\t\tYour Rating: " << v[index].first << endl;
                cout << "\t\t\tYour Review: " << v[index].second << endl;
                cout << endl;
            }
            _getch();
            current = current->right;
        }
        cout << endl;
    }

    // Function to delete a node from the list
    void deleteNode(T value)
    {
        BSTNode<T> *current = head;
        BSTNode<T> *prev = nullptr;
        while (current)
        {
            if (current->data == value)
            {
                if (prev)
                {
                    prev->right = current->right;
                }
                else
                {
                    head = current->right;
                }
                if (current == tail)
                {
                    tail = prev;
                }
                delete current;
                break;
            }
            prev = current;
            current = current->right;
        }
    }

    // Destructor to free the memory used by the list
    ~List()
    {
        BSTNode<T> *current = head;
        BSTNode<T> *nextNode;
        while (current)
        {
            nextNode = current->right;
            delete current;
            current = nextNode;
        }
        head = tail = nullptr;
    }
};

struct CompareMovies
{
    bool operator()(const Node *a, const Node *b)
    {
        // Compare based on movie ID
        return a->rating > b->rating; // Using '>' to prioritize based on ID in ascending order
    }
};

class HashTable
{
private:
    BSTNode<Node *> **arr;
    int size;
    bool isEmpty;

public:
    HashTable(int size)
    {
        isEmpty = true;
        this->size = size;
        arr = new BSTNode<Node *> *[size];
        for (int i = 0; i < size; i++)
        {
            arr[i] = nullptr;
        }
    }

    void insert(string u, string name, string id, string rating, string genre, string year, string dur, string imdb, string d_name, string description)
    {
        isEmpty = false;
        Node *temp = new Node(u, name, id, rating, genre, year, dur, imdb, d_name, description);
        int index = hashfunc(id, size);
        BSTNode<Node *> *curr = arr[index];

        if (arr[index] == nullptr)
        {
            arr[index] = new BSTNode<Node *>(temp);
        }
        else
        {
            curr->right = arr[index];
            arr[index] = new BSTNode<Node *>(temp);
        }
    }

    void insert(Node *root)
    {
        insert(root->userid, root->name, root->id, root->rating, root->genre, root->year, root->dur, root->imdb, root->d_name, root->description);
    }

    bool searchTable(const string movieID)
    {
        int index = hashfunc(movieID, size);

        BSTNode<Node *> *curr = arr[index];
        while (curr)
        {
            if (curr->data->id == movieID)
            {
                return true;
            }
            curr = curr->right;
        }
        return false;
    }

    void print(List<Node *> &watchList)
    {
        // Create a priority queue to store the movies based on ID
        priority_queue<Node *, vector<Node *>, CompareMovies> movieQueue;

        // Iterate through the hash table and insert movies into the priority queue
        for (int i = 0; i < size; ++i)
        {
            BSTNode<Node *> *curr = arr[i];
            while (curr)
            {
                movieQueue.push(curr->data);
                curr = curr->left;
            }
        }

        // Print the movies from the priority queue
        cout << "Recommended Movies:" << endl;
        while (!movieQueue.empty())
        {
            system("CLS");
            cout << "Recommended Movies:" << endl;
            Node *movie = movieQueue.top();
            movieQueue.pop();
            display(movie);
            // cout << "Movie ID: " << movie->id << ", Title: " << movie->name << ", Genre: " << movie->genre << endl;
            cout << "+: Add to watchlist\t";
            cout << "n: next movies\n";
            cout << "e: exit\n";
            char choice;
        op:
            choice = _getch();
            if (choice == '+')
            {
                BSTNode<Node *> *curr = watchList.head;
                while (curr)
                {
                    if (curr->data->name == movie->name)
                    {
                        cout << "Already in the list\n";
                        goto op;
                    }
                    curr = curr->right;
                }
                cout << "Adding movie to watchlist. Please wait..." << flush;
                Sleep(1000);
                watchList.pushBack(movie);
                cout << "\n\nAdded to watchlist" << endl;
                Sleep(1000);
                goto op;
            }
            else if (choice == 'n')
            {
                continue;
            }
            else if (choice == 'e')
            {
                return;
            }
            else
            {
                int ch;
                ch = _getch();
                while (ch != 'n' && ch != '+')
                {
                    ch = _getch();
                }
            }
        }
    }
};
class Movies
{

    Node *root;

public:
    Movies()
    {
        root = 0;
    }

    Node *getRoot()
    {
        return root;
    }
    // Inserting in AVL like BST

    // Returns the height of the tree
    int heightFinder(Node *n)
    {
        if (n == 0)
            return 0;

        return n->height;
    }

    // Returns max of two heights given
    int get_max(int a, int b)
    {
        return (a > b) ? a : b;
    }

    // Checks whether left-sub tree is heavier or right one
    int checkBalance(Node *r)
    {
        if (r == 0)
        {
            return 0;
        }
        else
        {
            return (heightFinder(r->left) - heightFinder(r->right));
        }
    }

    // Insertion starts from here
    void insert(string a, string b, string c, string d, string e, string f, string g, string h, string i, string j)
    {
        //	cout << "d";
        root = insert(a, b, c, d, e, f, g, h, i, j, root);
    }

    Node *insert(string a, string b, string c, string d, string e, string f, string g, string h, string i, string j, Node *r)
    {

        if (r == 0)
        {
            r = new Node(a, b, c, d, e, f, g, h, i, j);
            r->height = 1;
        }
        else if (c < r->id)
        {
            r->left = insert(a, b, c, d, e, f, g, h, i, j, r->left);
        }
        else if (c > r->id)
        {
            r->right = insert(a, b, c, d, e, f, g, h, i, j, r->right);
        }
        else
        {
            return r;
        }

        r->height = 1 + get_max(heightFinder(r->left), heightFinder(r->right)); // updating height of ancestor

        int balancing_Factor = checkBalance(r); // checking balancing factor

        // Right rotation to be performed as nodes alligned to the left
        if (balancing_Factor > 1 && c < r->left->id)
        {
            return RightRotation(r);
        }

        // Left rotation to be performed as nodes alligned to the right
        if (balancing_Factor < -1 && c > r->right->id)
        {
            return LeftRotation(r);
        }

        // LR ROTATION
        if (balancing_Factor > 1 && c > r->left->id)
        {
            r->left = LeftRotation(r->left);
            return RightRotation(r);
        }

        //  RL ROTATION
        if (balancing_Factor < -1 && c < r->right->id)
        {
            r->right = RightRotation(r->right);
            return LeftRotation(r);
        }

        return r;
    }

    Node *RightRotation(Node *y)
    {
        Node *x = y->left;
        Node *z = x->right;

        x->right = y;
        y->left = z;

        y->height = get_max(heightFinder(y->left), heightFinder(y->right)) + 1;
        x->height = get_max(heightFinder(x->left), heightFinder(x->right)) + 1;

        return x;
    }

    Node *LeftRotation(Node *x)
    {
        Node *y = x->right;
        Node *z = y->left;

        y->left = x;
        x->right = z;

        x->height = get_max(heightFinder(x->left), heightFinder(x->right)) + 1;
        y->height = get_max(heightFinder(y->left), heightFinder(y->right)) + 1;

        return y;
    }

    // Returns the min left node
    Node *get_min(Node *r)
    {
        if (r == 0)
        {
            return 0;
        }
        else if (r->left == 0)
        {
            return r;
        }
        else
        {
            return get_min(r->left);
        }
    }


    // search by the name of the movie
    bool containsSubstring(const string s1, const string s2)
    {
        string lowerS1 = s1;
        string lowerS2 = s2;

        transform(lowerS1.begin(), lowerS1.end(), lowerS1.begin(), [](unsigned char c)
                  { return tolower(c); });
        transform(lowerS2.begin(), lowerS2.end(), lowerS2.begin(), [](unsigned char c)
                  { return tolower(c); });

        return lowerS1.find(lowerS2) != string::npos;
    }

    Node *searchName(Node *root, string substring, queue<Node *> &q)
    {
        if (!root)
        {
            return nullptr;
        }
        if (containsSubstring(root->name, substring))
        {
            q.push(root);
        }
        root->left = searchName(root->left, substring, q);
        root->right = searchName(root->right, substring, q);
        return root;
    }

    void searchName(string n, queue<Node *> &q)
    {
        root = searchName(root, n, q);
    }


    // This function is linked with top Ratings. It displays the movies which have 5 star rating in InOrder treversal.
    void ratingInorder(Node *r, queue<Node *> &q)
    {
        if (r == 0)
        {
            return;
        }

        if (r->rating == "5" || r->rating == "4.5")
        {
            q.push(r);
        }
        ratingInorder(r->left, q);
        ratingInorder(r->right, q);
    }

    // This function displays all the movies which have a 5 star rating
    void searchTopRatedMovies(queue<Node *> &q)
    {
        cout << "\n\n\t\t\t\t\tTop rated Movies:  " << endl;
        _getch();
        ratingInorder(root, q);
        getchar();
        cout << endl
             << endl;
    }

    // This function displays all the movies w.r.t the rating which the user provided in InOrder treversal.
    void preferedInorder(string a, Node *r, queue<Node *> &q)
    {
        if (r == 0)
        {
            return;
        }

        if (r->rating == a)
        {
            q.push(r);
        }
        preferedInorder(a, r->left, q);
        preferedInorder(a, r->right, q);
    }

    // This function displays all the movies w.r.t the rating which the user provided.
    void searchByRating(string a, queue<Node *> &q)
    {
        cout << "\n\n\t\t\t\t\tUser preferred rating movies: " << endl;
        _getch();
        preferedInorder(a, root, q);
        getchar();
        cout << endl
             << endl;
    }

    // search by the Genre of the movie
    void searchByGenre(string val, queue<Node *> &q)
    {
        ByGenre(val, root, q);
        getchar();
        system("CLS");
        cout << endl
             << endl;
    }

    // search by the Genre of the movie and displays in InOrder Treversal.
    void ByGenre(string a, Node *r, queue<Node *> &q)
    {
        if (r == 0)
        {
            return;
        }

        if (containsSubstring(r->genre, a))
        {
            q.push(r);
        }
        ByGenre(a, r->left, q);
        ByGenre(a, r->right, q);
    }

    // search by the Date of release of the movie
    void searchByDate(string d, queue<Node *> &q)
    {
        cout << "\n\n\t\t\t\t\tMovies released on date " << d << " are: \n\n";
        inorderByDate(d, root, q);
        getchar();
        system("CLS");
        cout << endl
             << endl;
    }

    // This function displays all the movies w.r.t the date of release which the user provided.
    void inorderByDate(string a, Node *r, queue<Node *> &q)
    {
        if (r == 0)
        {
            return;
        }

        if (containsSubstring(r->year, a))
        {
            q.push(r);
        }
        inorderByDate(a, r->left, q);
        inorderByDate(a, r->right, q);
    }

    void inorder(Node *r, queue<Node *> &q)
    {
        if (r == 0)
        {
            return;
        }

        inorder(r->left, q);
        cout << "rn: " << root->name << endl;
        q.push(r);

        inorder(r->right, q);
    }

    Node *traverse(Node *root, HashTable &recTable, List<string> &favGenres, List<Node *> &likedMovies)
    {
        if (root == nullptr)
        {
            return root;
        }

        if (favGenres.head)
        {
            BSTNode<string> *currGenre = favGenres.head;
            while (currGenre != nullptr)
            {
                int idHashGenre = stringtoint(root->id); // Use movie ID for hashing
                if (containsSubstring(root->genre, currGenre->data) && !recTable.searchTable(root->id))
                {
                    recTable.insert(root);
                }
                currGenre = currGenre->right;
            }
        }

        if (!favGenres.head)
        {
            int idHash = stringtoint(root->id);
            if (!recTable.searchTable(root->id) && (root->rating == "5" || root->rating == "4.5" || root->rating == "4"))
            {
                recTable.insert(root);
            }
        }

        // Check for favorite movies
        if (likedMovies.head)
        {
            BSTNode<Node *> *currMovie = likedMovies.head;
            while (currMovie != nullptr)
            {
                int idHashMovie = stringtoint(currMovie->data->id); // Use movie ID for hashing
                if (root->id == currMovie->data->id && !recTable.searchTable(root->id))
                {
                    recTable.insert(root);
                }
                currMovie = currMovie->right;
            }
        }

        root->left = traverse(root->left, recTable, favGenres, likedMovies);
        root->right = traverse(root->right, recTable, favGenres, likedMovies);

        return root;
    }

    void traverse(HashTable &recTable, List<string> &favGenres, List<Node *> &likedMovies)
    {
        root = traverse(root, recTable, favGenres, likedMovies);
    }
};

void print(BSTNode<Node *> *temp)
{
    cout << "\n\n\t\t\t\t\tMovie Details: \n\n";
    // cout << "\t\t\t\t\tID: " << temp->data->id << endl;
    cout << "\t\t\t\t\tMOVIE NAME: " << temp->data->name << endl;
    cout << "\t\t\t\t\tMOVIE RATING: " << temp->data->rating << endl;
    cout << "\t\t\t\t\tMOVIE GENRE: " << temp->data->genre << endl;
    cout << "\t\t\t\t\tDate: " << temp->data->year << endl;
    cout << "\t\t\t\t\tDuration: " << temp->data->dur << endl;
    cout << "\t\t\t\t\tIMDB: " << temp->data->imdb << endl;
    cout << "\t\t\t\t\tDirector Name:" << temp->data->d_name << endl;
    cout << "\n\t\t\t\t\t\t:Overview:\n\n"
         << temp->data->description << endl
         << endl;
}

void display(Node *temp)
{
    cout << "\n\n\t\t\t\t\tMovie Details: \n\n";
    //cout << "\t\t\t\t\tID: " << temp->id << endl;
    cout << "\t\t\t\t\tMOVIE NAME: " << temp->name << endl;
    cout << "\t\t\t\t\tMOVIE RATING: " << temp->rating << endl;
    cout << "\t\t\t\t\tMOVIE GENRE: " << temp->genre << endl;
    cout << "\t\t\t\t\tDate: " << temp->year << endl;
    cout << "\t\t\t\t\tDuration: " << temp->dur << endl;
    cout << "\t\t\t\t\tIMDB: " << temp->imdb << endl;
    cout << "\t\t\t\t\tDirector Name:" << temp->d_name << endl;
    cout << "\n\t\t\t\t\t\t:Overview:\n\n"
         << temp->description << endl
         << endl;
}

// USER CLASS
class User
{
public:
    int userID;
    string name;
    List<Node *> likedMovies;
    List<Node *> MoviesWatced;
    List<Node *> watchlist;
    List<string> likedGenre;
    bool signedin = false;
    vector<pair<string, string>> userResponse;

    User() : userResponse(30, {"", ""}) {}
    Movies data;

public:
    User(int id, const string &userName, Movies &data) : userID(id), name(userName), userResponse(30, {"", ""})
    {
        this->data=data;
    }

    void addLikedGenre(const string &genre)
    {
        BSTNode<string> *curr = likedGenre.head;
        while (curr)
        {
            if (curr->data == genre)
            {
                cout << "Already in the list\n";
                return;
            }
            curr = curr->right;
        }
        cout<<"Added "<<genre<<" to the list!"<<endl;
        likedGenre.pushBack(genre);
    }

    void singin()
    {
        signedin=true;
    }


    void recommendMovies()
    {
        HashTable t(20);
        data.traverse(t, likedGenre, likedMovies);
        t.print(watchlist);
    }

    void addMoviesWatched(Node *root)
    {
        MoviesWatced.pushBack(root);
    }
    void displayWatchlist()
    {
        watchlist.printList();
        cout << endl;
        cout << "Press any key to continue.";
        _getch();
    }

    void displayFavourites()
    {
        likedMovies.printList();
        cout << "press any key to continue.\n";
        _getch();
        system("CLS");
    }
    void displayMoviesWatched()
    {
        MoviesWatced.printList(userResponse, signedin);
        cout << "press any key to continue.\n";
        _getch();
        system("CLS");
    }


    void addRating(const string &i)
    {
        int index = stringtoint(i);
        if (index >= 0 && index < userResponse.size())
        {
            string userRating;
            string userReview;

            cout << "Enter your rating for the movie (e.g., 4.5): ";
            cin >> userRating;

            cout << "Enter your review for the movie: ";
            cin.ignore();
            getline(cin, userReview);

            userResponse.at(index).first = userRating;
            userResponse.at(index).second = userReview;

            cout << "Rating and review added successfully!" << endl;
        }
        else
        {
            cout << "Invalid movie index." << endl;
        }
    }

    // Function to search for movies based on different criteria
    void searchMovies()
    {
        int choice = 0;
        while (choice != 7)
        {
        here:
            system("CLS");
            queue<Node *> q;
            cout << "\n\t\tSearch Options:" << endl;
            cout << "\t\t1. Display all movies" << endl;
            cout << "\t\t2. Search by name" << endl;
            cout << "\t\t3. Search by genre" << endl;
            cout << "\t\t4. Search by release date" << endl;
            cout << "\t\t5. Search by preferred rating" << endl;
            cout << "\t\t6. Search top-rated movies" << endl;
            cout << "\t\t7. Exit" << endl;
            cout << "\nEnter your choice (1-7): ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                // Display all movies
                data.inorder(data.getRoot(), q);
                q.pop();
                break;

            case 2:
            {
                // Search by name
                string name;
                cout << "Enter the name to search: ";
                cin >> name;
                data.searchName(name, q);
                break;
            }

            case 3:
            {
                // Search by genre
                string genre;
                cout << "Enter the genre to search: ";
                cin >> genre;
                data.searchByGenre(genre, q);
                break;
            }

            case 4:
            {
                // Search by release date
                string releaseDate;
                cout << "Enter the release date/year to search: ";
                cin >> releaseDate;
                data.searchByDate(releaseDate, q);

                break;
            }

            case 5:
            {
                // Search by preferred rating
                string rating;
                cout << "Enter the preferred rating to search: ";
                cin >> rating;
                data.searchByRating(rating, q);
                break;
            }

            case 6:
                // Search top-rated movies
                data.searchTopRatedMovies(q);
                break;

            case 7:
                cout << "\n\t\tExiting browse menu\n";
                cout << "\n\t\tPress any key to continue." << endl;
                _getch();
                system("CLS");
                return;
            default:
                cout << "\n\t\tInvalid choice!" << endl;
                cout << "\n\t\tPress any key to continue." << endl;
                _getch();
                continue;
            }
            system("CLS");
            if (q.empty())
            {
                cout << "\n\t\tNo results found!" << endl;
                cout << "\n\t\tPress any key to continue" << endl;
                _getch();
            }
            while (!q.empty())
            {
                Node *r = q.front();
                q.pop();
                system("CLS");

                display(r);

                cout << "+: Add to watchlist\t";
                cout << "m: Movies watched\t";
                cout << "l: Liked movies\t\t";
                cout << "g: Genre liked\t\t";
                cout << "n: next movie\t";
                cout << "e: exit\n";
            options:
                cout << "Enter option\n ";
                char c;
                c = _getch();
                if (c == '+')
                {
                    BSTNode<Node *> *curr = watchlist.head;
                    while (curr)
                    {
                        if (curr->data->name == r->name)
                        {
                            cout << "Already in the list\n";
                            goto options;
                        }
                        curr = curr->right;
                    }

                    cout << "Adding movie to watchlist. Please wait..." << flush;
                    Sleep(2000);

                    watchlist.pushBack(r);
                    cout << "\n\nAdded to watchlist" << endl;
                    Sleep(2000);
                    // watchlist.printList();
                    goto options;
                }
                else if (c == 'l')
                {
                    BSTNode<Node *> *curr = likedMovies.head;
                    while (curr)
                    {
                        if (curr->data->name == r->name)
                        {
                            cout << "Already in the list\n";
                            goto options;
                        }
                        curr = curr->right;
                    }
                    cout << "Adding movie to Favourites . Please wait..." << flush;
                    Sleep(2000);
                    likedMovies.pushBack(r);
                    cout << "Movie added to liked list!\n"
                         << endl;
                    Sleep(2000);
                    // likedMovies.printList();
                    goto options;
                }
                else if (c == 'g')
                {
                    likedGenre.pushBack(r->genre);
                    cout << "Genre added to liked genres!\n"
                         << endl;
                    goto options;
                }
                else if (c == 'm')
                {
                    BSTNode<Node *> *curr = MoviesWatced.head;
                    while (curr)
                    {
                        if (curr->data->name == r->name)
                        {
                            cout << "Already in the list\n";
                            goto options;
                        }
                        curr = curr->right;
                    }
                    cout << "Adding movie to Watched-Movies . Please wait..." << flush;
                    Sleep(700);
                    MoviesWatced.pushBack(r);
                    cout << "Movie added to the list!\n"
                         << endl;
                    cout << "Want to rate and leave a review for the movie? (y/n)" << endl;
                    c = _getch();
                    if (signedin)
                    {
                        if (c == 'y' || c == 'Y')
                        {
                            addRating(r->id);
                            cout << "Thank you for your review!" << endl;
                            // MoviesWatced.printList(userResponse);
                            _getch();
                        }
                    }
                    else
                    {
                        cout << "Please sign in to get the oppurtunity to rate and review the movies :)" << endl;
                        cout << "press any key to continue\n";
                        getch();
                    }
                    goto options;
                }
                else if (c == 'n')
                {
                    continue;
                }
                else if (c == 'e')
                {
                    goto here;
                }
                else
                {
                    cout << "invalid key pressed. ";
                    goto options;
                }
            }
        }
    }
};
void changecolor(int foreground, int background)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, foreground + background * 16);
}
void text_animation(string a)
{ // For the animation at the starting of application

    int i;
    long double j;
    changecolor(12,0);
    for (i = 0; a[i] != '\0'; i++)
    {

        for (j = 0; j < 10000000; j++)
            ;
        cout << a[i];
    }
    changecolor(15,0);
}


void intro()
{
    changecolor(15, 0);
    cout << "\n";
    cout << "\t\t\t\t***************************************************************************" << endl;
   
    text_animation("\n\t\t\t\t\t\tWELCOME TO OUR IMDb MOVIE RECOMMENDER SYSTEM\n");
    
    cout << "\n\t\t\t\t***************************************************************************" << endl;
   
    text_animation("\t\nMade By:\n\tShuja, Omer & Ashhal\n");
   
    cout << "________________________________________________________\n"
         << endl;
         changecolor(12,0);
    cout << "PRESS ANY KEY TO EXPLORE THE MENU: ";
    changecolor(15,0)
    ;_getch();
    system("CLS");
}

void displayMainMenu()
{
    system("CLS");
    cout << "\t\t\t\t\t  ====================================================";
    cout << endl;
changecolor(12,0);
    cout     << "\n\t\t\t\t\t\t\t\t MENU " << endl
         << endl;
          changecolor(15,0);
    cout << "\t\t\t\t\t  ====================================================";
    cout << endl
         << endl
         << endl
         << endl;
    cout << "   =================================          =================================          =================================     \n";
    changecolor(12,0);
            cout<<"   *(1) Search Movie with Preference          *(2)    Show Recommendations               *(3)     View Your Watchlist          \n"
            ;
            changecolor(15,0);
            cout<<"   =================================          =================================          =================================     \n\n"
            ;
            cout<<"   =================================          =================================          =================================     \n"
            ;
            changecolor(12,0);
            cout<<"   *(4)       View Favorites                  *(5)    View Movies Watched                *(6)          Exit                    \n"
             ;changecolor(15,0);
            ;cout<<"   =================================          =================================          =================================     \n"

         << endl;
}

void gettingStarted(User &u1)
{
    system("CLS");
    cout << "\n***GOOD TO SEE YOU HERE***"<<endl;
    cout << "-> Add genres in your favourite list so we can recommend you better\n"<<endl;;
    cout << "\t\t1) COMEDY\t2) Sci-Fi\t3) Horror\t4) Thriller\n";
    cout << "\n\t\t5)Noir\t\t6) Romantic\t7) Drama\t8) Fiction\n";
    cout << "\n9) Done adding genres\ns) Skip\n"<<endl;

    char choice;
    do
    {
        cout << " Enter your choice: ";
        choice = _getch(); 
        cout << choice << endl;

        switch (choice)
        {
        case '1':
            u1.addLikedGenre("Comedy");
            break;
        case '2':
            u1.addLikedGenre("Sci-Fi");
            break;
        case '3':
            u1.addLikedGenre("Horror");
            break;
        case '4':
            u1.addLikedGenre("Thriller");
            break;
        case '5':
            u1.addLikedGenre("Noir");
            break;
        case '6':
            u1.addLikedGenre("Romantic");
            break;
        case '7':
            u1.addLikedGenre("Drama");
            break;
        case '8':
            u1.addLikedGenre("Fiction");
            break;
        case '9':
            cout << "\tGenres added. Exiting...\nPress key to continue\n";
            getch();
            break;
        case 's':
            cout << "Okay\nPress key to continue\n";
            getch();
            break;
        default:
            cout << "\tInvalid choice. Please enter a number from 1 to 9.\n";
        }

    } while (choice != '9' && choice != 's');
    return;
}

void mainpage(Movies &d)
{
    int choice;
     string username, password;
        User u1(50, "shuja", d);
        changecolor(0,6);
        cout << "IMDb";
        changecolor(15, 0);
        cout << endl
             << "\n\n\t\t\t\t\t\t(1)Login" << endl
             << "\n\n\t\t\t\t\t\t(2)Visit as guest" << endl;
         choice = _getch();
        if (choice == '1')
        {
            retr:
            cout << "\n\t\t\t\t\tEnter username: ";
            cin >> username;

            cout << "\t\t\t\t\tEnter password: ";
            cin >> password;
            
            if (!username.empty())
            {
                string storedPassword = "admin"; 
                if (password == storedPassword)
                {
                    cout << "\t\t\t\t\tLogin successful!\n";
                    u1.singin();
                    system("CLS");
                    // return;
                }
                else
                {
                    cout << "\t\t\t\t\tIncorrect password. Login failed.\n";
                    goto retr;
                }
            }
        }
        else
        {
            cout<<"\t\t\t\t\tVisiting as Guest!"<<endl;
            system("CLS");
            // return;
        }
        system("cls");
        gettingStarted(u1);
        
    choice=0;
    do
    {
        // Display the main menu
        displayMainMenu();

        // Get user input
        cout << "\nPlease Select your desired choice: ";
        cin >> choice;

        // Process user choice
        switch (choice)
        {
        case 1:
            u1.searchMovies();
            break;
        case 2:
            u1.recommendMovies();
            break;
        case 3:
            u1.displayWatchlist();
            break;
        case 4:
            u1.displayFavourites();
            break;
        case 5:
            u1.displayMoviesWatched();
            break;
        case 6:
            cout << "Exiting program. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
            cout << " Press any key to continue" << endl;
            _getch();
            system("CLS");
        }

    } while (choice != 6);
}

void menu()
{
    Movies data;
    int c = 0, opt = 0;

    ifstream ip("movies_dataset.csv");
    ifstream ic("data1.csv");
    ifstream iz("overview.csv");

    if (!ip.is_open() || !ic.is_open() || !iz.is_open())
    {
        cout << "\n\n\t\t\terror file open!" << endl;
    }

    string id, userid, name, rating, genres, date, dur, imdb, d_name, overview, rate;

    // Reading data from the CSV file and inserting in AVL BST.
    while (ip.good() || ic.good() || iz.good())
    {
        c++;
        getline(ip, id, ',');
        getline(ip, userid, ',');
        getline(ip, name, ',');
        getline(ip, rating, ',');
        getline(ip, genres, '\n');
        getline(ic, date, ',');
        getline(ic, dur, ',');
        getline(ic, imdb, ',');
        getline(ic, d_name, ',');
        getline(ic, rate, '\n');
        getline(iz, overview, '\n');

        data.insert(userid, name, id, rate, genres, date, dur, imdb, d_name, overview);

        if (c == 30)
        {
            break;
        }
    }

    ip.close();
    ic.close();
    iz.close();

    mainpage(data);
}
void loginMenu(User &user){
        string username, password;
        changecolor(0, 6);
        cout << "IMDb";
        changecolor(15, 0);
        cout << endl
             << "\n\n\t\t\t\t\t\t(1)Login" << endl
             << "\n\n\t\t\t\t\t\t(2)Visit as guest" << endl;
        int choice = _getch();
        if (choice == '1')
        {
            retr:
            cout << "\n\t\t\t\t\tEnter username: ";
            cin >> username;

            cout << "\t\t\t\t\tEnter password: ";
            cin >> password;

            if (!username.empty())
            {
                string storedPassword = "admin123"; 
                if (password == storedPassword)
                {
                    cout << "\t\t\t\t\tLogin successful!\n";
                    system("CLS");
                    return;
                }
                else
                {
                    cout << "\t\t\t\t\tIncorrect password. Login failed.\n";
                    goto retr;
                }
            }
        }
        else if (choice == '2')
        {
            cout<<"\t\t\t\t\tVisiting as Guest!"<<endl;
            system("CLS");
            return;
        }
        system("cls");
}

int main()
{
    intro();
    menu();
}