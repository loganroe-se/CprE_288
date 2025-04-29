import pygame
import math
import time

import socket
import sys
import threading

import json


server=socket.socket()
host=socket.gethostname()

server=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)


try:
    server.connect(("192.168.1.1",288))
    print("Connection Successful")
except:
    print("Connection Failed")


objects=[]
scanObjects=[]
halos=[]


class CyBot:
    def __init__(s,x,y,ang):
        s.x=x
        s.y=y
        s.ang=ang
    def getPos(s):
        return (s.x,s.y)
    def drawCyBot(s):
        #Draw base
        pygame.draw.circle(screen,(0,0,0),(s.x,s.y),13.3)

        #Draw face indicator
        pygame.draw.circle(screen,(255,0,0),(s.x+5*math.cos(s.ang*math.pi/180),s.y-10*math.sin(s.ang*math.pi/180)),5)

class ScanObject:
    def __init__(s):
        s.x=1
    def draw(s):
        pygame.draw.rect(screen,(0,255,0),(100,100,5,5))
   
class Object:
    def __init__(s,angle,dist,width,color,cyBot):
        s.angle=angle
        s.dist=dist
        s.width=width
        s.color=color
        s.cyBot=cyBot
        
        s.x=s.cyBot.getPos()[0]
        s.y=s.cyBot.getPos()[1]

        s.y-=s.dist*math.sin(s.angle*math.pi/180)
        s.x+=s.dist*math.cos(s.angle*math.pi/180)

    def draw(s):
        pygame.draw.circle(screen,s.color,(s.x,s.y),s.width)

class Halo:
    def __init__(s,x,y):
        s.x=x
        s.y=y
    def draw(s):
        pygame.draw.circle(screen,(0,255,255),(s.x,s.y),55,1)

def run():
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
           pygame.quit()
    
    cyBot.drawCyBot()

    ang=0

    for i in range(len(objects)):
        objects[i].draw()
    
    # update the screen with what we've drawn
    pygame.display.flip()

    # control the draw update speed
    clock.tick(50)

    pygame.display.update()

    screen.fill((255,255,255))

# initialise pygame
pygame.init()
screen = pygame.display.set_mode([1450,800])
screen.fill(pygame.Color('white'))
pygame.display.set_caption("CyBot GUI")
clock = pygame.time.Clock()

i=0

# loop until the user clicks the close button

cyBot=CyBot(0,0,0)

w,h = pygame.display.get_surface().get_size()

cyBot.x=w/2
cyBot.y=h/2
cyBot.ang=90

def receive():
    scanCheck=False
    scanTime=0



    while(True):
        printData=True
        keys=pygame.key.get_pressed()
        
        if keys[pygame.K_w]:
            server.send('w'.encode())
        elif keys[pygame.K_a]:
            server.send('a'.encode())
        elif keys[pygame.K_d]:
            server.send('d'.encode())
        elif keys[pygame.K_s]:
            server.send('s'.encode())
        elif keys[pygame.K_p]:
            currentTime=time.time()
            
            if scanCheck==False:
                halo=Halo(cyBot.x,cyBot.y)
                halos.append(halo)
                halo.draw()

                scan=ScanObject()
                scanObjects.append(scan)
                scanTime=time.time()
                server.send('p'.encode())
                scanCheck=True
            else:
                if currentTime-scanTime>1:
                    halo=Halo(cyBot.x,cyBot.y)
                    halos.append(halo)
                    halo.draw()

                    scan=ScanObject()
                    scanObjects.append(scan)

                    scanTime=time.time()
                    server.send('p'.encode())
                    scanCheck=True
        elif keys[pygame.K_m]:
            server.send('m'.encode())
        if not keys[pygame.K_w] and not keys[pygame.K_a] and not keys[pygame.K_d] and not keys[pygame.K_s] and not keys[pygame.K_p] and not keys[pygame.K_m]:
            server.send('l'.encode())

        try:
            data = server.recv(4096).decode()
            data = data.replace('!','')

            if "Line" in data:
                printData=False
                dataSplit=data.split(' ')

                leftLine=int(dataSplit[1])
                frontLeftLine=int(dataSplit[2])
                frontRightLine=int(dataSplit[3])
                rightLine=int(dataSplit[4])

                if leftLine>2700:
                    object=Object(cyBot.ang+90,10,5,(0,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if frontLeftLine>2700:
                    object=Object(cyBot.ang+45,10,5,(0,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if frontRightLine>2700:
                    object=Object(cyBot.ang-45,10,5,(0,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if rightLine>2700:
                    object=Object(cyBot.ang-90,10,5,(0,0,255),cyBot)
                    object.draw()
                    objects.append(object)

                if frontLeftLine>2700 and frontRightLine>2700:
                    object=Object(cyBot.ang,10,5,(0,0,255),cyBot)
                    object.draw()
                    objects.append(object)

            if data=="D":
                print("Done Scanning")
                scanObjects.clear()

            '''if "Bump Left" in data:
                object=Object(135,10,5,(255,0,0),cyBot)
                object.draw()
                objects.append(object)
                print("Bump Left")
            elif "Bump Right" in data:
                object=Object(45,10,5,(255,0,0),cyBot)
                object.draw()
                objects.append(object)
                print("Bump Right")
            elif "Bump Middle" in data:
                object=Object(90,10,5,(255,0,0),cyBot)
                object.draw()
                objects.append(object)
                print("Bump Front")'''

            '''if "Cliff" in data:
                cliff_data=data.split(" ")

                print(cliff_data)

                if cliff_data[1]=='1':
                    object=Object(180,10,10,(255,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if cliff_data[2]=='1':
                    object=Object(135,10,10,(255,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if cliff_data[3]=='1':
                    object=Object(45,10,10,(255,0,255),cyBot)
                    object.draw()
                    objects.append(object)
                if cliff_data[4]=='1':
                    object=Object(0,10,10,(255,0,255),cyBot)
                    object.draw()
                    objects.append(object)'''

            '''if "\n"  in data or "\r" in data:
                data=data.replace("\n", '')
                data=data.replace("\r", '')'''

            if printData:
                print(data)

            start = data.find('{')
            while start != -1:
                end = data.find('}', start) + 1
                if (end == 0):
                    break

                for char in data[start:end]:
                    if not char.isalnum:
                        data=data.replace(char,'')

                jsonObjStr = data[start:end]
                try:
                    jsonData = json.loads(jsonObjStr)

                    if jsonData["function"]=="move":
                        cyBot.y-=jsonData["distance"]*math.sin(cyBot.ang*math.pi/180)
                        cyBot.x+=jsonData["distance"]*math.cos(cyBot.ang*math.pi/180)
                        cyBot.ang-=jsonData["angle"]
                        #print(data[loc:])
                        #print(cyBot.ang)
                    else:
                        #print(data)
                        if jsonData["type"]=="skinny":
                            color=(0,255,0)
                        elif jsonData["type"]=="short":
                            color=(255,255,0)
                        elif jsonData["type"]=="thick":
                            color=(255,0,0)
                        elif jsonData["type"]=="hole":
                            color=(255,0,255)

                        cyBotAngle=90-cyBot.ang

                        object=Object(jsonData["angle"]-cyBotAngle,jsonData["distance"],jsonData["width"],color,cyBot)
                        objects.append(object)

                        object.draw()
                except json.JSONDecodeError:
                    pass
                start = end

                
            '''if '\n' in data:
                data=data.replace('\n','')
            if '\r' in data:
                data=data.replace('\r','')

            
            if '{' in data:
                loc=data.index("{")
                if data.count("{")>=1:
                    for char in data:
                        if char=='"':
                            if char+1=='\n' or char+1=='\r':
                                data[char+1]=''

                    last=data.find('}')

                    jsonData=json.loads(data[loc:last+1])

                    #data=data[last:]

                    if jsonData["function"]=="move":
                        cyBot.y-=jsonData["distance"]*math.sin(cyBot.ang*math.pi/180)
                        cyBot.x+=jsonData["distance"]*math.cos(cyBot.ang*math.pi/180)
                        cyBot.ang-=jsonData["angle"]
                        #print(data[loc:])
                        #print(cyBot.ang)
                    else:
                        #print(data)
                        object=Object(jsonData["angle"],jsonData["distance"],jsonData["width"],(255,0,0),cyBot)
                        objects.append(object)

                        object.draw()'''

                        

        except:
            a=1

thread = threading.Thread(target = receive)
thread.start()

while True:
    cyBot.drawCyBot()

    for object in scanObjects:
        object.draw()

    for object in objects:
        object.draw()

    for halo in halos:
        halo.draw()
    run()

  