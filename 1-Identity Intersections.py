import sys
import re

name_rx = '\".+\"'
num_rx = '-?\d+'
coord_rx = r'\('+num_rx+','+num_rx+'\)'
cmd_a_rx = '\s*add\s+'+name_rx+'\s+('+coord_rx+'\s+){2,}\s*$'
cmd_m_rx = '\s*mod\s+'+name_rx+'\s+('+coord_rx+'\s*){2,}\s*$'
cmd_r_rx = '\s*rm\s+'+name_rx+'\s*$'
cmd_g_rx = '\s*gg\s*$'
cmd_a_chk = re.compile(cmd_a_rx)
cmd_m_chk = re.compile(cmd_m_rx)
cmd_r_chk = re.compile(cmd_r_rx)
cmd_g_chk = re.compile(cmd_g_rx)

class Vertex:
    def __init__ (self, x, y):
        self.x = float(x)
        self.y = float(y)
    def __str__ (self):
        return '(' + str(self.x) + ',' + str(self.y) + ')'

class indexV : 
    def __init__ (self,index,v):
        self.index = index
        self.vertex = Vertex(v.x,v.y)
    def __str__ (self):
        self.vertex.x = float(self.vertex.x)
        self.vertex.y = float(self.vertex.y)
        return str(self.index) + ': (' + str("{0:.2f}".format(self.vertex.x)) + ',' + str("{0:.2f}".format(self.vertex.y)) + ')'

class Edge :
    def __init__ (self, src, dst):
        self.src = src
        self.dst = dst
    def __str__(self):
        return '<' + str(self.src) + ',' + str(self.dst) + '>'

class inPointEdge :
    def __init__ (self,edge):
        self.pList = []
        self.edge = Edge(edge.src,edge.dst)
    def getPoints (self,point):
        if not judgeRecur(self.pList,point):
            self.pList.append(point)
    def seperate (self):
        if self.edge.dst.x > self.edge.src.x :
            for i in range(0,len(self.pList) - 1) :
                if self.pList[i].x > self.pList[i+1].x :
                    temp = self.pList[i]
                    self.pList[i] = self.pList[i+1]
                    self.pList[i+1] = temp
        elif self.edge.dst.x < self.edge.src.x :
            for i in range(0,len(self.pList) - 1) :
                if self.pList[i].x < self.pList[i+1].x :
                    temp = self.pList[i]
                    self.pList[i] = self.pList[i+1]
                    self.pList[i+1] = temp
        else :
            if self.edge.dst.y >= self.edge.src.y :
                for i in range(0,len(self.pList) - 1) :
                    if self.pList[i].y > self.pList[i+1].y :
                        temp = self.pList[i]
                        self.pList[i] = self.pList[i+1]
                        self.pList[i+1] = temp
            else :
                for i in range(0,len(self.pList) - 1) :
                    if self.pList[i].y < self.pList[i+1].y :
                        temp = self.pList[i]
                        self.pList[i] = self.pList[i+1]
                        self.pList[i+1] = temp
        segments = []
        sepePoints = [self.edge.src]
        for x in self.pList :
            if not judgeRecur(sepePoints,x) :
                sepePoints.append(x)
        if not judgeRecur(sepePoints,self.edge.dst) :
            sepePoints.append(self.edge.dst)
        for i in range(0,len(sepePoints) - 1) :
            singleEdge = Edge(sepePoints[i],sepePoints[i+1])
            segments.append(singleEdge)
        return segments

class Street :
    name = ""
    vertexes=[] 
    Lines = [] 
    def __init__(self,name,inputVertex) :
        self.name = name 
        self.vertexes = inputVertex
    def calculateLine(self,inputVertex) :
        self.Lines = []
        for i in range(0,len(inputVertex)-1) :
            singleLine = Edge(inputVertex[i],inputVertex[i+1])
            self.Lines.append(singleLine)
    def mod(self,inputVertex):  
        self.vertexes=[]  
        self.Lines = [] 
        for x in inputVertex :
            self.vertexes.append(x)
        self.calculateLine(self.vertexes)

class Database : 
    def __init__(self) :
        self.Streets =[]
    def add(self,street): #judge overlap Street in database
        flag = 1
        for x in self.Streets :
            if re.search(x.name,street.name,re.I) :
                flag = 0
        if flag == 1 :
            self.Streets.append(street)
            return True
        else :
            return False
    def mod(self,name,vertexes):
        for x in self.Streets :
            if re.search(x.name,name,re.I) :   #recall mod function of Street
                x.mod(vertexes)  
                return True
        return False
    def rm(self,name):
        for x in self.Streets :
            if re.search(x.name,name,re.I) :   
                self.Streets.remove(x)
                return True
        return False

class Graph :
    def __init__(self) :
        self.edges = []
        self.vertexes = []
        self.wholeEdges = []
    def calculate(self,database) :
        for x in database.Streets :
            x.calculateLine(x.vertexes)
        for i in range(0,len(database.Streets)) :
            for l1 in database.Streets[i].Lines :
                for j in range(0,len(database.Streets)) :
                    if j != i :
                        for l2 in database.Streets[j].Lines :
                            inPoint = intersect (l1, l2)
                            if inPoint != None:
                                if not judgeRecur(self.vertexes,l1.src) :
                                    self.vertexes.append(l1.src)
                                if not judgeRecur(self.vertexes,l2.src) :
                                    self.vertexes.append(l2.src)
                                if not judgeRecur(self.vertexes,inPoint) :
                                    self.vertexes.append(inPoint)
                                if not judgeRecur(self.vertexes,l1.dst) :
                                    self.vertexes.append(l1.dst)
                                if not judgeRecur(self.vertexes,l2.dst) :
                                    self.vertexes.append(l2.dst)
                                if not judgeRecur(self.wholeEdges,l1) :
                                    self.wholeEdges.append(l1)
                                if not judgeRecur(self.wholeEdges,l2) :
                                    self.wholeEdges.append(l2)
        eList = []
        for x in self.wholeEdges :
            edgeInPoint = inPointEdge(x)
            eList.append(edgeInPoint)
        for i in range(0,len(self.wholeEdges)) :
            for j in range(0,len(self.wholeEdges)) :
                if i != j :
                    inPoint = intersect (self.wholeEdges[i],self.wholeEdges[j]) 
                    if inPoint != None :
                        eList[i].getPoints(inPoint)
                        eList[j].getPoints(inPoint)
        for x in eList :
           self.edges = self.edges + x.seperate()                                                       
    def gg(self,database):
        self.calculate(database)
        i = 1
        inEdges = []
        inVertexes =[]
        for x in self.vertexes : 
            inVertex = indexV(i,x)
            inVertexes.append(inVertex)
            i = i + 1
        if len(self.vertexes) == 0 :
            i = 0
        print('V ' +  str(i), flush=True)
        edgeString = 'E {'
        index = 0
        for e in self.edges :   
            for v in inVertexes :
                if e.src.x == v.vertex.x and e.src.y == v.vertex.y :
                    src = v.index
                    break
            for v in inVertexes :
                if e.dst.x == v.vertex.x and e.dst.y == v.vertex.y :
                    dst = v.index
                    break
            newEdge = Edge(src, dst)
            inEdges.append(newEdge)
            edgeString = edgeString + '<' + str(src) + ',' + str(dst) + '>'
            if index < len(self.edges) - 1 :
                edgeString = edgeString + ','                
            index = index + 1
        edgeString = edgeString + '}'
        print(edgeString, flush=True)

def intersect (l1, l2):
    x1, y1 = l1.src.x, l1.src.y
    x2, y2 = l1.dst.x, l1.dst.y
    x3, y3 = l2.src.x, l2.src.y
    x4, y4 = l2.dst.x, l2.dst.y
    xnum = ((x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4))
    xden = ((x1-x2)*(y3-y4) - (y1-y2)*(x3-x4))
    if xden != 0 : xcoor =  xnum / xden
    else : return None
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4)
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4)
    if yden != 0 : ycoor = ynum / yden
    else : return None
    if xcoor >= min(x1, x2) and xcoor <= max(x1, x2) and xcoor >= min(x3, x4) and xcoor <= max(x3, x4) and ycoor >= min(y1, y2) and ycoor <= max(y1, y2) and ycoor >= min(y3, y4) and ycoor <= max(y3, y4) :
        return Vertex (xcoor, ycoor)
    else : return None

def getName(readline) :
    tupleName = re.search(name_rx,readline,re.I)
    if tupleName == None :
        return False
    tupleName = tupleName.span()
    if tupleName[1] > tupleName[0] + 2 :
        name = readline[tupleName[0]+1:tupleName[1]-1] #name
    else :
        name = readline[tupleName[0]+1]
    return name

def getCoords(readline) :
    vList = []
    tupleV = re.search('('+coord_rx+'\s*){2,}',readline)
    if tupleV == None :
        return False
    corrds = re.findall('('+coord_rx+'\s*)',readline)
    for x in corrds :
        tuplex = re.search(num_rx,x).span()
        Vx = int(x[tuplex[0]:tuplex[1]])
        cropY = x[tuplex[1]+1:]
        tupley = re.search(num_rx,cropY).span()
        Vy = int(cropY[tupley[0]:tupley[1]])
        vertex = Vertex(Vx,Vy)
        vList.append(vertex)
    return vList

def judgeRecur(List,v):
    for x in List :
        if type(x) == Vertex :
            if x.x == v.x and x.y == v.y :
                return True
        elif type(x) == Edge :
            if x.src.x == v.src.x and x.src.y == v.src.y and x.dst.x == v.dst.x and x.dst.y == v.dst.y : 
                return True
        else :
            return False

def judgeInt(inputNum) :
    x,y=inputNum.split ('.')
    flag = 1
    for i in y :
        if i != '0' :
            flag = 0
            break
    if flag == 1 :
        return True
    else :
        return False

def main():
    database = Database()  #Firstly,a new database is needed.
    while True:
        line = sys.stdin.readline() 
        if line == "":
            break
        if re.match(cmd_a_chk, line) != None: 
            stName = getName(line)
            stCorrds = getCoords(line)
            if stName == False :
                if stCorrds == False:
                    print('Error: The input of coords is invalid!')
            if stName != False and stCorrds != False:
                street = Street(stName,stCorrds)
                if not database.add(street) :
                    print("Error: The street being added already exists!")     
        elif re.match(cmd_m_chk, line) != None: 
            stName = getName(line)
            stCorrds = getCoords(line)
            if stName == False :
                if stCorrds == False:
                    print('Error: The input of coords is invalid!')
            if stName != False and stCorrds != False:
                if not database.mod(stName, stCorrds) :
                    print("Error: `mod' specified for a street that does not exist.")
        elif re.match(cmd_r_chk, line) != None: 
            stName = getName(line)
            if stName != False : 
                if not database.rm(stName) :
                    print("Error: `rm' specified for a street that does not exist..")
        elif re.match(cmd_g_chk, line) != None:     #gg
            graph = Graph()
            graph.gg(database)
        else :
            print('Error: The input of commands is invalid!')

if __name__ == "__main__":
    main()