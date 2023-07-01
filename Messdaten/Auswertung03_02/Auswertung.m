% Projekt Sensorsysteme
% Auswertung erste Messungen 

%% Daten einlesen
clc;
clear;
close all;

g = 9.81;
fs = 500;           % sampling frequency
fc = 25;            % cut frequency

file =  importdata("Statisch1.TXT", ',',2);         % Beschleunigung in g
% Split the data at $

positionSensor = file.data(:,1);
t_static = linspace(0.002, 0.002*length(positionSensor), length(positionSensor))'; %Zeitvektor
accelY_static_unten = (file.data(:,3)/2);      % Sensor unten   Due to offset (2G instead 1G we divide /2)
accelY_static_oben = (file.data(:,4)/2);      % Sensor oben    Due to offset (2G instead 1G we divide /2)

% Average or mean value of array
mw_1 = mean(accelY_static_unten)
mw_2 = mean(accelY_static_oben)

OffsetBetweenSensors = mw_2-mw_1

% Load next file
file_1 =  importdata("Heftig1.TXT", ',',2);         % Beschleunigung in g
% Split the data at $

positionSensor_1 = file_1.data(:,1);
t = linspace(0.002, 0.002*length(positionSensor_1), length(positionSensor_1))'; %Zeitvektor
accelY_unten = (file_1.data(:,3)./2);      % Sensor unten   Due to offset (2G instead 1G we divide /2)
accelY_oben = (file_1.data(:,4)./2);      % Sensor oben    Due to offset (2G instead 1G we divide /2)

accelY_1_zero = accelY_unten - mw_1;
accelY_2_zero = accelY_oben - mw_2;

%% Filter

aDiff = (accelY_1_zero - accelY_2_zero)*g*1000;      % Differenzsignal der beiden Accelerometer in [mm/s^2]
aFilteredDiff = lowpass(aDiff,fc,fs);                % filtered signal

%% Differentation und Integration

vRef = gradient(positionSensor_1,t);    % Referenzsignal ableiten -> Geschwindigkeit
aRef = gradient(vRef,t);                % brechnete Referenzgeschwindigkeit ableiten -> Beschleunigung

v = cumtrapz(t,aFilteredDiff);    % Accelerometer integrieren -> Geschwindigkeit

x = cumtrapz(t,v);                % Zweite Integration Accelerometer -> Strecke

% %% Fit Funktion der raw Daten mithilfe Basic Fitting Funktion in Matlab Plot (Figure(5))
% load fit_Figure5.mat
% p = fit_Figure5.coeff
% fitted = polyval(p,t)
% x1 = x1-fitted

%% Pull Up Funktion zur Nulllinie

upperLimit = 0.41*1000;
lowerLimit = -0.39*1000;
plateauLength = 110;

plateausXValues = FindPlateausInData(aFilteredDiff, lowerLimit, upperLimit, plateauLength);
vPlateausRemoved = RemoveOffsetFromPlateaus(v, plateausXValues);

xCrazy = cumtrapz(t,vPlateausRemoved);                % Zweite Integration Accelerometer -> Strecke
%% Plots

figure(1)
plot(t_static,accelY_static_unten)
hold on
plot(t_static,accelY_static_oben)
legend("sensor unten","sensor oben")
xlabel('time t in s');
ylabel('Beschleunigung in [g]');
title("Raw Data Beschleunigung Static")
grid on;

figure(2)
plot(t,accelY_unten)
hold on 
plot(t,accelY_oben)
legend("sensor unten","sensor oben")
title('Raw Data Beschleunigung')
xlabel('time t in s');
ylabel('Beschleunigung in [g]');
grid on


figure(3)
plot(t,aFilteredDiff);
hold on
plot(t,aRef)
xlabel('time t in s');
ylabel('acceleration in [mm/s^2]');
legend("filtered_diff","Referenzsignal");
title("Beschleunigung Differenzsignal");
grid on;

figure(4)
plot(t,v)
hold on
plot(t,vRef)
title('Velocity')
xlabel('time t in s');
ylabel('velocity in [mm/s]');
legend("Accelerometer berechnet","Referenzsignal");
grid on

figure(5)
plot(t,x)
hold on
plot(t,positionSensor_1)
legend("Accelerometer berechnet",'Referenzsignal')
xlabel('time in s')
ylabel('distance in [mm]')
title('Strecke')
grid on

figure(6)
plot(t,aDiff)
hold on
plot(t,aFilteredDiff)
xlabel('time in s')
ylabel('acceleration in [mm/s^2]')
legend("diff signal","filterd diff")
title('gefiltertes Differenzsignal vs. ungefiltertes Differenzsignal')
grid on

figure(7)
plot(t, v)
hold on
for i = 1 : length(plateausXValues)
    xline(t(plateausXValues(i)));
end 
ylabel('velocity in [mm/s]')
xlabel('time in s')
title("Plateaus nach Funktion")

figure(8)
plot(t,vPlateausRemoved)
hold on
plot(t,v)
title('Geschwindigkeit')
xlabel('time in s')
ylabel('velocity in [mm/s]')
legend('v_{crazy}','v')
grid on

figure(9)
plot(t,xCrazy)
hold on
plot(t,positionSensor_1)
legend("Accelerometer berechnet",'Referenzsignal')
xlabel('time in s')
ylabel('distance in [mm]')
title('Strecke')
grid on

