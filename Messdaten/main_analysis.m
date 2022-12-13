%% Datenanalyse Wegmessung Gabel
clear; clc; close all;

% Read Data from .txt
Data = readmatrix('data334');
r = Data(:,1)*40/(4*500);                          % gemessener Wegvektor 4-fach (minus beachten, je nach Anschluss)
% r = r(5000:15000);                                % Fenster auswählen
t = linspace(0.001, 0.001*length(r), length(r))';  % Zeitvektor
mark = Data(:,2);                                  % Markervektor

% Rohdaten für Differentiation vorbereiten
rsample = [r, t];
reck = eckdetec(rsample);                          % Eckpunkte der Stufenfunktion ermitteln
r = interpn(reck(:,2),reck(:,1),t,'linear');       % zwischen ermittelten Eckpunkten interpolieren

% Ableitungen bilden
vsample = [[0;(r(2:end) - r(1:(end-1))) ./ (t(2,1) - t(1,1))],t]; % Differentenquotient Geschwindigkeit
v = aalias(vsample(:,1),5);                                       % "Antialiasing"
asample = [0;(v(2:end) - v(1:(end-1))) ./ (t(2,1) - t(1,1))];     % Differentenquotient Beschleunigung
a = aalias(asample,20) /9810;                                     % "Antialiasing"

%% -------- direct-Plots --------

figure('color','white');
tiledlayout(3,1);

% -------- Displacement --------
nexttile
hold on
plot(t,r);
for i = 1:length(mark)
    if mark(i) == 1
        xline(t(i),'r');
    end
end
xlabel('time [s]');
ylabel('displacement [mm]');
xlim([0, t(end)]);
grid on
hold off

% -------- Velocity --------
nexttile
hold on
plot(t, v);
for i = 1:length(mark)
    if mark(i) == 1
        xline(t(i),'r');
    end
end
xlabel('time [s]');
ylabel('velocity [mm/s]');
xlim([0, t(end)]);
grid on
hold off

% -------- Acceleration --------
nexttile
hold on
plot(t, a);
for i = 1:length(mark)
    if mark(i) == 1
        xline(t(i),'r');
    end
end
xlabel('time [s]');
ylabel('acceleration [g]');
xlim([0, t(end)]);
grid on
hold off

%% -------- position-Histogram --------
figure('color','white');
edges = 0:5:200;
x1 = histcounts(r,edges)/numel(r)*100;
histogram('BinEdges',edges,'BinCounts',x1)
ytickformat('%g%%');
xlabel('travel-position [mm]');
ylabel('percentage of time');
grid on
title('axle-position histogram');

%% -------- speed-Histograms --------
idx = v<0;
neg = v(idx);
pos = v(~idx);

% pos
figure('color','white');
edges = linspace(0,max(pos),40);
x2_pos = histcounts(pos,edges)/numel(pos)*100;
histogram('BinEdges',edges,'BinCounts',x2_pos)
ytickformat('%g%%');
xlabel('shaft-speed [mm/s]');
ylabel('percentage of time');
grid on
title('compression-speed histogram');

% neg
figure('color','white');
edges = linspace(min(neg),0,40);
x2_neg = histcounts(neg,edges)/numel(neg)*100;
histogram('BinEdges',edges,'BinCounts',x2_neg);
ytickformat('%g%%');
xlabel('shaft-speed [mm/s]');
ylabel('percentage of time');
grid on
title('rebound-speed histogram');

%% simfork
% Aufruf simfork
% input.step = 1;                           %Schrittweite [mm]
% input.t_sim = ceil(max(t));                         %Simulationszeit [s]
% 
% front axle path
% input.pathz = CreateTimeSeries(-r,input.t_sim); %Timeseries  
% input.pathy = CreateTimeSeries(zeros(size(r)),input.t_sim); %Timeseries
% input.pathx = CreateTimeSeries(zeros(size(r)),input.t_sim); %Timeseries 
% 
% input.v = r;
% input.t_step = double(input.t_sim / (length(input.v)-1));
% 
% sim('simfork');
