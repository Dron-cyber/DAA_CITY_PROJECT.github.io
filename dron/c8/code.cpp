/*
    NOTE:
    The book dataset used in this program consists of sample records created 
    specifically for demonstrating Trie-based prefix matching and optional 
    binary search operations. These entries represent a realistic but limited 
    example of library data.

    The system is fully scalable — more books, categories, branches, or 
    metadata fields can be added easily without modifying the underlying 
    Trie logic or search functionality.
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm> // For binary search if needed

using namespace std;

// Struct to represent a Book
struct Book {
    string id;
    string branch;
    string title;
    string category;
    string author;
    int year;
    double rating;

    // Constructor for easy initialization
    Book(string i, string b, string t, string c, string a, int y, double r)
        : id(i), branch(b), title(t), category(c), author(a), year(y), rating(r) {}
};

// Trie Node class
class TrieNode {
public:
    map<char, TrieNode*> children;
    bool isEnd;
    Book* book;

    TrieNode() : isEnd(false), book(nullptr) {}
};

// Trie class for prefix matching on book titles
class Trie {
private:
    TrieNode* root;

    // Helper function to collect all books in the subtree
    void collectAll(TrieNode* node, vector<Book*>& results) {
        if (node->isEnd && node->book) {
            results.push_back(node->book);
        }
        for (auto& pair : node->children) {
            collectAll(pair.second, results);
        }
    }

public:
    Trie() {
        root = new TrieNode();
    }

    // Insert a book into the Trie based on its title
    void insert(Book* b) {
        TrieNode* node = root;
        for (char c : b->title) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
        node->book = b;
    }

    // Search for books with titles starting with the given prefix
    vector<Book*> searchPrefix(string prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                return {}; // Prefix not found
            }
            node = node->children[c];
        }
        // Collect all books in the subtree
        vector<Book*> results;
        collectAll(node, results);
        return results;
    }
};

// Function to print book details
void printBook(const Book* b) {
    cout << "BookID: " << b->id << endl;
    cout << "Library Branch: " << b->branch << endl;
    cout << "Book Title: " << b->title << endl;
    cout << "Category: " << b->category << endl;
    cout << "Author: " << b->author << endl;
    cout << "Year: " << b->year << endl;
    cout << "Rating: " << b->rating << endl;
    cout << "------------------------" << endl;
}

int main() {
    // Dataset of books (hardcoded as per the problem)
    vector<Book> books = {
        {"B001", "University Library", "Data Structures and Algorithms", "Computer Science", "Mark Weiss", 2019, 4.6},
        {"B002", "University Library", "Database System Concepts", "Computer Science", "Silberschatz", 2020, 4.7},
        {"B003", "University Library", "Digital Logic Design", "Electronics", "Morris Mano", 2017, 4.5},
        {"B004", "School Zone Library", "The Adventures of Tom Sawyer", "Fiction", "Mark Twain", 2003, 4.3},
        {"B005", "School Zone Library", "The Jungle Book", "Children", "Rudyard Kipling", 2001, 4.4},
        {"B006", "School Zone Library", "Introduction to Science Basics", "Education", "Laura King", 2015, 4.1},
        {"B007", "Market Public Library", "History of Indian Culture", "History", "R.S. Sharma", 2010, 4.2},
        {"B008", "Market Public Library", "World Atlas Guide", "Geography", "David Smith", 2018, 4.5},
        {"B009", "Market Public Library", "Quick Cooking for Busy People", "Lifestyle", "Sonia Rao", 2021, 4.0},
        {"B010", "Residential Zone A Library", "The Psychology of Money", "Finance", "Morgan Housel", 2020, 4.7},
        {"B011", "Residential Zone A Library", "Atomic Habits", "Self-Help", "James Clear", 2018, 4.8},
        {"B012", "Residential Zone B Library", "Rich Dad Poor Dad", "Finance", "Robert Kiyosaki", 2017, 4.6},
        {"B013", "Residential Zone B Library", "How to Win Friends and Influence People", "Self-Help", "Dale Carnegie", 2009, 4.6},
        {"B014", "IT Park Reading Hall", "Clean Code", "Software Engineering", "Robert C. Martin", 2008, 4.8},
        {"B015", "IT Park Reading Hall", "You Don't Know JS", "JavaScript", "Kyle Simpson", 2017, 4.7},
        {"B016", "IT Park Reading Hall", "Introduction to Machine Learning", "AI", "Tom Mitchell", 2021, 4.5},
        {"B017", "Industrial Area Community Library", "Mechanical Engineering Basics", "Engineering", "A.K. Gupta", 2016, 4.4},
        {"B018", "Industrial Area Community Library", "Electrical Machines", "Engineering", "P.S. Bimbhra", 2015, 4.6},
        {"B019", "Temple Area Library", "Bhagavad Gita", "Religion", "Vyasa", 1998, 4.9},
        {"B020", "Temple Area Library", "Life Lessons from Ramayana", "Philosophy", "Arun Sharma", 2012, 4.6}
    };

    // Build the Trie with all books
    Trie trie;
    for (auto& book : books) {
        trie.insert(&book);
    }

    // Take prefix input from user for prefix matching
    string prefix;
    cout << "Enter a prefix to search for book titles: ";
    getline(cin, prefix); // Use getline to handle spaces in titles if needed

    // Perform prefix search
    vector<Book*> results = trie.searchPrefix(prefix);

    // Display results
    if (results.empty()) {
        cout << "No books found with the given prefix." << endl;
    } else {
        cout << "Books matching the prefix \"" << prefix << "\":" << endl;
        for (const auto& book : results) {
            printBook(book);
        }
    }

    // Optional: Demonstrate Binary Search as secondary mechanism
    // For example, sort books by title and search for an exact title
    // (This is for illustration; the primary is Trie for prefix)
    vector<Book*> sortedBooks;
    for (auto& b : books) sortedBooks.push_back(&b);
    sort(sortedBooks.begin(), sortedBooks.end(), [](const Book* a, const Book* b) {
        return a->title < b->title;
    });

    string exactTitle;
    cout << "Enter an exact title to search (using binary search): ";
    getline(cin, exactTitle);

    // Binary search for exact title
    auto it = lower_bound(sortedBooks.begin(), sortedBooks.end(), nullptr, [&](const Book* a, const Book* b) {
        return a->title < exactTitle;
    });
    if (it != sortedBooks.end() && (*it)->title == exactTitle) {
        cout << "Book found:" << endl;
        printBook(*it);
    } else {
        cout << "No book found with the exact title." << endl;
    }

    return 0;
}
