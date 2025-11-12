#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <bitset>
#include <string>
#include <chrono>
#include <list>

using namespace std;

#include <chrono>

class Timer {
public:
    Timer() : elapsed_time(0.0), running(false) {}

    void start() {
        if (!running) {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
        }
    }

    void pause() {
        if (running) {
            auto now = std::chrono::high_resolution_clock::now();
            elapsed_time += std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count();
            running = false;
        }
    }

    void resume() {
        if (!running) {
            start_time = std::chrono::high_resolution_clock::now();
            running = true;
        }
    }

    // Return elapsed time in seconds (as double)
    double elapsedNanoseconds() {
        if (running) {
            auto now = std::chrono::high_resolution_clock::now();
            return (elapsed_time + std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time).count()) / 1'000'000'000.0;
        }
        return elapsed_time / 1'000'000'000.0;
    }

private:
    std::chrono::high_resolution_clock::time_point start_time;
    double elapsed_time; // in nanoseconds
    bool running;
};






































// AVL Tree Node
struct AVLNode {
    string key;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const string& val) : key(val), left(nullptr), right(nullptr), height(1) {}
};

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

bool contains(AVLNode* node, const string& key) {
    if (!node) return false;
    if (key == node->key) return true;
    if (key < node->key) return contains(node->left, key);
    return contains(node->right, key);
}

AVLNode* insert(AVLNode* node, const string& key) {
    if (!node) return new AVLNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // duplicate

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->key)
        return rotateRight(node);
    if (balance < -1 && key > node->right->key)
        return rotateLeft(node);
    if (balance > 1 && key > node->left->key) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && key < node->right->key) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void inorderTraversal(AVLNode* root) {
    if (root) {
        inorderTraversal(root->left);
        cout << root->key << " ";
        inorderTraversal(root->right);
    }
}















string toBinary(string addressStr) {
    addressStr.erase(0, addressStr.find_first_not_of(" \t\n\r"));
    addressStr.erase(addressStr.find_last_not_of(" \t\n\r") + 1);

    if (addressStr.empty()) return "";

    try {
        uint64_t address = stoull(addressStr);

        return bitset<64>(address).to_string();
    } catch (const invalid_argument& e) {
        cerr << "Invalid address in file: '" << addressStr << "'" << endl;
        return "";
    } catch (const out_of_range& e) {
        cerr << "Address out of range: '" << addressStr << "'" << endl;
        return "";
    }
}



















void outputCache(const vector<string>& cache, const string& filename, int accesses, int misses, int hits, int uniques, double timer) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Could not open output file.\n";
        return;
    }

    double miss_rate = (double)misses / accesses * 100.0;
    double hit_rate = (double)hits / accesses * 100.0;

    out << "Cache_accesses = " << accesses << "\n";
    out << "Total_number_of_unique_addresses = " << uniques << "\n";
    out << "Total_number_of_misses = " << misses << "\n";
    out << "Total_number_of_hits = " << hits << "\n";
    out << fixed << setprecision(3);
    out << "Miss_rate = " << miss_rate << "\n";
    out << "Hit_rate = " << hit_rate << "\n";
    out << "Total_time = " << timer << " seconds\n\n";
    out << "--------Cache Contents---------\n";
    for (size_t i = 0; i < cache.size(); ++i) {
        out << "\n# " << i << " " << cache[i];
    }

    out.close();
}

void outputFA(const vector<string>& cache, const string& filename, int accesses, int misses, int hits, int uniques, double timer) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: Could not open output file.\n";
        return;
    }

    double miss_rate = (double)misses / accesses * 100.0;
    double hit_rate = (double)hits / accesses * 100.0;

    out << "Cache_accesses = " << accesses << "\n";
    out << "Total_number_of_unique_addresses = " << uniques << "\n";
    out << "Total_number_of_misses = " << misses << "\n";
    out << "Total_number_of_hits = " << hits << "\n";
    out << fixed << setprecision(3);
    out << "Miss_rate = " << miss_rate << "\n";
    out << "Hit_rate = " << hit_rate << "\n";
    out << "Total_time = " << timer << " seconds\n\n";

    out << "--------Cache Contents--------\n\n";
    out << "# 0 ";

    for (const string& addr : cache) {
        out << addr << " ";
    }

    out << "\n";
    out.close();
}

void outputSA(int cache_accesses, int unique_addresses, int miss, int hit, const vector<vector<string>>& cache, double timer) {
    ofstream outfile("output.txt");

    if (!outfile) {
        cerr << "Error opening output.txt for writing." << endl;
        return;
    }

    double miss_rate = static_cast<double>(miss) / cache_accesses * 100.0;
    double hit_rate = static_cast<double>(hit) / cache_accesses * 100.0;

    outfile << "Cache_accesses = " << cache_accesses << endl;
    outfile << "Total_number_of_unique_addresses = " << unique_addresses << endl;
    outfile << "Total_number_of_misses = " << miss << endl;
    outfile << "Total_number_of_hits = " << hit << endl;

    outfile << fixed << setprecision(3);
    outfile << "Miss_rate = " << miss_rate << endl;
    outfile << "Hit_rate = " << hit_rate << endl;
    outfile << "Total_time = " << timer <<" seconds"<< endl;

    outfile << "\n--------Cache Contents--------\n" << endl;

    for (int i = 0; i < cache.size(); ++i) {
        outfile << "# " << i;
        for (int j = 0; j < cache[i].size(); ++j) {
            outfile << " " << cache[i][j];
        }
        outfile << endl;
    }

    outfile.close();
}























int main (){

    //one block=one word ==> single-word blocks

    AVLNode* root = nullptr;
    Timer timer;
    int cacheSize=0;
    int typeofcache=0;
    int numberofways=0;
    int unique_addresses=0;
    int blockreplace=0;
    cout<<"Please provide the size of the memory in blocks"<<endl;
    cin>>cacheSize;

    cout<<"Give\n1. for DM\n2. for SA \n3. for FA"<<endl;
    cin>>typeofcache;


    //DM cache
    if(typeofcache==1){

        timer.start();

        numberofways=1;
        int numberOfSets = cacheSize / numberofways;
        vector<string> cache(numberOfSets, "0");
        int indexbits=log2(numberOfSets);
        int tagbits=64-indexbits;
        string addr;
        int cache_accesses=0,miss=0,hit=0;

        //open file
        ifstream infile("A_trace.s");
        string address;
        if (!infile) {
            cerr << "Error: could not open file.\n";
            return 1;
        }



        //itirate through the address
        while(getline(infile, address)){
            cache_accesses++;
            if (!contains(root, address)) {
                unique_addresses++;
                root = insert(root, address);
            }

            //make address to binary
            addr=toBinary(address);
            if(addr==""){ cache_accesses--; continue; }

            //get the tag string
            string indexStr = addr.substr(64-indexbits,64);
            int setIndex = stoi(indexStr, nullptr,2);

            if(cache[setIndex]==address){ hit++; }
            else { miss++; }

            cache[setIndex]=address;

        }
        outputCache(cache,"output.txt", cache_accesses, miss, hit,unique_addresses,timer.elapsedNanoseconds());
        infile.close();
    }
    else if(typeofcache==2){

        cout<<"Give the number of ways you want"<<endl;
        cin>>numberofways;

        cout<<"Give the block replacement policy\n1. LRU\n2. SRRIP"<<endl;
        cin>>blockreplace;

        timer.start();
        int numberofsets=cacheSize/numberofways;
        vector<vector<string>> cache(numberofsets, vector<string>(numberofways, "0"));
        int indexbits=log2(numberofsets);
        int tagbits=64-indexbits;
        int cache_accesses=0,miss=0,hit=0;
        string addr;
        unordered_map<string, pair<int, int>> addressToIndex;


         //open file
        ifstream infile("D_trace.s");
        string address;
        if (!infile) {
            cerr << "Error: could not open file.\n";
            return 1;
        }


        if(blockreplace==1){

            vector<list<string>> lru_queue(numberofsets);

            //itirate through the address
            while(getline(infile, address)){
                cache_accesses++;
                if (!contains(root, address)) {
                    unique_addresses++;
                    root = insert(root, address);
                }

                //make address to binary
                addr=toBinary(address);
                if(addr==""){cache_accesses--; continue; }

                //get the tag string
                string indexStr = addr.substr(64-indexbits,64);
                int setIndex = stoi(indexStr, nullptr,2);

                if (addressToIndex.count(address)) {
                    // HIT
                    hit++;
                    lru_queue[setIndex].remove(address);
                    lru_queue[setIndex].push_front(address);
                }
                else {
                    // MISS
                    miss++;
                    int way=-1;
                    for(int i=0; i<numberofways;i++){ if(cache[setIndex][i]=="0"){ way=i; break; } }
                    if(way!=-1) {
                        // way empty
                        //cout<<setIndex<<"   "<<way<<endl;
                        cache[setIndex][way] = address;
                        addressToIndex[address] = {setIndex, way};
                        lru_queue[setIndex].push_front(address);
                    }
                    else {
                        // Cache full
                        string lru = lru_queue[setIndex].back();
                        lru_queue[setIndex].pop_back();
                        int way = addressToIndex[lru].second;
                        addressToIndex.erase(lru);

                        cache[setIndex][way] = address;
                        addressToIndex[address] = {setIndex, way};
                        lru_queue[setIndex].push_front(address);
                    }
                }
            }
        }
        else if(blockreplace==2){

            int m;
            cout<<"Give the number of bits for replacement"<<endl;
            cin>>m;

            vector<vector<int>> rrpc(numberofsets, vector<int>(numberofways, 0));

            while(getline(infile, address)){
                cache_accesses++;
                if (!contains(root, address)) {
                    unique_addresses++;
                    root = insert(root, address);
                }

                //make address to binary
                addr=toBinary(address);
                if(addr==""){cache_accesses--; continue; }

                //get the tag string
                string indexStr = addr.substr(64-indexbits,64);
                int setIndex = stoi(indexStr, nullptr,2);

                int hitway=-1;
                ///////////////////////////////////////////////////////////////////////
                for(int i=0;i<numberofways;i++){ if(cache[setIndex][i]==address) { hitway=i; break;}}

                if (hitway!=-1){
                    // HIT
                    hit++;
                    rrpc[setIndex][hitway]=0;
                }
                else {
                    // MISS
                    miss++;
                    int way=-1;
                    for(int i=0; i<numberofways;i++){ if(cache[setIndex][i]=="0"){ way=i; break; } }
                    if(way!=-1) {
                        // way empty
                        //cout<<setIndex<<"   "<<way<<endl;
                        cache[setIndex][way] = address;
                        rrpc[setIndex][way]=(1 << m)-2;
                    }
                    else {
                        // Cache full
                        int replaceway=-1;
                        while(replaceway==-1){
                            for(int i=0;i<numberofways;i++){if(rrpc[setIndex][i]==(1 << m)-1) { replaceway=i; break; } }
                            if(replaceway==-1){ for(int i=0;i<numberofways;i++){if(rrpc[setIndex][i] < (1 << m)-1) { rrpc[setIndex][i]++; } } }
                        }
                        cache[setIndex][replaceway] = address;
                        rrpc[setIndex][replaceway] = (1 << m)-2;
                    }
                }
            }
        }
        outputSA(cache_accesses, unique_addresses, miss, hit, cache,timer.elapsedNanoseconds());
        infile.close();
    }
    else if(typeofcache==3){

        cout<<"Give the block replacement policy\n1. LRU\n2. SRRIP"<<endl;
        cin>>blockreplace;

        timer.start();
        numberofways=cacheSize;
        vector<string> cache(numberofways, "0");
        int indexbits=log2(numberofways);
        int tagbits=64-indexbits;
        string addr;
        int cache_accesses=0,miss=0,hit=0;
        int nextFreeIndex = 0;
        unordered_map<string, int> addressToIndex;

        //open file
        ifstream infile("B_trace.s");
        string address;
        if (!infile) {
            cerr << "Error: could not open file.\n";
            return 1;
        }

        if(blockreplace==1){

            list<string> lruOrder;

            //itirate through the address
            while(getline(infile, address)){
                cache_accesses++;
                if (!contains(root, address)) {
                    unique_addresses++;
                    root = insert(root, address);
                }

                //make address to binary
                addr=toBinary(address);
                if(addr==""){ cache_accesses--; continue; }

                if (addressToIndex.count(address)) {
                    // HIT
                    hit++;
                    lruOrder.remove(address);
                    lruOrder.push_front(address);
                }
                else {
                    // MISS
                    miss++;
                    if (nextFreeIndex < numberofways) {
                        // Cache not full
                        cache[nextFreeIndex] = address;
                        addressToIndex[address] = nextFreeIndex;
                        lruOrder.push_front(address);
                        nextFreeIndex++;
                    }
                    else {
                        // Cache full
                        string lru = lruOrder.back();
                        lruOrder.pop_back();

                        int indexToEvict = addressToIndex[lru];
                        addressToIndex.erase(lru);

                        cache[indexToEvict] = address;
                        addressToIndex[address] = indexToEvict;
                        lruOrder.push_front(address);
                    }
                }
            }
        }
        else if(blockreplace==2){

            int m;
            cout<<"Give the number of bits for replacement"<<endl;
            cin>>m;

            vector<int> rrpc(numberofways, 0);

            while(getline(infile, address)){
                cache_accesses++;
                if (!contains(root, address)) {
                    unique_addresses++;
                    root = insert(root, address);
                }

                //make address to binary
                addr=toBinary(address);
                if(addr==""){ cache_accesses--; continue; }

                if (addressToIndex.count(address)) {
                    // HIT
                    hit++;
                    rrpc[addressToIndex[address]]=0;
                }
                else {
                    // MISS
                    miss++;
                    if (nextFreeIndex < numberofways) {
                        // Cache not full
                        cache[nextFreeIndex] = address;
                        addressToIndex[address] = nextFreeIndex;
                        rrpc[nextFreeIndex]=(1 << m) - 2;
                        nextFreeIndex++;
                    }
                    else {
                        // Cache full
                        while(true){
                            bool flag=false;
                            int indexToEvict=-1;
                            for(int i:rrpc){ indexToEvict++; if(i==(1 << m) - 1){ flag=true; break; } }
                            if(flag==false){ for(int i=0;i<numberofways;i++){ if (rrpc[i] < (1 << m) -1) {rrpc[i]++; } } }
                            else if(flag==true){
                                addressToIndex.erase(cache[indexToEvict]);
                                cache[indexToEvict]=address;
                                addressToIndex[address] = indexToEvict;
                                rrpc[indexToEvict]=(1 << m) - 2;
                                break;
                            }
                        }
                    }
                }
            }
        }
        outputFA(cache, "output.txt", cache_accesses, miss, hit, unique_addresses,timer.elapsedNanoseconds());
        infile.close();
    }
    return 0;
}
