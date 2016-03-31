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

 
class proloState:
    def __init__(self, themap):
        global OR_INITIAL
        self.k = 2
        self.playerJustMoved = 2 # At the root pretend the player just moved is p2 - p1 has the first move
        coord = []
        self.origin = [[0,0,0,0]]*2 #iLig, iCol, type, money on
        self.score = [0,0]
        for i in range(2):
            self.origin[i] = map(int,themap.split('\n')[i].split(' '))+[0,0]
        l_coords = []
        self.themap = copy.deepcopy(themap)
        for (i,row) in enumerate(themap):
            for (j,c) in enumerate(row):
                if c != '~':
                    t = 0 if c == 'o' else 1
                    l_coords.append([j,i,t,0])
       
        self.n_islands = l_coords[:]
        self.p_islands = [[],[]]
       
        for i in range(2):
            self.n_islands.remove(self.origin[i])
            self.p_islands[i].append(self.origin[i][:])
           
            self.p_islands[i][-1][-1] = OR_INITIAL
        self.p_boats = [[],[]]
       
    def Clone(self):
        """ Create a deep clone of this game state.
        """
        return copy.deepcopy(self)
 
    def DoMove(self, move):
        """ Update a state by carrying out the given move.
            Must update playerJustMoved.
        """
        self.playerJustMoved = 3 - self.playerJustMoved
        self.k +=1
        global CARAVELLE_DEPLACEMENT
        #move boats
       
 
        for (i,b) in enumerate(self.p_boats[self.playerJustMoved-1]):
            dmini = 1000
            imini = 0
            for (j,isl) in enumerate(self.n_islands):
                if self.manDist(b,isl) <= dmini:
                    dmini = self.manDist(b,isl)
                    imini = j
            if dmini != 1000:
                themin=1000
                cmin = [0,0]
                for x in range(0,32):
                    for y in range(0,32):
                        if self.manDist(b, [x,y]) <= CARAVELLE_DEPLACEMENT:
                            if self.manDist([x,y],self.n_islands[imini]) <= themin:
                                themin = self.manDist([x,y],self.n_islands[imini])
                                cmin = [x,y]
                if themin == 0:
                    toErase = []
                    for isl in self.n_islands:
                        if isl[0] == cmin[0] and isl[1] == cmin[1]:
                            toErase = isl
                            break
                    if toErase != []:
                        self.n_islands.remove(toErase)
                        self.p_islands[self.playerJustMoved-1].append(toErase)
                self.p_boats[self.playerJustMoved-1][i] = [k for k in cmin]
       
        #construct boats
        for coupleIslandBoat in move:
            for boatToConstruct in range(0, coupleIslandBoat[1]):
                self.p_islands[self.playerJustMoved-1][coupleIslandBoat[0]][3] -= CARAVELLE_COUT
                self.p_boats[self.playerJustMoved-1].append([self.p_islands[self.playerJustMoved-1][coupleIslandBoat[0]][i] for i in range(2)])
       
    def manDist(self, p1, p2):
        return abs(p1[0]-p2[0])+abs(p1[1]-p2[1])
 
    def GetMoves(self):
        """ Get all possible moves from this state.
        """
        global REVENU_ILE,REVENU_VOLCAN,CARAVELLE_COUT,K
        playerToMove = 3-self.playerJustMoved-1
        if self.k % 2 == 1:
            for i in range(2):
                self.score[i] = sum([x for x in map(lambda x: x[3], self.p_islands[i])])+CARAVELLE_COUT*len(self.p_boats[i])
        #giveMoney
        if self.k != 2:
            for isl in self.p_islands[playerToMove]:
                isl[3] += REVENU_ILE + isl[2]*(REVENU_VOLCAN-REVENU_ILE)
       
        if len(self.n_islands) != 0:
            islandHeuristic = [(i,np.mean([self.manDist(isl,isl_n) for isl_n in self.n_islands])) for (i,isl) in filter(lambda x: x[1][2] == 0,enumerate(self.p_islands[playerToMove]))]
       
            islandHeuriticMean = np.mean(map(lambda x: x[1], islandHeuristic))
            islandHeuristic = map(lambda x: x[0],filter(lambda x: x[1] <= islandHeuriticMean, islandHeuristic))
            possibleConstruction = [[(m[0],i) for i in range(0,m[1]+1)] for m in map(lambda x: (x,int(self.p_islands[playerToMove][x][3]/CARAVELLE_COUT)), islandHeuristic)]
            islandActions = list(itertools.product(*possibleConstruction))
 
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
                
def UCTPlayGame():
    """ Play a sample game between two UCT players where each player gets a different number 
        of UCT iterations (= simulations = tree nodes).
    """
    # state = OthelloState(4) # uncomment to play Othello on a square board of the given size
    # state = OXOState() # uncomment to play OXO
    state = ProloState(15) # uncomment to play Nim with the given number of starting chips
    while (state.GetMoves() != []):
        print str(state)
        if state.playerJustMoved == 1:
            m = UCT(rootstate = state, itermax = 1000, verbose = False) # play with values for itermax and verbose = True
        else:
            m = UCT(rootstate = state, itermax = 100, verbose = False)
        print "Best Move: " + str(m) + "\n"
        state.DoMove(m)
    if state.GetResult(state.playerJustMoved) == 1.0:
        print "Player " + str(state.playerJustMoved) + " wins!"
    elif state.GetResult(state.playerJustMoved) == 0.0:
        print "Player " + str(3 - state.playerJustMoved) + " wins!"
    else: print "Nobody wins!"
    
tab=[]
state=0

# Fonction appelée au début de la partie
def partie_init():
   #global tab, state
   for i in range(TAILLE_TERRAIN):
      tab.append(['~']*TAILLE_TERRAIN)
      for j in range(TAILLE_TERRAIN):
         terr = info_terrain(pos(x=j,y=i))
         if terr == TERRAIN_ILE:
            tab[i][j] = 'o'
         elif terr == TERRAIN_VOLCAN:
            tab[i][j] = '^'
         else
            tab[i][j] = '~'
    state = ProloState(tab)

def update_state():
    pass

# Fonction appelée à chaque tour
def jouer_tour():
    update_state() #l'autre vient de jouer, éventuellement
    if(state.GetMoves() != []):
        m = UCT(rootstate = state, itermax = 1000, verbose = False)
        state.DoMove(m)

# Fonction appelée à la fin de la partie
def partie_fin():
   pass # Place ton code ici
