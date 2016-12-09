

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
	int b=1;
	struct fb * p;
	p= *(struct fb**)mem_heap;
	
	if (p->size >= size+ sizeof(struct ff)) {return mem_heap;}
	else {
		while (b==1&& p!=NULL&&p->next!=NULL){
			if (p->next->size >= size+ sizeof(struct ff)) b=0;
			else p=p->next;
		}
		
		if (p->next==NULL) { return (NULL);} // si il y a pas la place 
		else {
			return p;
		}
	}
}// si p =NULL on n'as pas trouve de place pour mettre le truc.



void *mem_alloc(size_t size){
	
	struct fb * p;
	void * resultat;
	size_t taille_espace_libre;
	if (size+ sizeof (struct ff)< sizeof (struct fb)) size= sizeof (struct fb)-sizeof (struct ff);
	// On cherche la premiere zone memoire libre
	p= parcours_first_fit(size);
	//Si on trouve null il n'y a pas de place 
	if (p==NULL)  {printf (" pas de place \n");resultat= NULL;}  
	//Si il y a de la place 
	else {
		struct fb ecureuil; // nouvelle zone libre 
		if (p==(struct fb *)mem_heap){ // cas ou la place est dans la premiere zone libre de la memoire
		
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
				*((struct fb*)(resultat+size))=ecureuil;

				*(struct fb**)mem_heap=(struct fb*)(resultat+size);
				
			}
			else {
				size= taille_espace_libre- sizeof (struct ff);  //si on as pas la place de mettre une zone libre on atribue toute la place
				struct ff taille_alloue;
				taille_alloue.size=size; 
				*((struct ff *)p)= taille_alloue;
				
				resultat= ((char *)p)+sizeof(struct ff);
				
				*(struct fb**)mem_heap= p->next;
				
			}
			
		}
		else { // autre cas  -> ne marche pas mettre comme au dessus 
		
		
		
			taille_espace_libre= p->next->size;
			
			// Si l'espace est plus grand que la zone donc on as besoin et que on peut dire qu'il y a une zone libre.
			if (taille_espace_libre >= size + sizeof (struct ff)+ sizeof (struct fb)){
			
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
			
			else { 
				
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
	int taille = (int)*(((char*)zone)-sizeof(struct ff));
	// On cherche la plus proche zone libre avant.
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
	
	//zone libre apres 
	struct fb * zone_libre_apres =NULL;
	if (zone_libre_avant == (struct fb*)mem_heap){
		zone_libre_apres=* (struct fb **)mem_heap;
	}
	else zone_libre_apres=p->next;
	
	
	
	// sans fusion
	struct fb prout;
	if (zone_libre_avant == (struct fb*)mem_heap){
		
		
		*((struct fb **)mem_heap)=((struct fb *)(((char*)zone)-sizeof(struct ff)));// on 8 dans M
	}
	else zone_libre_avant->next=((struct fb *)(((char*)zone)-sizeof(struct ff)));
	
	
	prout.size= ((int)taille+ sizeof(struct ff));
	prout.next=zone_libre_apres;
	
	
	*((struct fb *)(((char*)zone)-sizeof(struct ff)))=prout;
	

	if (zone_libre_apres== ((struct fb*)(((char*)zone)+taille))){//fusion a droite
		prout.size= prout.size+ zone_libre_apres->size;//peut etre faut 
		prout.next=zone_libre_apres->next;
		*((struct fb*)(((char*)zone)-sizeof(struct ff)))=prout;
	}
	
	if (zone_libre_avant!= NULL &&(zone_libre_avant != (struct fb*)mem_heap) && ((void *)((char *)zone_libre_avant)+zone_libre_avant->size+sizeof(struct ff))==zone){//fusion a gauche
		zone_libre_avant->next=prout.next; //zone_libre_apres;
		zone_libre_avant->size=zone_libre_avant->size+prout.size;
	}

}

void mem_show(void (*print)(void *zone, size_t size)){
	
	struct fb *p;
	
	p= * (struct fb **)mem_heap;
	

	while ( p!=NULL){
		print (p,p->size);
		p= p->next;
	}
	
	
}
