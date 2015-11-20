import numpy as np
import time



class WaterModel:
	intlength=20
	stepsPerSecond=10
	def __init__(self, sp, startheight, outflow, dia, pid):
		self.currentTime = 0
		self.h=[]
		self.e=[]
		self.fin=[]
		self.fout=outflow
		self.setpoint = sp
		self.diameter=dia
		self.PIDVals = pid

	def step(self):
		currentFin = getNewFinVal(self)
		self.fin.append(currentFin)



	def getError(self):
		return self.setpoint-self.h[self.currentTime]

	def getObjectVolume(self):
		ret = 0

	def dropObject(self, vol):
		self.h[self.currentTime]+=vol

	def getNewFinVal(self):
		ret = self.fin[self.currentTime]
		#P
		ret+=self.PIDVals[0]*getError
		#I
		for i in range(0, len(self.e)):
			ret+=self.PIDVals[1]*self.e[i]
		#D
		ret +=self.PIDVals[2]*(self.e[self.currentTime]-self.e[self.currentTime-1])

		return ret

	def getHeightDifference(self, fr):
		ret = (fr-self.fout)/self.diameter*1/self.stepsPerSecond

