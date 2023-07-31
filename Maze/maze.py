import pygame as pyg
import numpy as np
from function import readnext
from A_sao import treeCreator, A_Star, goodpath

class Player(pyg.Surface):
    def __init__(self, size: tuple[float,float], pos: tuple[int,int]):
        pyg.Surface.__init__(self, size)                    #   Khởi tạo kích thước của lớp
        self.fill((0,255,0))                                #   Tô màu nền
        self.position = np.array(pos)                       #   Biến lưu vị trí nhân vật
    
    def setPosition(self, coord: tuple[int, int]):          #   Set vị trí mới
        self.position = np.array(coord)

class Maze(pyg.Surface):
    
    def __init__(self, size: tuple[float,float]):
        pyg.Surface.__init__(self, size)                    #   Khởi tạo kích thước của lớp
        self.Board : np.ndarray                             #   Biến lưu ma trận mê cung
        self.fill((255,248,220))                            #   Tô màu nền
        self.exit : np.ndarray                              #   Biến lưu vị trí lối thoát

    def loadStage(self, path: str):
        file = open(path)
        size = []
        #   Lấy kích thước mê cung
        size.append(int(readnext(file)))
        size.append(int(readnext(file)))
        self.Board = np.zeros(size)

        #   Kích thước lớp (đồ hoạ) mê cung
        layerMazeSize = self.get_size()
        #   Kích thước mỗi đối tượng trên mê cung
        blockSize = np.array(layerMazeSize) / np.array(size)
        playerPosition = (0,0)

        for j in range(size[1]):                            #   y
            for i in range(size[0]):                        #   x
                self.Board[i,j] = int(readnext(file))       #   Đọc file gán dữ liệu ở vị trí (x,y) cho ma trận mê cung
                if self.Board[i,j] == 1:                    #   Nếu là vật cản
                    rect = pyg.Rect(blockSize[0]*i, blockSize[1]*j, blockSize[0], blockSize[1])
                    pyg.draw.rect(self, (0,0,0), rect)      #   Tô màu đen cho vật cản
                if self.Board[i,j] == 2:                    #   Nếu là nơi bắt đầu
                    playerPosition = (i,j)                  #   Lưu lại vị trí
                if self.Board[i,j] == 3:                    #   Nếu là lối ra hay đích đến
                    self.exit = (i,j)                       #   Lưu lại vị trí
                    rect = pyg.Rect(blockSize[0]*i, blockSize[1]*j, blockSize[0], blockSize[1])
                    pyg.draw.rect(self, (0,0,255), rect)    #   Tô màu xanh dương cho lối ra
        return blockSize, playerPosition

class System:

    def __init__(self, size: tuple[int, int]):
        pyg.init()                                              #   khởi tạo pygame
        self.size = np.array(size)                              #   lưu kích thước cửa sổ GUI
        screen = pyg.display.set_mode(size)                     #   tạo cửa sổ GUI
        screen.fill((0,0,0))                                    #   màu nền mặc định
        pyg.display.update()                                    #   cập nhật đồ hoạ

        self.mainscreen = screen                                #   lưu của sổ GUI như một biến
        self.LayerMaze = Maze((np.min(size), np.min(size)))     #   tạo lớp không gian trò chơi (mê cung)

        self.blocksize, position = self.LayerMaze.loadStage("./Maze_stage/stage2.txt")  #   Lấy ma trận mê cung từ file
        self.Player = Player(self.blocksize, position)          #   tạo nhân vật

    def EventHandle(self):                                      #   Xử lý sự kiện   
        events = pyg.event.get()                                #   Lấy sự kiện
        for event in events:                     
            if event.type == pyg.MOUSEBUTTONDOWN:               #   Xử lý sự kiện bấm chuột           
                self.mouseHandle(event)
            if event.type == pyg.KEYDOWN:                       #   Xử lý sự kiện bàn phím
                self.keyHandle(event)
            if event.type == pyg.QUIT:                          #   Nếu chọn thoát
                pyg.quit()   

    def mouseHandle(self, event: pyg.event.Event):
        pass

    def keyHandle(self, event: pyg.event.Event):
        if event.key == pyg.K_ESCAPE:
                pyg.quit()  
    
    def draw(self):     # hàm vẽ đồ hoạ
        layerMazeSize = np.asarray(self.LayerMaze.get_size())       #   Kích thước lớp mê cung
        padding = (self.size - layerMazeSize)/2                     #   khoảng đệm lớp mê cung trong cửa số GUI (căn giữa)
        playerPos = padding + self.Player.position*self.blocksize   #   Vị trí người chơi

        self.mainscreen.fill((100,100,100))                         #   Màu nền
        self.mainscreen.blit(self.LayerMaze, padding)               #   Tô lớp mê cung
        self.mainscreen.blit(self.Player, playerPos)                #   Tô nhân vật



def init():
    return System((1000,800))           #khởi tạo một đối tượng System

def update(Game: System):
    try:
        #   Tạo cây từ ma trận mê cung
        tree, startNode, exitNode = treeCreator(Game.LayerMaze.Board, Game.Player.position, Game.LayerMaze.exit)
        #   Khởi tạo chi phí gốc là 0
        startNode.cost = 0
        #   Sử dụng thuật toán A*
        result = A_Star(tree, startNode, exitNode)
        #   Lọc lại kết quả để lấy những đường đi tốt nhất
        paths = goodpath(result)
        #   Chỉ số đường đi
        pathid = 0
        i = 0
        while True:
            Game.draw()
            pyg.display.update()
            Game.EventHandle()
            
            if i < len(paths[pathid]):                      #   Chạy
                Game.Player.setPosition((paths[pathid][i])) 
                i +=1
            else:                                           #   Tiếp tục đường khác sau khi đến đích
                i = 0
                pathid = (pathid + 1)%len(paths)
            pyg.time.wait(300)
    except Exception as e:
        if "video system not initialized" or "display Surface quit" in e.args:
            print("display Surface quit")
        else:
            raise Exception(e)
    

if __name__== "__main__":
    Game = init()               #khởi tạo
    update(Game)                #cập nhật thay đổi
    