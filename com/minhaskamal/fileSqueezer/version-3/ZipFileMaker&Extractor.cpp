/**
* Developer: Minhas Kamal (IIT 5th Batch)
* Date: 22.Oct.2013
* Description: This program reads a file & Zips or Extracts it.
* Comment: I was in a puzzle at every step of developing this program, literally!
**/


#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;


struct node{
    int value; //value of the node
    char info; //character that it contains
    bool bit;
    node *r,   //root
         *t1,  //left sub tree
         *t2;  //right sub tree
};


void treeBuilder(int* num, node* tree, int x, int s, int e, long  *exactFileSize);  //creates the linked tree
void charCounter(int* num, int s, int e, string fileName); //counts characters in a file
void zipFileWriter(int* num, node* tree, int s, int e, string fileName, long *zipFileSize);   //reads a file and makes its zip file
void fileExtractor(node* tree, int x, FILE* input, string fileName, long *zipFileSize);  //writes the extracted file


int main(){
    long zipFileSize=0;    //counts the number of bits of zip file
    long exactFileSize=0;    //counts the number of bits of exact file

    int selection;
    cout << "What do you want to do? \nPress 1 for making zip file \n      2 for extracting\n";
    cout << "Selection- ";
    cin >> selection;

    string fileName;
    cout << "Enter name of the file: ";
    cin >> fileName;

    if(selection==1){
        int s=10,   //the starting char in the ascii table
            e=126;  //the ending char in the ascii table


        int num[e-s];   //holds the number of characters
        charCounter(num, s, e, fileName);  //reading a file & counting the number of different characters

        zipFileSize=(e-s+2)*16; //counts the number of bits ((int)e, (int)s, (char)\n, (int)e-s-1, (char)\n)

        int x=(e-s)*2-1;    //number of vertex
        node tree[x];   //the tree

        treeBuilder(num, tree, x, s, e, &exactFileSize);  //making the tree

        zipFileWriter(num, tree, s, e, fileName, &zipFileSize);   //writing zip file

        cout << "Input file size is: " << exactFileSize << "bits \n";
        cout << "Zip file size is: " << zipFileSize << "bits \n";
    }

    else{
        FILE *input;

        char fName[fileName.size()+1];
        for(int i=0; i<fileName.size(); i++){    //string to char* conversion
            fName[i]=fileName.at(i);
            //cout << fName[i]; ///test
        }
        fName[fileName.size()]='\0';    //null terminator

        if((input=fopen(fName, "rb"))==NULL){ //opening the file
            printf("File is not found!!\n");
        }

        else{
            int s,  //the starting char in the ascii table
                e;  //the ending char in the ascii table


            fscanf(input, "%d %d", &s, &e);
            //cout << s << " " << e <<endl;   ///test

            zipFileSize=(e-s+2)*16; //counts the number of bits ((int)e, (int)s, (char)\n, (int)e-s-1, (char)\n)

            int num[e-s];   //holds the number of characters
            for(int i=0; i<e-s; i++){    //taking the input of number of characters
                fscanf(input, "%d ", &num[i]);
                //cout << num [i] << " "; ///test
            }
            //cout << endl; ///test

            int x=(e-s)*2-1;    //number of vertex
            node tree[x];   //the tree

            treeBuilder(num, tree, x, s, e, &exactFileSize);    //making the tree

            fileExtractor(tree, x, input, fileName, &zipFileSize);

            fclose(input);

            cout << "\n# File Extracted.\n\n";

            cout << "Input file size is: " << zipFileSize << "bits \n";
            cout << "Extracted file size is: " << exactFileSize << "bits \n";
        }
    }


    return 0;
}

///////////////**************////////////////

void treeBuilder(int* num, node* tree, int x, int s, int e, long  *exactFileSize){
    for(int i=0; i<x; i++){ //initialization
        tree[i].value = -1;
        tree[i].info = (char)0;
        tree[i].bit = false;
        tree[i].r = NULL;
        tree[i].t1 = NULL;
        tree[i].t2 = NULL;
    }


    int position=0; //position in the array

    for(; position<e-s; position++){    //setting the value of the initial nodes
        tree[position].value = num[position];
        tree[position].info = (char)(position+s);
    }

    int lt,   //left sub tree
        rt;   //right sub tree
    while(position<x){   //building up the tree
        for(int i=0; i<x; i++) //initialization
            if(tree[i].r==NULL && tree[i].value>=0){
                lt=i;
                break;
            }

        for(int i=0; i<x; i++){ //finds left sub tree (smallest value)
            if(tree[i].r==NULL && tree[i].value>=0)
                if(tree[i].value < tree[lt].value) lt=i;
        }
        tree[lt].r = &tree[position];   //setting the root of the left sub trees


        for(int i=0; i<x; i++) //initialization
            if(tree[i].r==NULL && tree[i].value>=0){
                rt=i;
                break;
            }

        for(int i=0; i<x; i++){ //finds right sub tree (second smallest value)
            if(tree[i].r==NULL && tree[i].value>=0)
                if(tree[i].value < tree[rt].value) rt=i;
        }
        tree[rt].r = &tree[position];   //setting the root of the right sub trees


        tree[lt].bit = false;   //setting the bit of the sub trees
        tree[rt].bit = true;

        tree[position].value = (tree[lt].value + tree[rt].value);   //the value is the sum of the sub trees
        tree[position].t1 = &tree[lt];  //setting sub tree of new node
        tree[position].t2 = &tree[rt];

        position++; //increasing the position
    }

    long temp = tree[x-1].value*8;
    *exactFileSize=temp;    //exact file size

    /*/for(int i=0; i<x; i++){ //test of the node
        cout << tree[i].value << " " << tree[i].info << " " << tree[i].bit << " ";
        cout << tree[i].r << " " << tree[i].t1 << " " << tree[i].t2 << endl;
    }/**/

    return ;
}

///////////////**************////////////////

void charCounter(int* num, int s, int e, string fileName){
    FILE *input;

    char fName[fileName.size()+1];
    for(int i=0; i<fileName.size(); i++)    //string to char* conversion
        fName[i]=fileName.at(i);
    fName[fileName.size()]='\0';    //null terminator

    if((input=fopen(fName, "rb"))==NULL){ //opening the file, if file is not found return
        printf("File is not found!!\n");
        return ;
    }

    for(int i=0; i<e-s; i++)    //initializing
        num[i]=0;

    char ch;
    ch=fgetc(input);
    while(!feof(input)){ //reading the file & counting the characters
        //cout << ch; ///test
        if(ch>=s && ch<e) num[ch-s]++;
        ch=fgetc(input);
    }

    num[20-s]++;    //this char will be added at the end of the Zip file indicating the end

    /*/for(int i=s; i<e; i++){ //the output after the input file's characters are counted
        char c = (char) i;
        cout << i << ". " << c << "  " << num[i-s] << endl;
    }/**/

    fclose(input);

    return ;
}


void zipFileWriter(int* num, node* tree,int s, int e, string fileName, long *zipFileSize){
    FILE *input;    //main text file
    char fName[fileName.size()+1];  //input file name

    for(int i=0; i<fileName.size(); i++)    //string to char* conversion
        fName[i]=fileName.at(i);
    fName[fileName.size()]='\0';    //null terminator

    if((input=fopen(fName, "rb"))==NULL){ //opening the file, if file is not found return
        printf("File is not found!!\n");
        return ;
    }

    FILE *output;   //the zip file
    char oFileName[fileName.size()+3+1];  //output file name

    //creates a name depending on the input file name
    oFileName[0]='Z';oFileName[1]='i';oFileName[2]='p'; //first three letter
    for(int i=3; i<fileName.size()+3; i++)    //string to char* conversion
        oFileName[i]=fileName.at(i-3);
    oFileName[fileName.size()+3]='\0';    //null terminator

    output=fopen(oFileName, "wb");

    fprintf(output, "%d %d %c", s, e, 10);    //printing start & end of characters
    for(int i=0; i<e-s; i++)    //printing number of characters
        fprintf(output, "%d ", num[i]);
    fprintf(output, "\n");

    int flag=1; //this flag is necessary to set '20' char at the end of zip file & also controls the loop
    stack<bool> mystack;
    char ch;
    ch=fgetc(input);
    while(1){ //reading the file once more & writing in the output file
        //cout << ch <<" "; ///test

        int i;
        for(i=0; i<e-s; i++){   //finding the char in the node
            if(tree[i].info==ch) break;
        }

        node* nod;  //for temporary use
        nod=&tree[i];
        while(nod->r!=NULL){    //assigning bits in the stack
            mystack.push(nod->bit);   //bit of the node
            nod=nod->r; //assigning the root
        }


        while(!mystack.empty()){    //writing in the output file
            //cout << mystack.top();  ///test
            //fputc(mystack.top(), output);
            fprintf(output, "%d", mystack.top());

            *zipFileSize+=1; //counts the number of bits
            mystack.pop();
        }
        //cout << endl; ///test

        ch=fgetc(input);    //taking the input

        if(!flag) break;

        else if(flag && feof(input)){
            flag = 0;
            ch=20;
            continue;
        }
    }

    fclose(output);
    fclose(input);



    cout << "\n# Zip file is made.\n";

    return ;
}

///////////////**************////////////////

void fileExtractor(node* tree, int x, FILE *input, string fileName, long *zipFileSize){
    FILE *output;
    char oFileName[fileName.size()];  //output file name

    //creates a name depending on the input file name
    oFileName[0]='E';oFileName[1]='x';oFileName[2]='t'; //first three letter
    for(int i=3; i<fileName.size(); i++)    //string to char* conversion
        oFileName[i]=fileName.at(i);
    oFileName[fileName.size()]='\0';    //null terminator

    output=fopen(oFileName, "wb");

    char ch;
    ch=fgetc(input);
    while(!feof(input)){ //reading the file & counting the characters
        node* nod;  //for temporary use
        nod=&tree[x-1]; //the main root of tree

        while(nod->t1!=NULL){
            //cout << ch;   ///test

            *zipFileSize+=1;  //counts number of bits

            if(ch=='0') nod = nod->t1;
            else nod = nod->t2;

            ch=fgetc(input);
        }
        if(nod->info == 20) break;  //break when found 20th char

        fprintf(output, "%c", nod->info);
        //cout << nod->info << endl;  ///test
    }

    fclose(output);

    return ;
}




