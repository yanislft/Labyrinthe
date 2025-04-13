#include <ncurses.h>
#include "labIO.h"
#include<iostream>
#include<fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

//g++ votre_prog.cpp labIO.cpp -o votre_prog -lncurses

void init(Case & c)
{
	c.N=c.S=c.E=c.W=false;
	c.etat=false;
}

void init_laby(laby & L, int p, int q)
{
	L.p=p; L.q=q;
	L.tab=new Case *[p];
	for(int i=0;i<p;i++) L.tab[i]=new Case[q];
	for(int i=0;i<p;i++)
	{
		for(int j=0;j<q;j++) init(L.tab[i][j]);
	}
}

laby *lire_Laby(const char  *filename)
{
	ifstream my_cin;

	my_cin.open(filename);

	laby *L;
	L=new laby;
	my_cin>>(*L).p>>(*L).q;
	init_laby((*L),(*L).p,(*L).q);

	for(int i=0;i<(*L).p;i++)
	{
		for(int j=0;j<(*L).q;j++)
		{
			my_cin>>(*L).tab[i][j].W>>(*L).tab[i][j].N>>(*L).tab[i][j].S>>(*L).tab[i][j].E;
		}
	}
	return L;
}

void init_pile(pile & p, int taille) 
{
	p.taille=taille; 
	p.nbe=0;
	p.T=new couple[taille];
}

bool empiler(pile & p, couple c) 
{
	if((p.nbe)<p.taille) 
	{
		p.T[p.nbe]=c; (p.nbe)++; return true;
	}
	
	else {exit(1);};
}

bool vide(pile & P) 
{
	if(P.nbe==0) return true;
	else return false; 
}

couple depiler(pile & p) 
{
	if(p.nbe>0) 
	{
		p.nbe--;
		return(p.T[p.nbe]); 
	}

	else {exit(1);}
}

bool element_present_dans_pile(pile &p, couple &c)
{
	for (int i = 0; i < p.nbe; i++)
	{
		if (p.T[i].i == c.i && p.T[i].j == c.j) {return (true);}
	}
	return (false);
}

couple *test_cases_voisines(couple &c, int &a, laby &L, pile &pi)
{
	//Création d'un tableau de couple qui contiendra les cases voisines non visitées 
	couple *tab_cas_vois_non_vis = new couple[4];

	//Création du couple avec lequel on va effectuer les tests 
	couple c2;
	
	int cpt_nb_cas_vois = 0;

	//Tests pour chaque directions 
	if(c.i > 0 && L.tab[c.i][c.j].N == true)
	{
		c2.i = c.i-1;
		c2.j = c.j;
		
		if (element_present_dans_pile(pi,c2) == false)
		{
			tab_cas_vois_non_vis[cpt_nb_cas_vois].i = c2.i;
			tab_cas_vois_non_vis[cpt_nb_cas_vois].j = c2.j;
			cpt_nb_cas_vois++;
		}
	}
	
	if (c.i < L.p-1 && L.tab[c.i][c.j].S == true)
	{
		c2.i = c.i+1;
		c2.j = c.j;

		if (element_present_dans_pile(pi,c2) == false)
		{
			tab_cas_vois_non_vis[cpt_nb_cas_vois].i = c2.i;
			tab_cas_vois_non_vis[cpt_nb_cas_vois].j = c2.j;
			cpt_nb_cas_vois++;
		}
	}

	if (c.j > 0 && L.tab[c.i][c.j].W == true)
	{
		c2.i = c.i;
		c2.j = c.j-1;
		
		if (element_present_dans_pile(pi,c2) == false)
		{
			tab_cas_vois_non_vis[cpt_nb_cas_vois].i = c2.i;
			tab_cas_vois_non_vis[cpt_nb_cas_vois].j = c2.j;
			cpt_nb_cas_vois++;
		}
	}

	if (c.j < L.q-1 && L.tab[c.i][c.j].E == true)
	{
		c2.i = c.i;
		c2.j = c.j+1;

		if (element_present_dans_pile(pi,c2) == false)
		{
			tab_cas_vois_non_vis[cpt_nb_cas_vois].i = c2.i;
			tab_cas_vois_non_vis[cpt_nb_cas_vois].j = c2.j;
			cpt_nb_cas_vois++;
		}
	}
	//Retour par variable du nombre de direction 
	a = cpt_nb_cas_vois;

	//Retour de chacune des directions possibles via le tableau 
	return(tab_cas_vois_non_vis);
}

laby *My_Creation(int p, int q)
{
	//Crétaion + Initialisation du labyrinth
	laby *L;
	L = new laby;
	init_laby(*L, p, q);

	//Création + initialision de la pile
	pile pi;
	init_pile(pi, p*q);
	
	//Création + Empilement + Exploration de la case de départ
	couple dep;
	dep.i = 0;
	dep.j = 0;

	empiler(pi,dep);

	L->tab[dep.i][dep.j].etat = true;
	
	while (vide(pi) == false)
	{
		//Création de c1 à qui on donne la valeur du dernier élément empliler + dépiler
		couple c1 = *pi.T;
		*pi.T = depiler(pi);

		//Test pour connaître le nombre des cases accessibles non explorées à partir de c1
		bool poss_n = false, poss_s = false, poss_e = false, poss_w = false;
		int cpt_nb_pos = 0;

		if (c1.i > 0 && L->tab[c1.i-1][c1.j].etat == false) {poss_n = true; cpt_nb_pos ++;}
		if (c1.i < L->p-1 && L->tab[c1.i+1][c1.j].etat == false) {poss_s = true; cpt_nb_pos ++;}
		
		if (c1.j > 0 && L->tab[c1.i][c1.j-1].etat == false) {poss_w = true; cpt_nb_pos ++;}
		if (c1.j < L->q-1 && L->tab[c1.i][c1.j+1].etat == false) {poss_e = true; cpt_nb_pos ++;}
		

		//S'il y a au moins une case accessible 
		if (cpt_nb_pos > 0)
		{
			//Création d'un tableau de char pour savoir quelles sont les directions possibles
			char dir_poss[cpt_nb_pos];

			for (int i = 0; i < cpt_nb_pos; i++ )
			{
				if (poss_n == true) {dir_poss[i] = 'n'; i++;}
				if (poss_s == true) {dir_poss[i] = 's'; i++;}
				if (poss_e == true) {dir_poss[i] = 'e'; i++;}
				if (poss_w == true) {dir_poss[i] = 'w'; i++;}
			}
			
			//Choix de la direction à prendre
			char choix_dir = dir_poss[rand()%cpt_nb_pos];
	
			//Création + intialisation de la prochaine case à ouvrir
			couple c2 = c1;

			if (choix_dir == 'n') {c2.i--;}
			if (choix_dir == 's') {c2.i++;}

			if (choix_dir == 'e') {c2.j++;}
			if (choix_dir == 'w') {c2.j--;}
			
			//Ouverture des portes entre les deux cases
			if (choix_dir == 'n')
			{
				L->tab[c1.i][c1.j].N = true;
				L->tab[c2.i][c2.j].S = true;
			}

			if (choix_dir == 's')
			{
				L->tab[c1.i][c1.j].S = true;
				L->tab[c2.i][c2.j].N = true;
			}

			if (choix_dir == 'e')
			{
				L->tab[c1.i][c1.j].E = true;
				L->tab[c2.i][c2.j].W = true;
			}

			if (choix_dir == 'w')
			{
				L->tab[c1.i][c1.j].W = true;
				L->tab[c2.i][c2.j].E = true;
			}

			//Empilment des deux cases
			empiler(pi,c1);
			empiler(pi,c2);

			//Case vers laquelle on a ouvert la porte explorée
			L->tab[c2.i][c2.j].etat = true;
		}

		//Retour à la case précédente s'il n'y a pas de case voisine non visitée 
        if (cpt_nb_pos == 0)
        {
            if (vide(pi) == false)
            {
                c1 = depiler(pi);
				empiler(pi,c1);	
            }
        }

		//Fin du labyrinth lorsqu'on revient à la case de départ et qu'il n'y a plus de possibilités
		if (c1.i == dep.i && c1.j == dep.j && cpt_nb_pos == 0) {break;}
	}
	return(L);
}

pile *explorer_v1(laby &L)
{
	//Création + initialisation de la pile 
	pile *pi;
	pi = new pile;
	init_pile(*pi, L.p * L.q);
	
	//Création + Empilement + Exploration de la case de départ
	couple dep;
	dep.i = 0;
	dep.j = 0;

	empiler(*pi, dep);

	L.tab[dep.i][dep.j].etat2 = true;

	//Création de la case d'arrivée
	couple dest;
	dest.i = L.p-1;
	dest.j = L.q-1;

	while (vide(*pi) == false)
	{
		//Création de c1 à qui on donne la valeur du dernier élément empliler + dépiler
		couple c1 = *pi->T;
		*pi->T = depiler(*pi);

		// Test pour connaître le nombre des cases accessibles non explorées à partir de c1
		bool poss_n = false, poss_s = false, poss_e = false, poss_w = false;
		int cpt_nb_pos = 0;

		if (c1.i > 0 && L.tab[c1.i][c1.j].N == true && L.tab[c1.i-1][c1.j].etat2 == false) {poss_n = true; cpt_nb_pos ++;}
		if (c1.i < L.p-1 && L.tab[c1.i][c1.j].S == true && L.tab[c1.i+1][c1.j].etat2 == false) {poss_s = true; cpt_nb_pos ++;}
		
		if (c1.j > 0 && L.tab[c1.i][c1.j].W == true && L.tab[c1.i][c1.j-1].etat2 == false) {poss_w = true; cpt_nb_pos ++;}
		if (c1.j < L.q-1 && L.tab[c1.i][c1.j].E == true && L.tab[c1.i][c1.j+1].etat2 == false) {poss_e = true; cpt_nb_pos ++;}

		//S'il y a au moins une case accessible
		if (cpt_nb_pos > 0)
		{
			//Création d'un tableau de char pour savoir quelles sont les directions possibles
			char dir_poss[cpt_nb_pos];

			for (int i = 0; i < cpt_nb_pos; i++ )
			{
				if (poss_n == true) {dir_poss[i] = 'n'; i++;}
				if (poss_s == true) {dir_poss[i] = 's'; i++;}
				if (poss_e == true) {dir_poss[i] = 'e'; i++;}
				if (poss_w == true) {dir_poss[i] = 'w'; i++;}
			}
			
			//Choix de la direction à prendre
			char choix_dir = dir_poss[rand()%cpt_nb_pos];

			//Création + intialisation de la prochaine case à être empilée
			couple c2 = c1;

			if (choix_dir == 'n') {c2.i--;}
			if (choix_dir == 's') {c2.i++;}

			if (choix_dir == 'e') {c2.j++;}
			if (choix_dir == 'w') {c2.j--;}

			//Empiler c1 et c2
			empiler(*pi,c1);
			empiler(*pi,c2);

			//Exploration de la case c2
			L.tab[c2.i][c2.j].etat2 = true;

			//Fin de la boucle lorsque la case de destination est empilée
			if (c2.i == dest.i && c2.j == dest.j) {break;}
		}

		//S'il n'y a pas de case accessible
		if (cpt_nb_pos == 0)
		{
			//Retour à la case précédente
			if (vide(*pi) == false)
            {
                c1 = depiler(*pi);
            	empiler(*pi, c1);
            }
		}
	}
	return(pi);
}

pile *explorer2_v1(laby &L)
{
	//Création + initialisation de la pile 
	pile *pi;
	pi = new pile;
	init_pile(*pi, L.p * L.q);
	
	//Création + Empilement + Exploration de la case de départ
	couple dep;
	dep.i = 0;
	dep.j = 0;

	empiler(*pi, dep);
	Mark_Case(dep.i, dep.j);

	L.tab[dep.i][dep.j].etat2 = true;

	//Création de la case d'arrivée
	couple dest;
	dest.i = L.p-1;
	dest.j = L.q-1;

	while (vide(*pi) == false)
	{
		//Création de c1 à qui on donne la valeur du dernier élément empliler + dépiler
		couple c1 = *pi->T;
		*pi->T = depiler(*pi);
		UMark_Case(c1.i, c1.j);

		// Test pour connaître le nombre des cases accessibles non explorées à partir de c1
		bool poss_n = false, poss_s = false, poss_e = false, poss_w = false;
		int cpt_nb_pos = 0;

		if (c1.i > 0 && L.tab[c1.i][c1.j].N == true && L.tab[c1.i-1][c1.j].etat2 == false) {poss_n = true; cpt_nb_pos ++;}
		if (c1.i < L.p-1 && L.tab[c1.i][c1.j].S == true && L.tab[c1.i+1][c1.j].etat2 == false) {poss_s = true; cpt_nb_pos ++;}
		
		if (c1.j > 0 && L.tab[c1.i][c1.j].W == true && L.tab[c1.i][c1.j-1].etat2 == false) {poss_w = true; cpt_nb_pos ++;}
		if (c1.j < L.q-1 && L.tab[c1.i][c1.j].E == true && L.tab[c1.i][c1.j+1].etat2 == false) {poss_e = true; cpt_nb_pos ++;}

		//S'il y a au moins une case accessible
		if (cpt_nb_pos > 0)
		{
			//Création d'un tableau de char pour savoir quelles sont les directions possibles
			char dir_poss[cpt_nb_pos];

			for (int i = 0; i < cpt_nb_pos; i++ )
			{
				if (poss_n == true) {dir_poss[i] = 'n'; i++;}
				if (poss_s == true) {dir_poss[i] = 's'; i++;}
				if (poss_e == true) {dir_poss[i] = 'e'; i++;}
				if (poss_w == true) {dir_poss[i] = 'w'; i++;}
			}
			
			//Choix de la direction à prendre
			char choix_dir = dir_poss[rand()%cpt_nb_pos];

			//Création + intialisation de la prochaine case à être empilée
			couple c2 = c1;

			if (choix_dir == 'n') {c2.i--;}
			if (choix_dir == 's') {c2.i++;}

			if (choix_dir == 'e') {c2.j++;}
			if (choix_dir == 'w') {c2.j--;}

			//Empiler c1 et c2
			empiler(*pi,c1);
			Mark_Case(c1.i, c1.j);

			empiler(*pi,c2);
			Mark_Case(c2.i, c2.j);

			//Exploration de la case c2
			L.tab[c2.i][c2.j].etat2 = true;

			//Fin de la boucle lorsque la case de destination est empilée
			if (c2.i == dest.i && c2.j == dest.j) {break;}
		}

		//S'il n'y a pas de case accessible
		if (cpt_nb_pos == 0)
		{
			//Retour à la case précédente
			if (vide(*pi) == false)
            {
                c1 = depiler(*pi);
            	empiler(*pi, c1);
            }
		}
	}
	return(pi);
}

pile *explorer_v2(laby &L)
{
	//Création + Initialisation de la pile contenant le chemin vers la case de destination
	pile *pi;
	pi = new pile;
	init_pile(*pi, L.p*L.q);

	//Création + Initialisation de la pile contenant les cases visitées
	pile *pi_v;
	pi_v = new pile;
	init_pile(*pi_v, L.p*L.q);

	//Création de la case de dépârt + de la case de destination
	couple dep;
	dep.i = 0;
	dep.j = 0;

	couple dest;
	dest.i = L.p-1;
	dest.j = L.q-1;

	//Création du couple qui va parcourir le chemin + Initialisation à la position de départ + Empilement dans les deux piles 
	couple c;
	c.i = 0;
	c.j = 0;

	empiler(*pi,c);
	empiler(*pi_v,c);

	//Création du repère de case voisine non visitée
	int repere_cas_vois = 1;

	//Tant que la case qui parcour le labyrinth n'est pas égale à la case de destination
	while(repere_cas_vois > 0)
	{
		//Création d'un tableau de couple qui contient les cases vosines non visitées 
		couple *tab_cases_vois = test_cases_voisines(c,repere_cas_vois,L,*pi_v);

		//S'il y a au moins une case voisine non visitée
		if (repere_cas_vois > 0)
		{
			//Choix aléatoire de la case vosine dans le tableau
			int choix_cas_vois = rand() % repere_cas_vois;

			c = tab_cases_vois[choix_cas_vois];

			//Empilement de la case dans les deux piles
			empiler(*pi, c);
			empiler(*pi_v, c);

			if (c.i == dest.i && c.j == dest.j) {break;}
		}

		//S'il n'y à pas de case voisine non visitée
		if (repere_cas_vois == 0)
		{
			//On dépile c de la pile content le chemin 
			pi->nbe--;
			c = depiler(*pi);
			empiler(*pi, c);
			repere_cas_vois = 1;
		}
	}
	return (pi);
}

pile *explorer2_v2(laby &L)
{
	//Création + Initialisation de la pile contenant le chemin vers la case de destination
	pile *pi;
	pi = new pile;
	init_pile(*pi, L.p*L.q);

	//Création + Initialisation de la pile contenant les cases visitées
	pile *pi_v;
	pi_v = new pile;
	init_pile(*pi_v, L.p*L.q);

	//Création de la case de déârt + de la case de destination
	couple dep;
	dep.i = 0;
	dep.j = 0;

	couple dest;
	dest.i = L.p-1;
	dest.j = L.q-1;

	//Création du couple qui va parcourir le chemin + Initialisation à la position de départ + Empilement dans les deux piles 
	couple c;
	c.i = 0;
	c.j = 0;

	empiler(*pi,c);
	empiler(*pi_v,c);

	//Ceréation du repère de cases voisines non visitée
	int repere_cas_vois = 1;

	//Tant que la case qui parcour le labyrinth n'est pas égale à la case de destination
	while(repere_cas_vois > 0)
	{
		//Création d'un tableau de couple qui contient les cases vosines non visitées 
		couple *tab_cases_vois = test_cases_voisines(c,repere_cas_vois,L,*pi_v);

		//S'il y a au moins une case voisine non visitée
		if (repere_cas_vois > 0)
		{
			//Choix aléatoire de la case vosine dans le tableau
			int choix_cas_vois = rand() % repere_cas_vois;

			c = tab_cases_vois[choix_cas_vois];

			//Empilement de la case dans les deux piles
			empiler(*pi, c);
			empiler(*pi_v, c);

			if (c.i == dest.i && c.j == dest.j) {break;}
		}

		//S'il n'y à pas de case voisine non visitée
		if (repere_cas_vois == 0)
		{
			//On dépile c de la pile content le chemin 
			pi->nbe--;
			c = depiler(*pi);
			empiler(*pi, c);
			repere_cas_vois = 1;
		}
	}
	//Retour de la pile pi_v pour conaitre les différentes tentatuves de l'ordi
	return (pi_v);
}

void afficher_chemin(pile &P,laby &L)
{
 for(int i=0; i<P.nbe; i++) 
 {
	Mark_Case(P.T[i].i,P.T[i].j);
 }
}

void deplacement(laby &L)
{
	int ligne = 0, colonne = 0;
	Mark_Case(ligne,colonne);

	while (ligne != L.p && colonne != L.q)
	{
		int mouv = LireCommande();

		//Commande invalide -> arrêt de la partie
		if (mouv != 3 && mouv != 6 && mouv != 9 && mouv != 12 && mouv != -1)	
		{
			mvprintw(L.p*2+1,0, "ECHEC");
		}

		//Conditons pour pouvoir se déplacer
		if (mouv == 3 && L.tab[ligne][colonne].E == true)
		{
			UMark_Case(ligne,colonne);
			colonne ++;
			Mark_Case(ligne,colonne);
		}

		if (mouv == 9 && L.tab[ligne][colonne].W == true)
		{
			UMark_Case(ligne,colonne);
			colonne --;
			Mark_Case(ligne,colonne);
		}

		if (mouv == 6 && L.tab[ligne][colonne].S == true)
		{
			UMark_Case(ligne,colonne);
			ligne ++;
			Mark_Case(ligne,colonne);
		}

		if (mouv == 12 && L.tab[ligne][colonne].N == true)
		{
			UMark_Case(ligne,colonne);
			ligne --;
			Mark_Case(ligne,colonne);
		}

		//Arrivée à destination -> Partie terminée 
		if (ligne == L.p-1 && colonne == L.q-1)
		{
			mvprintw(L.p*2+1,0,"GAGNER ! ");
		}

		if (mouv == -1)
		{
			mvprintw(L.p*2+1,0,"ABANDON");
			mvprintw(L.p*2+2,0,"1- Afficher la solution.(Flèche du haut) / 2- Afficher les différentes tentaives d'exploations.(Flèche du bas)");

			mouv = LireCommande();

			if (mouv == 12)
			{
				pile *chemin_sortie;
				chemin_sortie = explorer_v2(L);
				afficher_chemin(*chemin_sortie, L);
			}
					
			if (mouv == 6)
			{	
				pile *diff_tenta;
				diff_tenta = explorer2_v2(L);
				afficher_chemin(*diff_tenta,L);
			}
		}

	}
}

laby *jeu()
{
	laby *LabyJeu = My_Creation(32,51); //ecran :37,67 / mac:32,51
	InitCurses();
    Show_Lab(*LabyJeu);		
    deplacement(*LabyJeu);
	int ch = wgetch(stdscr) ;
    EndCurses();
	return(LabyJeu);
}

int main()
{
	srand(time(NULL));
	laby *L1=jeu(); 
	L1 = lire_Laby("mon_laby.txt");
}