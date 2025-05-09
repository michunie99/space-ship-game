#include "collisions.h"
#include "raylib.h"
#include <stdlib.h>

void resolveCollision(ObjectList* objList) {
	if (objList->count <= 1) {return;}
	Object* n1, *n2;
	n1 = objList->tail;
	n2 = n1->prev;

	while (n1->prev) {
		while(n2) {
			bool collision = CheckCollisionRecs(n1->hitbox, n2->hitbox);
			if (collision && 
				n1->type != n2->type &&
				!n1->destroied && !n2->destroied  &&
				!(n1->type==PLAYER_TYPE && n2->type==LASER_TYPE) &&
				!(n2->type==PLAYER_TYPE && n1->type==LASER_TYPE)
			) {
				n1->destroied=true;
				n2->destroied=true;
				if (n1->type == ASTEROID_TYPE) {
					n1->animationActive=true; 
					objList->score++; 
					PlaySound(n1->soundEffect);
				}
				if (n2->type == ASTEROID_TYPE) {
					n2->animationActive=true;
					objList->score++;
					PlaySound(n2->soundEffect);
				}
			}
			n2 = n2->prev;
		}
		n1 = n1->prev;
		n2 = n1->prev;
	}
}
