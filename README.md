 lapin_RL
 Rabbit game in a grid using : apprentissage par renforcement: lapin(rabbit), renard, carotte, block
 use of this -> https://eduscol.education.fr/sti/sites/eduscol.education.fr.sti/files/ressources/pedagogiques/14756/14756-introduction-lapprentissage-par-renforcement-ensps.pdf

tested in linux env

 executable name for example : lapin (= name_exec) 
```
g++ -o lapin lapin_game.cpp
./lapin 
```
If you want double precision floating point of the rewards you can use:
```
g++ -o lapin_d lapin_game_double.cpp
./lapin_d
```
USAGE

to exit the program
```
Ctrl + c 
```
to launch with args: 

``
./name_exec param_learning_Rate param_discount_factor param_Explor_Factor x_size y_size number_episodes max_counter param_nb_boclk param_nb_renard param_nb_carotte param_nb_microsecond_Inter_Episode param_nb_microsecond_Inter_Test
``

You can ommit these args , the defaults values :`` 0.85 0.99 1.0 4 4 200 190 3 3 1 1000000 50000 ``

The program generate a random grid of size x_size * y_size.
Each cell can contain one of these elements: empty (V), start (S), carrot (S), fox (R), block (B), (and the rabbit that moves in the grid).
For contents: R (fox), B (block), C (carrot) we can give a maximal number of content : param_nb_contents; the program chose a radom number between 1 and param_n_contents and give a random position of each contents.

Goal: find  (good and optimized) path between start (S) and carrot (C) and avoid : fox (R), block (B) in the grid.

# Caption
```
s: state ,(x,y): position, contents

Contents:
S: Start
V: vide (empty)
C: Carrot
B: Bloc
R: renard (Fox)

H: haut (up)        reward when -action- choosen in this position (state) is -up-   
G: gauche (left)    reward when -action- choosen in this position (state) is -left-
D: droit (right)    reward when -action- choosen in this position (state) is -right-
B: bas (down)       reward when -action- choosen in this position (state) is -down-
```

```
s: 0,(0,0), V|s: 1,(1,0), V|s: 2,(2,0), V|s: 3,(3,0), V|          
--------------------------------------------------------          
 H:   0.0000 | H:   0.0000 | H:   0.0000 | H:   0.0000 |          
 G:   0.0000 | G:   0.0000 | G:   0.0000 | G:  -0.8500 |          
 D:   0.0000 | D:   0.0000 | D:   0.0000 | D:  -0.8500 |          
 B:   0.0000 | B:   0.0000 | B:  -0.8500 | B:  -0.8500 |          
--------------------------------------------------------          
s: 4,(0,1), V|s: 5,(1,1), C|s: 6,(2,1), V|s: 7,(3,1), V|          
--------------------------------------------------------          
 H:   0.0000 | H:   0.0000 | H:   0.0000 | H:  -0.9775 |          
 G:   0.0000 | G:   0.0000 | G:   0.0000 | G:   0.0000 |          
 D:   0.0000 | D:   0.0000 | D:   0.0000 | D:  -0.8500 |          
 B:   0.0000 | B:   0.0000 | B:  -0.9775 | B:   0.0000 |          
--------------------------------------------------------          
s: 8,(0,2), B|s: 9,(1,2), S|s:10,(2,2), V|s:11,(3,2), V|          
--------------------------------------------------------          
 H:   0.0000 | H:   9.9663 | H:  -0.8500 | H:  -0.8500 |          
 G:   0.0000 | G:   8.4629 | G:   0.0000 | G:   0.0000 |          
 D:   0.0000 | D:  -0.9775 | D:  -0.9775 | D:  -0.8500 |          
 B:   0.0000 | B:  -9.9663 | B:  -0.8500 | B:   0.0000 |          
--------------------------------------------------------          
s:12,(0,3), B|s:13,(1,3), R|s:14,(2,3), V|s:15,(3,3), V|          
--------------------------------------------------------          
 H:   0.0000 | H:   0.0000 | H:   0.0000 | H:   0.0000 |          
 G:   0.0000 | G:   0.0000 | G:  -8.5000 | G:   0.0000 |          
 D:   0.0000 | D:   0.0000 | D:   0.0000 | D:   0.0000 |          
 B:   0.0000 | B:   0.0000 | B:  -0.8500 | B:   0.0000 |          
-------------------------------------------------------- 
```
