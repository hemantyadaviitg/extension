#include<bits/stdc++.h>
using namespace std;

class BTreeNode
{
	vector <int> keys;
	int leaf_size;
	int non_leaf_size;
	vector <BTreeNode *> C;
	BTreeNode * prv;
	BTreeNode * next;
	bool leaf;
public:
	int getCsize();
	BTreeNode* getC(int i);
	bool isleaf();
	BTreeNode(int leaf_size, int non_leaf_size, bool leaf);
	void traverse();
	void insert(int key, int* val, BTreeNode* &newentry);
	void split(int* val, BTreeNode* &newentry);
	void enter(BTreeNode* newentry, int val, int pos);
	bool isFull();
	BTreeNode* makenewroot(int val, BTreeNode* newentry);
	bool search(int key);
};

bool BTreeNode :: search(int key){
	if (leaf == false){
		int i = 0;
		while(i < keys.size() && key >= keys[i])
			i++;
		return C[i]->search(key);
	}
	else{
        vector <int> :: iterator it = lower_bound(keys.begin(), keys.end(), key);
        return (*it == key);
	}

}

int  BTreeNode :: getCsize(){
	return C.size();
}

BTreeNode*  BTreeNode :: getC(int i){
	return this->C[i];
}


bool BTreeNode :: isleaf(){
    return leaf;
}


bool BTreeNode :: isFull(){
    if (leaf)
    	return (keys.size() == leaf_size);
    else
    	return (keys.size() == non_leaf_size);
}

BTreeNode::BTreeNode(int leaf_size, int non_leaf_size, bool leaf) {
	this->leaf_size = leaf_size;
	this->non_leaf_size = non_leaf_size;
	this->leaf = leaf;
	this->next = NULL;
	this->prv = NULL;
}


void BTreeNode::traverse() {
	int i;
	//string s;
	//for(int j = 0; j < tab; j++)
        //s += '\t';
	for (i = 0; i < keys.size(); i++)
	{
		//if (leaf == false)
			//C[i]->traverse(tab + 1);
		cout << keys[i] << " "; //<< endl;
	}

	//if (leaf == false)
		//C[i]->traverse(tab+1);
}

void BTreeNode :: enter(BTreeNode* newentry, int val, int pos){
    keys.insert(keys.begin() + pos, val);
    C.insert(C.begin() + pos + 1, newentry);
}

void BTreeNode :: split(int* val, BTreeNode* &newentry){
	int t = (non_leaf_size+1)/2;
	newentry = new BTreeNode(leaf_size, non_leaf_size, false);
	*val = this->keys[t];
	for(int i = t+1; i < 2*t; i++){
		newentry->keys.push_back(this->keys[i]);
	}
	this->keys.resize(t);
	for(int i = t+1; i <= 2*t; i++)
		newentry->C.push_back(this->C[i]);
	this->C.resize(t+1);
}

void BTreeNode:: insert(int key, int* val, BTreeNode* &newentry){
	if (leaf == false){
		int i = 0;
		while(i < keys.size() && key >= keys[i])
			i++;
		C[i]->insert(key, val, newentry);
		if (newentry == NULL)
			return;
		enter(newentry, *val, i);
		if (keys.size() <= non_leaf_size){
			newentry = NULL;
		}
		else{
			split(val, newentry);
		}
	}
	else{
        vector <int> :: iterator it = lower_bound(keys.begin(), keys.end(), key);
        keys.insert(it, key);
        if (keys.size() > leaf_size){
        	int t = leaf_size/2;
            newentry = new BTreeNode(leaf_size, non_leaf_size, true);
            *val = this->keys[t+1];
            for(int i = t+1; i <= 2*t; i++){
                newentry->keys.push_back(this->keys[i]);
            }
            this->keys.resize(t+1);
            newentry->next = this->next;
            if (newentry->next)
            	newentry->next->prv = newentry;
            this->next = newentry;
            newentry->prv = this;
        }
	}
}

class BTree{
    BTreeNode* root;
    int t;
    public:
    BTree(int leaf_size, int non_leaf_size);
    void insert(int key);
    void display();
    void search(int key);
};

void BTree :: search(int key){
	int status =  root->search(key);
	if (status)
		cout << key << " found in tree" << endl;
	else
		cout << key << " not found in tree" << endl;
}

BTree :: BTree(int leaf_size, int non_leaf_size){
    root = new BTreeNode(leaf_size, non_leaf_size, true);
}

BTreeNode* BTreeNode :: makenewroot(int val, BTreeNode* newentry){
    BTreeNode* root = new BTreeNode(leaf_size, non_leaf_size, false);
    root->keys.push_back(val);
    root->C.push_back(this);
    root->C.push_back(newentry);
    return root;
}

void BTree :: insert(int key){
    BTreeNode* newentry = NULL;
    int val = 0;  //serperator
    root->insert(key, &val, newentry);
    if (newentry != NULL){
        root = root->makenewroot(val, newentry);
    }
}

void BTree :: display(){
	cout<<"Prints Everything correcly including Level as well except Parent Information.\n";
    cout<<"Correctness can be checked using level of a node and by inserting into the appropriate node and see which node slits.\n";
    cout<<"\n\n\n\n";
    queue<pair<int,BTreeNode *> > q;
	queue<int> level;
	int nodeID = 1;
	pair<int, BTreeNode*> curNode;
	int curLevel=0,prevLevel=-1;
	q.push(make_pair(nodeID, root));
	level.push(0);
	cout<<"\n*** NodeID :: Values ***\n"<<endl;
	while(!q.empty()){
		curNode = q.front(); q.pop();
		if(prevLevel!=level.front()){
			prevLevel = curLevel++;
		}
		level.pop();
		cout<<curNode.first<< " :: ";
		BTreeNode *node = curNode.second;
		if(node->isleaf()){
			node->traverse();
			cout<<endl;
		}
		else{
			int arr[1000];
			int point=0;
			int x=nodeID;
			for(int i=0; i < node->getCsize(); i++){
				q.push(make_pair(++nodeID, node->getC(i)));
				arr[point++]=nodeID;
				level.push(curLevel);
			}
			cout<<"Children ID for nodeID of "<<x<<" ";
			for(int vali=0;vali<point;vali++)
				cout<<arr[vali]<<" ";
			node->traverse();
			cout<<endl;
		}
	}
}

int main(){

    BTree* tree = NULL;
    int inp;

    while(1){

			cout << "1. Create new tree" << endl;
			cout << "2. Add new key to the tree" << endl;
			cout << "3. Search a key in the tree" << endl;
			cout << "4. Display the Tree" << endl;
			cout << "Any other choice to exit" << endl;
			cout << "Enter your choice: ";
			cin >> inp;
			cout << "You entered " << inp << endl;

	    if (inp == 1){

				int numIndexPointers, numDataPointers;
				cout<<"Enter number of Index pointers: (Number of Number of Index Pointers = Number of Non-leaf Keys + 1)  ";
				cin>>numIndexPointers;
				cout<<"Enter number of Data pointers: (Number of Number of Data Pointers = Number of Leaf Keys)  ";
				cin>>numDataPointers;

				if(numIndexPointers%2 || numDataPointers%2){
					cout<<"Note:: Index pointers must be in Even number and Data pointers must be in Even number"<<endl;
					exit(0);
				}

	    	int non_leaf_size = numIndexPointers-1;
	    	int leaf_size = numDataPointers;

	    	if (tree)
	    		delete(tree);
	    	tree = new BTree(leaf_size, non_leaf_size);
	    }

	    else if (inp == 2){
				if(tree == NULL){
		        cout << "Create a new tree first" << endl;
		        break;
		     	}
	      int x;
				cout << "Enter key to add ";
	      cin >> x;
	      tree->insert(x);
	    }

        else if (inp == 3){
					if(tree == NULL){
	          cout << "Create a new tree first" << endl;
	          break;
	        }
	        cout << "Enter key to search: ";
        	int x;
        	cin >> x;
        	tree->search(x);
        }

        else if (inp == 4){
					if(tree == NULL){
						cout << "Create a new tree first" << endl;
						break;
					}
					tree->display();
					cout<<endl;
        }

				else break;
    }

    return 0;
}
