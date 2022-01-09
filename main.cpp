#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct tNode{
    char c;
    int freq;
    bool targetL; //determine whether it's the target leaves or not
    tNode * p; //pointer to parent
    tNode * left; //pointer to left child
    tNode * right; //pointer to right child
};

string readFile(char nFile[]){ //read the line from the input file
    ifstream fin;
    fin.open(nFile);
    if (!fin.is_open()){
        cout << "Error in file opening!" << endl;
        exit(1);
    }

    string s;
    getline(fin,s);
    fin.close();
    return s;
}

void freqTable(char cha[200], int freqT[200], int & n, string s){
    int sl = s.length();
    for(int i=0; i<sl; i++){
        bool in=false;
        for(int j=0; j<n+1; j++){
            if(cha[j] == s[i]) {
                freqT[j]++;
                in = true;
            }
        }
        if(in==false) {
            cha[n] = s[i];
            freqT[n] = 1;
            n++;
        }
    }
}

void sort(char cha[200],int freqT[200],int n){
    for(int i=1; i<n; i++){
        char temp = cha[i];
        int tempF = freqT[i];
        int j=i-1;
        while( j>=0 and cha[j]>temp ) {
            cha[j+1] = cha[j];
            freqT[j+1] = freqT[j];
            j--;
        }
        cha[j+1] = temp;
        freqT[j+1] = tempF;
    }
}

void small(int rep[2], tNode *iPtr[], int n1 ){ //find the indexes of the two representatives
    int s1=0; //s1,s2 denote the indexes of the smallest and 2nd smallest freq
    for(int i=0; i<n1; i++) {
        if(iPtr[i]->freq < iPtr[s1]->freq) s1=i;
    }
    int s2;
    if(s1!=0) s2=0;
    else s2=1;
    for(int i=0; i<s1; i++) {
        if (iPtr[i]->freq < iPtr[s2]->freq) s2=i;
    }
    for(int i=s1+1; i<n1; i++){
        if (iPtr[i]->freq < iPtr[s2]->freq) s2=i;
    }
    //cout << s1 << " " << s2 << endl;

    if(iPtr[s1]->c <= iPtr[s2]->c) {
        rep[0]=s1;
        rep[1]=s2;
    }
    else if(iPtr[s1]->c > iPtr[s2]->c) {
        rep[0] = s2;
        rep[1] = s1;
    }
}

tNode* huffman(char cha[200],int freqT[200],int n, tNode tParent[n], tNode tLeaf[n]){
    int n1=n;
    struct tNode *iPtr[n];
    for(int i=0; i<n;i++){ //create the leaves for the tree
        tLeaf[i] = {cha[i], freqT[i], true, NULL, NULL,NULL};
        iPtr[i] = & tLeaf[i];
    }

    while(n1>=2){ //each i represent a step in constructing the huffman tree
        int rep[2]; // find the two trees with the smallest representatives and combine them
        small(rep,iPtr,n1);

        tParent[n-n1]={iPtr[rep[0]]->c, iPtr[rep[0]]->freq+iPtr[rep[1]]->freq,false, NULL, iPtr[rep[0]], iPtr[rep[1]]};
        iPtr[rep[0]]->p = &tParent[n-n1];
        iPtr[rep[1]]->p = &tParent[n-n1];
        iPtr[rep[0]] = &tParent[n-n1];

        for(int i=rep[1]; i<n1; i++){ //renew the first reprez,delete the second reprez
            iPtr[i] = iPtr[i+1];
        }

        n1--;
    }

    return iPtr[0]; // return the root of the tree;
}

void extractRmLeaf(tNode *Root, string & code, int & rmleaf, char cha[200], int n){
    tNode *current = Root;
    while(current->left != NULL || current->right != NULL){
        if(current->left != NULL){
            current = current->left;
            code.append("0");
        }
        else{
            current = current->right;
            code.append("1");
        }
    }

    if(current->targetL == false){
        code = "";
        if(current == current->p->left) current->p->left = NULL; //delete the current node
        else current->p->right = NULL;
    }
    else{
        for(int i=0; i<n;i++){
            if(current->c == cha[i]) rmleaf = i;
        }
        if(current == current->p->left) current->p->left = NULL;
        else current->p->right = NULL;
    }
}

void getCode(tNode *Root, char cha[200],int n, string code[]){
    for(int i=0; i<n; i++){
        string cd;
        int rmleaf; //index of the character of the rightmost leaf
        extractRmLeaf(Root,cd,rmleaf,cha,n);
        while(cd=="") {
            extractRmLeaf(Root, cd, rmleaf, cha, n);
        }
        code[rmleaf] = cd;
    }
}

void writeFile1(char cha[200], string code[200], double ave, int n){
    ofstream fout;
    fout.open("code.txt");

    if(fout.fail()){
        cout << "Error in file opening!" << endl;
        exit(1);
    }
    for(int i=0; i<n; i++) {
        if(cha[i]==32){
            fout << "space" << ": " << code[i] << endl;
        }
        else{
            fout << cha[i] << ": " << code[i] << endl;
        }
    }
    fout << "Ave = " << setprecision(4) << ave << " bits per symbol" << endl;
    fout.close();
}

void writeFile2(char cha[100], string code[100], string s, int n){
    ofstream fout;
    fout.open("encodemsg.txt");
    if(fout.fail()){
        cout << "Error in file opening!" << endl;
        exit(1);
    }
    int l=s.length();
    for(int i=0; i<l; i++){
        for(int j=0; j<n; j++){
            if(s[i]==cha[j]) fout<<code[j];
        }
    }
    fout.close();
}

int main() {
    char nFile[]="test3_input_file.txt";
    string s=readFile(nFile); //read the input file

    char cha[200];
    int freqT[200];
    int n=0;
    freqTable(cha,freqT,n,s); //construct the frequency table
    sort(cha,freqT,n); //sort the frequency table s.t. representatives are in increasing order

    struct tNode tParent[n];
    struct tNode tLeaf[n];
    tNode *Root = huffman(cha,freqT,n,tParent,tLeaf);

    string code[n];
    getCode(Root,cha,n,code);

    double lf=0,f=0;
    for(int i=0; i< n;i++) {
        lf += code[i].length()*freqT[i];
        f += freqT[i];
    }
    double ave= lf/f;

    writeFile1(cha,code,ave,n);
    writeFile2(cha,code,s,n);
}
