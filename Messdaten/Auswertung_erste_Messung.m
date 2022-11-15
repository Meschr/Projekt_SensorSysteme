% Projekt Sensorsysteme
% Auswertung erste Messungen 
clc;
clear;

file =  csvread("ZYX_hinUndHer.TXT",1);
% Split the data at $

positionSensor = file(:,1);
t = linspace(0.002, 0.002*length(positionSensor), length(positionSensor))'; %Zeitvektor
accelX = file(:,3);
accelY = file(:,4);
accelZ = file(:,5);

figure(1)
plot(t,positionSensor);
grid on

figure(2)
title("accelerometer plot");
hold on;
grid on;
plot(t,accelX);
plot(t,accelY);
plot(t,accelZ);
legend("X-Axis","Y-Axis","Z-Axis");