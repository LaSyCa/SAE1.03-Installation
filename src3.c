/**
* Programme complet d'un jeu de morpion joueur contre I.A dans une grille 3x3
*/

#include <stdio.h>
#include <stdlib.h>

/// Configuration
char* prefix = "[Morpion] "; /** Nom du jeu */
char* symbols[] = {"X", "O"}; /** Définition des symboles de jeux pour différencier les joueurs*/


/// Variables principales
char* positions[9]; /** Ensemble des cases présentes dans la grille (3x3) */
int currentPlayer = 1; /** Joueur dont c'est le tour de joueur  */
int turn = 1; /** Nombre du tour */

/**
* \brief Affiche un trait de séparation (Résumé court du rôle de la fonction)
* \detail La procédure écrit à l'écran un trait de séparation composé de caractères "-"
* avec un retour à la ligne.
*/

void separator() {
    printf("\n------------------------------");

}
/**
* \brief 
* \detail 
*
*/

void clearAll() {
    system("cls");
}
/**
* \brief 
* \detail 
*
*/
void init() {
    int i;
    for (i = 0; i < 9; i++) {
        positions[i] = "_";
    }
    printf("%sJeu du morpion par Pablo Z. et Neil T.", prefix);

}
/**
* \brief 
* \detail 
*
*/
void draw() {
    int i;
    for (i = 0; i < 9; i++) {
        char* symbol = positions[i];
        printf(symbol);
        printf(" ");
        if (i == 2 || i == 5) printf("\n\n");
    }
    printf("\n\n");
}

int winPossibilities[8][3] = {
    {0,1,2},
    {3,4,5},
    {6,7,8},
    {0,3,6},
    {1,4,7},
    {2,5,8},
    {0,4,8},
    {2,4,6}
};

int aiGetCase() {
    /// Mon symbole
    char* symbol = symbols[1];
    int probaId, probaCaseId;
    /// Failover
    int failOverCase = 0;
    /// Démarrage d'un combo
    int comboStartCaseLenght = 0;
    int comboStartCases[50] = {};
    /// Démarrage du check de toutes les probabilités
    for (probaId = 0; probaId < 8; probaId++) {
        int owned = 0, free = 0, occupied = 0;
        int freeCase = 0;
        /// Analyse de chaque case
        for (probaCaseId = 0; probaCaseId < 3; probaCaseId++) {
            if(positions[winPossibilities[probaId][probaCaseId]] == "_") {
                    freeCase = winPossibilities[probaId][probaCaseId];
                    free++;
            }
            if(positions[winPossibilities[probaId][probaCaseId]] == symbol) owned++;
            if(positions[winPossibilities[probaId][probaCaseId]] == symbols[0]) occupied++;
        }
        /// Attribution du failover
        if(free >= 1) {
            failOverCase = freeCase;
        }
        /// Sur de gagner
        if(owned == 2 && free >= 1) {
            printf("\nRetourne sur de gagner (%i)\n", freeCase);
            return freeCase;
        }
        /// Bloquer une victoire
        // TODO Faire l'antivictoire
        /// Case libre
        if(free == 3) {
            /// Retourne une case libre pour commencer un combo
            comboStartCases[comboStartCaseLenght] = freeCase;
            comboStartCaseLenght++;
        }
    }
    /// Démarrage d'une série quelconque
    if(comboStartCaseLenght > 0) {
        return comboStartCases[rand()%comboStartCaseLenght];
    }
    /// Aucun combo fiable trouvé, l'IA retourne une case libre quelconque
    return failOverCase;
}

/**
* \brief Vérifie la victoire d'un joueur ou I.A
* \detail La fonction vérifie les positions des symboles
* chaque joueur pour voir si la condition de victoire est
*présente chez l'un des joueurs ou si un match nul est présent
* Il vérifie successivement les lignes, colonnes et diagonales
* dans toute la grille
* 
* \return 100 si il s'agit d'un match nul
* \return (i+1) si il s'agit de la victoire d'un joueur
* \return 0 dans tous les cas
*/

int checkWinner() {
    int i;
    /// Match nul
    int completed = 0;
    for (i = 0; i < 9; i++) {
        if (positions[i] != "_") completed++;
    }
    if (completed == 9) return 100;

    /// Victoire d'un joueur
    for (i = 0; i < 2; i++){
        int win = 0;
        char* checkingSymbol = symbols[i];
        /// Horizontale
        if (positions[0] == checkingSymbol && positions[1] == checkingSymbol && positions[2] == checkingSymbol) win = 1;
        if (positions[3] == checkingSymbol && positions[4] == checkingSymbol && positions[5] == checkingSymbol) win = 1;
        if (positions[6] == checkingSymbol && positions[7] == checkingSymbol && positions[8] == checkingSymbol) win = 1;
        /// Vertical
        if (positions[0] == checkingSymbol && positions[3] == checkingSymbol && positions[6] == checkingSymbol) win = 1;
        if (positions[1] == checkingSymbol && positions[4] == checkingSymbol && positions[7] == checkingSymbol) win = 1;
        if (positions[2] == checkingSymbol && positions[5] == checkingSymbol && positions[8] == checkingSymbol) win = 1;
        /// Diagonal
        if (positions[0] == checkingSymbol && positions[4] == checkingSymbol && positions[8] == checkingSymbol) win = 1;
        if (positions[2] == checkingSymbol && positions[4] == checkingSymbol && positions[6] == checkingSymbol) win = 1;

        if (win) return (i+1);
    }
    return 0;
}

int requestCaseNum() {
    int input;
    scanf("%d", &input);
    if (input < 1 || input > 9) {
        printf("\n%sCase invalide, veuillez reessayer : ", prefix);
        return requestCaseNum();
    }
    if (positions[input-1] != "_") {
        printf("\n%sCette case est deja prise, reessayez : ", prefix);
        return requestCaseNum();
    }
    return input;
}

void applySelectionToCase(int caseId) {
    positions[(caseId-1)] = symbols[(currentPlayer-1)];
}

void doTurn(int help, int isIa) {
    clearAll();
    if (help) {
        printf("     <<<< Morpion >>>>\n   By Pablo Z & Neil T\n\nBienvenue sur le jeu du Morpion !\nBattez votre adversaire en remplissant une série de case\nDiagonales, verticales et horizontales");
    }
    printf("\n\n%sGrille actuelle, tour %i:\n\n", prefix, turn);
    draw();
    printf(isIa && currentPlayer == 2 ? "\n%sC'est a l'IA (joueur %i) de jouer..." : "\n%sC'est au joueur %i de jouer, indiquz une case : ", prefix, currentPlayer);
    if (isIa && currentPlayer != 2) {
        int selection = requestCaseNum();
        applySelectionToCase(selection);
    } else {
        int caseId = aiGetCase();
        if(caseId == 9000) return;
        positions[caseId] = symbols[1];
    }
    int win = checkWinner();
    if (win > 0) {
        clearAll();
        draw();
        printf(win == 100 ? "\n\n%sFin de la partie, le systeme a retourne un code %i, match nul !" : "\n\n%sFin de la partie ! Le joueur %i gagne !\n\n", prefix, win);
        exit(0);
    }
    currentPlayer++;
    turn++;
    if(currentPlayer > 2) currentPlayer = 1;
    doTurn(0, isIa);
}

int main()
{
    init();
    doTurn(1, 1);
    return 0;
}
