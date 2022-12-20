% Projekt Sensorsysteme
% Auswertung erste Messungen 

%% Daten einlesen
clc;
clear;
close all;

g = 9.81;

file =  importdata("Statisch_1.TXT", ',',2);
% Split the data at $

positionSensor = file.data(:,1);
t = linspace(0.002, 0.002*length(positionSensor), length(positionSensor))'; %Zeitvektor
accelX_1 = (file.data(:,3)./2);      % Sensor unten   Due to offset (2G instead 1G we divide /2)
accelX_2 = (file.data(:,4)./2);      % Sensor oben    Due to offset (2G instead 1G we divide /2)

% Average or mean value of array

mw_1 = mean(accelX_1)
mw_2 = mean(accelX_2)

% accelX_1_zero = accelX_1 - mw_1;
% accelX_2_zero = accelX_2 - mw_2;
% 
% mwz_1 = mean(accelX_1_zero)
% mwz_2 = mean(accelX_2_zero)


% Load next file

file_1 =  importdata("Messung_1.TXT", ',',2);
% Split the data at $

positionSensor_1 = file_1.data(:,1);
t = linspace(0.002, 0.002*length(positionSensor_1), length(positionSensor_1))'; %Zeitvektor
accelX_11 = (file_1.data(:,3)./2);      % Sensor unten   Due to offset (2G instead 1G we divide /2)
accelX_21 = (file_1.data(:,4)./2);      % Sensor oben    Due to offset (2G instead 1G we divide /2)


accelX_1_zero = accelX_11 - mw_1;
accelX_2_zero = accelX_21 - mw_2;

diff = (accelX_1_zero - accelX_2_zero).*g ;

figure(1)
plot(t,positionSensor_1);
xlabel('time t in s');
ylabel('distance in mm');
grid on

figure(2)
title("accelerometer plot");
hold on;
grid on;
plot(t,accelX_1_zero);
plot(t,accelX_2_zero);
xlabel('time t in s');
ylabel('acceleration in G');
legend("X-Unten","X-Oben");

figure(3)
plot(t,diff)
xlabel('time t in s');
ylabel('distance in mm');
grid on
hold on 
plot(t,positionSensor_1)


windowSize = 5; 
b = (1/windowSize)*ones(1,windowSize);
a = 1;

%% Verarbeitung (1.Glätten ...)
% filteredX =  filter(b,a,accelX_1);
% figure(3)
% hold on 
% plot(t,accelX_1);
% plot(t,filteredX);

FX = 1000 * gradient(positionSensor_1);
v = 1000 * cumtrapz(t,diff);

% % Test Fehler bestimmen
% fehler_v = v - FX;
% p = polyfit(t,fehler_v,2);
% pf = p(1).*t+p(2).*t+p(3).*t.^2;
% figure(22)
% plot(t,pf)

figure(4)
plot(t,v*2)
hold on
plot(t,FX)
grid on
legend('v -> acc','v -> ref')
xlabel('time in t')
ylabel('velocity in m/s')
title('Integral 1 -> velocity')

x = cumtrapz(t,v);

figure(5)
plot(t,x)
hold on
plot(t,positionSensor_1)
grid on
legend('s -> acc','s -> ref')
xlabel('time in t')
ylabel('distance in mm')
title('Integral 2 -> Distance')