% Projekt Sensorsysteme
% Auswertung erste Messungen 

file =  fileread("MEssung30mm.TXT");
% Split the data at $
data = regexp(file,',','split')