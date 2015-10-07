clear variables
close all
clc

T = [1/296; 1/273; 1/423];

R = [1, log(28000), (log(28000))^3;...
     1, log(85000), (log(85000))^3;...
     1, log(600)  , (log(600))^3];
 
 A = R\T;