# Project 3

# Team Assignment

## [Team 1](https://github.com/CourseReps/ECEN489-Fall2015/wiki/project3-team1)

* Kartic Bhargav
* Thomas Branyon
* Henry Xiangdong Yu
* Kan Zheng

## [Team 2](https://github.com/CourseReps/ECEN489-Fall2015/wiki/project3-team2)

* Anthony Ramirez
* Keaton Brown
* Travis Taghavi
* Austin Taghavi
* Blade Roybal (Free Agent)

## [Team 3](https://github.com/CourseReps/ECEN489-Fall2015/wiki/project3-team3)

* Kaimen Walters
* Steven Jinpeng You
* Hai Li
* Pranay Eedara

## [Team 4](https://github.com/CourseReps/ECEN489-Fall2015/wiki/project3-team4)

* Udaya Bhaskar Kothapalli
* Abhay Anand
* Cooper Rehn
* Seungwon Yoon

# Simulation/Model Testing
In order to verify our models and algorithms, all the teams will test their models using the same values/input files.
##Problem 1
For the single-bucket volume estimation problem, we define the model to have the following parameters (equivalent to a standart 500ml beaker):
* Bucket diameter - 8cm
* Bucket height - 12cm
* Hole/leak diameter - 5mm
* Max flowrate - 3.5 L/min
    - Flowrate is assumed to be linear with PWM value between 0 and 255. i.e., PWM of 255 will yield a flowrate of 3.5L/min, and a PWM of 0 will yield a flow of 0L/min

Our input to the simulation will just be these four values, plus two more - a setpoint, and a height "jump" that occurs when the object is dropped in. To start, we will use the following test cases:
* Setpoint = 6cm, jump = 1cm
* Setpoint = 3cm, jump = 2cm

The input file will be all 6 values comma-separated. Example:

File 1:

    8, 12, 5, 3.5, 6, 1
    
File 2:

    8, 12, 5, 3.5, 3, 2
