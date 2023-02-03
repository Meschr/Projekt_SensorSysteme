% Projekt Sensorsysteme
% Auswertung erste Messungen 

%% Daten einlesen
clc;
clear;
close all;

g = 9.81;
fs = 500;           % sampling frequency
fc = 25;            % cut frequency

file =  importdata("Statisch1.TXT", ',',2);
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
file_1 =  importdata("Heftig1.TXT", ',',2);
% Split the data at $

positionSensor_1 = file_1.data(:,1);
t = linspace(0.002, 0.002*length(positionSensor_1), length(positionSensor_1))'; %Zeitvektor
accelY_unten = (file_1.data(:,3)./2);      % Sensor unten   Due to offset (2G instead 1G we divide /2)
accelY_oben = (file_1.data(:,4)./2);      % Sensor oben    Due to offset (2G instead 1G we divide /2)

accelY_1_zero = accelY_unten - mw_1;
accelY_2_zero = accelY_oben - mw_2;

%% Filter

diff = (accelY_1_zero - accelY_2_zero)*g ;      % Differenzsignal der beiden Accelerometer
filtered_diff = lowpass(diff,fc,fs);            % filtered signal

%% Differenzbildung und Korrektur

% s und off werden als Fitparameter verwendet, 
% um die Referenzdaten möglichst exakt zu erhalten. 
%(Wertebereich s ca. 0,95 - 1,05, off ca. -0,2 bis + 0,2 m/s²)

s = 0.99;
off = 0.045;
% off = OffsetBetweenSensors
a_diff_korr = eye(length(accelY_unten),1);
NewCorrectedValue = 0;
CompleteCorretion = 0;
d = gradient(filtered_diff);
meand = mean(d)
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
    
for i=1:length(accelY_unten) 
   a_diff_korr(i)=  (s*(filtered_diff(i)-off));
end 
%% Differentation und Integration

vRef = gradient(positionSensor_1,t); % Referenzsignal ableiten -> Geschwindigkeit
v = cumtrapz(t,a_diff_korr)*1000;  % Accelerometer integrieren -> Geschwindigkeit
v1 = cumtrapz(t,filtered_diff)*1000;        % Accelerometer integrieren -> Geschwindigkeit

x = cumtrapz(t,v); % Zweite Integration Accelerometer -> Strecke
x1 = cumtrapz(t,v1); % Zweite Integration Accelerometer -> Strecke

aRef = gradient(vRef,t)/(1000*g); % brechnete Referenzgeschwindigkeit ableiten -> Beschleunigung


%% Pull Up Funktion zur Nulllinie
checkedValue = v(1);
nullLinie = 0;
upperLimit = 1;
lowerLimit = 1;
for i=2:length(v)
   if (v(i) >= checkedValue-lowerLimit) && (v(i) <= checkedValue+upperLimit) % Falls der folgewert der velocity +- upperLimit/lowerLimit liegt
       nullLinie = nullLinie + 1;
       if nullLinie == 1
           pullUpValue = v(i); % Der Wert, um den die gesamte Funktion gehoben / gesenkt wird
           FirstPullUpIndex = i; % Der Index ab dem gehoben /gesenkt werden soll
       end
       checkedValue = v(i); % nächste Wert im Array Velocity
       if nullLinie == 100 % Wenn 100 Werte in Folge im Plataeu Bereich liegen
           for k=FirstPullUpIndex:i % Vom ersten index bis aktuellen index bei dem Funktion gehoben/gesenkt werden soll
               if (v(k) < 0) % Wenn Wert negativ, dann soll Wert gehoben werden
                 v(k)= 0;
               else          % Wenn Wert positiv, dann soll Wert gesenkt werden
                 v(k)= 0;
               end
           end
           for j=i:length(v) % Vom aktuellen index bis letzen index bei dem Funktion gehoben/gesenkt werden soll
               if (v(j) < 0) % Wenn Wert negativ, dann soll Wert gehoben werden
                 v(j)= v(j) + pullUpValue;
               else          % Wenn Wert positiv, dann soll Wert gesenkt werden
                 v(j)= v(j) - pullUpValue;
               end
           end
           nullLinie = 0;
       end
   elseif (v(i) <= checkedValue-lowerLimit) && (v(i) >= checkedValue+upperLimit)
       
   else
       nullLinie = 0;      
   end 
end

%% Plots

figure(1)
title("Raw Data Static")
plot(t_static,accelY_static_unten)
hold on
plot(t_static,accelY_static_oben)
legend("sensor unten","sensor oben")
title('Static Acceleration')

figure(7)
title('Raw data')
plot(t,accelY_unten)
hold on 
plot(t,accelY_oben)
legend("sensor unten","sensor oben")

figure(2)
title("Acceleration");
hold on;
grid on;
plot(t,a_diff_korr/g);
plot(t,filtered_diff/g);
plot(t,aRef)
stem(t,diff/g)
xlabel('time t in s');
ylabel('acceleration in G');
legend("a_diff_korr","filtered_diff","Referenzsignal","diff");

figure(6)
plot(t, a_diff_korr)


figure(3)
plot(t,v)
hold on
plot(t,v1)
plot(t,vRef)
title('Velocity')
legend("Accelerometer corrected","Accelerometer raw","Referenzsignal");

figure(4)
plot(t,x)
hold on
plot(t,x1)
plot(t,positionSensor_1)
grid on
legend("Accelerometer corrected","Accelerometer raw",'Referenzsignal')
xlabel('time in s')
ylabel('distance in mm')
title('Position')

figure(5)
plot(t,diff)
hold on
plot(t,filtered_diff)