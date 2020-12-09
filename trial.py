import pybullet as p
import pybullet_data
from time import sleep
p.connect(p.GUI)
p.setAdditionalSearchPath(pybullet_data.getDataPath())
p.loadURDF("plane.urdf")
botpos=[0,0,0.1]
bot = p.loadURDF("urdf/Paucibot.urdf",*botpos)
p.setGravity(0,0,-10)
numJoints = p.getNumJoints(bot)
for joint in range(numJoints):
	print(p.getJointInfo(bot,joint))
wheels = [ 2, 5 ]
targetVel = 15
maxForce = 6
while(1):
	p.setJointMotorControl2(bot, wheels[0], p.VELOCITY_CONTROL, targetVelocity=targetVel, force=maxForce)
	p.setJointMotorControl2(bot, wheels[1], p.VELOCITY_CONTROL, targetVelocity=-targetVel, force=maxForce)

	p.stepSimulation()
	sleep(0.05)

