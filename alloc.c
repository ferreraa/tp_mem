

#include <stdio.h>

//#define HEAP_SIZE (1 << MAX_INDEX)
//vania.marangozova@imag.fr
#include "alloc.h"



// convention la taille de l'espace libre est la taille de l'espace libre sans les strcut fb et ff
struct fb {
	size_t size;
	struct fb * next;
};
struct ff{
	size_t size;
};

void mem_init(){  // fonctionne
	*((struct fb **)mem_heap) = (struct fb*)(mem_heap+ sizeof (struct fb *));
	struct fb *madame =(struct fb*)(mem_heap+ sizeof (struct fb *)) ;
	madame->size=HEAP_SIZE-sizeof (struct fb *);
	madame->next=NULL;
}

char * get_mem_start (){
	return mem_heap;
}

void print_adresse (char * A){
	printf("Adresse : %d\n", (int) (A-get_mem_start()));
}

void affiche( void * zone, size_t t){
	
	print_adresse (zone);
	printf("Taille espace libre %d\n",(int) t);
}

void *parcours_first_fit(size_t size){
	printf("je suis dans parcours first fit \n");
	int b=1;
	struct fb * p;
	p= *(struct fb**)mem_heap;//potentielement un probleme ou pas 
	
	if (p->size >= size+ sizeof(struct ff)) {return mem_heap;}
	else {
		printf("dans le else \n");
		while (b==1&& p!=NULL&&p->next!=NULL){
			if (p->next->size >= size+ sizeof(struct ff)) b=0;
			else p=p->next;
		}
		
		if (p->next==NULL) { return (NULL);} // si il y a pas la place 
		else {
			return p;
		}
	}
}// si p =NULL on n'as pas trouve de place pour mettre le truc ... verifier si on retourne M.



void *mem_alloc(size_t size){
	
	struct fb * p;
	void * resultat;
	size_t taille_espace_libre;
	// On cherche la premiere zone memoire libre
	p= parcours_first_fit(size);
	//Si on trouve null il n'y a pas de place 
	if (p==NULL)  {printf (" pas de place \n");resultat= NULL;}  
	//Si il y a de la place 
	else {
		struct fb ecureuil; // nouvelle zone libre 
		printf ("j'essaie d'allouer un truc de taille : %d\n",(int)size);
		if (p==(struct fb *)mem_heap){ // cas ou la place est dans la premiere zone libre de la memoire
		
			print_adresse((char*)p);
			p= *(struct fb**)mem_heap;
			
			// on recupere la taille de l'espace qu'on vient de recuperer 
			taille_espace_libre= p->size;
			// Si l'espace est plus grand que la zone donc on as besoin et que on peut dire qu'il y a une zone libre.
			if (taille_espace_libre >= size + sizeof (struct ff)+ sizeof (struct fb)){
				
				ecureuil.size=p->size-(int)size- sizeof(struct ff);
				ecureuil.next=p->next;
				
				struct ff taille_alloue;
				taille_alloue.size=size; 
				*((struct ff *)p)= taille_alloue;
				
				resultat= ((char *)p)+sizeof(struct ff);
				printf("res:");
				print_adresse((char*)resultat);
				*((struct fb*)(resultat+size))=ecureuil;

				*(struct fb**)mem_heap=(struct fb*)(resultat+size);
				
			}
			else {
				size= taille_espace_libre- sizeof (struct ff);  //si on as pas la place de mettre une zone libre on atribue toute la place
				struct ff taille_alloue;
				taille_alloue.size=size; 
				*((struct ff *)p)= taille_alloue;
				
				resultat= ((char *)p)+sizeof(struct ff);
				printf("res:");
				print_adresse((char*)resultat);
				
				*(struct fb**)mem_heap= p->next;
				
			}
			
		}
		else { // autre cas  -> ne marche pas mettre comme au dessus 
		
			printf(" Je suis dans l'autre cas \n");
			printf("p:");
			print_adresse((char*)p);//OK

			taille_espace_libre= p->next->size;
			printf("taille_espace_libre: %lu\n",taille_espace_libre);//OK
			// Si l'espace est plus grand que la zone donc on as besoin et que on peut dire qu'il y a une zone libre.
			if (taille_espace_libre >= size + sizeof (struct ff)+ sizeof (struct fb)){
				printf(" je suis la \n");
				struct ff * q;
				q= (struct ff *) p->next;
				
				ecureuil.size=taille_espace_libre - (int)size - sizeof(struct ff) ;
				ecureuil.next=p->next->next;
				
				struct ff taille_alloue;
				taille_alloue.size=size; 
				
				*q =taille_alloue;
				
				resultat =((char*)p->next)+sizeof(struct ff);
				
				*((struct fb*)(resultat+size))=ecureuil;
				
				struct fb ecureuil2;
				ecureuil2.size=p->size ;
				ecureuil2.next=(struct fb*)(resultat+size);
				
				*(struct fb*)(p)=ecureuil2;
			}
			
			else { // acompleter 
				printf(" je suis pas la \n");
				size= taille_espace_libre- sizeof (struct ff);  //si on as pas la place de mettre une zone libre on atribue toute la place
				
				struct ff * q;
				q= (struct ff *) p->next;
				struct ff taille_alloue;
				taille_alloue.size=size; 
				*q =taille_alloue;
				
				resultat =((char*)p->next)+sizeof(struct ff);
				
				struct fb ecureuil2;
				ecureuil2.size=p->size ;
				ecureuil2.next=p->next->next;
				
				*(struct fb*)(p)=ecureuil2;
			}
		}
	}
	return resultat;
}
	
	
void mem_free(void *zone, size_t size){
	//On recupere la taille de la zone a liberer 
	printf("On recupere la taille de la zone a liberer \n");
	int taille = (int)*(((char*)zone)-sizeof(struct ff));//arithmetique des pointeurs ?
	printf(" Taille : %d\n",taille);
	// On cherche la plus proche zone libre avant.
	printf("On cherche la plus proche zone libre avant.\n");
	struct fb * p=*(struct fb **)mem_heap;
	struct fb * zone_libre_avant=NULL;
	
	if (*(struct fb **)mem_heap> (struct fb*)zone){
		zone_libre_avant =(struct fb*) mem_heap;
	}
	else {
		while (p->next!=NULL && p->next< (struct fb *)zone){
			p=p->next;
		}
		zone_libre_avant=p;
	}
	printf("zone_libre_avant:");
	print_adresse((char*)zone_libre_avant);
	
	//zone libre apres 
	printf(" On cherche la zone libre apres\n");
	struct fb * zone_libre_apres =NULL;
	if (zone_libre_avant == (struct fb*)mem_heap){
		zone_libre_apres=* (struct fb **)mem_heap;
	}
	else zone_libre_apres=p->next;
	
	printf("zone_libre_apres:");
	print_adresse((char*)zone_libre_apres);
	
	
	// sans fusion
	printf(" Sans fusion\n");
	struct fb prout;
	if (zone_libre_avant == (struct fb*)mem_heap){
		
		
		*((struct fb **)mem_heap)=((struct fb *)(((char*)zone)-sizeof(struct ff)));// on 8 dans M
	}
	else zone_libre_avant->next=((struct fb *)(((char*)zone)-sizeof(struct ff)));
	
	
	prout.size= ((int)taille+ sizeof(struct ff));
	prout.next=zone_libre_apres;
	
	
	*((struct fb *)(((char*)zone)-sizeof(struct ff)))=prout;
	

	if (zone_libre_apres== ((struct fb*)(((char*)zone)+taille))){//fusion a droite
		printf("fusion droite\n");
		prout.size= prout.size+ zone_libre_apres->size;//peut etre faut 
		prout.next=zone_libre_apres->next;
		*((struct fb*)((char*)zone)-sizeof(struct ff))=prout;
	}
	
	if (zone_libre_avant!= NULL &&(zone_libre_avant != (struct fb*)mem_heap) && ((void *)((char *)zone_libre_avant)+zone_libre_avant->size+sizeof(struct ff))==zone){//fusion a gauche
		printf(" fusion gauche\n");
		zone_libre_avant->next=prout.next; //zone_libre_apres;
		zone_libre_avant->size=zone_libre_avant->size+prout.size;
	}
	
}

void mem_show(void (*print)(void *zone, size_t size)){
	struct fb *p;
	p= * (struct fb **)mem_heap;
	
	int g;
	printf("\n");
	for(g=0;g<28;g++)
		printf("%zu, ", ((size_t*)mem_heap)[g]);
	printf("\n%zu\n", (size_t)mem_heap);
	
	while ( p!=NULL){
		print (p,p->size);
		p= p->next;
	}
	
	
}
/*
int main (){
	int d;
	printf("ff:%d\n",(int) sizeof(struct ff));
	printf("fd:%d\n",(int) sizeof(struct fb));
	mem_init();
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	struct fb * p1= mem_alloc(32);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	struct fb * p2= mem_alloc(32);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	struct fb * p3= mem_alloc(32);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	struct fb * p4= mem_alloc(32);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	mem_free(p1,4);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	mem_free(p3,4);
	mem_show(affiche);
	
	printf("->:");
	scanf("%d",&d);
	p1= mem_alloc(40);
	mem_show(affiche);
}*/

