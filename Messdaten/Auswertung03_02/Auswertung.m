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




%% Savitzky-Golay Filter for Acceleration
outlierIndexes = isoutlier(aFilteredDiff); %Find outliners (Hochpunkt, Tiefpunkt, Ausreißer)

% Extract the good data.
tGood = t(~outlierIndexes); %outliner für zeit
accelGood = aFilteredDiff(~outlierIndexes); %outliner für beschleunigung

windowWidth = 51; % Smaller for tighter following of original data, bigger for smoother curve.
smoothedy = sgolayfilt(accelGood, 2, windowWidth); %Anwendung Savitzky-Golay Filter

% But smoothedy has fewer points so if we're to subtract it from the original
% we have to fill in the missing points.
smoothedy = interp1(tGood, smoothedy, t);

% Now subtract the smoothed signal to get the variation
signal_accel = aFilteredDiff - smoothedy; %korrigiertes Beschleunigungssignal

smoothedy_velo = cumtrapz(t, smoothedy); % Integration von Korrekturwerten


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

%% Detrend Signal
v_detrend = detrend(v,6); %removes the nth-degree polynomial trend



%% Savitzky-Golay Filter for Velocity
% outlierIndexes = isoutlier(v); %Find outliners 

% Extract the good data.
%tGood = t(~outlierIndexes);
%veloGood = v(~outlierIndexes);

%windowWidth = 51; % Smaller for tighter following of original data, bigger for smoother curve.
%smoothedy = sgolayfilt(veloGood, 2, windowWidth);

% But smoothedy has fewer points so if we're to subtract it from the original
% we have to fill in the missing points.
%smoothedy = interp1(tGood, smoothedy, t);

% Now subtract the smoothed signal to get the variation
signal_velo = v - smoothedy_velo; % korrigiertes Geschwindigkeitssignal

x_smooth = cumtrapz(t,v_detrend); % Streckensignal integriert aus detrend korrigiertem Geschwindigkeitssignal


%% Pull Up Funktion zur Nulllinie

upperLimit = 0.41*1000;
lowerLimit = -0.39*1000;
plateauLength = 110;

plateausXValues = FindPlateausInData(aFilteredDiff, lowerLimit, upperLimit, plateauLength);
vPlateausRemoved = RemoveOffsetFromPlateaus(v, plateausXValues);

xCrazy = cumtrapz(t,vPlateausRemoved);                % Zweite Integration Accelerometer -> Strecke
%% Plots
k = 1;
fig = figure(k)
plot(t_static,accelY_static_unten)
hold on
plot(t_static,accelY_static_oben)
legend("sensor unten","sensor oben")
xlabel('time t in s');
ylabel('Beschleunigung in [g]');
title("Raw Data Beschleunigung Static")
grid on;
saveas(fig,fullfile(pwd,'\plots\Raw_Data_Acceleration_static.png'),'png');

k=k+1;
fig = figure(k)
plot(t,accelY_unten)
hold on 
plot(t,accelY_oben)
legend("sensor unten","sensor oben")
title('Raw Data Beschleunigung')
xlabel('time t in s');
ylabel('Beschleunigung in [g]');
grid on
saveas(fig,fullfile(pwd,'\plots\Raw_Data_Acceleration.png'),'png');

k=k+1;
fig = figure(k)
plot(t,aFilteredDiff);
hold on
plot(t,aRef)
xlabel('time t in s');
ylabel('acceleration in [mm/s^2]');
legend("filtered_diff","Referenzsignal");
title("Beschleunigung Differenzsignal");
grid on;
saveas(fig,fullfile(pwd,'\plots\Beschleunigung_Diffenzsignal.png'),'png');

k=k+1;
fig = figure(k)
plot(t,v)
hold on
plot(t,vRef)
title('Velocity')
xlabel('time t in s');
ylabel('velocity in [mm/s]');
legend("Accelerometer berechnet","Referenzsignal");
grid on
saveas(fig,fullfile(pwd,'\plots\Geschwindigkeit.png'),'png');

k=k+1;
fig = figure(k)
plot(t,x)
hold on
plot(t,positionSensor_1)
legend("Accelerometer berechnet",'Referenzsignal')
xlabel('time in s')
ylabel('distance in [mm]')
title('Strecke')
grid on
saveas(fig,fullfile(pwd,'\plots\Strecke.png'),'png');

k=k+1;
fig = figure(k)
plot(t,aDiff)
hold on
plot(t,aFilteredDiff)
xlabel('time in s')
ylabel('acceleration in [mm/s^2]')
legend("diff signal","filterd diff")
title('gefiltertes Differenzsignal vs. ungefiltertes Differenzsignal')
grid on
saveas(fig,fullfile(pwd,'\plots\gefiltert_VS_ungefiltert_Differenzsignal.png'),'png');

k=k+1;
fig = figure(k)
plot(t, v)
hold on
for i = 1 : length(plateausXValues)
    xline(t(plateausXValues(i)));
end 
ylabel('velocity in [mm/s]')
xlabel('time in s')
title("Plateaus nach Funktion")
saveas(fig,fullfile(pwd,'\plots\Plateaus.png'),'png');

k=k+1;
fig = figure(k)
plot(t,vPlateausRemoved)
hold on
plot(t,v)
title('Geschwindigkeit')
xlabel('time in s')
ylabel('velocity in [mm/s]')
legend('v_{crazy}','v')
grid on
saveas(fig,fullfile(pwd,'\plots\Geschwindigkeit_plateaus_korrigiert.png'),'png');

k=k+1;
fig = figure(k)
plot(t,xCrazy)
hold on
plot(t,positionSensor_1)
legend("Accelerometer berechnet",'Referenzsignal')
xlabel('time in s')
ylabel('distance in [mm]')
title('Strecke')
grid on
saveas(fig,fullfile(pwd,'\plots\Strecke_real_vs_ref.png'),'png');

k=k+1;
fig = figure(k)
plot(t, signal_accel);
hold on;
plot(t,aFilteredDiff);
grid on;
title('Corrected Signal');
xlabel('Time');
ylabel('Acceleration');
saveas(fig,fullfile(pwd,'\plots\Beschleunigung_korrigiert.png'),'png');

k=k+1;
fig = figure(k)
plot(t, v_detrend);
hold on;
plot(t,v);
title('Corrected Signal');
xlabel('Time');
ylabel('Velocity');
saveas(fig,fullfile(pwd,'\plots\Geschwindikeit_korrigiert.png'),'png');

k=k+1;
fig = figure(k)
plot(t, x);
hold on;
plot(t,x_smooth);
hold on;
plot(t,positionSensor_1)
legend("Strecke integriert","Strecke mit detrend korrigiert", "Strecke Referenz Sensor")
title('Strecken Signale');
xlabel('Time');
ylabel('Strecke');
saveas(fig,fullfile(pwd,'\plots\Strecke_nach_detrend.png'),'png');
