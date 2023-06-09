/*
Créer le : jeu. 04 mai 2023 00:21:04 CEST

@author: Fanasina à partir du code python3 d' ajuton() à partir d'éléments du code de Corentin Dancette)
*/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<unistd.h>  
//#include <algorithm>



enum action {
    haut,
    gauche,
    droite,
    bas,
    countAct
};

enum contenu {
    vide,
    start,      //renard,
    carotte,    //start,
    block,      //carotte,
    renard,      //block
    countCont
};


const char act_name[countAct] = { 'H','G','D','B' };
//const char cont_name[countCont] = { 'V','R','S','C','B' };
const char cont_name[countCont] = { 'V','S','C','B','R' };



struct position {
    int x;
    int y;
    position& operator=(const position& p);
};
position& position::operator=(const position& p) {
    x = p.x;
    y = p.y;
    return *this;
}
position operator+(const position& p, const position& g) {
    position tmp;
    tmp.x = p.x + g.x;
    tmp.y = p.y + g.y;
    return tmp;
}


struct retour {
    int state;
    int reward;
    bool fin_game;

    retour(int s = 0, int rew = 0, bool fin = false) : state(s), reward(rew), fin_game(fin) {}

};

struct param {
    int max_counter;
    int p_blk;
    int p_rnd;
    int p_crt;
    param& operator=(const param& p);
};
param& param::operator=(const param& p) {
    max_counter = p.max_counter;
    p_blk = p.p_blk;
    p_rnd = p.p_rnd;
    p_crt = p.p_crt;
    return *this;
}

const struct param dftPrm = { .max_counter = 200, .p_blk = 3, .p_crt = 1 };

struct cellule {
    int state; // rank ou id
    struct position pos;
    enum contenu c;
    double Q[countAct];
    cellule& operator=(const cellule& cell_);
};

cellule& cellule::operator=(const cellule& cell_) {
    state = cell_.state;
    pos = cell_.pos;
    c = cell_.c;
    for (int k = 9; k < countAct; k++) Q[k] = cell_.Q[k];
    return *this;
}

struct game {
    struct position dim; // n * m
    size_t counter;
    struct position start_pos;
    struct position lapin_pos;
    struct param prm;

    game(int n = 1, int m = 1, struct param parm = dftPrm) { dim.x = n; dim.y = m; prm = parm; generate_game(); }
    game(struct position Dim) { dim = Dim; generate_game(); }

    struct cellule generate_game();
    std::vector<struct cellule> cells;
    int positionTostate(struct position p);
    int getState();
    int reset();
    struct retour move(enum action act);
    void print();

};

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

void printLine(char c, int l, bool prec) {
    //printf("\033[00m");
    printf("\033[%d;0%dm", 0, 1); // noir // vide
    if (prec) printf("%*c\n", 10, ' ');
    for (int i = 0; i < l; i++)  printf("%c", c);
    printf("%*c\n", 10, ' ');
}
void printLinec(char c, int l) {
    printLine(c, l, true);
}

struct position codeCoul(enum contenu c) {
    struct position color;
    if (c) {
        color.x = c + 31;  color.y = 1;
    }
    else {
        color.x = 0; color.y = 1;
    }
    return color;
}

void game::print() {
    struct position coul;
    int mult = dim.x * 14;
    char sep = '-';
    gotoxy(0, 10);
    for (int j = 0; j < dim.y; j++) {
        for (int i = 0; i < dim.x; i++) {
            int cur = i + j * dim.x;
            coul = codeCoul(cells[cur].c);
            printf("\033[%d;0%dm", coul.x, coul.y);
            printf("s:%2d,(%d,%d),%2c|", cells[cur].state, cells[cur].pos.x, cells[cur].pos.y, cont_name[cells[cur].c]);
        }
        printLinec(sep, mult);
        for (int k = 0; k < countAct; k++) {
            for (int i = 0; i < dim.x; i++) {
                int cur = i + j * dim.x;
                coul = codeCoul(cells[cur].c);
                printf("\033[%d;%dm", coul.x, coul.y);
                if (lapin_pos.x == i && lapin_pos.y == j) {
                    printf("\033[37;01m");
                }
                printf("%2c: %8.4f |", act_name[k], cells[cur].Q[k]);
            }
            printf("%*c\n", 10, ' ');
        }
        printLine(sep, mult, false);
    }
}

int game::positionTostate(struct position p) {
    return p.x + p.y * dim.x;
}
int game::getState() {
    return positionTostate(lapin_pos);
}

int game::reset() {
    lapin_pos = start_pos;
    counter = 0;
    return getState();
}

struct position mouvement(enum action a) {
    struct position pos;
    if (a == haut) { pos.x = 0; pos.y = -1; }
    else if (a == gauche) { pos.x = -1; pos.y = 0; }
    else if (a == droite) { pos.x = 1; pos.y = 0; }
    else if (a == bas) { pos.x = 0; pos.y = 1; }
    return pos;
}

cellule game::generate_game() {
    size_t cur = 0;
    struct cellule cell;
    for (auto j = 0; j < dim.y; j++) {
        for (auto i = 0; i < dim.x; i++) {
            cell.state = cur++;
            cell.pos.x = i;
            cell.pos.y = j;
            cell.c = vide;
            for (int k = 9; k < countAct; k++) cell.Q[k] = 0;
            cells.push_back(cell);
        }
    }
    srand(std::time(NULL));
    int random = rand() % cells.size();
    if (cells[random].c == vide) {
        cells[random].c = renard;
    }
    int rRen = rand() % prm.p_rnd;
    for (int i = 0; i < rRen; i++) {
        while (cells[random].c != vide) { random = rand() % cells.size(); }
        cells[random].c = renard;
    }
    while (cells[random].c != vide) { random = rand() % cells.size(); }
    cells[random].c = start;
    cell = cells[random];

    int rCarot = rand() % prm.p_crt + 1;
    for (int i = 0; i < rCarot; i++) {
        while (cells[random].c != vide) { random = rand() % cells.size(); }
        cells[random].c = carotte;
    }
    int rBlock = rand() % prm.p_blk + 1;
    for (int i = 0; i < rBlock; i++) {
        while (cells[random].c != vide) { random = rand() % cells.size(); }
        cells[random].c = block;
    }

    counter = 0;
    start_pos = cell.pos;
    lapin_pos = start_pos;

    return cell;
}

struct retour game::move(enum action act) {
    counter += 1;
    struct position newPos;
    newPos = lapin_pos + mouvement(act);
    int newstate = positionTostate(newPos);
    struct retour ret;

    if (newPos.x >= dim.x || newPos.y >= dim.y || newPos.x < 0 || newPos.y < 0) {
        ret.fin_game = false; ret.state = getState(); ret.reward = -1;
    }
    else if (counter > prm.max_counter) {
        lapin_pos = newPos; ret.state = getState(); ret.fin_game = true; ret.reward = -10;
    }
    else if (cells[newstate].c == block) {
        ret.state = getState(); ret.fin_game = false; ret.reward = -1;
    }
    else if (cells[newstate].c == renard) {
        lapin_pos = newPos; ret.state = getState(); ret.fin_game = true; ret.reward = -10;
    }
    else if (cells[newstate].c == carotte) {
        lapin_pos = newPos; ret.state = getState(); ret.fin_game = true; ret.reward = 10;
    }
    else {
        lapin_pos = newPos; ret.fin_game = false; ret.state = getState(); ret.reward = -1;
    }

    return ret;
}

int argmax(double Q[countAct]) {
    int iMx = 0;
    double mx = Q[0];
    for (int i = 1; i < countAct; i++) {
        if (Q[i] > mx) {
            iMx = i; mx = Q[i];
        }
    }
    return iMx;
}

double max(double Q[countAct]) {
    double mx = Q[0];
    for (int i = 1; i < countAct; i++) {
        if (Q[i] > mx)  mx = Q[i];
    }
    return mx;
}

void print_cumul(std::vector<int> cumul) {
    printf("cumul reward:\n");
    printf("\033[00m");
    for (auto i : cumul) {
        printf(" %d ", i);
    }
    printf("%*c\n", 400, ' ');
}

void mainQlearning(
    double p_lr = 0.85,
    double p_y = 0.99,
    double p_facExpl = 1,
    int n = 4,
    int m = 4,
    int num_episodes = 200,
    struct param prm = dftPrm,
    long unsigned p_uInterEp = 1000000,
    long unsigned p_uInterTest = 50000
) {

    // Algorithme d'aprentissage

    //const int states_n = n * m;
    //const int num_episodes = 200;

    while (true) {
        // initialisations
        std::vector<int> cumul_reward_list;
        std::vector<std::vector<enum action>> actions_list;
        std::vector<std::vector<int>> states_list;

        struct game gm(n, m, prm);
        gm.print();
        //Démarrage de l'apprentissage sur un nombre d'épisodes fixé
        long unsigned uduree_entre_2_episodes = p_uInterEp;

        for (int i = 0; i < num_episodes; i++) {

            //remis à Zero variable au debut de l'épisode
            std::vector<enum action> actions;
            int s = gm.reset();
            std::vector<int> states(s);
            int cumul_reward = 0;
            struct retour ret(s, 0, false);
            gm.print();

            enum action a;

            // demarrage de l'apprentissage jusqu'à la fin de l'épisode (renard ou carotte trouvés)
            while (true) {
                double lr = p_lr;
                double y = p_y;
                double facteur_explo = p_facExpl;
                long unsigned u_duree_entre_2_tests = p_uInterTest;

                // on choisit une action aléatoire avec une certaine probabilité, qui décroît petit à petit
                // ou on choisit l'action permettant d'espérer la meilleure récompense

                int random = rand() % (num_episodes * num_episodes);
                double r = (double)random / (num_episodes * num_episodes);
                if (r < facteur_explo * (1. / ((i / 10) + 1))) {
                    a = static_cast<action>(rand() % countAct);
                }
                else {
                    a = static_cast<action>(argmax((gm.cells[s]).Q));
                }

                //obtention par l'environnement de l'état suivant et de la récompense
                ret = gm.move(a);
                //Mis à jour de la table Q
                gm.cells[s].Q[a] = gm.cells[s].Q[a] + lr * (ret.reward + y * max(gm.cells[ret.state].Q) - gm.cells[s].Q[a]);
                cumul_reward += ret.reward;
                s = ret.state;

                // Mise à jour des historiques des actions et états
                actions.push_back(a);
                states.push_back(s);

                gm.print();

                usleep(u_duree_entre_2_tests);

                if (ret.fin_game) break;

            }
            //sauvegarde des historiques des états et actions
            states_list.push_back(states);
            actions_list.push_back(actions);
            cumul_reward_list.push_back(cumul_reward);
        }
        print_cumul(cumul_reward_list);
        usleep(uduree_entre_2_episodes);
    }
}

int main(int argc, char* argv[]) {

    srand(std::time(NULL));

    double p_lr = 0.85;
    double p_y = 0.99;
    double p_facExpl = 1;
    int n = 4;
    int m = 4;
    int num_episodes = 200;
    struct param prm;
    prm.max_counter = 190;
    prm.p_blk = 3;
    prm.p_rnd = 3;
    prm.p_crt = 1;
    long unsigned p_uInterEp = 1000000;
    long unsigned p_uInterTest = 50000;

    //mainQlearning(0.7, 0.8, 0.9, 7, 5, 150, 100, 4, 5, 2, 500000, 10000);
    //mainQlearning();

    printf("usage: Ctrl + c to exit\n to launch %s p_lr p_y p_facExpl n m num_episodes max_counter p_blk p_rnd p_crt p_uInterEp p_uInterTest \n", argv[0]);
    printf("by default: %s\n", argv[0]);
    printf("== %s 0.85 0.99 1.0 4 4 200 190 3 3 1 1000000 50000 \n", argv[0]);
    printf("== %s  lf   lf   lf d d  d   d  d d d    ld     ld \n", argv[0]);

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (i == 1) p_lr = atof(argv[i]);
            else if (i == 2) p_y = atof(argv[i]);
            else if (i == 3) p_facExpl = atof(argv[i]);
            else if (i == 4) n = atoi(argv[i]);
            else if (i == 5) m = atoi(argv[i]);
            else if (i == 6) num_episodes = atoi(argv[i]);
            else if (i == 7) prm.max_counter = atoi(argv[i]);
            else if (i == 8) prm.p_blk = atoi(argv[i]);
            else if (i == 9) prm.p_rnd = atoi(argv[i]);
            else if (i == 10) prm.p_crt = atoi(argv[i]);
            else if (i == 11) p_uInterEp = atol(argv[i]);
            else if (i == 12) p_uInterTest = atol(argv[i]);
        }
    }
    printf("commande pour cette session : %s %.2lf %.2lf %.2lf %d %d %d %d %d %d %d %ld %ld \n",
        argv[0], p_lr, p_y, p_facExpl, n, m, num_episodes, prm.max_counter, prm.p_blk, prm.p_rnd, prm.p_crt, p_uInterEp, p_uInterTest);

    mainQlearning(p_lr, p_y, p_facExpl, n, m, num_episodes, prm, p_uInterEp, p_uInterTest);

    return 0;
}