% Projekt Sensorsysteme
% Auswertung erste Messungen 

file =  csvread("MEssung30mm.TXT",1);
% Split the data at $


plot(file(:,6),file(:,1))
grid on