# -*- coding: utf-8 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

from api import *
from math import *
import heapq
import random
import copy
import itertools
import Queue
import numpy as np
from graphviz import Digraph
from sets import Set
from collections import OrderedDict
import resource, sys

LMAXID=1 #attention à cette variable, mais pour l'instant on la met dans un endroit useless. Source de bugs
 
class proloState:
    def __init__(self, monX, monY, advX, advY, themap):
        global OR_INITIAL
        self.k = 2
        self.playerJustMoved = 2 # At the root pretend the player just moved is p2 - p1 has the first move
        coord = []
        self.origin = [[0,0] for i in range(2)] #iLig, iCol, type, money on
        self.score = [0,0]
        self.origin[0] = position(x=monX,y=monY)
        self.origin[1] = position(x=advX,y=advY)
        l_coords = []
        self.themap = copy.deepcopy(themap)
        for (i,row) in enumerate(themap):
            for (j,c) in enumerate(row):
                self.or_sur_ile[i][j] = info_ile_or(position(x=j,y=i))
                if c != '~':
                    #t = 0 if c == 'o' else 1
                    l_coords.append(position(x=j,y=i))
       
        self.n_islands = l_coords[:]
        self.p_islands = [[],[]]
       
        for i in range(2):
            self.n_islands.remove(self.origin[i])
            self.p_islands[i].append(self.origin[i])
           
            #self.p_islands[i][-1][-1] = OR_INITIAL pas nécessaire normalement grâce à self.or_sur_ile[i][j] plus haut
        self.p_boats = [[],[]]
       
    def Clone(self):
        """ Create a deep clone of this game state.
        """
        return copy.deepcopy(self)
 
    def DoMove(self, move, joueVraiment=False):
        """ Update a state by carrying out the given move.
            Must update playerJustMoved.
        """
        global LMAXID
        self.playerJustMoved = 3 - self.playerJustMoved
        self.k +=1
        global CARAVELLE_DEPLACEMENT
        #move boats
       
 
        for (i,b) in enumerate(self.p_boats[self.playerJustMoved-1]):
            dmini = 1000
            imini = 0
            for (j,isl) in enumerate(self.n_islands):
                if self.manDist(b,isl) <= dmini:
                    dmini = self.manDist(b.pos,isl)
                    imini = j
            if dmini != 1000:
                themin=1000
                cmin = position(x=0,y=0)
                for x in range(1,33):
                    for y in range(1,33):
                        if self.manDist(b.pos, position(x=x, y=y)) <= CARAVELLE_DEPLACEMENT:
                            if self.manDist(position(x=x,y=y),self.n_islands[imini]) <= themin:
                                themin = self.manDist(position(x=x,y=y),self.n_islands[imini])
                                cmin = position(x=x,y=y)
                if themin == 0:
                    toErase = []
                    for isl in self.n_islands:
                        if isl.x == cmin.x and isl.y == cmin.y:
                            toErase = isl
                            break
                    if toErase != []:
                        self.n_islands.remove(toErase)
                        self.p_islands[self.playerJustMoved-1].append(toErase)
                self.p_boats[self.playerJustMoved-1][i].pos = cmin
                if joueVraiment:
                    deplacer(self.p_boats[self.playerJustMoved-1][i].id, cmin) #oui ?
                    coloniser(cmin)
       
        #construct boats
        for coupleIslandBoat in move:
            for typeBoatToConstruct in range(0, coupleIslandBoat[1]):
                self.or_sur_ile[coupleIslandBoat[0].y][coupleIslandBoat[0].x] -= CARAVELLE_COUT
                self.p_boats[self.playerJustMoved-1].append(bateau(id=LMAXID+1,pos=coupleIslandBoat[0],joueur=self.playerJustMoved-1],bateau_type=typeBoatToConstruct,nb_or=0,deplacable=False))
                LMAXID +=1
                if(joueVraiment):
                    construire(typeBoatToConstruct, coupleIslandBoat[0])
       
    def manDist(self, p1, p2):
        return abs(p1.x-p2.x)+abs(p1.y-p2.y)
 
    def GetMoves(self):
        """ Get all possible moves from this state.
        """
        global REVENU_ILE,REVENU_VOLCAN,CARAVELLE_COUT,K#,TERRAIN_VOLCAN
        playerToMove = 3-self.playerJustMoved-1
        #if self.k % 2 == 1:
         #   for i in range(2):
          #      self.score[i] = sum([x for x in map(lambda x: x[3], self.p_islands[i])])+CARAVELLE_COUT*len(self.p_boats[i])
        #giveMoney
        if self.k != 2:
            for isl in self.p_islands[playerToMove]:
                self.or_sur_ile[isl.y][isl.x] += REVENU_ILE + (info_terrain(isl)==TERRAIN_VOLCAN)*(REVENU_VOLCAN-REVENU_ILE)
       
        if len(self.n_islands) != 0:
            islandHeuristic = [(i,np.mean([self.manDist(isl,isl_n) for isl_n in self.n_islands])) for (i,isl) in filter(lambda x: info_terrain(x[1])!=TERRAIN_VOLCAN,enumerate(self.p_islands[playerToMove]))]
       
            islandHeuriticMean = np.mean(map(lambda x: x[1], islandHeuristic))
            islandHeuristic = map(lambda x: x[0],filter(lambda x: x[1] <= islandHeuriticMean, islandHeuristic))
            possibleConstruction = [[(m[0],BATEAU_CARAVELLE) for i in range(0,m[1]+1)] for m in map(lambda x: (x,int(self.or_sur_ile[self.p_islands[playerToMove][x].y][self.p_islands[playerToMove][x].x]/CARAVELLE_COUT)), islandHeuristic)]
            islandActions = list(itertools.product(*possibleConstruction))
            #encore un conflit ici : possibleConstruction contient une liste d'id d'iles + id de nouveau bateau sur cette ile
            #moi je veux plutot des paires (idIle, typeBateau), peut etre que là c bon
 
            #actual_k = min(K,len(self.n_islands))
            #boatActions = list(itertools.product([i for i in range(actual_k+1)], repeat=len(self.p_boats[playerToMove])))
            #actions = list(itertools.product(islandActions,boatActions))
   
            return islandActions
        return []
    def GetResult(self, playerjm):
        if self.score[playerjm-1] == self.score[3-playerjm-1]:
            return 0.5
        return self.score[playerjm-1] > self.score[3-playerjm-1]
        """ Get the game result from the viewpoint of playerjm.
        """

    def __repr__(self):
        """ Don't need this - but good style.
        """
        pass

class Node:
    """ A node in the game tree. Note wins is always from the viewpoint of playerJustMoved.
        Crashes if state not specified.
    """
    def __init__(self, move = None, parent = None, state = None):
        self.move = move # the move that got us to this node - "None" for the root node
        self.parentNode = parent # "None" for the root node
        self.childNodes = []
        self.wins = 0
        self.visits = 0
        self.untriedMoves = state.GetMoves() # future child nodes
        self.playerJustMoved = state.playerJustMoved # the only part of the state that the Node needs later
        
    def UCTSelectChild(self):
        """ Use the UCB1 formula to select a child node. Often a constant UCTK is applied so we have
            lambda c: c.wins/c.visits + UCTK * sqrt(2*log(self.visits)/c.visits to vary the amount of
            exploration versus exploitation.
        """
        s = sorted(self.childNodes, key = lambda c: c.wins/c.visits + sqrt(2*log(self.visits)/c.visits))[-1]
        return s
    
    def AddChild(self, m, s):
        """ Remove m from untriedMoves and add a new child node for this move.
            Return the added child node
        """
        n = Node(move = m, parent = self, state = s)
        self.untriedMoves.remove(m)
        self.childNodes.append(n)
        return n
    
    def Update(self, result):
        """ Update this node - one additional visit and result additional wins. result must be from the viewpoint of playerJustmoved.
        """
        self.visits += 1
        self.wins += result

    def __repr__(self):
        return "[M:" + str(self.move) + " W/V:" + str(self.wins) + "/" + str(self.visits) + " U:" + str(self.untriedMoves) + "]"

    def TreeToString(self, indent):
        s = self.IndentString(indent) + str(self)
        for c in self.childNodes:
             s += c.TreeToString(indent+1)
        return s

    def IndentString(self,indent):
        s = "\n"
        for i in range (1,indent+1):
            s += "| "
        return s

    def ChildrenToString(self):
        s = ""
        for c in self.childNodes:
             s += str(c) + "\n"
        return s


def UCT(rootstate, itermax, verbose = False):
    """ Conduct a UCT search for itermax iterations starting from rootstate.
        Return the best move from the rootstate.
        Assumes 2 alternating players (player 1 starts), with game results in the range [0.0, 1.0]."""

    rootnode = Node(state = rootstate)

    for i in range(itermax):
        node = rootnode
        state = rootstate.Clone()

        # Select
        while node.untriedMoves == [] and node.childNodes != []: # node is fully expanded and non-terminal
            node = node.UCTSelectChild()
            state.DoMove(node.move)

        # Expand
        if node.untriedMoves != []: # if we can expand (i.e. state/node is non-terminal)
            m = random.choice(node.untriedMoves) 
            state.DoMove(m)
            node = node.AddChild(m,state) # add child and descend tree

        # Rollout - this can often be made orders of magnitude quicker using a state.GetRandomMove() function
        while state.GetMoves() != []: # while state is non-terminal
            state.DoMove(random.choice(state.GetMoves()))

        # Backpropagate
        while node != None: # backpropagate from the expanded node and work back to the root node
            node.Update(state.GetResult(node.playerJustMoved)) # state is terminal. Update node with result from POV of node.playerJustMoved
            node = node.parentNode

    # Output some information about the tree - can be omitted
    if (verbose): print rootnode.TreeToString(0)
    else: print rootnode.ChildrenToString()

    return sorted(rootnode.childNodes, key = lambda c: c.visits)[-1].move # return the move that was most visited
    
tab=[]
state=0
idMoi=0
idAdv=0

# Fonction appelée au début de la partie
def partie_init():
   global tab, state, idMoi, idAdv, TAILLE_TERRAIN
   
   #retire la limite de la pile pour les appels rec
   resource.setrlimit(resource.RLIMIT_STACK, (resource.RLIM_INFINITY, resource.RLIM_INFINITY))
   #sys.setrecursionlimit(10**6)
   
   for i in range(TAILLE_TERRAIN):
      tab.append(['~']*TAILLE_TERRAIN)
      for j in range(TAILLE_TERRAIN):
         terr = info_terrain(pos(x=j,y=i))
         if terr == TERRAIN_ILE:
            tab[i][j] = 'o'
         elif terr == TERRAIN_VOLCAN:
            tab[i][j] = '^'
         else:
            tab[i][j] = '~'
    idMoi = mon_joueur()
    idAdv = adversaire()
    advX = 0
    advY = 0
    moiX = 0
    moiY = 0
    moiX = mes_iles()[0].x
    moiY = mes_iles()[0].y
    for pos in liste_iles:
      if info_ile_joueur(pos) == idAdv:
         advX = pos.x
         advY = pos.y
    state = ProloState(moiX, moiY, advX, advY, tab)

def update_state():
    global state,idAdv,TAILLE_TERRAIN
    state.playerJustMoved=2
    state.p_islands[0]=[]
    state.p_islands[1]=[]
    state.p_boats[0]=[]
    state.p_boats[1]=[]
    state.n_islands=[]
    for isl in liste_iles():
      state.or_sur_ile[isl.y][isl.x] = info_ile_or(isl) #en gros, OK
      if info_ile_joueur(isl) != -1:
        state.p_islands[info_ile_joueur(isl)==idAdv].append(isl) #en gros, OK
      else:
        state.n_islands.append(isl)
    for py in range(1,TAILLE_TERRAIN+1):
      for px in range(1,TAILLE_TERRAIN+1):
        for boat in liste_bateaux_position(position(x=px, y=py)):
            state.p_boats[idAdv == boat.joueur].append(boat) #en gros
    state.score[0] = score(mon_joueur())
    state.score[1] = score(adversaire())

def joue(move):
    global state
    pass #todo : recopier DoMove en mettant les appels de fonctions prolo ici

# Fonction appelée à chaque tour
def jouer_tour():
    global state
    update_state() #l'autre vient de jouer, éventuellement
    if(state.GetMoves() != []):
        m = UCT(rootstate = state, itermax = 1000, verbose = False)
        state.DoMove(m,True)
        state.k+=1
        #joue(m)

# Fonction appelée à la fin de la partie
def partie_fin():
   pass # Place ton code ici
