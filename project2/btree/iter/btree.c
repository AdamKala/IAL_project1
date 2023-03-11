/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */
#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  (*tree) = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bst_node_t* tree1 = tree;
  while(tree1 != NULL){ //dokud neni *tree null
    if(tree1-> key == key){ //nasli jsme stejny key, zapiseme hodnotu uzlu do value
      *value = tree1->value;
      return true;
    }

    if(tree1->key > key){ //pokud je key vetsi, jdeme doleva
      tree1 = tree1->left;
    } else { //pokud mensi, jdeme doprava
      tree1 = tree1->right;
    }
  }
  return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  bst_node_t* tree0 = NULL; //rodic
  bst_node_t* tree1 = *tree; //*tree

  if(tree1 == NULL){ //pokud je *tree null 
    tree1 = (bst_node_t*)malloc(sizeof(bst_node_t)); //tree1 do pameti
    tree1->key = key;
    tree1->value = value;
    tree1->left = NULL;
    tree1->right = NULL;
    *tree = tree1; //dame nove tree1 do *tree
  }
  while(tree1){ //dokud tree1 neni null
    if(tree1->key == key){ //nalez
      tree1->value = value; //prirazeni hodnoty
      return;
    }
    tree0 = tree1; //tree1 priradime jako noveho rodice

    if(tree1->key > key){ //pokud je tree1key vetsi, jdeme doleva
      tree1 = tree1->left;
    } else if(tree1->key < key){ //jinak doprava
      tree1 = tree1->right;
    }
    if(tree1 == NULL){ 
      tree1 = (bst_node_t*)malloc(sizeof(bst_node_t));
      tree1->key = key;
      tree1->value = value;
      tree1->left = NULL;
      tree1->right = NULL;
      if(tree0->key > key){
        tree0->left = tree1;
      } else if(tree0->key < key){
        tree0->right = tree1;
      }
      return;
    }
  }
}


/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  bst_node_t *tree0 = NULL; //rodic
  bst_node_t *tree1 = *tree; //*tree
  if(tree1->right == NULL){ //pokud vpravo nic neni, jdeme do leva
    tree1 = tree1->left;
    return;
  } else { //pokud je, jdeme dokud vpravo neni null
    while(tree1->right != NULL){
      tree0 = tree1; //tree1 se stalo novym rodicem
      tree1 = tree1->right; 
    }

    tree0->right = tree1->left;
    target->value = tree1->value;
    target->key = tree1->key;
    free(tree1); //uvolneni pameti
  }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
  bst_node_t *tree0 = NULL; //rodic
  bst_node_t *tree1 = *tree; //dite

  while(tree1 != NULL){ //dokud *tree neni null, projizdime
    if(tree1->key > key){ //pokud je tree1key vetsi, jdeme doleva
      tree0 = tree1;
      tree1 = tree1->left;
    } else if(tree1->key < key){ //pokud je tree1key mensi, jdeme doprava
      tree0 = tree1;
      tree1 = tree1->right;
    } else if(tree1->key == key){ //pokud se rovna
      if((tree1->right != NULL) && (tree1->left != NULL)){ //ma obe deti
        bst_replace_by_rightmost(tree1, &tree1->left);
        return;
      } else if((tree1->right == NULL) && (tree1->left == NULL)){ //nema zadne dite
        free(tree1);
        if(tree0 != NULL){
          if(tree0->key > key){
            tree0->left = NULL;
          } else if(tree0->key < key){
            tree0->right = NULL;
          }
        }
      } else if(tree1->right != NULL){ //prave dite
        bst_node_t *childleft = tree1->right;
        tree1->key = childleft->key;
        tree1->value = childleft->value;
        tree1->right = childleft->right;
        tree1->left = childleft->left;
        free(childleft);
      } else if(tree1->left != NULL){ //leve dite
        bst_node_t *childright = tree1->left;
        tree1->key = childright->key;
        tree1->value = childright->value;
        tree1->right = childright->right;
        tree1->left = childright->left;
        free(childright);
      }
      return;
    }
  }
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if(*tree == NULL){ //pokud je tree null, vratime se
    return;
  } else { //pokud neni null
    stack_bst_t dispose;
    stack_bst_init(&dispose);
    stack_bst_push(&dispose, *tree);

    while(!stack_bst_empty(&dispose)){ //dokud neni zasobnik prazdny
      bst_node_t *pop = stack_bst_pop(&dispose); //pop prvniho prvku
      if(pop->left != NULL){ //pokud levy neni null
        stack_bst_push(&dispose, pop->left); //pop leveho
      }
      if(pop->right != NULL){ //pokud pravy neni null
        stack_bst_push(&dispose, pop->right); //pop praveho
      }
      if(pop != NULL){ //pokud je pop prazdny, uvolnime ho
      free(pop);
      pop = NULL;
      }
    }
    *tree = NULL;
  }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while(tree){ 
      stack_bst_push(to_visit, tree);
      bst_print_node(tree);
      tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
    stack_bst_t pre_stack;
    stack_bst_init(&pre_stack);

    bst_leftmost_preorder(tree, &pre_stack);

    while(!stack_bst_empty(&pre_stack)){
      tree = stack_bst_top(&pre_stack);
      stack_bst_pop(&pre_stack);
      bst_leftmost_preorder(tree->right, &pre_stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
    while(tree){
      stack_bst_push(to_visit, tree);
      tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
    stack_bst_t in_stack;
    stack_bst_init(&in_stack);

    bst_leftmost_inorder(tree, &in_stack);

    while(!stack_bst_empty(&in_stack)){
    tree = stack_bst_top(&in_stack);
    stack_bst_pop(&in_stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right, &in_stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) {
    while(tree){
      stack_bst_push(to_visit, tree);
      stack_bool_push(first_visit, true);
      tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree){
  stack_bst_t post_stack;
  stack_bst_init(&post_stack);

  stack_bool_t post_stack2;
  stack_bool_init(&post_stack2);

  bst_leftmost_postorder(tree, &post_stack, &post_stack2);

  while(!stack_bst_empty(&post_stack)){
    bst_node_t* tree1 = stack_bst_pop(&post_stack);
    bool post = stack_bool_pop(&post_stack2);
    if(post){ 
      stack_bst_push(&post_stack, tree1);
      stack_bool_push(&post_stack2, false);
      bst_leftmost_postorder(tree1->right, &post_stack, &post_stack2);      
    } else {
      bst_print_node(tree1);
    }
  }
}
