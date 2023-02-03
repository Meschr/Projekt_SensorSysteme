function [dataCorrected] = RemoveOffsetFromPlateaus(data,foundPlateaus)
%REMOVEOFFSETFROMPLATEAUS Summary of this function goes here
%   Detailed explanation goes here

    dataCorrected = data;
    for i = 1 : length(foundPlateaus)
        if (mod(i,2) == 1)
            dataCorrected = dataCorrected(foundPlateaus(i):end)-data(foundPlateaus(i));
        end
    end     
end

