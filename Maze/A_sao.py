import heapq
import numpy as np
class Node:
    def __init__(self, label, goal_cost):
        self.label = label
        self.cost = 10000
        self.goal_cost = goal_cost
        self.save_cost =  None
        self.pr = []
        self.chld = []
    def __repr__(self):
        return str(dict({
            "label": self.label,
            "cost": self.cost,
            "goal cost": self.goal_cost
        }))
    def __eq__(self, other):
        return self.label == other.label
    def __lt__(self, other):
        if self.save_cost == 10000:
            return self.cost < other.cost
        else:
            return self.goal_cost + self.cost < other.goal_cost + other.cost
    
    def get_label(self):
        return self.label
    def neighbors(self):
        return self.chld + self.pr
class Tree:
    def __init__(self):
        self.nodes = []
        self.edges = []
    def add_nodes(self, data):
        for node in data:
            self.nodes.append(node)
        pass
    def add_node(self, node):
        self.nodes.append(node)
    def get_index(self, node):
        for i, n in enumerate(self.nodes):
            if n.get_label() == node.get_label():
                return i
        return -1
    def add_edges(self, tuple_edges):
        for t in tuple_edges:
            start_label = t[0]
            end_label = t[1]
            w = t[2]
            index_start_label = self.get_index(Node(start_label, None))
            index_end_label = self.get_index(Node(end_label, None))

            self.nodes[index_start_label].chld.append(self.nodes[index_end_label])
            self.nodes[index_end_label].pr.append(self.nodes[index_start_label])
            self.edges.append((self.nodes[index_start_label], self.nodes[index_end_label], t[2]))
    def show_nodes(self):
        return [node.get_data() for node in self.nodes]
    def get_edge(self, start_node, end_node):
        try:
            return [edges for edges in self.edges if edges[0] == start_node 
            and edges[1] == end_node][0]
        except:
            return None



def update_cost(tree, current_node, prev_node):
    if tree.get_edge(prev_node, current_node) is not None:
        if current_node.cost > prev_node.cost + tree.get_edge(prev_node, current_node)[2]:
            current_node.cost = prev_node.cost + tree.get_edge(prev_node, current_node)[2]

def A_Star(tree, start, end):
    frontier = [start]
    heapq.heapify(frontier)
    explored = []
    while len(frontier) > 0:
        state = heapq.heappop(frontier)
        explored.append(state)
        if state == end:
            return explored
        for child in state.neighbors():
            update_cost(tree, child, state)
            if child.get_label() not in list(set(node.get_label() for node in frontier + explored)):
                heapq.heappush(frontier, child)
        
    return False



if __name__ == "__main__":
    tree = Tree()
    tree.add_nodes([
        Node("A", 6),
        Node("B", 3),
        Node("C", 4),
        Node("D", 5),
        Node("E", 3),
        Node("F", 1),
        Node("G", 6),
        Node("H", 2),
        Node("I", 5),
        Node("J", 4),
        Node("K", 2),
        Node("L", 0),
        Node("M", 4),
        Node("N", 0),
        Node("O", 4)
    ])
    tree.add_edges([
        ("A", "B", 2),
        ("A", "C", 1),
        ("A", "D", 3),
        ("B", "E", 5),
        ("B", "F", 4),
        ("C", "G", 6),
        ("C", "H", 3),
        ("D", "I", 2),
        ("D", "J", 4),
        ("F", "K", 2),
        ("F", "L", 1),
        ("F", "M", 4),
        ("H", "N", 2),
        ("H", "O", 4),
    ])
    tree.nodes[0].cost = 0
    result = A_Star(tree, tree.nodes[0], tree.nodes[14])
    if result:
        s = 'explored: '
        for i in result:
            s += i.label + " "
            print(s)
    else:
        print('404 Not Found')

def h(start: np.ndarray, end: np.ndarray):  # Hàm ước lượng h tính theo khoảng cách tới nơi kết thúc
    temp = end - start
    return np.linalg.norm(temp)


def treeCreator(Board: np.ndarray, start: np.ndarray, end: np.ndarray):
    tree = Tree()
    nodes = []                                              #   Chỉ để theo dõi code, không có đóng góp cho việc tính toán
    edges = []                                              #   Chỉ để theo dõi code, không có đóng góp cho việc tính toán
    explored = []                                           #   những đường đã khám phá
    exitNode = None                                         #   Node kết thúc
    startNode = None                                        #   Node bắt đầu
    shape = Board.shape

    explored.append(tuple(start))                           #   Đã khám phá nơi bắt đầu
    nodes.append(Node(str(tuple(start)), h(start, end)))    
    tree.add_nodes([Node(str(tuple(start)), h(start, end))])#   Nạp nơi bắt đầu vào cây dữ liệu, label là tuple vị trí trên ma trận mê cung, goal_cost là hàm ước định
    startNode = tree.nodes[0]                               
    index =0
    while index<len(explored):
        adventurer = explored[index]                        #   Lấy một ô trong những ô đã khám phá
        if adventurer == tuple(end):                        #   Nếu là ô kết thúc, thực hiện vòng lặp tiếp theo với ô khác
            exitNode = tree.nodes[index]
            index+=1
            continue
        i,j = adventurer
        sides = [(i-1,j),(i+1,j),(i,j-1),(i,j+1)]           #   Lấy 4 ô kề xung quang ô đang xét
        for side in sides:
            
            if (-1<side[1]<shape[1]) and (-1<side[0]<shape[0]): #   Nếu ô không vượt quá ma trận mê cung
                
                if (Board[side] != 1):                      #   Nếu không phải là vật cản
                    if side not in explored:                #   Nếu chưa được khám phá, thêm ô, thêm cạnh vào cây
                        explored.append(side)               #   Đánh dấu đã khám phá
                        node = Node( str(side), h(np.asarray(side), end) )
                        nodes.append(node)
                        tree.add_nodes([node])
                        edges.append((str(adventurer), str(side), 1))
                        tree.add_edges([(str(adventurer), str(side), 1)])
                    else:                                   #   Nếu đã khám phá, xét xem có cạnh nào chưa thêm không
                        sideindex = tree.get_index(Node(str(side), None))
                        currentindex = tree.get_index(Node(str(adventurer), None))

                        if (tree.nodes[sideindex] not in tree.nodes[currentindex].pr):  #   ô kề ô đang xét trong cây không phải là nút cha của ô đang xét
                            edges.append((str(adventurer), str(side), 1))
                            tree.add_edges([(str(adventurer), str(side), 1)])           #   Thêm cạnh
        index+=1    

    return tree, startNode, exitNode

def goodpath(Asaoresult: list):
    junctions = [(0,len(Asaoresult)-1)]             #   Khởi tạo các giao lộ với vị trí node cuối (node kết thúc của cây) trong mảng Asaoresult
    paths = []                                      #   Mảng các đường đi
    while len(junctions) != 0:
        junction = junctions.pop(0)                 #   Lấy giao lộ đầu tiên trong danh sách
        index = junction[1]                         #   Lấy vị trí node
        node = Asaoresult[index]                    #   lấy node
        path = []
        while index != 0:                           #   Lấy chưa đến node bắt đầu
            label = node.get_label()
            label : str
            label = label.replace("(","").replace(")","")
            label = label.split(",")
            path.insert(0, (int(label[0]), int(label[1])))   # cập nhật đường đi

            if len(node.pr) > 1:
                for nde in node.pr[1:]:             #   Nếu có nhiều hơn một lối đi
                    if nde in Asaoresult:
                        junctions.append((len(path), Asaoresult.index(nde)))  #   Thêm vào mảng giao lộ
                    
            index = Asaoresult.index(node.pr[0])    #   Địa chỉ node của ô tiếp theo
            node = Asaoresult[index]                #   Đi đến ô tiếp theo, có thể viết là node = node.pr[0]
        if len(paths) != 0:                         #   Nếu hiện tại đã có ít nhất 1 đường đi hoàn chỉnh
            path = path + paths[-1][len(paths[-1])-junction[0]:]    #   Cập nhật phần còn thiếu của các giao lộ từ đường chính
        paths.append(path)                          #   Thêm vào mảng các đường đi
    return paths

        