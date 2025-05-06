#include "collisions.h"
#include <stdlib.h>

void resolveCollision(ObjectList* objList) {
	if (objList->count <= 1) {return;}
	Object* n1, *n2;
	n1 = objList->head;
	n2 = n1->next;

	while (n1->next) {
		while(n2->next) {
			bool collision = CheckCollisionRecs(n1->hitbox, n2->hitbox);
			if (collision && 
				n1->type != n2->type &&
				!n1->destroied && !n2->destroied 
			) {
				n1->destroied=true;
				n2->destroied=true;
				if (n1->type == ASTEROID_TYPE) {n1->animationActive=true;}
				if (n2->type == ASTEROID_TYPE) {n2->animationActive=true;}
			}
			n2 = n2->next;
		}
		n1 = n1->next;
		n2 = n1->next;
	}
}
