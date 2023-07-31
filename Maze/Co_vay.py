import pygame
import numpy as np

ObjOnScreen = {}
padding = {"left": 140, "right": 140, "top": 40, "bottom": 40}
lineSpace= {"vertical": 40, "horizontal": 40}
BOARDMARKER = np.zeros((19,19))
STONESIZE = 40
TURN = 1

class Stone(pygame.Surface) :
    class ColorStone:
        DARK = (1,1,1)
        WHITE = (255,255,255)
    def __init__(self, stonesize : float, color: tuple[int,int,int]) -> None:
        pygame.Surface.__init__(self, (stonesize, stonesize))
        self.stonesize = stonesize
        self.color = color
        self.set_colorkey((0,0,0))
        self.coord = None
        pygame.draw.circle(self, color, self.get_rect().center, STONESIZE/2)

    def changeColor(self):
        if self.color == Stone.ColorStone.DARK:
            pygame.draw.circle(self, Stone.ColorStone.WHITE, self.get_rect().center, STONESIZE/2)
            self.color = Stone.ColorStone.WHITE
        else:
            pygame.draw.circle(self, Stone.ColorStone.DARK, self.get_rect().center, STONESIZE/2)
            self.color = Stone.ColorStone.DARK

    def setCoord(self, coord):
        self.coord = coord
    

class objdrawed:
    def __init__(self, surface: pygame.Surface, pos: tuple[int, int]) -> None:
        self.surface = surface
        self.drawedPos = pos
    def copy(self):
        return objdrawed(self.surface, self.drawedPos)

def check():
    exploredStone = []
    for obj in list(ObjOnScreen.values())[2:]:
        if obj.surface.coord in exploredStone:
            continue
        stone = obj.surface
        stone : Stone
        team = teamClassiffer(stone)
        exploredStone += team
        judgment = teamChecker(team=team)
        executeJudgment(team[:-1], judgment)
        
def executeJudgment(team, judgment):
    if judgment == "dead":
        for coord in team:
            key = "{i},{j}".format(i=coord[0], j=coord[1])
            del ObjOnScreen[key]
            BOARDMARKER[coord] = 0

def teamChecker(team):
    for stoneCoord in team[:-1]:
        i,j = stoneCoord
        sides = [(i-1,j), (i+1,j), (i, j-1), (i, j+1)]
        for side in sides:
            if BOARDMARKER[side] == 0:
                return "alive"
    return "dead"

def teamClassiffer(firstStone):
    frontier = [firstStone.coord]
    explored = []
    while frontier:
        state = frontier.pop(0)
        explored.append(state)

        neighbors = []
        i,j = state
        sides = [(i-1,j), (i+1,j), (i, j-1), (i, j+1)]
        for side in sides:
            key = "{i},{j}".format(i=side[0], j=side[1])
            if key in ObjOnScreen.keys():
                if ObjOnScreen[key].surface.color == firstStone.color:
                    neighbors.append(ObjOnScreen[key].surface.coord)   
        for neighbor in neighbors:
            if neighbor not in (explored + frontier):
                frontier.append(neighbor)
    return explored + [0]

def changeTurn():
    global TURN
    if TURN == 1:
        TURN = 2
    elif TURN == 2:
        TURN = 1
    co = ObjOnScreen["unplacedStone"].surface
    co : Stone
    co.changeColor()

def init():
    pygame.init()
    size = (1000, 800)
    screen = pygame.display.set_mode(size)
    screen.fill((0,0,0))

    pygame.display.update() 
    return screen

def Background():
    surface = pygame.Surface(mainscreen.get_size())
    surface.fill((150,150,150))
    for i in range(19):
        pygame.draw.line(surface, (255,255,255),(padding["left"]+i*lineSpace["horizontal"],padding["top"]), 
                        (padding["left"]+i*lineSpace["horizontal"],padding["top"]+lineSpace["vertical"]*18), 1)
        pygame.draw.line(surface, (255,255,255),(padding["left"],padding["top"]+i*lineSpace["vertical"]), 
                        (padding["left"]+18*lineSpace["horizontal"],padding["top"]+i*lineSpace["vertical"]), 1)
    return surface

def handleMouseMotion(event: pygame.event.Event):
    co = ObjOnScreen["unplacedStone"]
    co : objdrawed
    if padding["left"]-STONESIZE/2 < event.pos[0] < mainscreen.get_size()[0]-padding["right"]+STONESIZE/2 \
        and padding["top"]-STONESIZE/2 < event.pos[1] < mainscreen.get_size()[1]-padding["bottom"]+STONESIZE/2:
        pos = event.pos
        i = int((pos[0]+STONESIZE/2-padding["left"]) / STONESIZE)
        j = int((pos[1]+STONESIZE/2-padding["top"]) / STONESIZE)
        pos = [padding["left"] + STONESIZE * i, padding["top"] + STONESIZE * j]
        co.drawedPos = pygame.Vector2(pos) - pygame.Vector2(co.surface.get_rect().center)
    else:
        co.drawedPos = pygame.Vector2(event.pos) - pygame.Vector2(co.surface.get_rect().center)

def handleMouseDown(event: pygame.event.Event):
    if padding["left"]-STONESIZE/2 < event.pos[0] < mainscreen.get_size()[0]-padding["right"]+STONESIZE/2 \
        and padding["top"]-STONESIZE/2 < event.pos[1] < mainscreen.get_size()[1]-padding["bottom"]+STONESIZE/2 :

        pos = event.pos
        j = int((pos[0]+STONESIZE/2-padding["left"]) / STONESIZE)
        i = int((pos[1]+STONESIZE/2-padding["top"]) / STONESIZE)
        if BOARDMARKER[i,j] == 0:
            BOARDMARKER[i,j] = TURN
            co = Stone(40, ObjOnScreen["unplacedStone"].surface.color)
            co.setCoord((i,j))
            ObjOnScreen["{i},{j}".format(i=i, j=j)] = objdrawed(co, ObjOnScreen["unplacedStone"].drawedPos)
            changeTurn()
            check()
        else:
            pass
    else:
        pass

def UpdateGraph():
    for obj in ObjOnScreen.values():
        mainscreen.blit(obj.surface, obj.drawedPos)
    pygame.display.update() 

mainscreen = init()
background = Background()
ObjOnScreen["background"] = objdrawed(background, (0,0))
cotam = Stone(40, Stone.ColorStone.DARK)
ObjOnScreen["unplacedStone"]=objdrawed(cotam, (0,0))
while True:
    UpdateGraph()
    events = pygame.event.get()
    if events == pygame.QUIT:
        pygame.quit()
    for event in events:
        if event.type == pygame.MOUSEMOTION:
            handleMouseMotion(event)
        if event.type == pygame.MOUSEBUTTONDOWN:
            handleMouseDown(event)
            
        if event.type == pygame.QUIT:
            pygame.quit()

