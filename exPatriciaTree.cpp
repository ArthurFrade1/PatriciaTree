#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef struct Node{
    char word;
    Node* right=NULL;
    Node* left=NULL;
}Node;

string dir;//Only tests

bool pesquisa(Node *atual, char value, string codigo, int count){
    if(atual==NULL)   return false; //Se na procura acabou encontrando um nó nulo

    if(value==atual->word){dir+="X"; //Se for igual é isso msm
        return true;
    }
    else if('1'==codigo[count]){dir+="D";//Se for 1 recursivamente com o atual sendo da esquerda
        count++;
        return pesquisa(atual->right, value, codigo, count);
    }
    else if('0'==codigo[count]){dir+="E";//Se for 0 recursivamente com o atual sendo da direita
        count++;
        return pesquisa(atual->left, value, codigo, count);
    }
    return false; //Caso inalcançável
    
}

Node* insere(Node *atual, char value, string codigo, int count){
    if(atual==NULL){dir+="X";//Se o ní atual não conter nada, é aqui que devo inserir o valor
        atual=new Node(); atual->word=value;//Crio um novo nó na memoria heap, pra não se perder na chamada da pilha
        atual->right=NULL; atual->left=NULL;
        return atual;
    } 

    if(value==atual->word){dir+="X"; //Se for igual é isso msm NÃO TA FUNCIONANDO
        cout<<"Valor já existente na árvore";  
    }
    else if('1'==codigo[count]){dir+="D";//Se for maior chamo recursivamente com o atual sendo da direita
        count++;
        atual->right=insere(atual->right, value, codigo, 0);
    }
    else if('0'==codigo[count]){dir+="E";//Se for menor chamo recursivamente com o atual sendo da esquerda
        count++;
        atual->left=insere(atual->left, value, codigo, 0);
    }
    return atual;  
}


int main(){
    Node root;
    root.word='A';

    string codigos[10];
    codigos[0]="11110000";
    codigos[1]="11001100";
    codigos[2]="01111111"; 
    codigos[3]="00000111";
    codigos[4]="00000011";
   
    insere(&root, 'B', codigos[0], 0);
    insere(&root, 'C', codigos[1], 0);
    insere(&root, 'Z', codigos[2], 0);
    insere(&root, 'W', codigos[3], 0);
    insere(&root, 'X', codigos[4], 0);

    dir="";
    string resp=pesquisa(&root, 'C', codigos[1], 0)?"Valor encontrado":"Valor não encontrado";
    cout<<resp<<"\n";
    cout<<"Direções percorridas: " << dir<<"\n"; 


}