#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

typedef struct ocorrencia{
    int linha, coluna;
}ocorrencia;

typedef struct palavras{
    string palavra;//Guarda palavras do arquivo
    int linha=0, coluna=0;
    vector<ocorrencia> oc;
}palavras;


typedef struct Node{
    //Conjunto usado em caso de nó referencia
    int index; //usado para guardar o indice que difere os dois filhos
    bool ref=false; 

    //Usado em caso de nó de nó folha
    palavras word;
    string codigo;
    Node* right=NULL;
    Node* left=NULL;

}Node;
    Node *ocurr; 


string dir;//Only tests
unordered_map<char, string> hashMap;
string letras[40];
Node root;

int posicao;
bool toDo=false;
Node* insereToDo(Node *atual, Node* no);

void filler(){// Preencher o mapa com os códigos binários corretos para caracteres ASCII visíveis
 
    // Preencher o mapa com os códigos binários corretos para caracteres ASCII visíveis
    for (int i = 0; i < 128; i++) {
        char character = static_cast<char>(i);
        std::string binaryCode;

        for (int j = 7; j >= 0; j--) {
            if (i & (1 << j)) {
                binaryCode += "1";
            } else {
                binaryCode += "0";
            }
        }

        hashMap[character] = binaryCode;
    }
    // Imprimir o mapa
    // for (const auto& pair : hashMap) {
    //     std::cout << "Caractere: '" << pair.first << "', Código Binário: " << pair.second << std::endl;
    // }
}


palavras words[5000];
int conta=0;//pra saber em que palavra está

void readWords(string conteudo) {
    int linhas=0;
    string word; // Variável para armazenar a palavra atual
    for (int i = 0; i < conteudo.length(); i++) {
        char caractere = conteudo[i];
        if (caractere != ' ' && caractere != '\t' && caractere != '\n' && caractere != '.' && caractere != ',' && caractere != '!') {
            // Adicione o caractere à palavra atual
            word += caractere;
        } else {
            // Se encontrarmos um caractere de separação, verifique se a palavra não está vazia
            if (!word.empty()) {
                // Adicione a palavra válida ao vetor palavras e redefina a palavra
                words[conta].palavra = word;
                words[conta].linha=linhas;

                conta++;
                word = "";
            }
            if(caractere=='\n') linhas++;
        }
    }

    // Verifique se a última palavra não está vazia
    if (!word.empty()) {
        words[conta].palavra = word;
        conta++;
    }
}

int barraenes=0;
void openFile(){
    std::ifstream arquivo("exemplo1.txt");
    std::string conteudo;

    if (!arquivo.is_open()) {
        std::cout << "Erro ao abrir o arquivo.\n";
        return ;
    }

    arquivo.seekg(0, std::ios::end);
    conteudo.reserve(arquivo.tellg());
    arquivo.seekg(0, std::ios::beg);

    conteudo.assign(
        (std::istreambuf_iterator<char>(arquivo)),
        std::istreambuf_iterator<char>()
    );
    std::cout << "Conteúdo do arquivo:\n" << conteudo << std::endl;

    readWords(conteudo);

        for (int i = 0; i < conteudo.length(); i++) if(conteudo[i]=='\n') barraenes++;

}

bool pesquisa(Node *atual, palavras word){
    if(atual==NULL)  return false; //Se na procura acabou encontrando um nó nulo

    // string codigo=hashMap[word];
    for(int i=word.palavra.length(); i< 16 ; i++){
        word.palavra+=" ";
    }

    string codigo="";
    for(int i=0; i< 16 ; i++){
        char temp=word.palavra[i];
        codigo+=hashMap[temp];
    }
    
    if(atual->ref){//Se o atual for um tipo referencia
        if('1'==codigo[atual->index-1]){dir+="D";//Se for 1 recursivamente com o atual sendo da esquerda
            return pesquisa(atual->right, word);
        }
        else if('0'==codigo[atual->index-1]){dir+="E";//Se for 0 recursivamente com o atual sendo da direita
            return pesquisa(atual->left, word);
        }
    }
    else{  //Se não é referencia logo é um folha, provido de valor, se for o procurado achou, se não é, como não existem filhos a partir desse ponto então não encontrou
        if(word.palavra==atual->word.palavra){dir+="X"; //Se for igual é isso msm
            ocurr=atual;
            
            return true;
        }else{dir+='F';
            return false;
        }
    }
    return false; //Caso inalcançável
    
}


Node* insere(Node *atual, Node* no);

void fun(Node *atual, palavras word){
    //Inserindo espaços em branco até ter 16 caracteres
    for(int i=word.palavra.length(); i< 16 ; i++){
        word.palavra+=" ";
    }

    string codigo="";
    for(int i=0; i< 16 ; i++){
        char temp=word.palavra[i];
        codigo+=hashMap[temp];
    }



    Node *no=new Node(); no->word.palavra=word.palavra;//Crio um novo nó na memoria heap, pra não se perder na chamada da pilha
    no->right=NULL; no->left=NULL; no->codigo=codigo; no->word.linha=word.linha;
    insere(atual, no);
}

Node* insere(Node *atual, Node* no){


    toDo=false;
    if(atual==NULL){dir+="X";//Se o ní atual não conter nada, é aqui que devo inserir o valor
        atual=no;//Crio um novo nó na memoria heap, pra não se perder na chamada da pilha
        ocorrencia oc; oc.linha=atual->word.linha;
        atual->word.oc.push_back(oc);
        return atual;
    } 
    if(atual->ref){//Se o atual for um tipo referencia
        if('1'==no->codigo[atual->index-1]){//dir+="D";//Se for 1 recursivamente com o atual sendo da esquerda
            atual->right= insere(atual->right, no);
        }
        else if('0'==no->codigo[atual->index-1]){//dir+="E";//Se for 0 recursivamente com o atual sendo da direita
            atual->left= insere(atual->left, no);
        }
    }
    else{  
        if(atual->word.palavra==no->word.palavra){//Se for igual é isso msm
            cout<<"Valor já existente na árvore\n";  
            //Ajeito os campos referentes à ocorrencias
            // no->vezes++; CONTADOR DE VEZES EM QUE A PALAVRA APARECE
            ocorrencia ocorr;          ocorr.linha=no->word.linha;
            atual->word.oc.push_back(ocorr);
        }else{//Inicia-se o processo de descobrir o pnrimeiro bit diferente entre eles
            int i=0;//Começo na posiçãp da análise de bir atual    
            while(atual->codigo[i]==no->codigo[i]){
                i++;
            }//Vai sair desse loop assim que essa regra for quebrada, ou seja quando atual->codigo[i]!=codigo[i] pela primeira vez
            i++;
            //cout<<"O bit que os difere eh o :"<<i<<"\n";

            //Simbolizando que o procedimento precisa ser feito
            toDo=true;
            posicao=i;

            
        }
    }
    if(atual==&root && toDo){
        insereToDo(&root, no);
        
    }
    return atual;  
}

Node* insereToDo(Node *atual, Node * no){
 
    if(atual->ref){//Se o atual for um tipo referencia
        if(atual->index>posicao){//Se encontrou um referencia maior
            //Hora do procedimento
            Node *temp=atual;
            atual=new Node();
            atual->ref=true;
            atual->index=posicao;

              //Olhar esse bit diferente no nó atual para ver se vai reinserir esse nó a direita ou a à esquerda do novo
            if(no->codigo[posicao-1]=='0'){
                atual->left=  insere(atual->left, no);
                atual->right=temp;//lado com o resto da arvore
            }
            if(no->codigo[posicao-1]=='1'){
                atual->right=  insere(atual->right, no);
                atual->left==temp;//lado com o resto da arvore
            }
        }
        else{
            if('1'==no->codigo[atual->index-1]){//dir+="D";//Se for 1 recursivamente com o atual sendo da esquerda
                atual->right= insereToDo(atual->right, no);
            }
            else if('0'==no->codigo[atual->index-1]){//dir+="E";//Se for 0 recursivamente com o atual sendo da direita
                atual->left= insereToDo(atual->left, no);
            }
        }
    }else{//Se encontrar um folha devo trocar aqui msm, seria o caso padrão da versão anterior
        Node *temp=atual;
        atual=new Node();
        atual->ref=true;
        atual->index=posicao;

        if(no->codigo[posicao-1]=='0'){
            atual->left=  insere(atual->left, no);
            atual->right= insere(atual->right, temp);//lado com o folha que estava no lugar
        }
        if(no->codigo[posicao-1]=='1'){
            atual->right=  insere(atual->right, no);
            atual->left=   insere(atual->left, temp);//lado com o folha que estava no lugar
        }
    }

    return atual;
}


int main(){
    filler();
    openFile();

    root.ref=true;
    root.index=1;

    //Colocando na árvore as palavras do arquivo lido
    for(int i=0; i<conta; i++){
        // cout<<words[i].palavra<<"\n";
        fun(&root, words[i]);
    }

    int find=0, nfind=0;

    //Pesquisa de todas as palavas contidas no arquivo
    // for(int i=0; i<conta; i++){
    //     dir="";
    //     string resp; cout<<words[i].palavra;
    //     if(pesquisa(&root, words[i])){
    //         resp=" encontrado";
    //         find++;
    //     }else{
    //         resp=" não foi encontrado";
    //         nfind++;
    //     }
    //     cout<<resp<<"\n";
    //     // cout<<"primeira ocorrencia na linha: "<<linha<<"\n";
    //     vector< ocorrencia> oc=ocurr->word.oc;
    //     cout<<"Ocorrencias: "<<"\n";
    //     for(int j=0; j<oc.size(); j++){
    //         cout<<oc[j].linha<<"\n";
    //     }
    //     cout<<"Direções percorridas: " << dir<<"\n"; 
    // }

    //Palavras para a pesquisa dos exemplos do professor
    string words[] = {
    "trabalho", "computacao", "governo", "educacao", "tecnologia", 
    "formacao", "desenvolvimento", "que", "informatica", "em", "crise"
};

    //Para pesqusar as palavras para a pesquisa dos exemplos do professor
    for(int i=0; i<11; i++){
        dir="";
        string resp; cout<<words[i];
        palavras pala; pala.palavra=words[i];
        if(pesquisa(&root, pala)){
            resp=" encontrado";
            find++;
        }else{
            resp=" não foi encontrado";
            nfind++;
        }
        cout<<resp<<"\n";
        // vector< ocorrencia> oc=ocurr->word.oc;
        // cout<<"Ocorrencias: "<<"\n";
        // for(int j=0; j<oc.size(); j++){
        //     cout<<oc[j].linha<<"\n";
        // }
        cout<<"Direções percorridas: " << dir<<"\n"; 
    }

    //Estatisticas finais
    cout<<"Palavras: "<<conta<<"\n"; 
    cout<<"Encontrados: "<<find<<"\n";
    cout<<"Não encontrados: "<<nfind<<"\n";

    cout<<"Barra enes: "<<barraenes;
}
