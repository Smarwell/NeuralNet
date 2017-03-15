
from NNWrapper import *
import pygame
import random
import sys

while not pygame.font.get_init():
    pygame.font.init()

font=pygame.font.Font(None,24)

NETS = 100

BBOUND=600
RBOUND=600
TBOUND=0
LBOUND=0

should_load = len(sys.argv)>1 and sys.argv[1]=='load'

net = NeuralNet(4,20,2,1,num_nets=NETS,survivors=NETS//20+1,load_nets=should_load)

pygame.init()

screen = pygame.display.set_mode((600,600))

def integer(item):
    return [int(num) for num in item]

class Thing:
    def __init__(self,net_index):
        self.net=net_index
        self.location=[random.randint(200,400),random.randint(200,400)]
        self.velocity=[0.,0.]
        self.acceleration=2.
        self.score=0

    def update(self):
        self.cover_up()
        out=net.run(i,
                    [
            (RBOUND-self.location[0])/300.-1,
            (self.location[0]-LBOUND)/300.-1,
            (BBOUND-self.location[1])/300.-1,
            (self.location[1]-TBOUND)/300.-1
            ]
                    )
        if out[0] > 0.5:
            self.xjump()
        if out[1] > 0.5:
            self.yjump()
        self.velocity[0]+=self.acceleration
        self.velocity[1]+=self.acceleration
        self.location[1]+=self.velocity[1]
        self.location[0]+=self.velocity[0]
        pygame.draw.circle(screen,(255,255,255),tuple(integer(self.location)),10)

    def cover_up(self):
        pygame.draw.circle(screen,(0,0,0),tuple(integer(self.location)),11)

    def valid(self):
        return TBOUND<self.location[1]<BBOUND and LBOUND<self.location[0]<RBOUND

    def yjump(self):
        self.velocity[1]=-20

    def xjump(self):
        self.velocity[0]=-20

    def kill(self):
        out=net.run(i,[0.,0.])
        #thing.score+=abs(0.0001/(out[0]-.5)) if out[0]!=0.5 else 10
        #thing.score+=abs(0.0001/(out[1]-.5)) if out[1]!=0.5 else 10
        if net.run(i,[-5.,0]) != net.run(i,[5.,0]):
            thing.score+=1000000
        if net.run(i,[0,-5.]) != net.run(i,[0,5.]):
            thing.score+=1000000
        net.score_net(i,float(thing.score))
        self.cover_up()
        self.location[1]=10000000


things=[Thing(i) for i in range(NETS)]

clock=pygame.time.Clock()

i=0
gen=1

max_score=0

alt=False
ctrl=False
running=True
while running:
    for event in pygame.event.get():
        if event.type==pygame.QUIT:
            running=False
        if event.type==pygame.KEYDOWN:
            if event.key==pygame.K_s:
                for i,thing in enumerate(things):
                    if thing is not None:
                        net.save(i,ctrl)
                        if alt:
                            break
            if event.key==pygame.K_k:
                for thing in things:
                    if thing is not None:
                        thing.score*=10
                        thing.kill()
            if event.key==pygame.K_LCTRL or event.key==pygame.K_RCTRL:
                ctrl=True
            if event.key==pygame.K_LALT or event.key==pygame.K_RALT:
                alt=True
            if event.key==pygame.K_F4 and alt:
                running=False
        if event.type==pygame.KEYUP:
            if event.key==pygame.K_LCTRL or event.key==pygame.K_RCTRL:
                ctrl=False
            if event.key==pygame.K_LALT or event.key==pygame.K_RALT:
                alt=False
    things_still_alive=False
    for i,thing in enumerate(things):
        if thing is not None:
            things_still_alive=True
            thing.update()
            if not thing.valid():
                thing.kill()
                if thing.score > max_score:
                    max_score=thing.score
                things[i]=None
            thing.score+=1
    if not things_still_alive:
        gen+=1
        net.reproduce()
        max_score=0
        things=[Thing(i) for i in range(NETS)]
    screen.blit(font.render("Gen: "+str(gen)+"     ",False,(255,255,255),(0,0,0)),(450,10))
    screen.blit(font.render("Max: "+str(max_score)+"         ",False,(255,255,255),(0,0,0)),(450,30))
    pygame.display.update()
    clock.tick(30)

pygame.quit()
