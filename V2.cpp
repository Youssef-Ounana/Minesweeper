#include <iostream>
#include <cstdlib>
#include <ctime>


//la seule difference c'est que J'ai fait le main du main.cpp dans une fonction

using namespace std;

struct matrice{   //La structure pour cree les matrice 
    int nbl;
    int nbc;
    int * *T;     //pointeur de pointeur ou tableau de pointeur pour allouer un tableau de n pointeurs sur entiers 
};

//la fonction qui permet de créer la structure d'une matrice 


int * *cree_tab2D(int n,int m){
    int * *p=new int *[n];
    for (int i=0;i<n;i++) p[i]=new int[m];    
    return p;          //p[i][j] par example la case de ligne i et colonne j
}



//pour cree une matrice 
matrice *creer_matrice(int n, int m){
    matrice *p=new matrice;
    (*p).nbl=n;                     
    (*p).nbc=m;
    (*p).T=cree_tab2D(n,m);            //matrice de n ligne et m colonne
    return p;
}


//avec cette fonction tout les case de la matrice seront 0
void initZero(matrice &J){
    for(int i=0;i<J.nbl;i++){
        for (int j=0;j<J.nbc;j++) 
            J.T[i][j]=0;
    }
}


//le nombre des mine depend de la diffuculté donnée par l'utilisateur
int nbbombe(int d){
    int b;
    if (d==1) b=15;
    if (d==2) b=25;
    if (d==3) b=40;
    return b;
}



matrice Init_jeu(int n, int m, int d){
    int b,i,j,k=0;
    srand(time(NULL));              
    matrice *J=creer_matrice(n,m);          //creation de la matrice J
    initZero(*J);                          
    b=nbbombe(d);                           //determination de nombre des bombes

    //pour placer les bombe dans la matrice 

    /*do{
    	i=rand() % n;
        j=rand() % m;
		if (J->T[i][j]!=2){
            J->T[i][j]=2;
            k++;
        }
    }while(k!=b);
    */

   /*
    while(k<b){
        i=rand() % n;
        j=rand() % m;
        if(J->T[i][j]!=2){
            J->T[i][j]=2;
            k++;
        }
    }
    */


   while(b!=0){
       i=rand() % n;
        j=rand() % m;
        if(J->T[i][j]!=2){
            J->T[i][j]=2;
            b--;
        }  
   }  
    return *J;
}

//pour calculer le nombre des mines autour de chaque case qui contien pas de mine
matrice mine_autour(int n, int m, matrice &J){
    matrice *M=creer_matrice(n,m);
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int m_autour=0;
            if (J.T[i][j]!=2 && i>0 && j>0 && J.T[i-1][j-1] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && i>0  && J.T[i-1][j] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && i>0  && j<m-1 && J.T[i-1][j+1] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && j>0 && J.T[i][j-1] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && j<m-1 && J.T[i][j+1] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && i<n-1 && j>0 && J.T[i+1][j-1] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && i<n-1 && J.T[i+1][j] == 2)
                m_autour++;
            if (J.T[i][j]!=2 && i<n-1 && j<m-1 && J.T[i+1][j+1] == 2)
                m_autour++;
            (*M).T[i][j]=m_autour;
        }
    }
    return (*M);
}

//foction pour afficher la matrice
void affichage (matrice &J, matrice *M){
    for(int j=-1;j<J.nbc;j++){
        if (j==-1) cout <<" | ";
        else cout<<j<<" | ";
    }
    
    cout<<endl;
    for (int i=0; i<J.nbl; i++){
        for (int j=-1; j<J.nbc; j++){
            if (j==-1) cout<<i<<"|";
            if (J.T[i][j]==0) cout<<" ? |";
            if (J.T[i][j]==2) cout<<" ? |";
            if (J.T[i][j]==3) cout<<" d |";
            if (J.T[i][j]==4) cout<<" d |"; 
            if (J.T[i][j]==1) cout<<" "<<M->T[i][j]<<" |"; 
        }
        cout<<endl;
    }
}

void poser_drapeau(matrice &J, int i, int j)
{
    if (J.T[i][j] == 0)
        J.T[i][j]= 3;
    else if (J.T[i][j] == 2)
        J.T[i][j]= 4;
}


void lever_drapeau(matrice &J, int i, int j)
{
    if (J.T[i][j] == 3)
        J.T[i][j]= 0;
    else if (J.T[i][j]== 4)
        J.T[i][j]= 2;
}

//pour creuser une case
bool creuser(int i, int j, int & N, matrice *J){
    if (i<0 || i>=J->nbl || j<0 || j>=J->nbc){    //si la case n'existe pas
        return true;
    }
    else{
        switch (J->T[i][j])
        {
            case 2:                     //contien mine
                return false;
            case 3:
            case 4:                     //contien un drapeau
                return true;
            case 1:                     //deja creuser
                return true;
            case 0:                     //n'est pas creuser et ne contien de mine
                J->T[i][j]=1;           //la case ca contien 1 donc on va la creuser 
                N++;                    //nombre de case decouverte va augmenté
                return false;
        }
    }
}


//creuser recusivement càd creuse la case les case autour si le nombre de mines autour est 0
bool creuser_recu(int i, int j, int & N, matrice *J,matrice *M){
    if(J->T[i][j]==0){              //si la case n'est pas creuse 
        creuser(i,j,N,J);           //on va la creuse et return false car dans la fct creuser on return false si on creuse
        if(M->T[i][j]==0){          //s'il y a pas de mines autour
            if(i>0 && j>0) creuser_recu(i-1,j-1,N,J,M);           //les if contien les condition pour qu'une case exist 
            if(i>0) creuser_recu(i-1,j,N,J,M);
            if(i>0 && j<J->nbc-1) creuser_recu(i-1,j+1,N,J,M);
            if(j>0) creuser_recu(i,j-1,N,J,M);
            if(j<J->nbc-1) creuser_recu(i,j+1,N,J,M);
            if(j>0 && i<J->nbl-1) creuser_recu(i+1,j-1,N,J,M);
            if(i<J->nbl-1) creuser_recu(i+1,j,N,J,M);
            if(i>J->nbl-1 && j<J->nbc-1) creuser_recu(i+1,j+1,N,J,M);
        }
    }
}

void jouer(int dif){
    int action,i,j;
    bool game_over=false;   //le jeux n'est pas encore terminer
    int N=0;                //le nbr de mines decouvet est 0
    int joker=1;           //l'utlisateur a le droit d'un seul joker
    int score=100;
    time_t deb,fin;       
    deb=time(0);          
    cout<<"-----------------Jeux Demineur-------------------"<<endl;
    cout<<"Par: OUNANA Youssef MI-I3  (◠﹏◠)"<<endl;
    cout<<"-----------------Jeux Demineur-------------------"<<endl;
    cout<<"il vous rest "<<joker<<"  joker  "<<endl;

    int case_lib=(10*10)-nbbombe(dif);
    matrice J=Init_jeu(10,10,dif);
    matrice M=mine_autour(10,10,J);
    int nb_drapeau=nbbombe(dif);
    affichage(J,&M);
    cout<<endl;
    cout<<"---Tapez 1 pour creuser, 2 pour poser un drapeau ou 3 pour lever un drapeau-------------------"<<endl;
    while(!game_over){
        cout<<"1 ou 2 ou 3 --> ";
        cin>>action;
        if (action==1){
            if (joker!=0){
                int cj;
                cout<<"vous voulez utiliser les joker? si oui tapez 1 sinon tapez 2: ";
                cin>>cj;
                while(cj!=1 && cj!=2){
                    cout<<"vous voulez utiliser les joker? si oui tapez 1 sinon tapez 2: ";
                    cin>>cj;
                }
                if (cj==1){
                    cout<<"entrez les coordonnées souhaité i et j --->";
                    cin>>i>>j;
                    creuser(i,j,N,&J);
                    joker--;
                    score=score-20;
                }
                else{
                    cout<<"entrez les coordonnées souhaité i et j --->";
                    cin>>i>>j;
                    creuser_recu(i,j,N,&J,&M);
                    if(J.T[i][j]==2){
                        cout<<"--------Game Over---------";
                        game_over=true;
                        score=0;
                        break;
                    }
                }
            }
            else{
                cout<<"entrez les coordonnées souhaité i et j --->";
                cin>>i>>j;
                creuser_recu(i,j,N,&J,&M);
                if(J.T[i][j]==2){
                    cout<<"--------Game Over---------";
                    game_over=true;
                    score=0;
                    break;
                }
            }
        }
        if (action==2){
            cout<<"entrez les coordonnées souhaité i et j --->";
            cin>>i>>j;
            poser_drapeau(J,i,j);
            nb_drapeau--;
        }
        if (action==3){
            cout<<"entrez les coordonnées souhaité i et j --->";
            cin>>i>>j;
            lever_drapeau(J,i,j);
            nb_drapeau++;
        }
        if(N>=case_lib){
            cout<<"------nadi canadi-------";
            game_over=true;
            break;
        }
        cout<<endl;
        cout<<"---------------------------------------------"<<endl;
        cout<<"il vous rest "<<joker<<"  joker";
        cout<<endl;
        affichage(J,&M);
        cout<<endl;
        cout<<"---------------------------------------------";
    }
    fin= time(0);
    int tmp_passe=fin-deb;
    cout<<"vous avez passé : "<<tmp_passe<<" dans le jeux"<<endl;
    if(score!=0){
        if(joker==0)  score=score+N-(tmp_passe/2);
        else score=score+N+100-(tmp_passe/2);
        cout<<"votre score est: "<<score<<"  ( ͡° ͜ʖ ͡°)";
    }
    else{
        cout<<"votre score est: 0  (._.)";
    }
}


int main(){
    int dif;
    char re;
    bool rejoué;
    cout<<"entrez une difficulte 1 2 ou 3:   ";
    cin>>dif;
    while(rejoué){
        jouer(dif);
        cout<<endl;
        cout<<"tu veux rejoue y/n:   ";
        cin>>re;
        if(re=='n' || re=='N') rejoué=false;
    }
    
}