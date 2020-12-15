import pybullet as p
import pybullet_data

class Bot:
    def __init__(self,url="urdf/Paucibot.urdf",botpos=[0.0, 0.0, 0.1], pClient=None):
        if pClient is None:
            pClient = 0
        self.pClient = pClient
        self.id = p.loadURDF(url,*botpos)
    def setpoint(self):
        pass
    def control(self):
        pass
