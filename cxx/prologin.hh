///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_cxx.rb
//

#ifndef PROLOGIN_HH_
# define PROLOGIN_HH_

# include <vector>

# include <string>

///
// Taille du terrain (longueur et largeur)
//
# define TAILLE_TERRAIN            32

///
// Nombre de tours par partie
//
# define FIN_PARTIE                200

///
// Nombre maximum de joueurs dans la partie
//
# define MAX_JOUEURS               2

///
// Revenu en or par île à chaque tour
//
# define REVENU_ILE                5

///
// Revenu en or par volcan à chaque tour
//
# define REVENU_VOLCAN             10

///
// Or initialement possédé par chaque joueur
//
# define OR_INITIAL                20

///
// Coût de construction d'une Caravelle
//
# define CARAVELLE_COUT            15

///
// Coût de construction d'un Galion
//
# define GALION_COUT               4

///
// Déplacement de la Caravelle
//
# define CARAVELLE_DEPLACEMENT     4

///
// Déplacement du Galion
//
# define GALION_DEPLACEMENT        6

///
// Limite du nombre de bateaux pour chaque joueur
//
# define LIMITE_BATEAUX            500

///
// Type de bateau
//
typedef enum bateau_type {
  BATEAU_CARAVELLE, /* <- Caravelle */
  BATEAU_GALION, /* <- Galion */
  BATEAU_ERREUR, /* <- Bateau inexistant */
} bateau_type;


///
// Type de terrain
//
typedef enum terrain {
  TERRAIN_ILE, /* <- Île */
  TERRAIN_VOLCAN, /* <- Volcan */
  TERRAIN_MER, /* <- Mer */
  TERRAIN_ERREUR, /* <- Erreur, case impossible */
} terrain;


///
// Erreurs possibles
//
typedef enum erreur {
  OK, /* <- L'action a été exécutée avec succès */
  OR_INSUFFISANT, /* <- Vous ne possédez pas assez d'or pour cette action */
  ILE_INVALIDE, /* <- La position spécifiée n'est pas une île */
  POSITION_INVALIDE, /* <- La position spécifiée n'est pas valide */
  TROP_LOIN, /* <- La destination est trop éloignée */
  ILE_COLONISEE, /* <- L'île est déjà colonisée */
  ILE_ENNEMIE, /* <- L'île ne vous appartient pas */
  BATEAU_ENNEMI, /* <- Le bateau ne vous appartient pas */
  ID_INVALIDE, /* <- L'ID spécifiée n'est pas valide */
  NON_DEPLACABLE, /* <- Le bateau n'est pas déplaçable */
  AUCUNE_CARAVELLE, /* <- Il n'y a aucune caravelle susceptible de coloniser l'île */
  LIMITE_ATTEINTE, /* <- La limite de bateaux est atteinte */
} erreur;



typedef struct position {
  int x;  /* <- Coordonnée en X */
  int y;  /* <- Coordonnée en Y */

  bool operator == (const position& b) const
  {
  	return b.x == x && b.y == y;
  }

  bool operator < (const position& b) const
  {
  	return b.x < x || (b.x == x && b.y < y);
  }

} position;


static bool valide(const position& a)
{
	return a.x >= 0 && a.x < TAILLE_TERRAIN && a.y >= 0 && a.y < TAILLE_TERRAIN;
}

// Représente la position sur la carte
//


///
// Représente un bateau
//
typedef struct bateau {
  int id;  /* <- Identifiant unique du bateau */
  position pos;  /* <- Position */
  int joueur;  /* <- Joueur */
  bateau_type btype;  /* <- Type */
  int nb_or;  /* <- Or contenu dans le bateau */
  bool deplacable;  /* <- Le bateau n'a pas encore été déplacé ce tour-ci */
} bateau;


///
// Retourne la nature du terrain désigné par ``pos``.
//
extern "C" terrain api_info_terrain(position pos);
static inline terrain info_terrain(position pos)
{
  return api_info_terrain(pos);
}


///
// Retourne le joueur qui possède l'île à l'emplacement ``pos``. Retourne -1 si l'île est libre ou si la position indiquée n'est pas une île
//
extern "C" int api_info_ile_joueur(position pos);
static inline int info_ile_joueur(position pos)
{
  return api_info_ile_joueur(pos);
}


///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1 si la case spécifiée n'est pas une île.
//
extern "C" int api_info_ile_or(position pos);
static inline int info_ile_or(position pos)
{
  return api_info_ile_or(pos);
}


///
// Retourne le bateau ayant pour identifiant ``id``
//
extern "C" bateau api_info_bateau(int id);
static inline bateau info_bateau(int id)
{
  return api_info_bateau(id);
}


///
// Retourne vrai si le bateau ayant pour identifiant ``id`` existe et est encore à flots
//
extern "C" bool api_bateau_existe(int id);
static inline bool bateau_existe(int id)
{
  return api_bateau_existe(id);
}


///
// Retourne la liste de bateaux à la position ``pos``
//
extern "C" std::vector<bateau> api_liste_bateaux_position(position pos);
static inline std::vector<bateau> liste_bateaux_position(position pos)
{
  return api_liste_bateaux_position(pos);
}


///
// Retourne la liste des ID des bateaux à la position ``pos``
//
extern "C" std::vector<int> api_liste_id_bateaux_position(position pos);
static inline std::vector<int> liste_id_bateaux_position(position pos)
{
  return api_liste_id_bateaux_position(pos);
}


///
// Retourne la liste des positions des îles de la carte
//
extern "C" std::vector<position> api_liste_iles();
static inline std::vector<position> liste_iles()
{
  return api_liste_iles();
}


///
// Retourne la liste des positions des îles qui vous appartiennent
//
extern "C" std::vector<position> api_mes_iles();
static inline std::vector<position> mes_iles()
{
  return api_mes_iles();
}


///
// Retourne l'ID du dernier bateau construit. Son comportement n'est pas défini si vous n'avez pas encore créé de bateau à ce tour-ci.
//
extern "C" int api_id_dernier_bateau_construit();
static inline int id_dernier_bateau_construit()
{
  return api_id_dernier_bateau_construit();
}


///
// Retourne la distance entre deux positions
//
extern "C" int api_distance(position depart, position arrivee);
static inline int distance(position depart, position arrivee)
{
  return api_distance(depart, arrivee);
}


///
// Construire un bateau de type ``btype`` sur l'île à la position ``pos``
//
extern "C" erreur api_construire(bateau_type btype, position pos);
static inline erreur construire(bateau_type btype, position pos)
{
  return api_construire(btype, pos);
}


///
// Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la position `pos`` (si elle est dans la portée du bateau)
//
extern "C" erreur api_deplacer(int id, position pos);
static inline erreur deplacer(int id, position pos)
{
  return api_deplacer(id, pos);
}


///
// Colonise l'île à la position ``pos``
//
extern "C" erreur api_coloniser(position pos);
static inline erreur coloniser(position pos)
{
  return api_coloniser(pos);
}


///
// Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
extern "C" erreur api_charger(int id, int nb_or);
static inline erreur charger(int id, int nb_or)
{
  return api_charger(id, nb_or);
}


///
// Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
extern "C" erreur api_decharger(int id, int nb_or);
static inline erreur decharger(int id, int nb_or)
{
  return api_decharger(id, nb_or);
}


///
// Transfère ``montant`` or de la caravelle ``id_source`` à la caravelle ``id_dest``
//
extern "C" erreur api_transferer(int montant, int id_source, int id_dest);
static inline erreur transferer(int montant, int id_source, int id_dest)
{
  return api_transferer(montant, id_source, id_dest);
}


///
// Retourne le numéro de votre joueur
//
extern "C" int api_mon_joueur();
static inline int mon_joueur()
{
  return api_mon_joueur();
}


///
// Retourne le numéro de votre adversaire
//
extern "C" int api_adversaire();
static inline int adversaire()
{
  return api_adversaire();
}


///
// Retourne les scores du joueur désigné par l'identifiant ``id``
//
extern "C" int api_score(int id_joueur);
static inline int score(int id_joueur)
{
  return api_score(id_joueur);
}


///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel();
static inline int tour_actuel()
{
  return api_tour_actuel();
}


///
// Retourne le nombre de bateaux que possède le joueur désigné par l'identifiant ``id``
//
extern "C" int api_nombre_bateaux(int id_joueur);
static inline int nombre_bateaux(int id_joueur)
{
  return api_nombre_bateaux(id_joueur);
}


///
// Affiche le contenu d'une valeur de type bateau_type
//
extern "C" void api_afficher_bateau_type(bateau_type v);
static inline void afficher_bateau_type(bateau_type v)
{
  api_afficher_bateau_type(v);
}


///
// Affiche le contenu d'une valeur de type terrain
//
extern "C" void api_afficher_terrain(terrain v);
static inline void afficher_terrain(terrain v)
{
  api_afficher_terrain(v);
}


///
// Affiche le contenu d'une valeur de type erreur
//
extern "C" void api_afficher_erreur(erreur v);
static inline void afficher_erreur(erreur v)
{
  api_afficher_erreur(v);
}


///
// Affiche le contenu d'une valeur de type position
//
extern "C" void api_afficher_position(position v);
static inline void afficher_position(position v)
{
  api_afficher_position(v);
}


///
// Affiche le contenu d'une valeur de type bateau
//
extern "C" void api_afficher_bateau(bateau v);
static inline void afficher_bateau(bateau v)
{
  api_afficher_bateau(v);
}



extern "C" {

///
// Fonction appelée au début de la partie
//
void partie_init();

///
// Fonction appelée à chaque tour
//
void jouer_tour();

///
// Fonction appelée à la fin de la partie
//
void partie_fin();

}
#endif
