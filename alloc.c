#ifndef __MEM_ALLOC_H
#define __MEM_ALLOC_H

#include <stddef.h>

#define MAX_INDEX 20
#define HEAP_SIZE 200
#include <stdio.h>
//#define HEAP_SIZE (1 << MAX_INDEX)
//vania.marangozova@imag.fr
//#include alloc.h

char M[HEAP_SIZE];//mem_heap[heap_size]
// amettre dan sle .h

// convention la taille de l'espace libre est la taille de l'espace libre sans les strcut fb et ff
struct fb {
	size_t size;
	struct fb * next;
};
struct ff{
	size_t size;
};

void mem_init(){
	*((struct fb **)M) = (struct fb*)(M+ sizeof (struct fb *));
	struct fb *madame =(struct fb*)(M+ sizeof (struct fb *)) ;
	madame->size=HEAP_SIZE-sizeof (struct fb *);
	madame->next=NULL;
}

char * get_mem_start (){
	return M;
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
	p= *(struct fb**)M;//potentielement un probleme ou pas 
	
	if (p->size >= size+ sizeof(struct ff)) {return M;}
	else {
		while (b==1|| p!=NULL||p->next!=NULL){
			if (p->next->size >= size+ sizeof(struct ff)) b=0;
			else p=p->next;
		}
		return p;
	}
}// si p =NULL on n'as pas trouve de place pour mettre le truc ... verifier si on retourne M.

void *mem_alloc(size_t size){
	struct fb * p;
	void * resultat;
	p= parcours_first_fit(size);
	printf ("j'essaie d'allouer un truc de taille : %d\n",(int)size);
	if (p==(struct fb *)M){
		printf("p:");
		print_adresse((char*)p);
		p= *(struct fb**)M;
		printf("p':");
		print_adresse((char*)p);
		printf ("je suis la \n");
		struct fb ecureuil;
		ecureuil.size=p->size-(int)size- sizeof(struct ff);
		ecureuil.next=p->next;
		
		struct ff taille_alloue;
		taille_alloue.size=size; // pas dans tous les cas  a changer ...
		*((struct ff *)p)= taille_alloue;
		
		resultat= ((char *)p)+sizeof(struct ff);
		printf("res:");
		print_adresse((char*)resultat);
		*((struct fb*)(resultat+size))=ecureuil;

		*(struct fb**)M=(struct fb*)(resultat+size);
		return resultat;
		
	}
	else {
		struct fb ecureuil;
		ecureuil.size=p->next->size;
		ecureuil.next=p->next->next;
		
		struct ff taille_alloue;
		taille_alloue.size=size; // TODO
		*((struct ff *)(p->next))=taille_alloue;
		
		resultat =((char*)p->next)+sizeof(struct ff);
		*((struct fb*)(resultat+size))=ecureuil;
		
		*(struct fb**)(p->next)=(struct fb*)(resultat+size);
		

		return resultat;
	}
}
	
	
void mem_free(void *zone, size_t size){
	//On recupere la taille de la zone a liberer 
	printf("On recupere la taille de la zone a liberer \n");
	int taille = (int)*(((char*)zone)-sizeof(struct ff));//arithmetique des pointeurs ?
	printf(" Taille : %d\n",taille);
	// On cherche la plus proche zone libre avant.
	printf("On cherche la plus proche zone libre avant.\n");
	struct fb * p=*(struct fb **)M;
	struct fb * zone_libre_avant=NULL;
	
	if (*(struct fb **)M> (struct fb*)zone){
		zone_libre_avant =(struct fb*) M;
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
	if (zone_libre_avant == (struct fb*)M){
		zone_libre_apres=* (struct fb **)M;
	}
	else zone_libre_apres=p->next;
	
	printf("zone_libre_apres:");
	print_adresse((char*)zone_libre_apres);
	
	
	// sans fusion
	printf(" Sans fusion\n");
	struct fb prout;
	if (zone_libre_avant == (struct fb*)M){
		
		
		*((struct fb **)M)=((struct fb *)(((char*)zone)-sizeof(struct ff)));// on 8 dans M
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
	
	if (zone_libre_avant!= NULL &&(zone_libre_avant != (struct fb*)M) && ((void *)((char *)zone_libre_avant)+zone_libre_avant->size+sizeof(struct ff))==zone){//fusion a gauche
		printf(" fusion gauche\n");
		zone_libre_avant->next=prout.next; //zone_libre_apres;
		zone_libre_avant->size=zone_libre_avant->size+prout.size;
	}
	
}

void mem_show(void (*print)(void *zone, size_t size)){
	struct fb *p;
	p= * (struct fb **)M;
	
	int g;
	printf("\n");
	for(g=0;g<20;g++)
		printf("%zu, ", ((size_t*)M)[g]);
	printf("\n%zu\n", (size_t)M);
	
	while ( p!=NULL){
		print (p,p->size);
		p= p->next;
	}
	
	
}

int main (){
	int d;
	printf("ff:%d\n",(int) sizeof(struct ff));
	printf("fd:%d\n",(int) sizeof(struct fb));
	mem_init();
	mem_show(affiche);
	scanf("%d",&d);
	struct fb * p1= mem_alloc(32);
	mem_show(affiche);
	scanf("%d",&d);
	struct fb * p2= mem_alloc(32);
	mem_show(affiche);
	
	scanf("%d",&d);
	struct fb * p3= mem_alloc(32);
	mem_show(affiche);
	
	scanf("%d",&d);
	mem_free(p1,4);
	mem_show(affiche);
	
	scanf("%d",&d);
	mem_free(p2,4);
	mem_show(affiche);
	
	scanf("%d",&d);
	mem_free(p3,4);
	mem_show(affiche);

}
#endif
