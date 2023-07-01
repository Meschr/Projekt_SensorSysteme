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

diff = (accelY_1_zero - accelY_2_zero)*g*1000;      % Differenzsignal der beiden Accelerometer in [mm/s^2]
filtered_diff = lowpass(diff,fc,fs);            % filtered signal

%% Differenzbildung und Korrektur

% s und off werden als Fitparameter verwendet, 
% um die Referenzdaten möglichst exakt zu erhalten. 
%(Wertebereich s ca. 0,95 - 1,05, off ca. -0,2 bis + 0,2 m/s²)
%
% 
% for i=1:length(accelY_11)    
%     
%     if(d(i) >= meand)
%         NewCorrectedValue = (s*(diff(i)-off))-diff(i);
%         CompleteCorretion = CompleteCorretion + NewCorrectedValue;        
%         a_diff_korr(i) = CompleteCorretion + (s*(diff(i)-off));    
% 
%     else
%         a_diff_korr(i) = CompleteCorretion + (s*(diff(i)-off));
%     end
% end

a_diff_korr = filtered_diff

% for i=1:length(accelY_unten) 
%    a_diff_korr(i)=  (s*(filtered_diff(i)-off));
% end 

%% Differentation und Integration

vRef = gradient(positionSensor_1,t);    % Referenzsignal ableiten -> Geschwindigkeit
aRef = gradient(vRef,t);                % brechnete Referenzgeschwindigkeit ableiten -> Beschleunigung

v1 = cumtrapz(t,a_diff_korr);      % Accelerometer integrieren -> Geschwindigkeit
v2 = cumtrapz(t,filtered_diff);    % Accelerometer integrieren -> Geschwindigkeit

x1 = cumtrapz(t,v1);                    % Zweite Integration Accelerometer -> Strecke
x2 = cumtrapz(t,v2);                    % Zweite Integration Accelerometer -> Strecke

% %% Fit Funktion der raw Daten mithilfe Basic Fitting Funktion in Matlab Plot (Figure(5))
% load fit_Figure5.mat
% p = fit_Figure5.coeff
% fitted = polyval(p,t)
% x1 = x1-fitted

%% Pull Up Funktion zur Nulllinie

checkedValue = v1(1);
upperLimit = 0.41;
lowerLimit = -0.39;
plateauLength = 110;

%plateausXValues = FindPlateausInData(a_diff_korr, lowerLimit, upperLimit, plateauLength);
plateausXValues = FindPlateausInData(v1, lowerLimit, upperLimit, plateauLength);
v_crazy = RemoveOffsetFromPlateaus(v1, plateausXValues)


%plot(filtered_diff);
%legend('InputAccel','OutputAccel');
for i = 1 : length(plateausXValues)
    xline(plateausXValues(i));
end   

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
plot(t,a_diff_korr);
hold on
plot(t,filtered_diff);
plot(t,aRef)
%stem(t,diff/g)
xlabel('time t in s');
ylabel('acceleration in [mm/s^2]');
legend("a_diff_korr","filtered_diff","Referenzsignal","diff");
title("Beschleunigung Differenzsignal");
grid on;

figure(5)
plot(t,v1)
hold on
plot(t,v2)
plot(t,vRef)
title('Velocity')
xlabel('time t in s');
ylabel('velocity in [mm/s]');
legend("Accelerometer corrected","Accelerometer raw","Referenzsignal","a diff korr");
grid on

figure(6)
plot(t,x1)
hold on
plot(t,x2)
plot(t,positionSensor_1)
legend("Accelerometer corrected","Accelerometer raw",'Referenzsignal')
xlabel('time in s')
ylabel('distance in [mm]')
title('Strecke')
grid on

figure(7)
plot(t,diff)
hold on
plot(t,filtered_diff)
xlabel('time in s')
ylabel('acceleration in [mm/s^2]')
legend("diff signal","filterd diff")
title('gefiltertes Diff Signal vs. ungefiltert')
grid on

figure(8)
plot(v_crazy)
title('Geschwindigkeit')
xlabel('time in s')
ylabel('velocity in [mm/s]')
legend('v_{crazy}')
grid on

figure(9)
plot(v1)
hold on
for i = 1 : length(plateausXValues)
    xline(plateausXValues(i));
end 
ylabel('velocity in [mm/s]')
xlabel('Data point n')
title("Plateaus nach Funktion")
