import numpy as np
import time

class PIDController:
	def __init__(self, pp, ii, dd, sp, maxv, minv, start):
		self.currentTime=0
		self.e=[]
		self.input=[start]
		self.output=[]
		self.P=pp
		self.I=ii
		self.D=dd
		self.SP=sp
		self.maxF = maxv
		self.minF = minv

	def compute(self):
		self.currentTime+=1
		self.e.append(self.SP-self.input[-1])

		#P
		currentP = self.P*self.e[-1]
		#I
		currentI = 0
		for i in range(0, len(self.e)):
			currentI+=self.I*self.e[i]
		#D
		currentD=0
		if(len(self.e)>=2):
			currentD=self.e[len(self.e)-1]-self.e[len(self.e)-2]
		cv = currentP+currentI+currentD
		if(cv>self.maxF):
			cv = self.maxF
		elif(cv<self.minF):
			cv = self.minF	
		self.output.append(cv)

	def pushInput(self, val):
		self.input.append(val)

	def getOutput(self):
		return self.output[-1]

	def isInSS(self):
		ss = False
		avge=0
		if(len(self.e)>5):
			avge = 0
			for i in range(1,6):
				avge+=abs(self.e[len(self.e)-i])
			avge/=5
			if(avge<0.001):
				ss = True
		return ss



class WaterModel:
	stepsPerSecond=10
	def __init__(self, sp, are, startheight, outflow):
		self.h=[startheight]
		self.inFlows = []
		self.fout=outflow
		self.setpoint = sp
		self.area=are

	def step(self, inflow):
		ch = self.h[-1]
		self.inFlows.append(inflow)
		self.h.append(max(0,ch+self.getHeightDifference(inflow)))


	def getObjectVolume(self):
		ret = 0

	def dropObject(self, vol):
		self.h[-1]+=vol


	def getHeightDifference(self, fr):
		ret = ((fr-self.fout)/.06)/self.area/self.stepsPerSecond
		return ret

	def getHeight(self):
		return self.h[-1]

	def getAvgFlow(self):
		avgf = 0
		if(len(self.inFlows)>5):
			for i in range(1, 6):
				avgf+=self.inFlows[len(self.inFlows)-i]
			avgf/=5
		return avgf

X = WaterModel(6, 25.13, 0, 0.3)
Y = PIDController(5,0.1,0,6,3.5,0,0)
while(Y.isInSS()==False):
	Y.compute()
	X.step(Y.getOutput())
	Y.pushInput(X.getHeight())
	print("Flow: "+str(Y.getOutput())+"  Height: "+str(X.getHeight())+"  Error: "+str(Y.e[-1]))

SSflow = X.getAvgFlow()
st = len(X.inFlows)	
X.dropObject(2)
Y.pushInput(X.getHeight())
Y.compute()

while(Y.isInSS()==False):
	Y.compute()
	X.step(Y.getOutput())
	Y.pushInput(X.getHeight())
	print("Flow: "+str(Y.getOutput())+"  Height: "+str(X.getHeight())+"  Error: "+str(Y.e[-1]))
et = len(X.inFlows)
objVol = 0
for i in range(st, et):
	objVol+=(SSflow-X.inFlows[i])
objVol/=(60*10)

print("\n\n"+str(objVol*1000))