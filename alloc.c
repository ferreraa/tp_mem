#ifndef __MEM_ALLOC_H
#define __MEM_ALLOC_H

#include <stddef.h>

#define MAX_INDEX 20
#define HEAP_SIZE (1 << MAX_INDEX)
#include <stdio.h>
//vania.marangozova@imag.fr
//#include alloc.h

char M[HEAP_SIZE];//mem_heap
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
	printf("%d\n", (int) (A-get_mem_start()));
}

void affiche( void * zone, size_t t){
	print_adresse (zone);
	printf("%d\n",(int) t);
}

void *parcours_first_fit(size_t size){
	int b=1;
	struct fb * p;
	p= *(struct fb**)M;//potentielement un probleme ou pas 
	if (p!=NULL){
		if (p->size >= size+ sizeof(struct ff)) b=0;// je pense qu'il y a des problemes
	}
	while (b==1|| p==NULL||p->next==NULL){
		if (p->next->size >= size+ sizeof(struct ff)) b=0;
		else p=p->next;
	}
	return p;
}// si p =NULL on n'as pas trouve de place pour mettre le truc ...

void *mem_alloc(size_t size){
	struct fb * p;
	p= parcours_first_fit(size);
	
	int taille_init= p->size;
	struct fb * next_init= p->next;
	
	struct ff *  dwf= p;
	dwf->size= size;
	
	struct fb * prout = p+size+sizeof(struct ff);
	prout->next=next_init;
	prout->size=size;
	

	return NULL;
}
	
	
void mem_free(void *zone, size_t size){}

void mem_show(void (*print)(void *zone, size_t size)){
	struct fb *p;
	p= * (struct fb **)M;
	
	while ( p!=NULL){
		print (p,p->size);
		p= p->next;
	}
}

int main (){
	mem_init();
	mem_show(affiche);
	return 0;
}
#endif
