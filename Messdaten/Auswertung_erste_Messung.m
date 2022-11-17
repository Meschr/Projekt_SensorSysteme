% Projekt Sensorsysteme
% Auswertung erste Messungen 
clc;
clear all;

file =  csvread("Data1.TXT",1);
% Split the data at $

positionSensor = file(:,1);
t = linspace(0.002, 0.002*length(positionSensor), length(positionSensor))'; %Zeitvektor
accelX = file(:,3);
accelY = file(:,4);
accelZ = file(:,5);

figure(1)
plot(t,positionSensor);
xlabel('time t in s');
ylabel('distance in mm');
grid on

figure(2)
title("accelerometer plot");
hold on;
grid on;
plot(t,accelX);
plot(t,accelY);
plot(t,accelZ);
xlabel('time t in s');
ylabel('acceleration in G');
legend("X-Axis","Y-Axis","Z-Axis");


windowSize = 5; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;

filteredX =  filter(b,a,accelX);
figure(3)
hold on 
plot(t,accelX);
plot(t,filteredX);