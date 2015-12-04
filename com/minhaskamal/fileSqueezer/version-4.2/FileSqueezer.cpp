/**
* Developer: Minhas Kamal (IIT 5th Batch)
* Date: 28.Oct.2013
* Description: This program reads a file & Zips or Extracts it.
* Comment: I was in a puzzle at every step of developing this program, literally! And I am proud
           that I could solve them all, without any help, myself.
**/


#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;


/*this structure is needed to know the bits in a char*/
union{
    char character;
    struct{
        unsigned char x8: 1;    //they are decrementing because of the RAM memory
        unsigned char x7: 1;
        unsigned char x6: 1;
        unsigned char x5: 1;
        unsigned char x4: 1;
        unsigned char x3: 1;
        unsigned char x2: 1;
        unsigned char x1: 1;
    } bit;
} inputChar;
int bitPointer=1;   //points to a specific bit

/*this structure is needed to make the tree*/
struct node{
    int value; //value of the node
    char info; //character that it contains
    bool bit;
    node *r,   //root
         *t1,  //left sub tree
         *t2;  //right sub tree
};


void treeBuilder(int* num, node* tree, int x, int s, int e, long  *exactFileSize);  //creates the linked tree
int charCounter(int* num, int s, int e, string fileName); //counts characters in a file
void zipFileWriter(int* num, node* tree, int s, int e, string fileName, long *zipFileSize);   //reads a file and makes its zip file
void bitToChar(FILE* output, int bi);   //reads bits, converts in a character & writes in a file
void fileExtractor(node* tree, int x, FILE* input, string fileName, long *zipFileSize, int stop);  //writes the extracted file
char charToBit(FILE* input);    //reads char in a file & converts into bits




int main(){
    while(1){
        long zipFileSize=0;    //counts the number of bits of zip file
        long exactFileSize=0;    //counts the number of bits of exact file

        int selection;
        cout << "What do you want to do? \nPress 1 for making zip file \n      2 for extracting\n";
        cout << "      3 for exit \nSelection- ";
        cin >> selection;

        if(selection!=1 && selection!=2){
              break;
        }


        string fileName;
        cout << "Enter name of the file: ";
        cin >> fileName;

        if(selection==1){
            ///by changing these variables the range of characters can be changed
            int s=0,   //the starting char in the ascii table//cannot be less than 20
                e=256;  //the ending char in the ascii table


            int num[e-s];   //holds the number of characters
            if(charCounter(num, s, e, fileName)==0);  //reading a file & counting the number of different characters

            else {
                zipFileSize=(e-s+2)*18; //counts the number of bits ((int)e, (int)s, (char)\n, (int)e-s-1, (char)\n)

                int x=(e-s)*2-1;    //number of vertex
                node tree[x];   //the tree

                treeBuilder(num, tree, x, s, e, &exactFileSize);  //making the tree

                zipFileWriter(num, tree, s, e, fileName, &zipFileSize);   //writing zip file

                cout << "Input file size is: " << exactFileSize << "bits \n";
                cout << "Zip file size is: " << zipFileSize << "bits \n";
            }
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

                zipFileSize=(e-s+2)*18; //counts the number of bits ((int)e, (int)s, (char)\n, (int)e-s-1, (char)\n)

                int num[e-s];   //holds the number of characters
                for(int i=0; i<e-s; i++){    //taking the input of number of characters
                    fscanf(input, "%d ", &num[i]);
                    //cout << num [i] << " "; ///test
                }
                //cout << endl; ///test

                int x=(e-s)*2-1;    //number of vertex
                node tree[x];   //the tree

                treeBuilder(num, tree, x, s, e, &exactFileSize);    //making the tree

                fileExtractor(tree, x, input, fileName, &zipFileSize, num[20-s]);

                fclose(input);

                cout << "\n# File Extracted.\n\n";

                cout << "Input file size is: " << zipFileSize << "bits \n";
                cout << "Extracted file size is: " << exactFileSize << "bits \n";
            }
        }

        getchar();//delays for pressing any key
        cout << "\n\n";
    }

    return 0;
}

///////////////**************************************////////////////

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

    long temp = (tree[x-1].value-1)*8;  //1 is subtracted as 20th char is added to indicate the end
    *exactFileSize=temp;    //exact file size

    /*/for(int i=0; i<x; i++){ //test of the node
        cout << tree[i].value << " " << tree[i].info << " " << tree[i].bit << " ";
        cout << tree[i].r << " " << tree[i].t1 << " " << tree[i].t2 << endl;
    }/**/

    return ;
}

///////////////***********************************************////////////////

int charCounter(int* num, int s, int e, string fileName){
    FILE *input;

    char fName[fileName.size()+1];
    for(int i=0; i<fileName.size(); i++)    //string to char* conversion
        fName[i]=fileName.at(i);
    fName[fileName.size()]='\0';    //null terminator

    if((input=fopen(fName, "rb"))==NULL){ //opening the file, if file is not found return
        printf("File is not found!!\n");
        return 0;
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

    return 1;
}

////////////

void zipFileWriter(int* num, node* tree,int s, int e, string fileName, long *zipFileSize){
    FILE *input;    //main text file
    char fName[fileName.size()+1];  //input file name

    for(int i=0; i<fileName.size(); i++)    //string to char* conversion
        fName[i]=fileName.at(i);
    fName[fileName.size()]='\0';    //null terminator

    if((input=fopen(fName, "rb"))==NULL){ //opening the file, if file is not found return
        //printf("File is not found!!\n");
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

            bitToChar(output, mystack.top());

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

    if(bitPointer>1){
        fprintf(output, "%c", inputChar.character);
    }

    fclose(output);
    fclose(input);



    cout << "\n# Zip file is made.\n";

    return ;
}

/////////

void bitToChar(FILE* output, int bi){

    if(bitPointer==1) inputChar.bit.x1=bi;
    else if(bitPointer==2) inputChar.bit.x2=bi;
    else if(bitPointer==3) inputChar.bit.x3=bi;
    else if(bitPointer==4) inputChar.bit.x4=bi;
    else if(bitPointer==5) inputChar.bit.x5=bi;
    else if(bitPointer==6) inputChar.bit.x6=bi;
    else if(bitPointer==7) inputChar.bit.x7=bi;
    else if(bitPointer==8) inputChar.bit.x8=bi;

    bitPointer++;

    if(bitPointer>8){
        fprintf(output, "%c", inputChar.character);
        bitPointer=1;
    }

    return;
}

///////////////****************************************************************////////////////

void fileExtractor(node* tree, int x, FILE *input, string fileName, long *zipFileSize, int stop){
    int st=0;

    FILE *output;
    char oFileName[fileName.size()];  //output file name

    //creates a name depending on the input file name
    oFileName[0]='E';oFileName[1]='x';oFileName[2]='t'; //first three letter
    for(int i=3; i<fileName.size(); i++)    //string to char* conversion
        oFileName[i]=fileName.at(i);
    oFileName[fileName.size()]='\0';    //null terminator

    output=fopen(oFileName, "wb");

    bitPointer=9;

    char ch;
    ch=charToBit(input);
    while(!feof(input)){ //reading the file & counting the characters
        node* nod;  //for temporary use
        nod=&tree[x-1]; //the main root of tree

        while(nod->t1!=NULL){
            //cout << ch;   ///test

            *zipFileSize+=1;  //counts number of bits

            if(ch==0) nod = nod->t1;
            else nod = nod->t2;

            ch=charToBit(input);
        }
        if(nod->info == 20){
            st++;

            if(st>=stop) break;  //break when found 20th char
        }
        fprintf(output, "%c", nod->info);
        //cout << nod->info << endl;  ///test
    }

    fclose(output);

    return ;
}

///////////

char charToBit(FILE* input){   //reads a file's char and returns one bit as char each time
    char ch;

    if(bitPointer>8){
        inputChar.character = fgetc(input);
        bitPointer=1;
    }

    if(bitPointer==1) ch=inputChar.bit.x1;
    else if(bitPointer==2) ch=inputChar.bit.x2;
    else if(bitPointer==3) ch=inputChar.bit.x3;
    else if(bitPointer==4) ch=inputChar.bit.x4;
    else if(bitPointer==5) ch=inputChar.bit.x5;
    else if(bitPointer==6) ch=inputChar.bit.x6;
    else if(bitPointer==7) ch=inputChar.bit.x7;
    else if(bitPointer==8) ch=inputChar.bit.x8;

    bitPointer++;

    //cout << ch << " ";  ///test
    return ch;
}



