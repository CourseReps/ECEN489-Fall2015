clear all;
close all;
D = 0.00635;
h1 = 0.02;
h2 = 0.03;
g = 9.8;
Ki = 4;
Kp = 1;
Kd = 0;
L = 0.02;
p = 1000;
size = 10;
u = 8.90 * 10^(-4);
Q2 = zeros (1,size);
T = Q2;
A = 0.0054;
V = 0.000162;

C = pid(Kp, Ki, Kd)
% d = 1193.8* pow(levelSetpoint,-0.905);
t=0;
i=2;
T(1)=0;
VolOfStone = 0;
tic
while h2 >= h1
    
    %     prompt = 'What is the current level of water?';
    %     h2 = input(prompt); 
    T(i) = toc;
    Q2(i) = (pi*D^(4)/(128*u))*(p*g*(h2))/L;
    V = V - Q2(i)*(T(i)-T(i-1));
    h2 = V/A;
    VolOfStone = VolOfStone + Q2(i)*(T(i)-T(i-1));
    i = i+1;
   
end
Volume = VolOfStone
time = toc;
plot(T(1,:), Q2(1,:),'.')


% scatter(Q2(:),T(:));
% plot([Q2' T']);
% for i = 1:size
%         plot(Q2(1,i), T(1,i),'mo')
%         hold
% end
